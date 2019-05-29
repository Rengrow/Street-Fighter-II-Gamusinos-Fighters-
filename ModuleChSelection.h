#ifndef __MODULECHSELECTION_H__
#define __MODULECHSELECTION_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"

struct SDL_Texture;

class ModuleChSelection : public Module
{
public:
	ModuleChSelection();
	~ModuleChSelection();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	SDL_Texture* graphics = nullptr;
	SDL_Rect map;
	SDL_Rect title;
	SDL_Rect thailandFlag;
	
	SDL_Rect player1Cursor;
	iPoint player1Position;
	SDL_Rect player2Cursor;
	iPoint player2Position;

	SDL_Rect airplane;

	SDL_Rect ryuFace;
	SDL_Rect hondaFace;
	SDL_Rect blankaFace;
	SDL_Rect guileFace;
	SDL_Rect kenFace;
	SDL_Rect chunLiFace;
	SDL_Rect zangiefFace;
	SDL_Rect dhalsimFace;
};

#endif // __MODULEENDBATTLE_H__