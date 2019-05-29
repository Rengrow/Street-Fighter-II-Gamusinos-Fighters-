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
	map.x = 0;
	map.y = 0;
	map.w = 192;
	map.h = 112;

	title.x = 77;
	title.y = 131;
	title.w = 95;
	title.h = 11;

	thailandFlag.x = 204;
	thailandFlag.y = 225;
	thailandFlag.w = 22;
	thailandFlag.h = 27;

	player1Cursor.x = 0;
	player1Cursor.y = 115;
	player1Cursor.w = 28;
	player1Cursor.h = 42;

	player2Cursor.x = 28;
	player2Cursor.y = 115;
	player2Cursor.w = 28;
	player2Cursor.h = 42;

	airplane.x = 60;
	airplane.y = 129;
	airplane.w = 15;
	airplane.h = 15;

	ryuFace.x = 0;
	ryuFace.y = 256;
	ryuFace.w = 28;
	ryuFace.h = 40;

	hondaFace.x = 38;
	hondaFace.y = 164;
	hondaFace.w = 28;
	hondaFace.h = 38;

	blankaFace.x = 77;
	blankaFace.y = 164;
	blankaFace.w = 28;
	blankaFace.h = 38;

	guileFace.x = 116;
	guileFace.y = 164;
	guileFace.w = 28;
	guileFace.h = 38;

	kenFace.x = 0;
	kenFace.y = 392;
	kenFace.w = 28;
	kenFace.h = 38;

	chunLiFace.x = 38;
	chunLiFace.y = 392;
	chunLiFace.w = 28;
	chunLiFace.h = 38;

	zangiefFace.x = 94;
	zangiefFace.y = 392;
	zangiefFace.w = 28;
	zangiefFace.h = 38;

	dhalsimFace.x = 141;
	dhalsimFace.y = 280;
	dhalsimFace.w = 28;
	dhalsimFace.h = 38;
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

	App->render->Blit(graphics, SCREEN_WIDTH / 2 - title.w / 2, 10, &title, false);
	App->render->Blit(graphics, SCREEN_WIDTH / 2 - map.w / 2, 25, &map, false);
	App->render->Blit(graphics, 25, 25, &thailandFlag, false);



	if (App->input->keyboard[SDL_SCANCODE_SPACE] == 1) {
		App->fade->FadeToBlack(this, (Module*)App->vsScreen, 2);
	}
	return UPDATE_CONTINUE;
}