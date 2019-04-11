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

	//void PushBack(const SDL_Rect& rect, const int maxFrames, int nColliders, Collider colliders[])
	void PushBack(const SDL_Rect& rect, const int maxFrames = 1)
	{
		frames[last_frame].frame = rect;
		frames[last_frame].maxFrames = maxFrames;

		/*for (int i = 0; i < nColliders; i++)
		{
			frames[last_frame].AddCollider(colliders[0].rect, colliders[0].type, colliders[0].callback);
		}*/

		last_frame++;
	}

	SDL_Rect& GetCurrentFrame()
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

		return frames[current_frame].frame;
	}

	SDL_Rect& GetCurrentHitboxes()
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

		return frames[current_frame].frame;
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