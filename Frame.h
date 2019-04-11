#ifndef __FRAME_H__
#define __FRAME_H__

#include "SDL/include/SDL_rect.h"
#define MAX_FRAMES 25

class Frame
{
public:
	int maxFrames;
	SDL_Rect frame;
};

#endif