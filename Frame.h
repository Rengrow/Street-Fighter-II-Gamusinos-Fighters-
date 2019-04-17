#ifndef __FRAME_H__
#define __FRAME_H__

#include "p2Point.h"
#include "SDL/include/SDL_rect.h"

enum COLLIDER_TYPE;

class Frame
{
public:
	int maxFrames;
	SDL_Rect frame;
	SDL_Rect hitBoxeRects[MAX_COLLIDERS_PER_FRAME];
	COLLIDER_TYPE types[MAX_COLLIDERS_PER_FRAME];
	Module* callbacks[MAX_COLLIDERS_PER_FRAME];

private:
	int last_collider = 0;

public:
	void AddColliderToFrame(SDL_Rect rect, COLLIDER_TYPE type, Module* callback) {
		hitBoxeRects[last_collider] = rect;
		types[last_collider] = type;
		callbacks[last_collider] = callback;
		last_collider++;
	}

	int GetColliderQnt() {
		return last_collider;
	}
};

#endif