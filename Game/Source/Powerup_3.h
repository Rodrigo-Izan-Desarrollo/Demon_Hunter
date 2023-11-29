#ifndef __POWERUP3_H__
#define __POWERUP3_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Timer.h"

struct SDL_Texture;

class Powerup_3 : public Entity
{
public:

	Powerup_3();
	virtual ~Powerup_3();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	Animation* currentAnimation;

	Animation idle;

	bool isPicked = false;

	SDL_Texture* texture;

	const char* texturePath;

	PhysBody* pbody;
};

#endif // __SLIME_H__