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

Slime::Slime() : Entity(EntityType::SLIME)
{
	name.Create("Enemy");
}

Slime::~Slime() {}

bool Slime::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepathenemy").as_string();

	return true;
}

bool Slime::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 13, bodyType::DYNAMIC);
	pbody->ctype = ColliderType::ENEMY;

	enemy.LoadAnimations("enemy");
	enemy_dead.LoadAnimations("enemy_dead");
	enemy_attack.LoadAnimations("enemy_attack");

	currentAnimation = &enemy;
	return true;
}

bool Slime::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	currentAnimation->Update();
	app->render->DrawTexture(texture, position.x, position.y+17, &currentAnimation->GetCurrentFrame());

	return true;
}

bool Slime::CleanUp()
{
	return true;
}