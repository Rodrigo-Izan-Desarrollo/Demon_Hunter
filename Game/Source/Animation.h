#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#define MAX_FRAMES 25

class Animation
{
public:
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];
	bool loop = true;
	// Allows the animation to keep going back and forth
	bool pingpong = false;
	int loopCount = 0;

	float currentFrame = 0.0f;
	int totalFrames = 0;
	int pingpongDirection = 1;

public:

	void PushBack(const SDL_Rect& rect)
	{
		frames[totalFrames++] = rect;
	}

	void Reset()
	{
		currentFrame = 0;
	}
	
	bool HasFinished()
	{
		return !loop && !pingpong && loopCount > 0;
	}

	void Update()
	{
		currentFrame += speed;
		if (currentFrame >= totalFrames)
		{
			currentFrame = (loop || pingpong) ? 0.0f : totalFrames - 1;
			++loopCount;

			if (pingpong)
				pingpongDirection = -pingpongDirection;
		}
	}

	const SDL_Rect& GetCurrentFrame() const
	{
		int actualFrame = currentFrame;
		if (pingpongDirection == -1)
			actualFrame = totalFrames - currentFrame;

		return frames[actualFrame];
	}

	void LoadAnimations(const char* name)
	{
		pugi::xml_document file;
		pugi::xml_parse_result parseresult = file.load_file("config.xml");

		pugi::xml_node animation_name = file.child("config").child("animations").child(name);
		for (pugi::xml_node animation = animation_name.child("animation"); animation; animation = animation.next_sibling("animation"))
		{
			PushBack({ animation.attribute("x").as_int(),
		animation.attribute("y").as_int(),
		animation.attribute("w").as_int(),
		animation.attribute("h").as_int() });



		}
		loop = animation_name.attribute("loop").as_bool();
		speed = animation_name.attribute("speed").as_float();
	}
};

#endif