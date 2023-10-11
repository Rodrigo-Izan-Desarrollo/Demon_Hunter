#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	//L03: DONE 2: Initialize Player parameters
	position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());
	player_1.PushBack({0,2,32,32});
	player_1.PushBack({ 31,2,32,32 });
	player_1.speed = 0.5f;

	player_speed_1.PushBack({0 ,100, 32, 32});
	player_speed_1.PushBack({ 31 ,100, 32, 32 });
	player_speed_1.PushBack({ 63 ,100, 32, 32 });
	player_speed_1.PushBack({ 95 ,100, 32, 32 });
	player_speed_1.PushBack({ 126 ,100, 32, 32 });
	player_speed_1.PushBack({ 159 ,100, 32, 32 });
	player_speed_1.PushBack({ 191 ,100, 32, 32 });
	player_speed_1.PushBack({ 222 ,100, 32, 32 });

	player_walk_1.PushBack({0, 67, 32, 32});
	player_walk_1.PushBack({ 31, 67, 32, 32 });
	player_walk_1.PushBack({ 63, 67, 32, 32 });
	player_walk_1.PushBack({ 95, 67, 32, 32 });

	player_down_1.PushBack({0, 131, 32, 32});
	player_down_1.PushBack({ 31, 131, 32, 32 });
	player_down_1.PushBack({ 63, 131, 32, 32 });
	player_down_1.PushBack({ 126, 131, 32, 32 });

	player_jump_1.PushBack({0, 163, 32, 32});
	player_jump_1.PushBack({ 31, 163, 32, 32 });
	player_jump_1.PushBack({ 63, 163, 32, 32 });
	player_jump_1.PushBack({ 95, 163, 32, 32 });
	player_jump_1.PushBack({ 126, 163, 32, 32 });
	player_jump_1.PushBack({ 159, 163, 32, 32 });
	player_jump_1.PushBack({ 191, 163, 32, 32 });
	player_jump_1.PushBack({ 222, 163, 32, 32 });

	player_atack_1.PushBack({0,259, 32, 32});
	player_atack_1.PushBack({ 31,259, 32, 32 });
	player_atack_1.PushBack({ 63,259, 32, 32 });
	player_atack_1.PushBack({ 95,259, 32, 32 });
	player_atack_1.PushBack({ 126,259, 32, 32 });
	player_atack_1.PushBack({ 159,259, 32, 32 });
	player_atack_1.PushBack({ 191,259, 32, 32 });
	player_atack_1.PushBack({ 222,259, 32, 32 });

	player_hurt_1.PushBack({0,227, 32, 32});
	player_hurt_1.PushBack({ 31,227, 32, 32 });

	player_dead_1.PushBack({63, 227, 32, 32});
	player_dead_1.PushBack({ 95, 227, 32, 32 });
	player_dead_1.PushBack({ 126, 227, 32, 32 });
	player_dead_1.PushBack({ 159, 227, 32, 32 });

	player_no_1.PushBack({0, 195, 32, 32});
	player_no_1.PushBack({ 31, 195, 32, 32 });
	player_no_1.PushBack({ 63, 195, 32, 32 });
	player_no_1.PushBack({ 95, 195, 32, 32 });

	return true;
}

bool Player::Start() {

	texture = app->tex->Load(config.attribute("texturePath").as_string());
	currentAnimation= &player_1;
	
	return true;
}

bool Player::Update(float dt)
{
	//L03: DONE 4: render the player texture and modify the position of the player using WSAD keys and render the texture
	
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		position.y -= (int) (speed * dt);

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		position.y += (int)(speed * dt);

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		position.x -= (int)(speed * dt);

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		position.x += (int)(speed * dt);

	app->render->DrawTexture(texture,position.x,position.y,&player_1.GetCurrentFrame());

	return true;
}

bool Player::CleanUp()
{

	return true;
}