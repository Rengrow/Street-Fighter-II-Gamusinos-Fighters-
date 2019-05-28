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
	typography = App->fonts->Load("assets/images/ui/font_2.png", "abcdefghijklmnopqrstuvwxyz_.?1234567890!", 1);
	music = App->audio->LoadSong("assets/music/title.ogg");
	App->audio->PlaySongDelay(music, -1, 10000);
	App->render->camera.x = App->render->camera.y = 0;

	fila = 0;
	columna = 0;
	segundaSheet = lettersOn = false;

	return ret;
}

// Load assets
bool ModuleWelcomePage::CleanUp()
{
	LOG("Unloading End battle screen");

	App->textures->Unload(graphics2);
	App->textures->Unload(graphics1);
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

	if (lettersOn)
		//App->fonts->BlitText(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 70, typography, "insert coin");    DEBUG FONT
		App->fonts->BlitText(5, SCREEN_HEIGHT / 2 + 70, typography, "insert_coin.");


	if (App->input->keyboard[SDL_SCANCODE_SPACE] == 1) {
		Mix_FadeOutMusic(2000);
		App->fade->FadeToBlack(this, (Module*)App->vsScreen, 2);
	}

	return UPDATE_CONTINUE;
}