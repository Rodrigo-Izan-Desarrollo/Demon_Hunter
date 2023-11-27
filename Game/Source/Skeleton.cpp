#include "Skeleton.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Skeleton::Skeleton() : Entity(EntityType::SKELETON)
{
	name.Create("Skeleton");
}

Skeleton::~Skeleton() {}

bool Skeleton::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePathSkeleton = parameters.attribute("texturepathenemy_2").as_string();

	return true;
}

bool Skeleton::Start() {

	//initilize textures
	texture = app->tex->Load(texturePathSkeleton);
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16  , 16, bodyType::DYNAMIC);
	pbody->ctype = ColliderType::ENEMY;

	skeleton.LoadAnimations("skeleton");
	skeleton_dead.LoadAnimations("skeleton_dead");
	skeleton_attack.LoadAnimations("skeleton_attack");

	currentAnimation = &skeleton;
	return true;
}

bool Skeleton::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	currentAnimation->Update();
	app->render->DrawTexture(texture, position.x, position.y-3, &currentAnimation->GetCurrentFrame());

	return true;
}

bool Skeleton::CleanUp()
{
	return true;
}