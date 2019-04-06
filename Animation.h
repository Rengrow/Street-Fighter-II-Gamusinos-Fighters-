#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#include "Frame.h"
#define MAX_FRAMES 25

class Animation
{
public:
	bool loop = true;
	Frame frames[MAX_FRAMES];

private:
	float current_frame;
	int last_frame = 0;
	int loops = 0;

public:

	void PushBack(const SDL_Rect& rect, const float speed = 0.15f)
	{
		frames[last_frame].frame = rect;
		frames[last_frame].speed = speed;
		last_frame++;
	}

	SDL_Rect& GetCurrentFrame()
	{
		current_frame += frames[(int)current_frame].speed;
		if(current_frame >= last_frame)
		{
			current_frame = (loop) ? 0.0f : last_frame - 1;
			loops++;
		}

		return frames[(int)current_frame].frame;
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