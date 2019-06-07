#ifndef __ModuleInput_H__
#define __ModuleInput_H__

#include "Module.h"
#include "Globals.h"
#include <string.h>
#include "SDL\include\SDL_scancode.h"
#include "SDL\include\SDL_gamecontroller.h"

#define MAX_KEYS 300
#define MAX_HISTORY 180
#define MAX_COMMAND_FRAMES 250
#define MAX_GAME_CONTROLLERS 2

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

struct Gamepad
{
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;

	bool a = false;
	bool b = false;
	bool x = false;
	bool y = false;
	bool R1 = false;
	bool R2 = false;
	bool start = false;
};

struct History
{
	uint frame = 0u;
	KEY_STATE keyboard[MAX_KEYS];
	Gamepad pads[MAX_GAME_CONTROLLERS];
};


class ModuleInput : public Module
{
public:

	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status PreUpdate();
	bool CleanUp();
	const History* GetPrevious(int);

	bool CheckYogaFire(uint, int, bool) const;
	bool CheckYogaFlame(uint, int, bool) const;
	void ClearHistory();

public:
	History history[MAX_HISTORY];
	KEY_STATE keyboard[MAX_KEYS];

	int history_cursor = 0;

	Gamepad pads[MAX_GAME_CONTROLLERS];

	Uint8 gameController1States[SDL_CONTROLLER_BUTTON_MAX];
	float gameController1AxisValues[SDL_CONTROLLER_AXIS_MAX];

	Uint8 gameController2States[SDL_CONTROLLER_BUTTON_MAX];
	float gameController2AxisValues[SDL_CONTROLLER_AXIS_MAX];

	SDL_GameController* gameController1 = NULL;
	SDL_GameController* gameController2 = NULL;

};

#endif // __ModuleInput_H__