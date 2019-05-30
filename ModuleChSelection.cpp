#include "Globals.h"
#include "Application.h"
#include "ModuleChSelection.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
#include "ModuleFonts.h"

#include "SDL\include\SDL.h"


ModuleChSelection::ModuleChSelection()
{
}

ModuleChSelection::~ModuleChSelection()
{}

// Load assets
bool ModuleChSelection::Start()
{
	LOG("Loading background assets");
	bool ret = true;
	graphics = App->textures->Load("assets/images/ui/character_selection2.png");

	App->render->camera.x = App->render->camera.y = 0;

#pragma region Read Sprites

	map.x = 0;
	map.y = 0;
	map.w = 192;
	map.h = 112;

	thailandFlag.x = 204;
	thailandFlag.y = 225;
	thailandFlag.w = 22;
	thailandFlag.h = 27;

	player1Cursor.x = 0;
	player1Cursor.y = 115;
	player1Cursor.w = 28;
	player1Cursor.h = 42;

	player2Cursor.x = 31;
	player2Cursor.y = 115;
	player2Cursor.w = 28;
	player2Cursor.h = 42;

	airplane.x = 60;
	airplane.y = 129;
	airplane.w = 15;
	airplane.h = 15;

	ryuFace.x = 0;
	ryuFace.y = 253;
	ryuFace.w = 28;
	ryuFace.h = 37;

	hondaFace.x = 38;
	hondaFace.y = 253;
	hondaFace.w = 28;
	hondaFace.h = 37;

	blankaFace.x = 78;
	blankaFace.y = 253;
	blankaFace.w = 28;
	blankaFace.h = 37;

	guileFace.x = 116;
	guileFace.y = 253;
	guileFace.w = 28;
	guileFace.h = 37;

	kenFace.x = 0;
	kenFace.y = 300;
	kenFace.w = 28;
	kenFace.h = 37;

	chunLiFace.x = 38;
	chunLiFace.y = 300;
	chunLiFace.w = 28;
	chunLiFace.h = 37;

	zangiefFace.x = 78;
	zangiefFace.y = 300;
	zangiefFace.w = 28;
	zangiefFace.h = 37;

	dhalsimFace.x = 116;
	dhalsimFace.y = 208;
	dhalsimFace.w = 28;
	dhalsimFace.h = 37;

	player1Position.x = SCREEN_WIDTH / 2 + 24;
	player1Position.y = SCREEN_HEIGHT / 2 + 62;

	player2Position.x = SCREEN_WIDTH / 2 - 60;
	player2Position.y = SCREEN_HEIGHT / 2 + 62;

#pragma endregion


	return ret;
}

// Load assets
bool ModuleChSelection::CleanUp()
{
	LOG("Unloading Vs Screen");

	App->textures->Unload(graphics);

	return true;
}

// Update: draw background
update_status ModuleChSelection::Update()
{
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 121, 255);

	if (timeSelector < App->frames) {
		lettersOn = !lettersOn;

		timeSelector = App->frames + 30;
	}






	App->render->Blit(graphics, SCREEN_WIDTH / 2 - map.w / 2, 18, &map, false);
	App->render->Blit(graphics, 165, 80, &thailandFlag, false);


	//Panel
	App->render->Blit(graphics, SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2 + 30, &ryuFace, false);
	App->render->Blit(graphics, SCREEN_WIDTH / 2 - 32, SCREEN_HEIGHT / 2 + 30, &hondaFace, false);
	App->render->Blit(graphics, SCREEN_WIDTH / 2 - 4, SCREEN_HEIGHT / 2 + 30, &blankaFace, false);
	App->render->Blit(graphics, SCREEN_WIDTH / 2 + 24, SCREEN_HEIGHT / 2 + 30, &guileFace, false);
	App->render->Blit(graphics, SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2 + 67, &kenFace, false);
	App->render->Blit(graphics, SCREEN_WIDTH / 2 - 32, SCREEN_HEIGHT / 2 + 67, &chunLiFace, false);
	App->render->Blit(graphics, SCREEN_WIDTH / 2 - 4, SCREEN_HEIGHT / 2 + 67, &zangiefFace, false);
	App->render->Blit(graphics, SCREEN_WIDTH / 2 + 24, SCREEN_HEIGHT / 2 + 67, &dhalsimFace, false);
	//

	if (lettersOn) {
		App->render->Blit(graphics, player1Position.x, player1Position.y, &player1Cursor, false);
		App->render->Blit(graphics, player2Position.x, player2Position.y, &player2Cursor, false);
	}


	if (App->input->keyboard[SDL_SCANCODE_SPACE] == 1)
		App->fade->FadeToBlack(this, (Module*)App->vsScreen, 2);


	return UPDATE_CONTINUE;
}