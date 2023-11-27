#ifndef __POWERUP1_H__
#define __POWERUP1_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Timer.h"

struct SDL_Texture;

class Powerup_1 : public Entity
{
public:

	Powerup_1();
	virtual ~Powerup_1();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	bool isPicked = false;

	SDL_Texture* texture;

	const char* texturePath;

	PhysBody* pbody;
};

#endif // __SLIME_H__