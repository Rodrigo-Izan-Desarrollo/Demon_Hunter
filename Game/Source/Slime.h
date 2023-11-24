#ifndef __SLIME_H__
#define __SLIME_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Timer.h"

struct SDL_Texture;

class Slime : public Entity
{
public:

	Slime();
	virtual ~Slime();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();


public:

	bool isPicked = false;

	Animation* currentAnimation;

	Animation enemy;
	
	Animation enemy_hurt;

	Animation enemy_dead;

	Animation enemy_attack;

	SDL_Texture* texture;

	const char* texturePath;

	PhysBody* pbody;
};

#endif // __SLIME_H__