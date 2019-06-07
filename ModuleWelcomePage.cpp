#include "Globals.h"
#include "Application.h"
#include "ModuleWelcomePage.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
#include "ModuleFonts.h"

ModuleWelcomePage::ModuleWelcomePage()
{
	background.x = 0;
	background.y = 0;
	background.w = 384;
	background.h = 224;
}

ModuleWelcomePage::~ModuleWelcomePage()
{}

// Load assets
bool ModuleWelcomePage::Start()
{
	LOG("Loading background assets");
	bool ret = true;
	graphics1 = App->textures->Load("assets/images/ui/logo1.png");
	graphics2 = App->textures->Load("assets/images/ui/logo2.png");
	typography = App->fonts->Load("assets/images/ui/FontMain.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789~'!@#$%^&*()-_+=[]{}| :;¨º<>,./?", 1);
	music = App->audio->LoadSong("assets/music/title.ogg");
	start = App->audio->LoadChunk("assets/sfx/effects/coin.wav");
	App->audio->PlaySongDelay(music, -1, 10000);
	App->render->camera.x = App->render->camera.y = fila = columna = timeLetters = 0;
	segundaSheet = lettersOn = false;

	return ret;
}

// Load assets
bool ModuleWelcomePage::CleanUp()
{
	LOG("Unloading End battle screen");

	App->textures->Unload(graphics2);
	App->textures->Unload(graphics1);

	App->audio->UnloadChunk(start);
	start = nullptr;

	App->audio->UnloadSong(music);
	music = nullptr;

	return true;
}

// Update: draw background
update_status ModuleWelcomePage::Update()
{
	// Draw everything --------------------------------------	
	SDL_Texture* graphics;

	if (!segundaSheet)
		graphics = graphics1;
	else
		graphics = graphics2;

	App->render->Blit(graphics, 0, 0, &background, false);

	if (!segundaSheet || fila != 4 || columna != 8) {
		if (fila != 4)
			fila++;
		else if (columna != 8) {
			fila = 0;
			columna++;
		}
		else {
			fila = 0;
			columna = 0;
			segundaSheet = true;
		}
		background.x = 384 * fila;
		background.y = 224 * columna;
	}


	if (timeLetters < App->frames) {
		lettersOn = !lettersOn;

		timeLetters = App->frames + 30;
	}

	App->fonts->BlitText(0, 0, typography, "Fanmade copy by: Gamusinos Fighters");
	App->fonts->BlitText(SCREEN_WIDTH / 2 - 95, SCREEN_HEIGHT / 2 + 90, typography, "@CAPCOM CO.. LTD.");

	if (lettersOn)
		App->fonts->BlitText(SCREEN_WIDTH / 2 - 65, SCREEN_HEIGHT / 2 + 50, typography, "INSERT COIN.");


	if (App->input->pads[0].start) {
		App->audio->PlayChunk(start);
		Mix_FadeOutMusic(2000);
		App->fade->FadeToBlack(this, (Module*)App->chSelectionScreen, 1);
	}

	return UPDATE_CONTINUE;
}