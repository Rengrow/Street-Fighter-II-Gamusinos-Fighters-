#ifndef __ModuleInput_H__
#define __ModuleInput_H__

#include "Module.h"
#include "Globals.h"
#include "SDL\include\SDL_scancode.h"
#include "SDL\include\SDL_gamecontroller.h"

#define MAX_KEYS 300

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{
public:

	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status PreUpdate();
	bool CleanUp();

public:
	KEY_STATE keyboard[MAX_KEYS];

	Uint8 gameController1States[SDL_CONTROLLER_BUTTON_MAX];
	float gameController1AxisValues[SDL_CONTROLLER_AXIS_MAX];

	Uint8 gameController2States[SDL_CONTROLLER_BUTTON_MAX];
	float gameController2AxisValues[SDL_CONTROLLER_AXIS_MAX];

	SDL_GameController* gameController1 = NULL;
	SDL_GameController* gameController2 = NULL;
};

#endif // __ModuleInput_H__