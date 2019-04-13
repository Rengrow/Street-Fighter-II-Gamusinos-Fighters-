#ifndef __FRAME_H__
#define __FRAME_H__

#include "p2Point.h"
#include "SDL/include/SDL_rect.h"
#define MAX_COLLIDERS 25

enum COLLIDER_TYPE;

class Frame
{
public:
	int maxFrames;
	SDL_Rect frame;
	p2Point<int> position[MAX_COLLIDERS];
	SDL_Rect hitBoxeRects[MAX_COLLIDERS];
	COLLIDER_TYPE types[MAX_COLLIDERS];
	Module* callbacks[MAX_COLLIDERS];

private:
	int last_collider = 0;

public:
	void AddColliderToFrame(p2Point<int> coordinates, SDL_Rect rect, COLLIDER_TYPE type, Module* callback) {
		position[last_collider] = coordinates;
		hitBoxeRects[last_collider] = rect;
		types[last_collider] = type;
		callbacks[last_collider] = callback;
		last_collider++;
	}

	int GetColliderQnt() {
		return last_collider;
	}

	p2Point<int>* GetPositionRelative() {
		return position;
	}

	SDL_Rect* GetFrameHitbox() {
		return hitBoxeRects;
	}

	COLLIDER_TYPE* GetFrameHitboxType() {
		return types;
	}

	Module** GetFrameCallback() {
		return callbacks;
	}

};

#endif