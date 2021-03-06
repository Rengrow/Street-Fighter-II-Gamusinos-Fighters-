#include "Globals.h"
#include "Application.h"
#include "ModuleEndBattle.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
#include "ModuleFonts.h"
#include "ModuleFight.h"
#include "ModulePlayer1.h"
#include "ModulePlayer2.h"
#include "SDL\include\SDL.h"


ModuleEndBattle::ModuleEndBattle()
{
	portrait1.x = 497;
	portrait1.y = 2;
	portrait1.w = 99;
	portrait1.h = 141;

	portrait2.x = 498;
	portrait2.y = 317;
	portrait2.w = 98;
	portrait2.h = 140;
}

ModuleEndBattle::~ModuleEndBattle()
{}

// Load assets
bool ModuleEndBattle::Start()
{
	LOG("Loading background assets");
	bool ret = true;
	portraits = App->textures->Load("assets/images/ui/portraits.png");
	typography = App->fonts->Load("assets/images/ui/FontMain.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789~'!@#$%^&*()-_+=[]{}| :;��<>,./?", 1);
	music = App->audio->LoadSong("assets/music/stage_end.ogg");
	App->audio->PlaySongDelay(music, 0, 10000);

	App->player1->position.x = 100;
	App->player1->position.y = 215;
	App->player2->position.x = 250;
	App->player2->position.y = 215;
	App->render->camera.x = App->render->camera.y = App->player1->puntuation = App->player2->puntuation = quoteLetters = timeScreen = 0;
	isFading = false;


	if (App->fight->player1RoundWinned > App->fight->player2RoundWinned) {
		portraitP1 = portrait1;
		portraitP2 = portrait2;
	}
	else {
		portraitP1 = portrait2;
		portraitP2 = portrait1;
	}

	quoteOption = SDL_GetTicks() % 2 == 0;

	return ret;
}

// Load assets
bool ModuleEndBattle::CleanUp()
{
	LOG("Unloading End battle screen");

	App->textures->Unload(portraits);
	App->fonts->UnLoad(typography);

	App->audio->UnloadSong(music);
	music = nullptr;

	delete cstr2;
	delete cstr;
	cstr2 = nullptr;
	cstr = nullptr;

	return true;
}

void ModuleEndBattle::UpdateQuote() {

	std::string text;
	std::string text2;

	if (quoteOption) {
		text = quote1.substr(0, quoteLetters);
		text2 = quote1sub.substr(0, quoteLetters);
	}
	else {
		text = quote2.substr(0, quoteLetters);
		text2 = quote2sub.substr(0, quoteLetters);
	}

	cstr = new char[text.length() + 1];
	strcpy_s(cstr, text.length() + 1, text.c_str());
	cstr2 = new char[text2.length() + 1];
	strcpy_s(cstr2, text2.length() + 1, text2.c_str());


}

// Update: draw background
update_status ModuleEndBattle::Update()
{
	// Draw everything --------------------------------------	
	App->render->Blit(portraits, 35, 10, &portraitP1, false);
	App->render->Blit(portraits, SCREEN_WIDTH - portraitP2.w - 35, 10, &portraitP2, true);

	if (!quoteOption) {
		App->fonts->BlitText(10, SCREEN_HEIGHT / 2 + 70, typography, cstr);
		App->fonts->BlitText(10, SCREEN_HEIGHT / 2 + 85, typography, cstr2);
	}
	else {
		App->fonts->BlitText(40, SCREEN_HEIGHT / 2 + 70, typography, cstr);
		App->fonts->BlitText(40, SCREEN_HEIGHT / 2 + 85, typography, cstr2);
	}

	if (timeLetters < App->frames && (quoteOption) ? quoteLetters < 36 : quoteLetters < 48) {
		quoteLetters++;
		timeLetters = App->frames + 180;
		UpdateQuote();
	}

	if (App->input->pads[0].start && !isFading) {
		Mix_FadeOutMusic(2000);
		App->fade->FadeToBlack(this, (Module*)App->welcomePage, 2);
		isFading = true;
		timeScreen = App->frames;
	}

	if (App->frames - timeScreen > 575)
		isFading = false;

		return UPDATE_CONTINUE;
}