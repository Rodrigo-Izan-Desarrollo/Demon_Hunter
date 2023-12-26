#ifndef __BIGSLIME_H__
#define __BIGSLIME_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Timer.h"
#include "DynArray.h"
#include "Physics.h"

struct SDL_Texture;

class BigSlime : public Entity
{
public:

	BigSlime();
	virtual ~BigSlime();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);


public:

	bool isPicked = false;

	Animation* currentAnimation;

	Animation bigslime;

	Animation bigslime_hurt;

	Animation bigslime_walking;
	Animation bigslime_dead;

	Animation bigslime_attack;

	SDL_Texture* texture;
	SDL_Texture* pathdraw;

	const char* texturePath;
	const char* musicpathslime;

	PhysBody* pbody;
	PhysBody* damage;

	int lives = 3;

	int tilesview = 7;

	bool onView = false;

	int tilesattack = 3;

	bool isAttacking = false;
	bool isHurt = false;

	bool leftmode = true;
	bool rightmode = false;
	bool reverse = false;
	bool iskilled = false;
	bool death = false;
	bool paint = false;

	b2Vec2 velocity;

	iPoint origPos;
	iPoint targPos;
	DynArray<iPoint> lastPath;
	const DynArray<iPoint>* lastPath2;
	uint slime_Fx;

};

#endif // __SLIME_H__