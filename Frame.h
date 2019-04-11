#ifndef __FRAME_H__
#define __FRAME_H__

#include "SDL/include/SDL_rect.h"
#define MAX_COLLIDERS 25

class Frame
{
public:
	int maxFrames;
	SDL_Rect frame;
	/*SDL_Rect hitBoxeRects[MAX_COLLIDERS];
	COLLIDER_TYPE types[MAX_COLLIDERS];
	Module* callbacks[MAX_COLLIDERS];*/

private:
	int last_collider = 0;

public:

	/*void AddCollider(SDL_Rect rect, COLLIDER_TYPE type, Module* callback) {
		hitBoxeRects[last_collider] = rect;
		types[last_collider] = type;
		callbacks[last_collider] = callback;
		last_collider++;
	}*/
};

#endif