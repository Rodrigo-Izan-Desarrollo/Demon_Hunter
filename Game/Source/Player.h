#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Timer.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:
	float speed = 0.2f;
	const char* texturePath;
	const char* texturePath_1;
	const char* texturePath_2;
	const char* texturePath_3;
	const char* texturePath_3_2;
	const char* texturePath_4;

	Uint32 tiempoInicial = NULL;
	Uint32 tiempoactual = NULL;
	Uint32 diferencia = NULL;

	SDL_Texture* texture = NULL;
	SDL_Texture* texture_1 = NULL;
	SDL_Texture* texture_2 = NULL;
	SDL_Texture* texture_3 = NULL;
	SDL_Texture* texture_3_2 = NULL;
	SDL_Texture* texture_4 = NULL;

	PhysBody* pbody;
	int pickCoinFxId;

	int respawn = 2;


	Animation* currentAnimation;

	Animation player;
	Animation player_sleep;
	Animation player_walk;

	Animation player_speed;

	Animation player_jump;
	Animation player_inair;

	Animation player_inv;

	Animation player_hurt;

	Animation player_dead;

	Animation player_attack;


	pugi::xml_node config;

	bool leftmode = false;
	bool rightmode = true;

	bool moving = false;

	bool sleeping = false;

	bool atacking = false;

	bool invisible = false;

	bool jumping = false;

	bool inground = false;

	bool Godmode=false;

	bool canmove = true;

	bool dead = false;

	bool check_1 = false;
	bool check_2 = false;

	bool powerup_1 = false;
	bool powerup_2 = false;
	bool powerup_3 = false;
	bool powerup_4 = false;

	uint32 startTime;
};

#endif // __PLAYER_H__