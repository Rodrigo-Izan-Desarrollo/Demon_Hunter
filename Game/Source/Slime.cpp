#include "Slime.h"
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


Slime::Slime() : Entity(EntityType::SLIME)
{
	name.Create("Slime");
}

Slime::~Slime() {}

bool Slime::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepathenemy_1").as_string();
	musicpathslime = parameters.attribute("musicpathslime").as_string();


	return true;
}

bool Slime::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 13, bodyType::DYNAMIC);
	pbody->ctype = ColliderType::ENEMY;
	pbody->listener = this;

	slime.LoadAnimations("slime");
	slime_dead.LoadAnimations("slime_dead");
	slime_attack.LoadAnimations("slime_attack");
	slime_walking.LoadAnimations("slime_walking");
	slime_Fx = app->audio->LoadFx(musicpathslime);


	velocity = { -0.5,0 };

	currentAnimation = &slime;
	return true;
}

bool Slime::Update(float dt)
{
	if (tp)
	{
		tp = false;
		pbody->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y) }, 0);
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
	targPos = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);

	LOG("LAST PATH X: %d enemy x: %d", targPos.x, origPos.x);

	if (dist(app->scene->player->position, position) < app->map->mapData.tileWidth * tilesview)
	{
		if (!(app->scene->player->dead || app->scene->player->invisible))
		{
			onView = true;
			iskilled = false;
			currentAnimation = &slime;

			app->map->pathfindingSuelo->CreatePath(origPos, targPos);
			lastPath = *app->map->pathfindingSuelo->GetLastPath();

			if (dist(app->scene->player->position, position) < app->map->mapData.tileWidth * tilesattack)
			{
				if (!isAttacking)
				{
					isAttacking = true;
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

		if (rightmode)
		{
			velocity.x = 0.5f;
		}
		if (leftmode)
		{
			velocity.x = -0.5f;
		}
	}
	





	if (isAttacking && !iskilled)
	{
		currentAnimation = &slime_attack;

	}

	if (currentAnimation == &slime_attack && currentAnimation->HasFinished()) { // Reiniciar el ataque
		isAttacking = false;
		slime_attack.Reset();
		currentAnimation->loopCount = 0;
	}
	// Cuando el slime esta a tilesattack que persiga al player
	// 
	// Que el slime se mueva y haga flip

	LOG("COUNTTTTTTTTTTT: %d", lastPath.Count());

	if (lastPath.Count() > 0)
	{
		iPoint* nextPathTile;
		nextPathTile = lastPath.At(lastPath.Count() - 1);


		if (nextPathTile->x < origPos.x)
		{
			rightmode = false;
			leftmode = true;
			velocity.x = -1;
		}
		else
		{
			rightmode = true;
			leftmode = false;
			velocity.x = +1;
		}
		if (nextPathTile->x == origPos.x) {
			lastPath.Pop(*nextPathTile);
		}


	}

	if (death)
	{
		currentAnimation = &slime_dead;
		app->audio->PlayFx(slime_Fx);

	}

	if (currentAnimation == &slime_dead && currentAnimation->HasFinished() ) { // Reiniciar el salto
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
		LOG("PATOTURMO");
		reverse=true;
		break;
	case ColliderType::PATACK:
		death = true;
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