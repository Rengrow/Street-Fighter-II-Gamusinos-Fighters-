#ifndef __ModuleInput_H__
#define __ModuleInput_H__

#include "Module.h"
#include "Globals.h"
#include "SDL\include\SDL_scancode.h"
#include "SDL\include\SDL_gamecontroller.h"

#define MAX_KEYS 300
#define MAX_HISTORY 180
#define MAX_COMMAND_FRAMES 9

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

enum class InputCommandTypes {
	yogaflame,
	yogafire,
	punch,
	max
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
};


struct History {
	uint frame = 0u;
	
	Uint8 buttons[MAX_KEYS];
};

struct InputCommand {
	InputCommandTypes type = InputCommandTypes::max;
	InputCommand(InputCommandTypes type) : type(type) {}
	virtual bool Check(uint past_frame) const = 0;
};

struct CommandPunch : public InputCommand {
	CommandPunch() : InputCommand(InputCommandTypes::punch) {}
	bool Check(uint frames_past) const override;
};

struct CommandYogaFire : public InputCommand {
	CommandYogaFire() : InputCommand(InputCommandTypes::yogafire) {}
	bool Check(uint frames_past) const override;
};

struct CommandYogaFlame : public InputCommand {
	CommandYogaFlame() : InputCommand(InputCommandTypes::yogaflame) {}
	bool Check(uint frames_past) const override;
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
	
	Gamepad p1;

	Uint8 gameController2States[SDL_CONTROLLER_BUTTON_MAX];
	float gameController2AxisValues[SDL_CONTROLLER_AXIS_MAX];
	
	Gamepad p2;


	SDL_GameController* gameController1 = NULL;
	SDL_GameController* gameController2 = NULL;
};

#endif // __ModuleInput_H__