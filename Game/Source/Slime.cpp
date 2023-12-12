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
	/*damage = app->physics->CreateCircle(position.x + 20, position.y + 15, 6.5, bodyType::STATIC);*/
	pbody->ctype = ColliderType::ENEMY;
	//damage->ctype = ColliderType::ENEMY;

	slime.LoadAnimations("slime");
	slime_dead.LoadAnimations("slime_dead");
	slime_attack.LoadAnimations("slime_attack");
	slime_walking.LoadAnimations("slime_walking");

	//velocity = { 0,0 };

	currentAnimation = &slime;
	lastPoition = position;
	return true;
}

bool Slime::Update(float dt)
{
	currentAnimation = &slime;
	

	float timeSinceLastChange = 0.0f;
	float changeDirectionInterval = 2.0f; 

	origPos = app->map->WorldToMap(position.x, position.y);
	targPos = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);

	LOG("LAST PATH X: %d enemy x: %d", targPos.x, origPos.x);

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


	}
	else {
		onView = false; // Asegurarse de que onView sea falso cuando el jugador no est� a la vista

		timeSinceLastChange += dt; // dt es el tiempo transcurrido desde el �ltimo frame

		if (timeSinceLastChange >= changeDirectionInterval) {
			if(llegadaPosicion){
				randomDir = rand() % 5 -2; // Genera un n�mero aleatorio entre 0 y 1 para cambiar la direcci�n
				llegadaPosicion = false;
				lastPoition = position;
			}

			if (randomDir <= 0) {
				velocity.x = -1; // Mueve hacia la izquierda
				leftmode = true;
				rightmode = false;
				if (reverse	==true && leftmode == true) {
					llegadaPosicion = true;
				}
			}
			else {
				velocity.x = +1; // Mueve hacia la izquierda
				leftmode = false;
				rightmode = true;
				if (lastPoition.x - randomDir >= position.x) {
					llegadaPosicion = true;
				}
			}

			timeSinceLastChange = 0.0f; // Reinicia el temporizador
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

	LOG("COUNTTTTTTTTTTT: %d", lastPath.Count());

	if (lastPath.Count() > 0)
	{
		iPoint* nextPathTile;
		nextPathTile = lastPath.At(lastPath.Count() - 1);


		if (nextPathTile->x < origPos.x)
		{
			rightmode = false;
			velocity.x = -1;
		}
		else
		{
			rightmode = true;
			velocity.x = +1;
		}
		if (nextPathTile->x == origPos.x) {
			lastPath.Pop(*nextPathTile);
		}


	}

	if (death)
	{
		currentAnimation = &slime_dead;
		currentAnimation->loopCount = 0;

	}
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

void Slime::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype) {
	
	case ColliderType::PLATFORM:
		break;
	case ColliderType::WALL:
		reverse=true;
		break;
	case ColliderType::PATACK:
		death = true;
		break;
	default:
		break;
	}
}