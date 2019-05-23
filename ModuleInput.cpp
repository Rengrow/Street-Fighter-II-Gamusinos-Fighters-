#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "SDL/include/SDL.h"


ModuleInput::ModuleInput() : Module()
{
	for (uint i = 0; i < MAX_KEYS; ++i)
		keyboard[i] = KEY_IDLE;

	for (uint i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i)
		gameController1States[i] = KEY_IDLE;

	for (uint i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i)
		gameController2States[i] = KEY_IDLE;
}

// Destructor
ModuleInput::~ModuleInput()
{
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0)
	{
		LOG("SDL_JOYSTICK could not initialize! SDL Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate()
{

#pragma region Keyboard
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}
#pragma endregion

#pragma region Gamepad

	//Load GameController
	if (!SDL_GameControllerGetAttached(gameController1)) {
		gameController1 = SDL_GameControllerOpen(0);
		if (gameController1 == NULL)
			LOG("Warning: Unable to open game controller 1! SDL Error: %s\n", SDL_GetError());
	}

	if (!SDL_GameControllerGetAttached(gameController2)) {
		gameController2 = SDL_GameControllerOpen(1);
		if (gameController2 == NULL)
			LOG("Warning: Unable to open game controller 2! SDL Error: %s\n", SDL_GetError());
	}

	SDL_GameControllerUpdate();

	//Obtain the current button values of GamePad 1
	for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i) {
		if (SDL_GameControllerGetButton(gameController1, (SDL_GameControllerButton)i) == 1) {
			if (gameController1States[i] == KEY_IDLE)
				gameController1States[i] = KEY_DOWN;
			else
				gameController1States[i] = KEY_REPEAT;
		}
		else {
			if (gameController1States[i] == KEY_REPEAT || gameController1States[i] == KEY_DOWN)
				gameController1States[i] = KEY_UP;
			else
				gameController1States[i] = KEY_IDLE;
		}
	}

	//Obtain the current axis value of GamePad 1
	for (int i = 0; i < SDL_CONTROLLER_AXIS_MAX; ++i) {
		gameController1AxisValues[i] = SDL_GameControllerGetAxis(gameController1, (SDL_GameControllerAxis)i);
	}

	//Obtaind the direction of Gamepad 1
	if (gameController1AxisValues[SDL_CONTROLLER_AXIS_LEFTY] < -JOYSTICK_DEAD_ZONE)
	{
		p1.up = true;
	}
	else
		p1.up = false;


	if (gameController1AxisValues[SDL_CONTROLLER_AXIS_LEFTY] > JOYSTICK_DEAD_ZONE)
	{
		p1.down = true;
	}
	else
		p1.down = false;


	if (gameController1AxisValues[SDL_CONTROLLER_AXIS_LEFTX] > JOYSTICK_DEAD_ZONE)
	{
		p1.left = true;
	}
	else
		p1.left = false;

	if (gameController1AxisValues[SDL_CONTROLLER_AXIS_LEFTX] < -JOYSTICK_DEAD_ZONE)
	{
		p1.right = true;
	}
	else
		p1.right = false;



	//Obtain the current button values of GamePad 2
	for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i) {
		if (SDL_GameControllerGetButton(gameController2, (SDL_GameControllerButton)i) == 1) {
			if (gameController2States[i] == KEY_IDLE)
				gameController2States[i] = KEY_DOWN;
			else
				gameController2States[i] = KEY_REPEAT;
		}
		else {
			if (gameController2States[i] == KEY_REPEAT || gameController2States[i] == KEY_DOWN)
				gameController2States[i] = KEY_UP;
			else
				gameController2States[i] = KEY_IDLE;
		}
	}

	//Obtain the current axis value of GamePad 2
	for (int i = 0; i < SDL_CONTROLLER_AXIS_MAX; ++i) {
		gameController2AxisValues[i] = SDL_GameControllerGetAxis(gameController2, (SDL_GameControllerAxis)i);
	}


	//Obtaind the direction of Gamepad 2
	if (gameController2AxisValues[SDL_CONTROLLER_AXIS_LEFTY] < -JOYSTICK_DEAD_ZONE)
	{
		p2.up = true;
	}
	else
		p2.up = false;


	if (gameController2AxisValues[SDL_CONTROLLER_AXIS_LEFTY] > JOYSTICK_DEAD_ZONE)
	{
		p2.down = true;
	}
	else
		p2.down = false;


	if (gameController2AxisValues[SDL_CONTROLLER_AXIS_LEFTX] < -JOYSTICK_DEAD_ZONE)
	{
		p2.left = true;
	}
	else
		p2.left = false;

	if (gameController2AxisValues[SDL_CONTROLLER_AXIS_LEFTX] > JOYSTICK_DEAD_ZONE)
	{
		p2.right = true;
	}
	else
		p2.right = false;




#pragma endregion

	if (keyboard[SDL_SCANCODE_ESCAPE])
		return update_status::UPDATE_STOP;

	return update_status::UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	//Close game controller 2
	SDL_GameControllerClose(gameController2);
	gameController2 = NULL;
	//Close game controller 1
	SDL_GameControllerClose(gameController1);
	gameController1 = NULL;

	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}


bool CommandYogaFire::Check(uint frames_past) const {
	int count = 0;
	uint frame = 0;
	for (uint i = 1u; i < frames_past; ++i) {
		if (count > 0 && (i - frame) > MAX_COMMAND_FRAMES) {
			return false;
		}
		const History* history = App->input->GetPrevious(i);
		if (!history) {
			break;
		}
		const gamecontroller* pad = &(history->pads[0]);
		switch (count) {
		case 0: { if (pad->LP) { ++count; frame = i; } } break;
		case 1: { if (pad->RIGHT) { ++count; frame = i; } } break;
		case 2: { if (pad->RIGHT_AND_DOWN) { ++count; frame = i; } } break;
		case 3: { if (pad->DOWN) { return true; } } break;
		}
	}
	return false;
}


bool CommandYogaFlame::Check(uint frames_past) const {
	int count = 0;
	uint frame = 0;
	for (uint i = 1u; i < frames_past; ++i) {
		if (count > 0 && (i - frame) > MAX_COMMAND_FRAMES) {
			return false;
		}
		const History* history = App->input->GetPrevious(i);
		if (!history) {
			break;
		}
		const gamecontroller* pad = &(history->pads[0]);
		switch (count) {
		case 0: { if (pad->LP) { ++count; frame = i; } } break;
		case 1: { if (pad->RIGHT) { ++count; frame = i; } } break;
		case 2: { if (pad->RIGHT_AND_DOWN) { ++count; frame = i; } } break;
		case 3: { if (pad->DOWN) { ++count; frame = i; } } break;
		case 4: { if (pad->LEFT_AND_DOWN) { return true; } } break;
		case 5: { if (pad->LEFT) { return true; } } break;
		}
	}
	return false;
}

bool CommandPunch::Check(uint frames_past) const {
	int count = 0;
	uint frame = 0;
	for (uint i = 1u; i < frames_past; ++i) {
		if (count > 0 && (i - frame) > MAX_COMMAND_FRAMES) {
			return false;
		}
		const History* history = App->input->GetPrevious(i);
		if (!history) {
			break;
		}
		const gamecontroller* pad = &(history->pads[0]);
		switch (count) {
		case 0: { if (pad->a) { ++count; frame = i; } } break;
		}
	}
	return false;
}