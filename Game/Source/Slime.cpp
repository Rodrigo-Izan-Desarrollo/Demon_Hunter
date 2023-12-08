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

	return true;
}

bool Slime::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 13, bodyType::DYNAMIC);
	pbody->ctype = ColliderType::ENEMY;

	slime.LoadAnimations("slime");
	slime_dead.LoadAnimations("slime_dead");
	slime_attack.LoadAnimations("slime_attack");
	slime_walking.LoadAnimations("slime_walking");

	velocity = { 0,0 };

	currentAnimation = &slime;
	return true;
}

bool Slime::Update(float dt)
{
	currentAnimation = &slime;

	if (dist(app->scene->player->position, position) < app->map->mapData.tileWidth * tilesview)
	{
		onView = true;
		currentAnimation = &slime_walking;

		app->map->pathfindingSuelo->CreatePath(origPos, targPos);
		lastPath = *app->map->pathfindingSuelo->GetLastPath();


		if (dist(app->scene->player->position, position) < app->map->mapData.tileWidth * tilesattack)
		{
			if (!isAttacking)
			{
			isAttacking = true;
			}
		}

		if (app->scene->player->position.x - position.x < 0)
		{
			rightmode = false;
			// move left
			velocity.x = -1;

		}
		else
		{
			rightmode = true;
			//move right
			velocity.x = 1;
		
		}


	}


	if (isAttacking)
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
	//if (lastPath.Count()>0)
	//{
	//	iPoint* nextPathTile;
	//	nextPathTile = lastPath.At(lastPath.Count() - 1);
	//	LOG("LAST PATH X: %d enemy x: %d", nextPathTile->x, origPos.x);
	//	if (nextPathTile->x < origPos.x)
	//	{
	//		rightmode = false;
	//		
	//	}
	//	else
	//	{
	//		rightmode = true;

	//	}
	//	if (nextPathTile->x == origPos.x) {
	//		/*lastPath.Pop(*nextPathTile);*/
	//	}


	//}
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
	pbody->body->SetLinearVelocity(velocity);


	currentAnimation->Update();
	if (leftmode && !rightmode)
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