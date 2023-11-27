#ifndef __BOMBER_H__
#define __BOMBER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Timer.h"

struct SDL_Texture;

class Bomber : public Entity
{
public:

	Bomber();
	virtual ~Bomber();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();


public:

	bool isPicked = false;

	Animation* currentAnimation;

	Animation bomber;
	
	Animation bomber_hurt;

	Animation bomber_dead;

	Animation bomber_attack;

	SDL_Texture* texture;

	const char* texturePathBomber;

	PhysBody* pbody;
};

#endif // __SLIME_H__