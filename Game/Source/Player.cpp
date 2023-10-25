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

	player.PushBack({ 0,2,32,32 });
	player.PushBack({ 31,2,32,32 });
	player.loop = true;
	player.speed = 0.075f;

	//Walking
	player_walk.PushBack({ 0, 67, 32, 32 });
	player_walk.PushBack({ 31, 67, 32, 32 });
	player_walk.PushBack({ 63, 67, 32, 32 });
	player_walk.PushBack({ 95, 67, 32, 32 });
	player_walk.loop = true;
	player_walk.speed = 0.1f;

	//Runing
	player_speed.PushBack({ 0 ,100, 32, 32 });
	player_speed.PushBack({ 31 ,100, 32, 32 });
	player_speed.PushBack({ 63 ,100, 32, 32 });
	player_speed.PushBack({ 95 ,100, 32, 32 });
	player_speed.PushBack({ 126 ,100, 32, 32 });
	player_speed.PushBack({ 159 ,100, 32, 32 });
	player_speed.PushBack({ 191 ,100, 32, 32 });
	player_speed.PushBack({ 222 ,100, 32, 32 });
	player_speed.loop = true;
	player_speed.speed = 0.15f;

	//Jumping
	player_jump.PushBack({ 31, 162, 32, 32 });
	player_jump.PushBack({ 63, 161, 32, 32 });
	player_jump.PushBack({ 63, 161, 32, 32 });
	player_jump.PushBack({ 95, 161, 32, 32 });
	player_jump.PushBack({ 95, 161, 32, 32 });
	player_jump.PushBack({ 126, 162, 32, 32 });
	player_jump.PushBack({ 126, 162, 32, 32 });
	player_jump.speed= 0.2f;
	player_jump.loop = false;

	player_inair.PushBack({ 95, 161, 32, 32 });
	player_inair.PushBack({ 95, 161, 32, 32 });
	player_inair.PushBack({ 126, 162, 32, 32 });
	player_inair.PushBack({ 126, 162, 32, 32 });
	player_inair.loop = false;

	//Normal atack
	player_attack.PushBack({ 0,259, 32, 32 });
	player_attack.PushBack({ 31,259, 32, 32 });
	player_attack.PushBack({ 63,259, 32, 32 });
	player_attack.PushBack({ 95,259, 32, 32 });
	player_attack.PushBack({ 126,259, 32, 32 });
	player_attack.PushBack({ 159,259, 32, 32 });
	player_attack.PushBack({ 191,259, 32, 32 });
	player_attack.loop = false;
	player_attack.speed = 0.3f;

	//Invisible
	player_inv.PushBack({ 31, 195, 32, 32 });
	player_inv.PushBack({ 63, 195, 32, 32 });
	player_inv.PushBack({ 95, 195, 32, 32 });
	player_inv.PushBack({ 1000, 100, 32, 32 });
	player_inv.PushBack({ 95, 193, 32, 32 });
	player_inv.PushBack({ 95, 193, 32, 32 });
	player_inv.PushBack({ 95, 193, 32, 32 });
	player_inv.PushBack({ 127, 128, 32, 32 });
	player_inv.speed = 0.2f;
	player_inv.loop = false;

	//Damage resive
	player_hurt.PushBack({ 0,227, 32, 32 });
	player_hurt.PushBack({ 31,227, 32, 32 });
	player_hurt.PushBack({ 31,227, 32, 32 });
	player_hurt.speed = 10.0f;
	player_hurt.loop = false;

	//Player dead
	player_dead.PushBack({ 0,227, 32, 32 });
	player_dead.PushBack({ 31,227, 32, 32 });
	player_dead.PushBack({ 31,227, 32, 32 });
	player_dead.PushBack({ 63, 227, 32, 32 });
	player_dead.PushBack({ 95, 227, 32, 32 });
	player_dead.PushBack({ 126, 227, 32, 32 });
	player_dead.PushBack({ 159, 227, 32, 32 });
	player_dead.speed = 0.18f;
	player_dead.loop = false;

	return true;
}

bool Player::Update(float dt)
{
	b2Vec2 veljump = pbody->body->GetLinearVelocity();

	if (!atacking && !jumping && inground && !dead)
	{
		currentAnimation = &player;
	}

	//Debug inputs
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		Godmode = !Godmode;
	}

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		pbody->body->SetTransform({ PIXEL_TO_METERS(-700 + 16), PIXEL_TO_METERS(700) }, 0);
	}

	//Movement inputs
	if (app->input->GetKey(SDL_SCANCODE_A)==KEY_IDLE && app->input->GetKey(SDL_SCANCODE_D)==KEY_IDLE)
	{
		veljump.x = 0;
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !dead)
	{
		leftmode = true;
		rightmode = false;
		speed = -speed;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !dead)
	{
		rightmode = true;
		leftmode = false;
	}

	if (Godmode)
	{
		veljump = b2Vec2(0.0, -0.1675);
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			veljump.y = -0.3 * dt;
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			veljump.y = 0.3 * dt;
		}
	}

	if (canmove)
	{
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			veljump.x = speed * dt;
			if (inground && !jumping)
			{
				currentAnimation = &player_speed;
			}
			if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			{
				speed = 0.3f;
			}
			else
			{
				speed = 0.2f;
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			veljump.x = speed * dt;
			if (inground && !jumping)
			{
				currentAnimation = &player_speed;
			}
			if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			{
				speed = 0.3f;
			}
			else
			{
				speed = 0.2f;
			}
		}
	}
	if (dead && app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		pbody->body->SetTransform({ PIXEL_TO_METERS(-700 + 16), PIXEL_TO_METERS(700) }, 0);
		canmove = true;
		dead = false;
		rightmode = true;
		leftmode = false;
	}
	if (!jumping && inground)
	{
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !dead)
		{
			jumping = true;
			inground = false;
			veljump.y = -0.375 * dt;
			pbody->body->SetLinearVelocity(veljump);
			if (jumping)
			{
				currentAnimation = &player_jump;
				currentAnimation->loopCount = 0;
			}
		}
	}

	//Ability inputs
	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT && !dead)
	{
		atacking = true;
		if (atacking)
		{
			canmove = false;
			currentAnimation->Reset();
			currentAnimation = &player_attack;
			currentAnimation->loopCount = 0;
		}
	}

	//Finished animations

	if (dead)
	{
		canmove = false;
		currentAnimation = &player_dead;
		currentAnimation->loopCount = 0;
	} 


	if (currentAnimation == &player_attack && currentAnimation->HasFinished()) {
		atacking = false;
		canmove = true;
	}

	if (currentAnimation == &player_jump && currentAnimation->HasFinished() && inground) {
		currentAnimation->Reset();
		jumping = false;
	}

	//Set the velocity of the pbody of the player
	pbody->body->SetLinearVelocity(veljump);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	if (rightmode == true)
	{
		app->render->DrawTexture(texture, position.x, position.y, &currentAnimation->GetCurrentFrame());
	}
	if (leftmode == true)
	{
		app->render->DrawTexture(texture, position.x, position.y, &currentAnimation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
	}

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
		if (!Godmode)
		{
			dead = true;
		}
		app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		inground = true;
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}