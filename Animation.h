#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "Frame.h"
#include "SDL/include/SDL_rect.h"

#define MAX_FRAMES 25

class Animation
{
public:
	bool loop = true;
	Frame frames[MAX_FRAMES];

private:
	int frames_passed = 0;
	int current_frame;
	int last_frame = 0;
	int loops = 0;

public:
	void PushBack(const SDL_Rect& rect, const int maxFrames, p2Point <int> pivotPosition, int nColliders, SDL_Rect hitbox[], COLLIDER_TYPE type[], Module* callback[])
	{
		frames[last_frame].frame = rect;
		frames[last_frame].maxFrames = maxFrames;
		frames[last_frame].pivotPosition = pivotPosition;

		for (int i = 0; i < nColliders; i++)
		{
			frames[last_frame].AddColliderToFrame(hitbox[i], type[i], callback[i]);
		}

		last_frame++;
	}

	Frame& GetCurrentFrame()
	{
		if (frames_passed < frames[current_frame].maxFrames) {
			frames_passed++;
		}
		else {
			frames_passed = 0;
			current_frame++;
		}

		if (current_frame >= last_frame)
		{
			current_frame = (loop) ? 0.0f : last_frame - 1;
			loops++;
		}

		return frames[current_frame];
	}

	SDL_Rect& GetCurrentFrameBox()
	{
		return GetCurrentFrame().frame;
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void ResetAnimation()
	{
		current_frame = 0;
	}
};

#endif