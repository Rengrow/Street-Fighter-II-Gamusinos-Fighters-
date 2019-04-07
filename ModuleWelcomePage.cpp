#include "Globals.h"
#include "Application.h"
#include "ModuleWelcomePage.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"

ModuleWelcomePage::ModuleWelcomePage()
{
	background.x = 50;
	background.y = 27;
	background.w = 283;
	background.h = 173;
}

ModuleWelcomePage::~ModuleWelcomePage()
{}

// Load assets
bool ModuleWelcomePage::Start()
{
	LOG("Loading background assets");
	bool ret = true;
	graphics = App->textures->Load("assets/images/wiki/title_screen.png");
	music = App->audio->LoadSong("assets/music/title.ogg");
	App->audio->PlaySongDelay(music, -1, 10000);

	return ret;
}

// Load assets
bool ModuleWelcomePage::CleanUp()
{
	LOG("Unloading End battle screen");

	App->textures->Unload(graphics);
	App->audio->UnloadSong(music);

	music = nullptr;

	return true;
}

// Update: draw background
update_status ModuleWelcomePage::Update()
{
	// Draw everything --------------------------------------	
	App->render->Blit(graphics, 45, 30, &background, false);

	if (App->input->keyboard[SDL_SCANCODE_SPACE] == 1) {
		Mix_FadeOutMusic(2000);
		App->fade->FadeToBlack(this, (Module*)App->scene_ken, 5);
	}

	return UPDATE_CONTINUE;
}