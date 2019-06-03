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
	if ((App->input->keyboard[SDL_SCANCODE_W] == KEY_STATE::KEY_DOWN) || (App->input->keyboard[SDL_SCANCODE_W] == KEY_STATE::KEY_REPEAT) || gameController1AxisValues[SDL_CONTROLLER_AXIS_LEFTY] < -JOYSTICK_DEAD_ZONE)
	{
		pads[0].up = true;
	}
	else
		pads[0].up = false;


	if ((App->input->keyboard[SDL_SCANCODE_S] == KEY_STATE::KEY_DOWN) || (App->input->keyboard[SDL_SCANCODE_S] == KEY_STATE::KEY_REPEAT) || gameController1AxisValues[SDL_CONTROLLER_AXIS_LEFTY] > JOYSTICK_DEAD_ZONE)
	{
		pads[0].down = true;
	}
	else
		pads[0].down = false;


	if ((App->input->keyboard[SDL_SCANCODE_A] == KEY_STATE::KEY_DOWN) || (App->input->keyboard[SDL_SCANCODE_A] == KEY_STATE::KEY_REPEAT) || gameController1AxisValues[SDL_CONTROLLER_AXIS_LEFTX] < -JOYSTICK_DEAD_ZONE)
	{
		pads[0].left = true;
	}
	else
		pads[0].left = false;

	if ((App->input->keyboard[SDL_SCANCODE_D] == KEY_STATE::KEY_DOWN) || (App->input->keyboard[SDL_SCANCODE_D] == KEY_STATE::KEY_REPEAT) || gameController1AxisValues[SDL_CONTROLLER_AXIS_LEFTX] > JOYSTICK_DEAD_ZONE)
	{
		pads[0].right = true;
	}
	else
		pads[0].right = false;

	if ((App->input->keyboard[SDL_SCANCODE_U] == KEY_STATE::KEY_DOWN) || (App->input->gameController1States[SDL_CONTROLLER_BUTTON_X] == KEY_DOWN))
	{
		pads[0].x = true;
	}
	else
		pads[0].x = false;

	if ((App->input->keyboard[SDL_SCANCODE_J] == KEY_STATE::KEY_DOWN) || (App->input->gameController1States[SDL_CONTROLLER_BUTTON_A] == KEY_DOWN))
	{
		pads[0].a = true;
	}
	else
		pads[0].a = false;

	if ((App->input->keyboard[SDL_SCANCODE_I] == KEY_STATE::KEY_DOWN) || (App->input->gameController1States[SDL_CONTROLLER_BUTTON_Y] == KEY_DOWN))
	{
		pads[0].y = true;
	}
	else
		pads[0].y = false;

	if ((App->input->keyboard[SDL_SCANCODE_K] == KEY_STATE::KEY_DOWN) || (App->input->gameController1States[SDL_CONTROLLER_BUTTON_B] == KEY_DOWN))
	{
		pads[0].b = true;
	}
	else
		pads[0].b = false;

	if ((App->input->keyboard[SDL_SCANCODE_O] == KEY_STATE::KEY_DOWN) || (App->input->gameController1States[SDL_CONTROLLER_BUTTON_RIGHTSHOULDER] == KEY_DOWN))
	{
		pads[0].R1 = true;
	}
	else
		pads[0].R1 = false;

	if ((App->input->keyboard[SDL_SCANCODE_L] == KEY_STATE::KEY_DOWN) || (App->input->gameController1AxisValues[SDL_CONTROLLER_AXIS_TRIGGERRIGHT] > JOYSTICK_DEAD_ZONE))
	{
		pads[0].R2 = true;
	}
	else
		pads[0].R2 = false;

	if ((App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN) || (App->input->gameController1States[SDL_CONTROLLER_BUTTON_START] == KEY_DOWN))
	{
		pads[0].start = true;
	}
	else
		pads[0].start = false;



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
	if ((App->input->keyboard[SDL_SCANCODE_UP] == KEY_STATE::KEY_DOWN) || (App->input->keyboard[SDL_SCANCODE_UP] == KEY_STATE::KEY_REPEAT) || gameController2AxisValues[SDL_CONTROLLER_AXIS_LEFTY] < -JOYSTICK_DEAD_ZONE)
	{
		pads[1].up = true;
	}
	else
		pads[1].up = false;


	if ((App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_STATE::KEY_DOWN) || (App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_STATE::KEY_REPEAT) || gameController2AxisValues[SDL_CONTROLLER_AXIS_LEFTY] > JOYSTICK_DEAD_ZONE)
	{
		pads[1].down = true;
	}
	else
		pads[1].down = false;


	if ((App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_STATE::KEY_DOWN) || (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_STATE::KEY_REPEAT) || gameController2AxisValues[SDL_CONTROLLER_AXIS_LEFTX] < -JOYSTICK_DEAD_ZONE )
	{
		pads[1].left = true;
	}
	else
		pads[1].left = false;

	if ((App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_STATE::KEY_DOWN) || (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_STATE::KEY_REPEAT) || gameController2AxisValues[SDL_CONTROLLER_AXIS_LEFTX] > JOYSTICK_DEAD_ZONE)
	{
		pads[1].right = true;
	}
	else
		pads[1].right = false;

	if ((App->input->keyboard[SDL_SCANCODE_KP_4] == KEY_STATE::KEY_DOWN) || (App->input->gameController2States[SDL_CONTROLLER_BUTTON_X] == KEY_DOWN))
	{
		pads[1].x = true;
	}
	else
		pads[1].x = false;

	if ((App->input->keyboard[SDL_SCANCODE_KP_1] == KEY_STATE::KEY_DOWN) || (App->input->gameController2States[SDL_CONTROLLER_BUTTON_A] == KEY_DOWN))
	{
		pads[1].a = true;
	}
	else
		pads[1].a = false;

	if ((App->input->keyboard[SDL_SCANCODE_KP_5] == KEY_STATE::KEY_DOWN) || (App->input->gameController2States[SDL_CONTROLLER_BUTTON_Y] == KEY_DOWN))
	{
		pads[1].y = true;
	}
	else
		pads[1].y = false;

	if ((App->input->keyboard[SDL_SCANCODE_KP_2] == KEY_STATE::KEY_DOWN) || (App->input->gameController2States[SDL_CONTROLLER_BUTTON_B] == KEY_DOWN))
	{
		pads[1].b = true;
	}
	else
		pads[1].b = false;

	if ((App->input->keyboard[SDL_SCANCODE_KP_3] == KEY_STATE::KEY_DOWN) || (App->input->gameController2States[SDL_CONTROLLER_BUTTON_RIGHTSHOULDER] == KEY_DOWN))
	{
		pads[1].R1 = true;
	}
	else
		pads[1].R1 = false;

	if ((App->input->keyboard[SDL_SCANCODE_KP_6] == KEY_STATE::KEY_DOWN) || (App->input->gameController2AxisValues[SDL_CONTROLLER_AXIS_TRIGGERRIGHT] > JOYSTICK_DEAD_ZONE))
	{
		pads[1].R2 = true;
	}
	else
		pads[1].R2 = false;

	if ((App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN) || (App->input->gameController2States[SDL_CONTROLLER_BUTTON_START] == KEY_DOWN))
	{
		pads[1].start = true;
	}
	else
		pads[1].start = false;

#pragma endregion

	if (history_cursor >= MAX_HISTORY) history_cursor = 0;

	memcpy(history[history_cursor].keyboard, keyboard, sizeof(KEY_STATE)*MAX_KEYS);
	memcpy(history[history_cursor].pads, pads, sizeof(Gamepad)*MAX_GAME_CONTROLLERS);
	history_cursor++;

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

const History* ModuleInput::GetPrevious(int pointer) {

	return (&history[pointer]);
}

bool ModuleInput::CheckYogaFire(uint frames_past, int player, bool flip) const {
	int count = 0;
	uint frame = 0;
	for (uint i = 1u; i < frames_past; ++i) {
		if (count > 0 && (i - frame) > MAX_COMMAND_FRAMES) {
			return false;
		}
		const History * history = App->input->GetPrevious(i);
		if (!history) {
			break;
		}
		const Gamepad * pad = &history->pads[player];

		if (flip == true)
		{
			switch (count) {
			case 0: { if (pad->right && !pad->down) { ++count; frame = i; } } break;
			case 1: { if (pad->right && pad->down) { ++count; frame = i; } } break;
			case 2: { if (pad->down) { return true; } } break;
			}
		}

		if (flip == false)
		{
			switch (count) {
			case 0: { if (pad->left && !pad->down) { ++count; frame = i; } } break;
			case 1: { if (pad->left && pad->down) { ++count; frame = i; } } break;
			case 2: { if (pad->down) { return true; } } break;
			}
		}

	}
	return false;
}

bool ModuleInput::CheckYogaFlame(uint frames_past, int player, bool flip) const {
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
		const Gamepad * pad = &history->pads[player];

		if (flip == true)
		{
			switch (count) {
			case 0: { if (pad->left && !pad->down) { ++count; frame = i; } } break;
			case 1: { if (pad->left && pad->down) { ++count; frame = i; } } break;
			case 2: { if (pad->down) { ++count; frame = i; } } break;
			case 3: { if (pad->right && pad->down) { return true; } } break;
			case 4: { if (pad->right && !pad->down) { return true; } } break;
			}
		}

		else if (flip == false)
		{
			switch (count) {
			case 0: { if (pad->right && !pad->down) { ++count; frame = i; } } break;
			case 1: { if (pad->right && pad->down) { ++count; frame = i; } } break;
			case 2: { if (pad->down) { ++count; frame = i; } } break;
			case 3: { if (pad->left && pad->down) { return true; } } break;
			case 4: { if (pad->left && !pad->down) { return true; } } break;
			}
		}
	}
	return false;
}
