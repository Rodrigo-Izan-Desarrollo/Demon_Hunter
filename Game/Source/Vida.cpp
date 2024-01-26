#include "Vida.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Vida::Vida() : Entity(EntityType::VIDA)
{
	name.Create("Vida");
}

Vida::~Vida() {}

bool Vida::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturevida").as_string();

	return true;
}

bool Vida::Start() {

	//Initilize textures
	texture = app->tex->Load(texturePath);

	//Create pbody
	pbody = app->physics->CreateCircle(position.x+16, position.y, 13, bodyType::STATIC);
	pbody->ctype = ColliderType::VIDA;
	pbody->listener = this;

	//Animation
	idle.PushBack({ 0, 0, 30, 32 });
	idle.PushBack({ 30, 0, 30, 32 });
	idle.PushBack({ 60, 0, 30, 32 });
	idle.loop = true;
	idle.speed = 0.1f;

	currentAnimation = &idle;

	return true;
}

bool Vida::Update(float dt)
{ 
	currentAnimation->Update();

	//When collision with player destroy the itslef
	if (isPicked)
	{
		SDL_DestroyTexture(texture);
		pbody->body->SetActive(false);
		app->entityManager->DestroyEntity(this);
		app->physics->world->DestroyBody(pbody->body);
	}

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x, position.y+13, &currentAnimation->GetCurrentFrame());

	return true;
}

bool Vida::CleanUp()
{
	return true;
}

void Vida::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		LOG("Collision ITEM");
		isPicked = true;
		break;
	}

}
