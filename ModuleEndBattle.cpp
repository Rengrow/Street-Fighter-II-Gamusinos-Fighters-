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
	background.x = 284;
	background.y = 35;
	background.w = 184;
	background.h = 265;
}

ModuleEndBattle::~ModuleEndBattle()
{}

// Load assets
bool ModuleEndBattle::Start()
{
	LOG("Loading background assets");
	bool ret = true;
	portraits = App->textures->Load("assets/images/ui/portraits_2.png");
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

	App->textures->Unload(portraits);

	App->audio->UnloadSong(music);

	music = nullptr;

	return true;
}

// Update: draw background
update_status ModuleEndBattle::Update()
{
	// Draw everything --------------------------------------	
	App->render->Blit(portraits, 40, 5, &background, false);

	if (App->input->keyboard[SDL_SCANCODE_SPACE] == 1) {
		Mix_FadeOutMusic(2000);
		App->fade->FadeToBlack(this, (Module*)App->welcomePage, 2);
	}

	return UPDATE_CONTINUE;
}