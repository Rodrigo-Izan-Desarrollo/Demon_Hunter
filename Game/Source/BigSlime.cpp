#include "BigSlime.h"
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


BigSlime::BigSlime() : Entity(EntityType::BIGSLIME)
{
	name.Create("BigSlime");
}

BigSlime::~BigSlime() {}

bool BigSlime::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepathenemy_4").as_string();
	musicpathslime = parameters.attribute("musicpathslime").as_string();


	return true;
}

bool BigSlime::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 18, bodyType::DYNAMIC);
	pbody->ctype = ColliderType::ENEMY;
	pbody->listener = this;

	bigslime.LoadAnimations("bigslime");
	bigslime_dead.LoadAnimations("bigslime_dead");
	bigslime_attack.LoadAnimations("bigslime_attack");
	bigslime_walking.LoadAnimations("bigslime_walking");
	slime_Fx = app->audio->LoadFx(musicpathslime);

	pathdraw = app->tex->Load("Assets/Maps/azul.png");
	velocity = { 0,0 };

	currentAnimation = &bigslime;
	return true;
}

bool BigSlime::Update(float dt)
{
	if (tp)
	{
		tp = false;
		pbody->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y) }, 0);
	}
	if (iskilled)
	{
		currentAnimation = &bigslime;
	}
	
	if (reverse && leftmode && !onView)
	{
		leftmode = false;
		rightmode = true;
		reverse = false;
		currentAnimation = &bigslime;
	}	
	if (reverse && rightmode && !onView)
	{
		leftmode = true;
		rightmode = false;
		reverse = false;
		currentAnimation = &bigslime;
	}


	origPos = app->map->WorldToMap(position.x, position.y);
	targPos = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);

	LOG("LAST PATH X: %d enemy x: %d", targPos.x, origPos.x);
	
	if (dist(app->scene->player->position, position) < app->map->mapData.tileWidth * tilesview)// tenemos una funcion que calcula la dist y calcula la del player y el enemigo y si esta en el rango de vision de tiles view que le siga y si esta mas cerca que le ataque
	{
		app->map->pathfindingSuelo->CreatePath(origPos, targPos);

		if (app->map->pathfindingSuelo->IsWalkable(targPos))
		{
			lastPath = *app->map->pathfindingSuelo->GetLastPath();
			lastPath2 = app->map->pathfindingSuelo->GetLastPath();
		}

		if (!(app->scene->player->dead || app->scene->player->invisible)) //que si el player no esta muerto o invisible no le ataque
		{
			onView = true;
			iskilled = false;
			currentAnimation = &bigslime;
	
			if (dist(app->scene->player->position, position) < app->map->mapData.tileWidth * tilesattack)
			{
				if (!isAttacking)
				{
					isAttacking = true;
				}
			}
			if (isAttacking && !iskilled)
			{
				currentAnimation = &bigslime_attack;
  			}
		}
		else
		{
			onView = false;
		}
	}
	else
	{
		onView = false; //generar un movimiento por las zonas determinadas cuando no este onView
		if (!onView && !isAttacking)
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
	
	

	if (currentAnimation == &bigslime_attack && currentAnimation->HasFinished()) { // Reiniciar el ataque
		isAttacking = false;
		bigslime_attack.Reset();
		currentAnimation->loopCount = 0;
	}


	LOG("COUNTTTTTTTTTTT: %d", lastPath.Count());
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


		if (nextPathTile->x < origPos.x)
		{
			rightmode = false;
			leftmode = true;
			velocity.x = -1;
			if (isAttacking)
			{
				velocity.x = -1.3f;
			}
		}
		else
		{
			rightmode = true;
			leftmode = false;
			velocity.x = +1;
			if (isAttacking)
			{
				velocity.x = 1.3f;
			}
		}
		if (nextPathTile->x == origPos.x) {
			lastPath.Pop(*nextPathTile);
		}


	}
	if (isHurt)
	{
		currentAnimation = &bigslime_hurt;
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
	if (currentAnimation == &bigslime_hurt && currentAnimation->HasFinished()) {
		
		currentAnimation = &bigslime;
		
	}
	if (lives==0)
	{
		death = true;
	}
	if (death)
		{
		currentAnimation = &bigslime_dead;

		}
	if (currentAnimation == &bigslime_dead && currentAnimation->HasFinished() ) { 
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
		app->render->DrawTexture(texture, position.x-4, position.y+6 , &currentAnimation->GetCurrentFrame());
	}
	if (rightmode)
	{
		app->render->DrawTexture(texture, position.x-4, position.y +6 , &currentAnimation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
	}



	return true;
}

bool BigSlime::CleanUp()
{
	return true;
}

void BigSlime::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype) {
	
	case ColliderType::ENEMY:
		reverse = true;
		break;
	case ColliderType::WALLE:
		LOG("PATOTURMO");
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