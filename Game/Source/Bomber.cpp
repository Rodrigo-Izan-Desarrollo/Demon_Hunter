#include "Bomber.h"
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


SlimeVolador::SlimeVolador() : Entity(EntityType::SLIMEVOLADOR)
{
	name.Create("SlimeVolador");
}

SlimeVolador::~SlimeVolador() {}

bool SlimeVolador::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepathenemy_3").as_string();
	//musicpathslime = parameters.attribute("musicpathslime").as_string();
	return true;
}

bool SlimeVolador::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 13, bodyType::DYNAMIC);
	pbody->ctype = ColliderType::ENEMY;
	pbody->listener = this;

	slimevolador.LoadAnimations("slimevolador");
	slimevolador_dead.LoadAnimations("slimevoladorr_dead");

	velocity = { -0.5,-0.165 };

	currentAnimation = &slimevolador;
	return true;
}

bool SlimeVolador::Update(float dt)
{


	if (reverse && leftmodeslimevolador && !onView)
	{
		leftmodeslimevolador = false;
		rightmodeslimevolador = true;
		reverse = false;
	}
	if (reverse && rightmodeslimevolador && !onView)
	{
		leftmodeslimevolador = true;
		rightmodeslimevolador = false;
		reverse = false;
	}


	origPos = app->map->WorldToMap(position.x, position.y);
	targPos = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);

	LOG("LAST PATH X: %d enemy x: %d", targPos.x, origPos.x);

	if (dist(app->scene->player->position, position) < app->map->mapData.tileWidth * tilesview)
	{
		onView = true;
		currentAnimation = &slimevolador;

		app->map->pathfindingVuelo->CreatePath(origPos, targPos);
		lastPath = *app->map->pathfindingVuelo->GetLastPath();


		if (dist(app->scene->player->position, position) < app->map->mapData.tileWidth * tilesattack)
		{
			if (!isAttacking)
			{
				isAttacking = true;
			}
		}


	}
	else {
		onView = false; // Asegurarse de que onView sea falso cuando el jugador no está a la vista

		if (rightmodeslimevolador)
		{
			velocity.x = 1.0f;
		}
		if (leftmodeslimevolador)
		{
			velocity.x = -1.0f;
		}
	}

	if (isAttacking && !damage)
	{
		currentAnimation = &slimevolador_attack;
		if (rightmodeslimevolador)
		{
			velocity.x = 0.0f;
			velocity.y = -1.0f;
		}
		if (leftmodeslimevolador)
		{
			velocity.x = 0.0f;
			velocity.y = -1.0f;
		}

	}

	if (currentAnimation == &slimevolador_attack && currentAnimation->HasFinished()) { // Reiniciar el ataque
		isAttacking = false;
		slimevolador_attack.Reset();
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
			rightmodeslimevolador = false;
			leftmodeslimevolador = true;
			velocity.x = -1;
		}
		else
		{
			rightmodeslimevolador = true;
			leftmodeslimevolador = false;
			velocity.x = +1;
		}
		if (nextPathTile->x == origPos.x) {
			lastPath.Pop(*nextPathTile);
		}


	}

	if (death)
	{
		currentAnimation = &slimevolador_dead;
	}

	if (currentAnimation == &slimevolador_dead && currentAnimation->HasFinished()) { // Reiniciar el salto
		pbody->body->SetActive(false);
		app->entityManager->DestroyEntity(this);
		app->physics->world->DestroyBody(pbody->body);
	}
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
	pbody->body->SetLinearVelocity(velocity);



	currentAnimation->Update();
	if (rightmodeslimevolador)
	{
		app->render->DrawTexture(texture, position.x, position.y + 7, &currentAnimation->GetCurrentFrame());
		if (isAttacking)
		{
			app->render->DrawTexture(texture, position.x, position.y + 7, &currentAnimation->GetCurrentFrame(), SDL_FLIP_NONE, NULL, 90);
		}
	}
	if (leftmodeslimevolador)
	{
		app->render->DrawTexture(texture, position.x, position.y + 7, &currentAnimation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
		if (isAttacking)
		{
			app->render->DrawTexture(texture, position.x, position.y + 7, &currentAnimation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL, NULL, 90);
		}
	}

	


	return true;
}

bool SlimeVolador::CleanUp()
{
	return true;
}

void SlimeVolador::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype) {

	case ColliderType::PLATFORM:
		break;
	case ColliderType::WALLE2:
		LOG("PATOTURMO");
		reverse = true;

		break;
	case ColliderType::PATACK:
		death = true;
		break;
	default:
		break;
	}
}