#ifndef __SKELETON_H__
#define __SKELETON_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Timer.h"

struct SDL_Texture;

class Skeleton : public Entity
{
public:

	Skeleton();
	virtual ~Skeleton();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();


public:

	bool isPicked = false;

	Animation* currentAnimation;

	Animation skeleton;
	
	Animation skeleton_hurt;

	Animation skeleton_dead;

	Animation skeleton_attack;

	SDL_Texture* texture;

	const char* texturePathSkeleton;

	PhysBody* pbody;
};

#endif // __SLIME_H__