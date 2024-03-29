#include "Boss.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Pathfinding.h"
#include "Map.h"


Boss::Boss() : Entity(EntityType::BOSS)
{
	name.Create("Boss");
}

Boss::~Boss() {}

bool Boss::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	initialPosition.x = parameters.attribute("x").as_int();
	initialPosition.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepathenemy_5").as_string();
	musicpathslime = parameters.attribute("musicpathslime").as_string();


	return true;
}

bool Boss::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 5, 40, bodyType::DYNAMIC);
	pbody->ctype = ColliderType::ENEMY;
	pbody->listener = this;

	boss.LoadAnimations("boss");
	boss_dead.LoadAnimations("boss_dead");
	boss_attack.LoadAnimations("boss_attack");
	boss_walking.LoadAnimations("boss_walking");
	slime_Fx = app->audio->LoadFx(musicpathslime);

	pathdraw = app->tex->Load("Assets/Maps/azul.png");
	velocity = { 0,10 };

	currentAnimation = &boss;
	return true;
}

bool Boss::Update(float dt)
{
	if (tp)
	{
		tp = false;
		pbody->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y) }, 0);
	}
	if (iskilled)
	{
		currentAnimation = &boss;
	}

	if (reverse && leftmode && !onView && canmove)
	{
		leftmode = false;
		rightmode = true;
		reverse = false;
		currentAnimation = &boss_walking;
	}
	if (reverse && rightmode && !onView && canmove)
	{
		leftmode = true;
		rightmode = false;
		reverse = false;
		currentAnimation = &boss_walking;
	}


	origPos = app->map->WorldToMap(position.x, position.y);
	;
	targPos = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);

	/*LOG("LAST PATH X: %d enemy x: %d", targPos.x, origPos.x);*/


	if (dist(app->scene->player->position, position) < app->map->mapData.tileWidth * tilesview)
	{
		app->map->pathfindingSuelo->CreatePath(origPos, targPos);

		if (app->map->pathfindingSuelo->IsWalkable(targPos))
		{
			lastPath = *app->map->pathfindingSuelo->GetLastPath();
			lastPath2 = app->map->pathfindingSuelo->GetLastPath();
		}

		if (!(app->scene->player->dead || app->scene->player->invisible))
		{
			onView = true;
			iskilled = false;
			if (!isAttacking && canmove)
			{
				currentAnimation = &boss_walking;
			}

			if (dist(app->scene->player->position, position) < app->map->mapData.tileWidth * tilesattack)
			{
				if (!isAttacking)
				{
					isAttacking = true;
				}
			}

			if (isAttacking && !iskilled)
			{
				if (attackTimer.ReadSec() > 3.0) // Controla el tiempo entre ataques
				{
					currentAnimation = &boss_attack;
					canmove = false;
					velocity.x = 0;
					attackTimer.Start(); // Reinicia el temporizador de ataque
				}
			}
		}
		else
		{
		onView = false;
		}
	}
	else
	{
		onView = false;

		if (!onView && !isAttacking && canmove)
		{
			if (rightmode)
			{
				velocity.x = 0.5f;
			}
			if (leftmode)
			{
				velocity.x = -0.5f;
			}
		}

		if (lastPath2 == app->map->pathfindingSuelo->GetLastPath())
		{
			app->map->pathfindingSuelo->ClearLastPath();
		}
	}

	if (currentAnimation == &boss_attack && currentAnimation-> currentFrame >= 9 && !Hasattacked)
	{
		Hasattacked = true;
		// Crear la colisi�n pbodyattack
		if (rightmode)
		{
			pbodyatack = app->physics->CreateRectangle(position.x + 45, position.y + 15, 20, 20, bodyType::DYNAMIC);
			pbodyatack->listener = this;
			pbodyatack->ctype = ColliderType::ENEMY_ATTACK;
		}
		else if (leftmode)
		{
			pbodyatack = app->physics->CreateRectangle(position.x - 2, position.y + 15, 20, 20, bodyType::DYNAMIC);
			pbodyatack->listener = this;
			pbodyatack->ctype = ColliderType::ENEMY_ATTACK;
		}

	}
	
	if (currentAnimation == &boss_attack && currentAnimation->HasFinished() )
	{
		Hasattacked = false;
		if (pbodyatack) {
			//Destroy pbodyatack
			pbodyatack->body->SetActive(false);
			app->physics->world->DestroyBody(pbodyatack->body);
			pbodyatack = nullptr;
		}

		isAttacking = false;
		canmove = true;

		currentAnimation->Reset();
		currentAnimation->loopCount = 0;
	}


	/*LOG("COUNTTTTTTTTTTT: %d", lastPath.Count());*/
	//pinta el pathfinding
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		paint = !paint;
	}

	if (paint)
	{
		const DynArray<iPoint>* path = app->map->pathfindingSuelo->GetLastPath();
			for (uint i = 0; i < path->Count(); ++i)
			{
				iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
				app->render->DrawTexture(pathdraw, pos.x, pos.y);
			}
	}
	

	if (lastPath.Count() > 0) //recorre el path
	{
		iPoint* nextPathTile;
		nextPathTile = lastPath.At(lastPath.Count() - 1);


		if (nextPathTile->x < origPos.x && !isAttacking)
		{
			rightmode = false;
			leftmode = true;
			velocity.x = -1.25f;
		}
		else if (!isAttacking)
		{
			rightmode = true;
			leftmode = false;
			velocity.x = +1.25f;
		}
		
		if (nextPathTile->x == origPos.x) {
			lastPath.Pop(*nextPathTile);
		}
	}
	if (isHurt)
	{
		currentAnimation = &boss_hurt;
		lives--;
		if (leftmode)
		{
			pbody->body->SetTransform({ PIXEL_TO_METERS(position.x + 0.5f), PIXEL_TO_METERS(position.y + 0.5f) }, 0);

		}
		if (rightmode)
		{
			pbody->body->SetTransform({ PIXEL_TO_METERS(position.x - 0.5f), PIXEL_TO_METERS(position.y + 0.5f) }, 0);
		}
		isHurt = false;
	}
	if (currentAnimation == &boss_hurt && currentAnimation->HasFinished()) {
		
		currentAnimation = &boss;
		
	}
	if (lives==0)
	{
		death = true;
	}
	if (death)
		{
		currentAnimation = &boss_dead;

		}
	if (currentAnimation == &boss_dead && currentAnimation->HasFinished() ) { 
		pbody->body->SetActive(false);
		app->entityManager->DestroyEntity(this);
		app->physics->world->DestroyBody(pbody->body);
	}
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics. 
	if (!tp)
	{
		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
		
	}

	pbody->body->SetLinearVelocity(velocity);

	currentAnimation->Update();
	if (leftmode )
	{
		app->render->DrawTexture(texture, position.x - 65 , position.y - 73, &currentAnimation->GetCurrentFrame());
	}
	if (rightmode)
	{
		app->render->DrawTexture(texture, position.x - 65, position.y - 73 , &currentAnimation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
	}



	return true;
}

bool Boss::CleanUp()
{
	return true;
}

void Boss::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype) {
	
	case ColliderType::ENEMY:
		reverse = true;
		break;
	case ColliderType::WALLE:
		/*LOG("PATOTURMO");*/
		reverse=true;
		break;
	case ColliderType::PATACK:
		isHurt=true;
		app->audio->PlayFx(slime_Fx);
		break;	
	case ColliderType::PLAYER:
		if (app->scene->player->dead)
		{
			iskilled = true;
			reverse = true;
			isAttacking = false;
		}
		break;
	default:
		break;
	}
}