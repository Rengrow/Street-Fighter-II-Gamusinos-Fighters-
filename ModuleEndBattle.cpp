#include "Globals.h"
#include "Application.h"
#include "ModuleEndBattle.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"

ModuleEndBattle::ModuleEndBattle()
{
	background.x = 0;
	background.y = 0;
	background.w = 308;
	background.h = 220;
}

ModuleEndBattle::~ModuleEndBattle()
{}

// Load assets
bool ModuleEndBattle::Start()
{
	LOG("Loading background assets");
	bool ret = true;
	graphics = App->textures->Load("assets/images/wiki/post_fight_screen.png");

	return ret;
}

// Load assets
bool ModuleEndBattle::CleanUp()
{
	LOG("Unloading End battle screen");

	App->textures->Unload(graphics);

	return true;
}

// Update: draw background
update_status ModuleEndBattle::Update()
{
	// Draw everything --------------------------------------	
	App->render->Blit(graphics, 40, 5, &background);

	if (App->input->keyboard[SDL_SCANCODE_SPACE] == 1)
		App->fade->FadeToBlack(this, (Module*)App->welcomePage, 5);

	return UPDATE_CONTINUE;
}