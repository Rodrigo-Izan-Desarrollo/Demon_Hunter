#include "Slime.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "SceneMenu.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Pathfinding.h"
#include "Map.h"


Slime::Slime() : Entity(EntityType::SLIME)
{
	name.Create("Slime");
}

Slime::~Slime() {}

bool Slime::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	initialPosition.x = parameters.attribute("x").as_int();
	initialPosition.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepathenemy_1").as_string();
	musicpathslime = parameters.attribute("musicpathslime").as_string();


	return true;
}

bool Slime::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	body = app->physics->CreateCircle(position.x + 16, position.y + 16, 13, bodyType::DYNAMIC);
	body->ctype = ColliderType::ENEMY;
	body->listener = this;

	slime.LoadAnimations("slime");
	slime_dead.LoadAnimations("slime_dead");
	slime_attack.LoadAnimations("slime_attack");
	slime_walking.LoadAnimations("slime_walking");
	slime_Fx = app->audio->LoadFx(musicpathslime);

	pathdraw = app->tex->Load("Assets/Maps/azul.png");
	velocity = { -0.5,0 };

	death = false;

	currentAnimation = &slime;
	return true;
}

bool Slime::Update(float dt)
{
	if (tp)
	{
		tp = false;
		if (app->sceneMenu->newgame)
		{
			body->body->SetTransform({ PIXEL_TO_METERS(initialPosition.x), PIXEL_TO_METERS(initialPosition.y) }, 0);
		}
		else
		{
			body->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y) }, 0);
		}
	}
	if (iskilled)
	{
		currentAnimation = &slime;
	}
	
	if (reverse && leftmode && !onView)
	{
		leftmode = false;
		rightmode = true;
		reverse = false;
		currentAnimation = &slime;
	}	
	if (reverse && rightmode && !onView)
	{
		leftmode = true;
		rightmode = false;
		reverse = false;
		currentAnimation = &slime;
	}


	origPos = app->map->WorldToMap(position.x, position.y);
;
	targPos = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);

	/*LOG("LAST PATH X: %d enemy x: %d", targPos.x, origPos.x);*/
	
		if (dist(app->scene->player->position, position) < app->map->mapData.tileWidth * tilesview)// tenemos una funcion que calcula la dist y calcula la del player y el enemigo y si esta en el rango de vision de tiles view que le siga y si esta mas cerca que le ataque
		{
		app->map->pathfindingSuelo->CreatePath(origPos, targPos);

		if (app->map->pathfindingSuelo->IsWalkable(targPos))
		{
			lastPath = *app->map->pathfindingSuelo->GetLastPath();
			lastPath2 = app->map->pathfindingSuelo->GetLastPath();
		}

		
			onView = true;
			iskilled = false;
			currentAnimation = &slime;
	
			if (dist(app->scene->player->position, position) < app->map->mapData.tileWidth * tilesattack)
			{
				if (!isAttacking)
				{
					isAttacking = true;
				}
			}
			if (isAttacking && !iskilled)
			{
				currentAnimation = &slime_attack;
  			}
	}
	else
	{
		movement();
		if (lastPath2 == app->map->pathfindingSuelo->GetLastPath())
		{
			app->map->pathfindingSuelo->ClearLastPath();
		}
		
	}
	
	

	if (currentAnimation == &slime_attack && currentAnimation->HasFinished()) { // Reiniciar el ataque
		isAttacking = false;
		slime_attack.Reset();
		currentAnimation->loopCount = 0;
	}


	//LOG("COUNTTTTTTTTTTT: %d", lastPath.Count());
	//pinta el pathfinding
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		paint = !paint;
	}

	paintpathfinding();
	

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
				velocity.x = -2.0f;
			}
		}
		else
		{
			rightmode = true;
			leftmode = false;
			velocity.x = +1;
			if (isAttacking)
			{
				velocity.x = 2.0f;
			}
		}
		if (nextPathTile->x == origPos.x) {
			lastPath.Pop(*nextPathTile);
		}


	}

	if (death)
	{
		currentAnimation = &slime_dead;

	}

	if (currentAnimation == &slime_dead && currentAnimation->HasFinished() ) { 
		body->body->SetActive(false);
		app->entityManager->DestroyEntity(this);
		app->physics->world->DestroyBody(body->body);
	}
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics. 
	if (!tp)
	{
		position.x = METERS_TO_PIXELS(body->body->GetTransform().p.x) - 16;
		position.y = METERS_TO_PIXELS(body->body->GetTransform().p.y) - 16;
		
	}

	body->body->SetLinearVelocity(velocity);

	currentAnimation->Update();
	if (leftmode )
	{
		app->render->DrawTexture(texture, position.x, position.y + 7, &currentAnimation->GetCurrentFrame());
	}
	if (rightmode)
	{
		app->render->DrawTexture(texture, position.x, position.y + 7, &currentAnimation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
	}



	return true;
}


bool Slime::CleanUp()
{
	return true;
}

void Slime::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype) {
	
	case ColliderType::ENEMY:
		reverse = true;
		break;
	case ColliderType::WALLE:
		/*LOG("PATOTURMO");*/
		reverse=true;
		break;
	case ColliderType::PATACK:
		death = true;
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

void Slime::paintpathfinding() {

		if (paint)
	{
		const DynArray<iPoint>* path = app->map->pathfindingSuelo->GetLastPath();
			for (uint i = 0; i < path->Count(); ++i)
			{
				iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
				app->render->DrawTexture(pathdraw, pos.x, pos.y);
			}
	}
}

void Slime::movement() {

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
}
