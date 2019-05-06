#include "Globals.h"
#include "Application.h"
#include "ModuleEndBattle.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"

#include "ModuleRyu.h"
#include "ModuleDhalsim.h"


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
	music = App->audio->LoadSong("assets/music/stage_end.ogg");
	App->audio->PlaySongDelay(music, 0, 10000);


	App->ryu->position.x = 100;
	App->ryu->position.y = 215;
	App->dhalsim->position.x = 250;
	App->dhalsim->position.y = 215;
	App->render->camera.x = App->render->camera.y = 0;

	return ret;
}

// Load assets
bool ModuleEndBattle::CleanUp()
{
	LOG("Unloading End battle screen");

	App->textures->Unload(graphics);

	App->audio->UnloadSong(music);

	music = nullptr;

	return true;
}

// Update: draw background
update_status ModuleEndBattle::Update()
{
	// Draw everything --------------------------------------	
	App->render->Blit(graphics, 40, 5, &background, false);

	if (App->input->keyboard[SDL_SCANCODE_SPACE] == 1) {
		Mix_FadeOutMusic(2000);
		App->fade->FadeToBlack(this, (Module*)App->welcomePage, 2);
	}

	return UPDATE_CONTINUE;
}