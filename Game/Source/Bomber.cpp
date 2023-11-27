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

Bomber::Bomber() : Entity(EntityType::BOMBER)
{
	name.Create("Bomber");
}

Bomber::~Bomber() {}

bool Bomber::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePathBomber = parameters.attribute("texturepathenemy").as_string();

	return true;
}

bool Bomber::Start() {

	//initilize textures
	texture = app->tex->Load(texturePathBomber);
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16  , 20, bodyType::DYNAMIC);
	pbody->ctype = ColliderType::ENEMY;

	bomber.LoadAnimations("bomber");
	bomber_dead.LoadAnimations("bomber_dead");
	bomber_attack.LoadAnimations("bomber_attack");

	currentAnimation = &bomber;
	return true;
}

bool Bomber::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	currentAnimation->Update();
	app->render->DrawTexture(texture, position.x-14, position.y-14, &currentAnimation->GetCurrentFrame());

	return true;
}

bool Bomber::CleanUp()
{
	return true;
}