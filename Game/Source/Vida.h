#ifndef __VIDA_H__
#define __VIDA_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Timer.h"

struct SDL_Texture;

class Vida : public Entity
{
public:

	Vida();
	virtual ~Vida();

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

	const char* textureVida;

	PhysBody* pbody;
};

#endif // __SLIME_H__