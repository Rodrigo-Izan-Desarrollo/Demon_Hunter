#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	player_1.PushBack({ 0,2,32,32 });
	player_1.PushBack({ 31,2,32,32 });
	player_1.loop = true;
	player_1.speed = 0.075f;

	player_2.PushBack({ 487,2,32,32 });
	player_2.PushBack({ 455,2,32,32 });
	player_2.loop = true;
	player_2.speed = 0.075f;
	//Walking
	player_walk_1.PushBack({ 0, 67, 32, 32 });
	player_walk_1.PushBack({ 31, 67, 32, 32 });
	player_walk_1.PushBack({ 63, 67, 32, 32 });
	player_walk_1.PushBack({ 95, 67, 32, 32 });
	player_walk_1.loop = true;
	player_walk_1.speed = 0.1f;

	player_walk_2.PushBack({ 487, 67, 32, 32 });
	player_walk_2.PushBack({ 455, 67, 32, 32 });
	player_walk_2.PushBack({ 423, 67, 32, 32 });
	player_walk_2.PushBack({ 391, 67, 32, 32 });
	player_walk_2.loop = true;
	player_walk_2.speed = 0.1f;
	//Runing
	player_speed_1.PushBack({ 0 ,100, 32, 32 });
	player_speed_1.PushBack({ 31 ,100, 32, 32 });
	player_speed_1.PushBack({ 63 ,100, 32, 32 });
	player_speed_1.PushBack({ 95 ,100, 32, 32 });
	player_speed_1.PushBack({ 126 ,100, 32, 32 });
	player_speed_1.PushBack({ 159 ,100, 32, 32 });
	player_speed_1.PushBack({ 191 ,100, 32, 32 });
	player_speed_1.PushBack({ 222 ,100, 32, 32 });
	player_speed_1.loop = true;
	player_speed_1.speed = 0.15f;

	player_speed_2.PushBack({ 487 ,100, 32, 32 });
	player_speed_2.PushBack({ 455 ,100, 32, 32 });
	player_speed_2.PushBack({ 423 ,100, 32, 32 });
	player_speed_2.PushBack({ 391 ,100, 32, 32 });
	player_speed_2.PushBack({ 359 ,100, 32, 32 });
	player_speed_2.PushBack({ 327 ,100, 32, 32 });
	player_speed_2.PushBack({ 295 ,100, 32, 32 });
	player_speed_2.PushBack({ 263 ,100, 32, 32 });
	player_speed_2.loop = true;
	player_speed_2.speed = 0.15f;
	//Sit
	player_down_1.PushBack({ 0, 130, 32, 32 });
	player_down_1.PushBack({ 31, 130, 32, 32 });
	player_down_1.PushBack({ 63, 130, 32, 32 });
	player_down_1.PushBack({ 126, 130, 32, 32 });
	player_down_1.speed = 0.1f;
	player_down_1.loop = false;

	player_down_2.PushBack({ 487, 129, 32, 32 });
	player_down_2.PushBack({ 455, 129, 32, 32 });
	player_down_2.PushBack({ 423, 129, 32, 32 });
	player_down_2.PushBack({ 359, 129, 32, 32 });
	player_down_2.speed = 0.1f;
	player_down_2.loop = false;
	//Jumping
	player_jump_2.PushBack({ 487, 163, 32, 32 });
	player_jump_2.PushBack({ 455, 163, 32, 32 });
	player_jump_2.PushBack({ 423, 163, 32, 32 });
	player_jump_2.PushBack({ 391, 163, 32, 32 });
	player_jump_2.PushBack({ 359, 163, 32, 32 });
	player_jump_2.PushBack({ 327, 163, 32, 32 });
	player_jump_2.PushBack({ 295, 163, 32, 32 });
	player_jump_2.PushBack({ 263, 163, 32, 32 });

	player_jump_1.PushBack({ 0, 163, 32, 32 });
	player_jump_1.PushBack({ 31, 163, 32, 32 });
	player_jump_1.PushBack({ 63, 163, 32, 32 });
	player_jump_1.PushBack({ 95, 163, 32, 32 });
	player_jump_1.PushBack({ 126, 163, 32, 32 });
	player_jump_1.PushBack({ 159, 163, 32, 32 });
	player_jump_1.PushBack({ 191, 163, 32, 32 });
	player_jump_1.PushBack({ 222, 163, 32, 32 });

	//Normal atack
	player_attack_1.PushBack({ 0,259, 32, 32 });
	player_attack_1.PushBack({ 31,259, 32, 32 });
	player_attack_1.PushBack({ 63,259, 32, 32 });
	player_attack_1.PushBack({ 95,259, 32, 32 });
	player_attack_1.PushBack({ 126,259, 32, 32 });
	player_attack_1.PushBack({ 159,259, 32, 32 });
	player_attack_1.PushBack({ 191,259, 32, 32 });
	player_attack_1.PushBack({ 224,259, 32, 32 });
	player_attack_1.PushBack({ 0,2,32,32 });
	player_attack_1.loop = false;
	player_attack_1.speed = 0.275f;

	player_attack_2.PushBack({ 487,259, 32, 32 });
	player_attack_2.PushBack({ 455,259, 32, 32 });
	player_attack_2.PushBack({ 423,259, 32, 32 });
	player_attack_2.PushBack({ 391,259, 32, 32 });
	player_attack_2.PushBack({ 359,259, 32, 32 });
	player_attack_2.PushBack({ 327,259, 32, 32 });
	player_attack_2.PushBack({ 291,259, 32, 32 });
	player_attack_2.PushBack({ 263,259, 32, 32 });
	player_attack_2.PushBack({ 487,2,32,32 });
	player_attack_2.loop = false;
	player_attack_2.speed = 0.275f;
	//Invisible
	player_no_1.PushBack({ 0, 195, 32, 32 });
	player_no_1.PushBack({ 31, 195, 32, 32 });
	player_no_1.PushBack({ 63, 195, 32, 32 });
	player_no_1.PushBack({ 95, 195, 32, 32 });
	player_no_1.speed = 0.1f;
	player_no_1.loop = false;

	player_no_2.PushBack({ 487, 195, 32, 32 });
	player_no_2.PushBack({ 455, 195, 32, 32 });
	player_no_2.PushBack({ 423, 195, 32, 32 });
	player_no_2.PushBack({ 391, 195, 32, 32 });
	player_no_2.speed = 0.1f;
	player_no_2.loop = false;
	//Damage resive
	player_hurt_1.PushBack({ 0,227, 32, 32 });
	player_hurt_1.PushBack({ 31,227, 32, 32 });
	player_hurt_1.PushBack({ 31,227, 32, 32 });
	player_hurt_1.speed = 10.0f;
	player_hurt_1.loop = false;

	player_hurt_2.PushBack({ 487,227, 32, 32 });
	player_hurt_2.PushBack({ 455,227, 32, 32 });
	player_hurt_2.speed = 10.0f;
	player_hurt_2.loop = false;
	//Player dead
	player_dead_1.PushBack({ 0,227, 32, 32 });
	player_dead_1.PushBack({ 31,227, 32, 32 });
	player_dead_1.PushBack({ 31,227, 32, 32 });
	player_dead_1.PushBack({ 63, 227, 32, 32 });
	player_dead_1.PushBack({ 95, 227, 32, 32 });
	player_dead_1.PushBack({ 126, 227, 32, 32 });
	player_dead_1.PushBack({ 159, 227, 32, 32 });
	player_dead_1.speed = 0.25f;
	player_dead_1.loop = false;

	player_dead_2.PushBack({ 487, 227, 32, 32 });
	player_dead_2.PushBack({ 455, 227, 32, 32 });
	player_dead_2.PushBack({ 423, 227, 32, 32 });
	player_dead_2.PushBack({ 391, 227, 32, 32 });
	player_dead_2.PushBack({ 359, 227, 32, 32 });
	player_dead_2.PushBack({ 327, 227, 32, 32 });
	player_dead_2.PushBack({ 291, 227, 32, 32 });
	player_dead_2.speed = 0.25f;
	player_dead_2.loop = false;

	currentAnimation = &player_1;

	return true;
}

bool Player::Update(float dt)
{
	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		leftmode = true;
		rightmode = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		rightmode = true;
		leftmode = false;
	}

	if (rightmode == true)
	{
		if (atacking == false && dead == false && leftmode == false)
		{
			currentAnimation = &player_1;
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			position.x += (int)(speed * dt);
			currentAnimation = &player_walk_1;
			if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			{
				currentAnimation = &player_speed_1;
				speed = 0.3f;
			}
			else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			{
				speed = 0.1f;
			}
			else
			{
				speed = 0.2f;
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			currentAnimation = &player_down_1;
		}

		if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)
		{
			atacking = true;
			if (atacking)
			{
				currentAnimation = &player_attack_1;
				if (currentAnimation)
				{
					currentAnimation->Reset();
				}
			}
		}

		//if (app->input->GetKey(SDL_SCANCODE_1) == KEY_REPEAT)
		//{
		//	currentAnimation = &player_hurt_1;
		//	position.x -= 7;
		//}

		//if (app->input->GetKey(SDL_SCANCODE_2) == KEY_REPEAT)
		//{
		//	dead = true;
		//	if (dead==true)
		//	{
		//		currentAnimation = &player_dead_1;
		//		currentAnimation->Reset();
		//	}
		//}
	}
	if (leftmode == true)
	{
		if (atacking == false && dead == false && rightmode == false)
		{
			currentAnimation = &player_2;
		}

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			position.x -= (int)(speed * dt);
			currentAnimation = &player_walk_2;
			if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			{
				currentAnimation = &player_speed_2;
				speed = 0.3f;
			}
			else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			{
				speed = 0.1f;
			}
			else
			{
				speed = 0.2f;
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)
		{
			atacking = true;
			if (atacking == true)
			{
				currentAnimation = &player_attack_2;
				currentAnimation->Reset();
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			currentAnimation = &player_down_2;
		}

		//	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_REPEAT)
		//	{
		//		currentAnimation = &player_hurt_2;
		//		position.x += 7;
		//	}
		//	if (app->input->GetKey(SDL_SCANCODE_2) == KEY_REPEAT)
		//	{
		//		dead = true;
		//		if (dead == true)
		//		{
		//			currentAnimation = &player_dead_2;
		//			currentAnimation->Reset();
		//		}
		//	}
		//}
		//	
	}

	//Set the velocity of the pbody of the player
	pbody->body->SetLinearVelocity(vel);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x, position.y, &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();

	return true;
}

bool Player::CleanUp()
{

	return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}