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
	leftmodeslimevolador = parameters.attribute("leftmode").as_bool();
	rightmodeslimevolador = parameters.attribute("rightmode").as_bool();
	texturePath = parameters.attribute("texturepathenemy_3").as_string();
	musicpathslime = parameters.attribute("musicpathslime").as_string();

	return true;
}

bool SlimeVolador::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 13, bodyType::DYNAMIC);
	pbody->ctype = ColliderType::ENEMY;
	pbody->listener = this;

	slimevolador.LoadAnimations("slimevolador");
	slimevolador_attack.LoadAnimations("slimevolador_attack");
	slimevolador_dead.LoadAnimations("slimevolador_dead");
	muelto_Fx = app->audio->LoadFx(musicpathslime);

	velocity = { -0.5,-0.165 };
	


	currentAnimation = &slimevolador;
	return true;
}

bool SlimeVolador::Update(float dt)
{
	if (tp)
	{
		tp = false;
		pbody->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y) }, 0);
	}

	origPos = app->map->WorldToMap(position.x, position.y);
	targPos = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);

	if (!isAttacking)
	{
		if (reverse && leftmodeslimevolador && !onView)
			{
				leftmodeslimevolador = false;
				rightmodeslimevolador = true;
				downmodeslimevolador = false;
				reverse = false;
			}
			if (reverse && rightmodeslimevolador && !onView)
			{
				leftmodeslimevolador = true;
				rightmodeslimevolador = false;
				downmodeslimevolador = false;
				reverse = false;
			}
		
		LOG("LAST PATH X: %d enemy x: %d", targPos.x, origPos.x);

		if (dist(app->scene->player->position, position) < app->map->mapData.tileWidth * tilesview)
		{
			onView = true;
			currentAnimation = &slimevolador;

			playerDetectedPosition = app->scene->player->position;

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
	}


	
	

	if (isAttacking )
	{
		currentAnimation = &slimevolador_attack;
	
	
		if (app->scene->player->position.x < position.x && !downmodeslimevolador )
		{
			rightmodeslimevolador = false;
			leftmodeslimevolador = true;
			downmodeslimevolador = false;
			
			velocity.x = -2.0f;
				
		}
		else if ( app->scene->player->position.x > position.x && !downmodeslimevolador)
		{
			rightmodeslimevolador = true;
			leftmodeslimevolador = false;
			downmodeslimevolador = false;
			velocity.x = +2.0f;
		}
		if (app->scene->player->position.x >= position.x - 10 && app->scene->player->position.x <= position.x + 10)
		{
			rightmodeslimevolador = false;
			leftmodeslimevolador = false;
			downmodeslimevolador = true;
		}
		if (downmodeslimevolador)
		{
			velocity.x = 0.0f;
			velocity.y = 20.0f;
		}

	}

	if (currentAnimation == &slimevolador_attack && currentAnimation->HasFinished()) { // Reiniciar el ataque
		isAttacking = false;
		slimevolador_attack.Reset();
		currentAnimation->loopCount = 0;
	}


	LOG("COUNTTTTTTTTTTT: %d", lastPath.Count());

	if (lastPath.Count() > 0)
	{
		iPoint* nextPathTile;
		nextPathTile = lastPath.At(lastPath.Count() - 1);

		if (playerDetectedPosition.x != INT_MAX) {
			
			if (position.x == playerDetectedPosition.x) {
				
				if (position.y < playerDetectedPosition.y) {
					velocity.y = 5.0f; 
					velocity.x = 0.0f;
				}
				else if (position.y > playerDetectedPosition.y) {
					velocity.y = -1.0f; 
				}
				else {
					velocity.y = 0.0f; 
				}
				velocity.x = 0.0f;
			}
		}

		if (nextPathTile->x < origPos.x)
		{
			if (!isAttacking)
			{
				rightmodeslimevolador = false;
				leftmodeslimevolador = true;
			}
			velocity.x = -1;
		}
		else
		{
			if (!isAttacking)
			{
				rightmodeslimevolador = true;
				leftmodeslimevolador = false;
			}
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

	if (currentAnimation == &slimevolador_dead && currentAnimation->HasFinished()) { 
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
	if (rightmodeslimevolador)
	{
		LOG("ESTA ENTRANDO rigth");
		app->render->DrawTexture(texture, position.x-10, position.y - 10, &currentAnimation->GetCurrentFrame());
		
	}
	if (leftmodeslimevolador)
	{
		LOG("ESTA ENTRANDO left");
		app->render->DrawTexture(texture, position.x, position.y - 10 , &currentAnimation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
	}
	if (downmodeslimevolador)
	{
		if (app->scene->player->position.x < position.x  )
		{
			LOG("ESTA ENTRANDO down");
			app->render->DrawTexture(texture, position.x - 5, position.y - 10, &currentAnimation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL, 1.0f, 270.0);

		}
		else if (app->scene->player->position.x > position.x)
		{
			app->render->DrawTexture(texture, position.x - 5, position.y - 10, &currentAnimation->GetCurrentFrame(), SDL_FLIP_NONE, 1.0f, 90.0);
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
		death=true;
		break;
	case ColliderType::ENEMY:
		if (isAttacking)
		{
			death = true;
			app->audio->PlayFx(muelto_Fx);
		}
		if (!isAttacking)
		{
			reverse = true;
		}
		break;
	case ColliderType::PLAYER:
		death = true;
		app->audio->PlayFx(muelto_Fx);
		break;
	case ColliderType::WALLE2:
		LOG("PATOTURMO");
		reverse = true;

		break;
	case ColliderType::PATACK:
		death = true;
		app->audio->PlayFx(muelto_Fx);
		break;
	default:
		break;
	}
}