#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

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
	SDL_Texture* texture = NULL;
	PhysBody* pbody;
	int pickCoinFxId;

	Animation* currentAnimation;

	Animation player_1;
	Animation player_2;

	Animation player_walk_1;
	Animation player_walk_2;

	Animation player_speed_1;
	Animation player_speed_2;

	Animation player_down_1;
	Animation player_down_2;

	Animation player_jumphigh_1;
	Animation player_jumphigh_2;

	Animation player_jumpdown_1;
	Animation player_jumpdown_2;

	Animation player_no_1;
	Animation player_no_2;

	Animation player_hurt_1;
	Animation player_hurt_2;

	Animation player_dead_1;
	Animation player_dead_2;

	Animation player_attack_1;
	Animation player_attack_2;

	pugi::xml_node config;

	bool leftmode = false;
	bool rightmode = false;

	bool atacking = false;
	bool dead = false;
	bool down = false;
	bool jumping = false;

};

#endif // __PLAYER_H__