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
	//Player speed
	float speed = 0.2f;

	//Repawns
	int lifes = 2;

	//Textures paths
	const char* texturePath;
	const char* texturePath_1;
	const char* texturePath_2;
	const char* texturePath_3;
	const char* texturePath_3_2;
	const char* texturePath_4;
	const char* texturePath_4_2;
	const char* texturePathcheck;

	//Sound efects path
	const char* musicpathatack;
	const char* musicpathjump;
	const char* musicpathpickup;
	const char* musicpathpowerup;

	//Timers
	Uint32 invtempo = NULL;
	Uint32 invtempo_2 = NULL;
	Uint32 deadtempo = NULL;
	Uint32 powertempo = NULL;
	Uint32 atacktempo = NULL;
	Uint32 atacktempo_2 = NULL;
	Uint32 dashtempo = NULL;

	//Textures
	SDL_Texture* currentTexture = nullptr;

	SDL_Texture* texture = NULL;
	SDL_Texture* texture_1 = NULL;
	SDL_Texture* texture_2 = NULL;
	SDL_Texture* texture_3 = NULL;
	SDL_Texture* texture_3_2 = NULL;
	SDL_Texture* texture_4 = NULL;
	SDL_Texture* texture_4_2 = NULL;
	SDL_Texture* texturecheck = NULL;

	//Sound efects
	uint atack_Fx;
	uint jump_Fx;
	uint pick_up_Fx;
	uint powerup_Fx;

	//Pbodys
	PhysBody* pbody;
	PhysBody* pbodyatack = nullptr;
	PhysBody* pbodyatack_2 = nullptr;

	//Animations
	Animation* currentAnimation;

		//Idle
	Animation player;
	Animation player_sleep;

		//Movement
	Animation player_speed;
	Animation player_jump;
	Animation player_inair;

		//Dead
	Animation player_dead;

		//Atack
	Animation player_attack;
	Animation player_attack_2;
	Animation player_dash;

	//Checkpoint
	Animation checkpoint;

	//Ativar modes
	
		//Movement
	bool leftmode = false;
	bool rightmode = true;

		//Idle
	bool sleeping = false;

		//Atack
	bool atacking = false;
	bool atacking_2 = false;

	bool canatack = true;	
	bool canatack_2 = true;

		//Dash
	bool dashing = false;
	bool candash = true;

		//Get invisible
	bool invisible = false;
	bool caninv = true;

		//Jump
	bool jumping = false;

		//Is inground
	bool inground = false;

		//Godmode activation
	bool Godmode=false;

		//Player canmove
	bool canmove = true;

		//Player is dead
	bool dead = false;
	bool respawning = false;

		//Check points
	bool check_1 = false;
	bool check_2 = false;
	bool check_3 = false;

	bool portal = false;
		
		//Power-ups
	bool powerup_1 = false;
	bool powerup_2 = false;
	bool powerup_3 = false;
	bool powerup_4 = false;

		//Habilitar powerup
	bool canpower_1 = false;
	bool canpower_2 = false;
	bool canpower_3 = false;
	
		// Bool to change powerups
	bool canchange = true; 


	pugi::xml_node config;

};

#endif // __PLAYER_H__