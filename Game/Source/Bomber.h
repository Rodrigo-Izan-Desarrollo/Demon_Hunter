#ifndef __BOMBER_H__
#define __BOMBER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Timer.h"
#include "DynArray.h"
#include "Physics.h"
struct SDL_Texture;

class SlimeVolador : public Entity
{
public:

	SlimeVolador();
	virtual ~SlimeVolador();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);


public:

	bool isPicked = false;

	Animation* currentAnimation;

	Animation slimevolador;

	Animation slimevolador_attack;

	Animation slimevolador_dead;

	SDL_Texture* texture;

	const char* texturePath;
	const char* musicpathslime;


	PhysBody* pbody;
	PhysBody* damage;

	int tilesview = 10;

	bool onView = false;

	int tilesattack =10;


	bool isAttacking = false;

	bool leftmodeslimevolador = true;
	bool rightmodeslimevolador = false;
	bool downmodeslimevolador = false;
	bool reverse = false;
	bool death = false;

	b2Vec2 velocity;
	iPoint origPos;
	iPoint targPos;
	DynArray<iPoint> lastPath;
	iPoint playerDetectedPosition;

	const char* texturePathBomber;

	uint muelto_Fx;

};

#endif // __SLIME_H__