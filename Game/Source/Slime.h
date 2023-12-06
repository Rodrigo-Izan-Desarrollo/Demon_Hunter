#ifndef __SLIME_H__
#define __SLIME_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Timer.h"
#include "DynArray.h"

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

	Animation slime;
	
	Animation slime_hurt;

	Animation slime_walking;

	Animation slime_dead;

	Animation slime_attack;

	SDL_Texture* texture;

	const char* texturePath;

	PhysBody* pbody;
	 
	int tilesview = 7;

	bool onView = false;

	int tilesattack = 3;

	bool isAttacking = false;

	bool leftmode = true;
	bool rightmode = false;

	const iPoint origPos;
	const iPoint targPos;
	DynArray<iPoint> lastPath;
	
};

#endif // __SLIME_H__