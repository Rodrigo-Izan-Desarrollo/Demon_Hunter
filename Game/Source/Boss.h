#ifndef __BOSS_H__
#define __BOSS_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Timer.h"
#include "DynArray.h"
#include "Physics.h"

struct SDL_Texture;

class Boss : public Entity
{
public:

	Boss();
	virtual ~Boss();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);


public:

	bool isPicked = false;

	Animation* currentAnimation;

	Animation boss;

	Animation boss_hurt;

	Animation boss_walking;
	Animation boss_dead;

	Animation boss_attack;

	SDL_Texture* texture;
	SDL_Texture* pathdraw;

	const char* texturePath;
	const char* musicpathslime;

	PhysBody* pbody;
	PhysBody* pbodyatack = nullptr;

	PhysBody* damage;

	int lives = 7;

	int tilesview = 20;

	bool onView = false;

	int tilesattack = 3;

	bool isAttacking = false;
	bool isHurt = false;

	bool leftmode = true;
	bool rightmode = false;
	bool reverse = false;
	bool iskilled = false;
	bool canmove = true;
	bool death = false;
	bool paint = false;

	b2Vec2 velocity;

	iPoint origPos;
	iPoint targPos;
	DynArray<iPoint> lastPath;
	const DynArray<iPoint>* lastPath2;
	uint slime_Fx;

};

#endif // __BOSS_H__