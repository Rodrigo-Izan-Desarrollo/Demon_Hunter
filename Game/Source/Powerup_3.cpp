#include "Powerup_3.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Powerup_3::Powerup_3() : Entity(EntityType::POWERUP_3)
{
	name.Create("Powerup");
}

Powerup_3::~Powerup_3() {}

bool Powerup_3::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepathpower_3").as_string();

	return true;
}

bool Powerup_3::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateCircle(position.x+16, position.y, 13, bodyType::STATIC);
	pbody->ctype = ColliderType::POWERUP_3;
	pbody->listener = this;


	return true;
}

bool Powerup_3::Update(float dt)
{ 
	if (isPicked)
	{
		SDL_DestroyTexture(texture);
		pbody->body->SetActive(false);
		app->entityManager->DestroyEntity(this);
		app->physics->world->DestroyBody(pbody->body);
	}
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool Powerup_3::CleanUp()
{
	return true;
}

void Powerup_3::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		LOG("Collision ITEM");
		isPicked = true;
		break;
	}

}
