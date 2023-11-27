#ifndef __POWERUP2_H__
#define __POWERUP2_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Timer.h"

struct SDL_Texture;

class Powerup_2 : public Entity
{
public:

	Powerup_2();
	virtual ~Powerup_2();

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