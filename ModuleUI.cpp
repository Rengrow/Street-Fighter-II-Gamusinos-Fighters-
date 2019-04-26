#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleFadeToBlack.h"
#include "ModulePlayer.h"
#include "ModuleSecondPlayer.h"
#include "ModuleFonts.h"
#include<string.h>

#include "SDL/include/SDL.h"

ModuleUI::ModuleUI()
{
	lifeBarP1.x = 0;
	lifeBarP1.y = 1;
	lifeBarP1.w = 150;
	lifeBarP1.h = 15;

	lifeBarP2.x = 0;
	lifeBarP2.y = 1;
	lifeBarP2.w = 150;
	lifeBarP2.h = 15;

	KO.x = 337;
	KO.y = 0;
	KO.w = 29;
	KO.h = 24;


	redKO.x = 304;
	redKO.y = 0;
	redKO.w = 29;
	redKO.h = 24;
}

ModuleUI::~ModuleUI()
{}

// Load assets
bool ModuleUI::Start()
{
	LOG("Loading UI assets");
	bool ret = true;

	typography1 = App->fonts->Load("assets/images/ui/Font_1.png", "abcdefghijklmnopqrstuvwxyz.;:1234567890", 1);
	numbers = App->fonts->Load("assets/images/ui/timer_list.png", "0123456789", 1);
	lifeBars = App->textures->Load("assets/images/ui/Life_bar.png");

	timerStarted = redKoEnabled = starFight = false;

	return ret;
}

// Load assets
bool ModuleUI::CleanUp()
{
	App->textures->Unload(lifeBars);
	App->fonts->UnLoad(numbers);
	App->fonts->UnLoad(typography1);

	lifeBars = nullptr;

	return true;
}

// Update: draw background
update_status ModuleUI::PostUpdate()
{
	KoBlit();
	LifeBarsBlit();
	TimerBlit(numbers);
	StartFightBlit(typography1);

	return UPDATE_CONTINUE;
}

void ModuleUI::TimerBlit(int font_id) {

	if (font_id < 0 || font_id >= MAX_FONTS || App->fonts->fonts[font_id].graphic == nullptr) {
		LOG("Unable to render text with bmp font id %d", font_id);
		return;
	}

	const Font* font = &App->fonts->fonts[font_id];

	int timeRemaining = (timeOut - SDL_GetTicks()) / 1000;

	if (timerStarted && timeRemaining > 0) {
		tiempo[0] = (char)48 + (timeRemaining / 10);
		tiempo[1] = (char)48 + (timeRemaining % 10);
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + lifeBarP1.w + 24, 40, font_id, tiempo);
	}
	else {
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + lifeBarP1.w + 24, 40, font_id, "99");
		timerStarted = false;
	}
}

void ModuleUI::LifeBarsBlit() {

	lifeBarP1.w = 150 * (App->player->life / 100);
	App->render->Blit(lifeBars, -App->render->camera.x / SCREEN_SIZE + 24, 20, &lifeBarP1, true);

	lifeBarP2.w = 150 * (App->player2->life / 100);
	App->render->Blit(lifeBars, -App->render->camera.x / SCREEN_SIZE + lifeBarP2.w + 51, 20, &lifeBarP2, false);
}

void ModuleUI::KoBlit() {
	App->render->Blit(lifeBars, -App->render->camera.x / SCREEN_SIZE + lifeBarP1.w + 23, 15, (redKoEnabled ? &redKO : &KO), false);
}

void ModuleUI::StartTimer() {
	timeOut = SDL_GetTicks() + 99000; //99 Seconds
	timerStarted = true;
}

void ModuleUI::StartFight() {
	countdownFight = SDL_GetTicks() + 5000; //5 seconds
	starFight = true;
}

void ModuleUI::StartFightBlit(int font_id) {
	const Font* font = &App->fonts->fonts[font_id];

	int timeRemaining = (countdownFight - SDL_GetTicks()) / 1000;

	if (starFight)
		if (timeRemaining > 3) {
			App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + lifeBarP1.w + 24, SCREEN_HEIGHT / 2 - 31, font_id, "3");
		}
		else if (timeRemaining > 2) {
			App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + lifeBarP1.w + 24, SCREEN_HEIGHT / 2 - 31, font_id, "2");
		}
		else if (timeRemaining > 1) {
			App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + lifeBarP1.w + 24, SCREEN_HEIGHT / 2 - 31, font_id, "1");
		}
		else if (timeRemaining > 0) {
			App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + lifeBarP1.w - 32, SCREEN_HEIGHT / 2 - 31, font_id, "fight");
		}
		else {
			starFight = false;
			App->player->freeze = false;
			App->player2->freeze = false;
			StartTimer();
		}
}