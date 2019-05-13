#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleRyu.h"
#include "ModuleDhalsim.h"
#include "ModuleFonts.h"
#include "ModuleInput.h"
#include "ModuleSceneKen.h"
#include "ModuleFight.h"
#include "ModuleAudio.h"
#include<string.h>

#include "SDL/include/SDL.h"
#include "SDL/include/SDL_gamecontroller.h"

ModuleUI::ModuleUI()
{
	lifeBarP1.x = 0;
	lifeBarP1.y = 1;
	lifeBarP1.w = 150;
	lifeBarP1.h = 16;

	lifeBarP2.x = 0;
	lifeBarP2.y = 1;
	lifeBarP2.w = 150;
	lifeBarP2.h = 16;

	lifeBarRedP1.x = 0;
	lifeBarRedP1.y = 19;
	lifeBarRedP1.w = 150;
	lifeBarRedP1.h = 16;

	lifeBarRedP2.x = 0;
	lifeBarRedP2.y = 19;
	lifeBarRedP2.w = 150;
	lifeBarRedP2.h = 16;

	KO.x = 34;
	KO.y = 37;
	KO.w = 26;
	KO.h = 22;

	redKO.x = 1;
	redKO.y = 36;
	redKO.w = 27;
	redKO.h = 24;

	iconRoundWinned.x = 65;
	iconRoundWinned.y = 36;
	iconRoundWinned.w = 20;
	iconRoundWinned.h = 20;

	roundRect.x = 2;
	roundRect.y = 63;
	roundRect.w = 76;
	roundRect.h = 31;

	round1Rect.x = 3;
	round1Rect.y = 99;
	round1Rect.w = 11;
	round1Rect.h = 30;

	round2Rect.x = 28;
	round2Rect.y = 99;
	round2Rect.w = 17;
	round2Rect.h = 30;

	round3Rect.x = 52;
	round3Rect.y = 98;
	round3Rect.w = 17;
	round3Rect.h = 31;

	roundFRect.x = 79;
	roundFRect.y = 99;
	roundFRect.w = 19;
	roundFRect.h = 30;

	fightRect.x = 3;
	fightRect.y = 135;
	fightRect.w = 89;
	fightRect.h = 28;
}

ModuleUI::~ModuleUI()
{}

// Load assets
bool ModuleUI::Start()
{
	LOG("Loading UI assets");
	bool ret = true;

	typography1 = App->fonts->Load("assets/images/ui/Font_1.png", "abcdefghijklmnopqrstuvwxyz.;:1234567890 ", 1);
	typographyDebug = App->fonts->Load("assets/images/ui/font_debug.png", "! @,_./0123456789$;<&?abcdefghijklmnopqrstuvwxyz", 1);
	numbers = App->fonts->Load("assets/images/ui/timer_list.png", "0123456789", 1);
	lifeBars = App->textures->Load("assets/images/ui/Life_bar_Rounds.png");

	round_snd = App->audio->LoadChunk("assets/sfx/voices/announcer_round.wav");
	one_snd = App->audio->LoadChunk("assets/sfx/voices/announcer_1.wav");
	two_snd = App->audio->LoadChunk("assets/sfx/voices/announcer_2.wav");
	three_snd = App->audio->LoadChunk("assets/sfx/voices/announcer_3.wav");
	final_snd = App->audio->LoadChunk("assets/sfx/voices/announcer_final.wav");
	fight_snd = App->audio->LoadChunk("assets/sfx/voices/announcer_fight.wav");
	you_snd = App->audio->LoadChunk("assets/sfx/voices/announcer_you.wav");
	win_snd = App->audio->LoadChunk("assets/sfx/voices/announcer_win.wav");
	lose_snd = App->audio->LoadChunk("assets/sfx/voices/announcer_lose.wav");

	timerStarted = redKoEnabled = starFight = roundSoundPlayed = numberRoundSoundPlayed = fightSoundPlayed = youFinalSound = winLoseFinalSound = false;

	return ret;
}

// Load assets
bool ModuleUI::CleanUp()
{
	App->audio->UnloadChunk(round_snd);
	round_snd = nullptr;
	App->audio->UnloadChunk(one_snd);
	one_snd = nullptr;
	App->audio->UnloadChunk(two_snd);
	two_snd = nullptr;
	App->audio->UnloadChunk(three_snd);
	three_snd = nullptr;
	App->audio->UnloadChunk(final_snd);
	final_snd = nullptr;
	App->audio->UnloadChunk(fight_snd);
	fight_snd = nullptr;
	App->audio->UnloadChunk(you_snd);
	you_snd = nullptr;
	App->audio->UnloadChunk(win_snd);
	win_snd = nullptr;
	App->audio->UnloadChunk(lose_snd);
	lose_snd = nullptr;

	App->textures->Unload(lifeBars);
	App->fonts->UnLoad(numbers);
	App->fonts->UnLoad(typographyDebug);
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
	RoundsWinnedBlit();
	StartFightBlit();
	EndFight();

	
		
		
	if (App->input->keyboard[SDL_SCANCODE_F2] == KEY_STATE::KEY_DOWN)
		debugGamepads = !debugGamepads;
	
	
	if (debugGamepads)
		BlitGamePadDebug();

	return UPDATE_CONTINUE;
}

void ModuleUI::TimerBlit(int font_id) {

	if (font_id < 0 || font_id >= MAX_FONTS || App->fonts->fonts[font_id].graphic == nullptr) {
		LOG("Unable to render text with bmp font id %d", font_id);
		return;
	}

	const Font* font = &App->fonts->fonts[font_id];

	int timeRemaining = App->fight->GetTimer();

	if (App->fight->stopedFight) {
		tiempo[0] = (char)48 + (stopedTimer / 10);
		tiempo[1] = (char)48 + (stopedTimer % 10);
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + LIFE_BAR_LENGHT + 24, 40, font_id, tiempo);
	}
	else if (timerStarted && timeRemaining > 0) {
		tiempo[0] = (char)48 + (timeRemaining / 10);
		tiempo[1] = (char)48 + (timeRemaining % 10);
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + LIFE_BAR_LENGHT + 24, 40, font_id, tiempo);
	}
	else {
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + LIFE_BAR_LENGHT + 24, 40, font_id, "99");
		timerStarted = false;
	}
}

void ModuleUI::LifeBarsBlit() {

	App->render->Blit(lifeBars, -App->render->camera.x / SCREEN_SIZE + 24, 20, &lifeBarRedP1, false);
	App->render->Blit(lifeBars, -App->render->camera.x / SCREEN_SIZE + LIFE_BAR_LENGHT + 51, 20, &lifeBarRedP2, true);

	lifeBarP1.x = lifeBarRedP1.w - App->ryu->life*1.5;
	App->render->Blit(lifeBars, (-App->render->camera.x / SCREEN_SIZE) + 24, 20, &lifeBarP1, true);

	lifeBarP2.w = App->dhalsim->life*1.5;
	App->render->Blit(lifeBars, -App->render->camera.x / SCREEN_SIZE + LIFE_BAR_LENGHT + 51, 20, &lifeBarP2, false);
}

void ModuleUI::KoBlit() {
	App->render->Blit(lifeBars, -App->render->camera.x / SCREEN_SIZE + LIFE_BAR_LENGHT + 24, 15, (redKoEnabled ? &redKO : &KO), false);
}

void ModuleUI::RoundsWinnedBlit() {

	if (App->fight->player1RoundWinned == 1)
		App->render->Blit(lifeBars, -App->render->camera.x / SCREEN_SIZE + 2, 20, &iconRoundWinned, false);
	else if (App->fight->player1RoundWinned == 2) {
		App->render->Blit(lifeBars, -App->render->camera.x / SCREEN_SIZE + 2, 20, &iconRoundWinned, false);
		App->render->Blit(lifeBars, -App->render->camera.x / SCREEN_SIZE + 22, 20, &iconRoundWinned, false);
	}

	if (App->fight->player2RoundWinned == 1)
		App->render->Blit(lifeBars, (-App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH) - 20, 20, &iconRoundWinned, false);
	else if (App->fight->player2RoundWinned == 2) {
		App->render->Blit(lifeBars, (-App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH) - 20, 20, &iconRoundWinned, false);
		App->render->Blit(lifeBars, (-App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH) - 40, 20, &iconRoundWinned, false);
	}
}

void ModuleUI::StartTimer() {
	App->fight->timeOutTimer = SDL_GetTicks() + 99000; //99 Secs
	timerStarted = true;
}

void ModuleUI::StartFight() {
	App->fight->countdownStartFight = SDL_GetTicks() + 4000; //5 seconds
	starFight = true;
}

void ModuleUI::StartFightBlit() {

	int timeRemaining = (App->fight->countdownStartFight - SDL_GetTicks()) / 1000;

	if (starFight) {

		if (timeRemaining == 3 && App->fight->round <= 3 && !roundSoundPlayed) {
			App->audio->PlayChunk(round_snd);
			roundSoundPlayed = true;
		}
		else if (timeRemaining == 3 && App->fight->round > 3 && !roundSoundPlayed) {
			App->audio->PlayChunk(final_snd);
			roundSoundPlayed = true;
		}
		else if (timeRemaining == 2 && !numberRoundSoundPlayed) {
			if (App->fight->round == 1) {
				App->audio->PlayChunk(one_snd);
				numberRoundSoundPlayed = true;
			}
			else if (App->fight->round == 2) {
				App->audio->PlayChunk(two_snd);
				numberRoundSoundPlayed = true;
			}
			else if (App->fight->round == 3) {
				App->audio->PlayChunk(three_snd);
				numberRoundSoundPlayed = true;
			}
			else if (App->fight->round > 3) {
				App->audio->PlayChunk(round_snd);
				numberRoundSoundPlayed = true;
			}
		}
		else if (timeRemaining == 1 && !fightSoundPlayed) {
			App->audio->PlayChunk(fight_snd);
			fightSoundPlayed = true;
		}

		if (timeRemaining >= 2) {
			App->render->Blit(lifeBars, -App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH / 2 - 45, SCREEN_HEIGHT / 2 - roundRect.h, &roundRect, false);
			if (App->fight->round == 1)
				App->render->Blit(lifeBars, -App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH / 2 + 35, SCREEN_HEIGHT / 2 - round1Rect.h, &round1Rect, false);
			else if (App->fight->round == 2)
				App->render->Blit(lifeBars, -App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH / 2 + 35, SCREEN_HEIGHT / 2 - round2Rect.h, &round2Rect, false);
			else if (App->fight->round == 3)
				App->render->Blit(lifeBars, -App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH / 2 + 35, SCREEN_HEIGHT / 2 - round3Rect.h, &round3Rect, false);
			else if (App->fight->round > 3)
				App->render->Blit(lifeBars, -App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH / 2 - 45 - roundFRect.w, SCREEN_HEIGHT / 2 - roundFRect.h, &roundFRect, false);
		}
		else if (timeRemaining >= 1) {
			App->render->Blit(lifeBars, -App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH / 2 - 45, SCREEN_HEIGHT / 2 - fightRect.h, &fightRect, false);
		}
		else {
			starFight = false;
			App->ryu->freeze = false;
			App->dhalsim->freeze = false;
			App->fight->roundStarted = true;
			StartTimer();
		}
	}
}

void ModuleUI::StartEndFight(int ryu) {
	stopedTimer = App->fight->GetTimer();
	winnerPlayer = ryu;
}

void ModuleUI::EndFight() {
	if (App->fight->endFightStarted) {
		int timeRemaining = (App->fight->endFightTimer - SDL_GetTicks()) / 1000;

		if (timeRemaining == 3 && !youFinalSound) {
			App->audio->PlayChunk(you_snd);

			if (winnerPlayer == 1) {
				App->ryu->inputs.Push(IN_VICTORY);
				App->dhalsim->inputs.Push(IN_LOOSE2);
			}
			else if (winnerPlayer == 2) {
				App->ryu->inputs.Push(IN_LOOSE);
				App->dhalsim->inputs.Push(IN_VICTORY2);
			}

			youFinalSound = true;
		}
		else if (timeRemaining == 2 && !winLoseFinalSound) {
			if (winnerPlayer == 1)
				App->audio->PlayChunk(win_snd);
			else if (winnerPlayer == 2)
				App->audio->PlayChunk(lose_snd);
			winLoseFinalSound = true;
		}

		if (timeRemaining > 0)
			if (winnerPlayer == 1)
				App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + 10, SCREEN_HEIGHT / 2 - 30, typography1, "player 1 win");
			else if (winnerPlayer == 2)
				App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + 10, SCREEN_HEIGHT / 2 - 30, typography1, "player 2 win");
	}
}

void ModuleUI::BlitGamePadDebug() {

	if (App->input->gameController1States[SDL_CONTROLLER_BUTTON_A] == KEY_DOWN || App->input->gameController1States[SDL_CONTROLLER_BUTTON_A] == KEY_REPEAT)
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + 10, SCREEN_HEIGHT / 2 - 30, typographyDebug, "a");

	if (App->input->gameController1AxisValues[SDL_CONTROLLER_AXIS_LEFTY] < -JOYSTICK_DEAD_ZONE)
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + 20, SCREEN_HEIGHT / 2 - 30, typographyDebug, "up");
	

	if (App->input->gameController1AxisValues[SDL_CONTROLLER_AXIS_LEFTY] > JOYSTICK_DEAD_ZONE)
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + 20, SCREEN_HEIGHT / 2 - 30, typographyDebug, "down");
	

	if (App->input->gameController1AxisValues[SDL_CONTROLLER_AXIS_LEFTX] > JOYSTICK_DEAD_ZONE)
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + 50, SCREEN_HEIGHT / 2 - 30, typographyDebug, "left");
	

	if (App->input->gameController1AxisValues[SDL_CONTROLLER_AXIS_LEFTX] < -JOYSTICK_DEAD_ZONE)
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + 50, SCREEN_HEIGHT / 2 - 30, typographyDebug, "right");
	
}