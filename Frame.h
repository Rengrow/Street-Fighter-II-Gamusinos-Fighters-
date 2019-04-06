#ifndef __FRAME_H__
#define __FRAME_H__

#include "SDL/include/SDL_rect.h"
#define MAX_FRAMES 25

class Frame
{
public:
	float speed = 0.15f;
	SDL_Rect frame;

private:
	float current_frame;
	int last_frame = 0;
	int loops = 0;

public:

	SDL_Rect& GetCurrentFrame()
	{
		current_frame += speed;
		if (current_frame >= last_frame)
		{
			current_frame = last_frame - 1;
			loops++;
		}

		return frame;
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Reset()
	{
		current_frame = 0;
	}
};

#endif