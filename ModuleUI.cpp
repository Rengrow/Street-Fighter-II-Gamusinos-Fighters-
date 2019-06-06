#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModulePlayer1.h"
#include "ModulePlayer2.h"
#include "ModuleFonts.h"
#include "ModuleInput.h"
#include "ModuleSceneKen.h"
#include "ModuleFight.h"
#include "ModuleAudio.h"
#include <string>

#include "SDL/include/SDL.h"
#include "SDL/include/SDL_gamecontroller.h"

ModuleUI::ModuleUI()
{
	lifeBars.x = 0;
	lifeBars.y = 0;
	lifeBars.w = 310;
	lifeBars.h = 18;

	yelllowBar1.x = 0;
	yelllowBar1.y = 18;
	yelllowBar1.w = 136;
	yelllowBar1.h = 10;

	yelllowBar2.x = 0;
	yelllowBar2.y = 18;
	yelllowBar2.w = 136;
	yelllowBar2.h = 10;

	iconRoundWinned.x = 61;
	iconRoundWinned.y = 38;
	iconRoundWinned.w = 18;
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

	typography1 = App->fonts->Load("assets/images/ui/Font_name.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789~·!@#$%^&*()-+=[]{}|:;ç'<>,./? ", 1);
	numbers = App->fonts->Load("assets/images/ui/Font_count.png", "0123456789<> ", 1);
	graphics = App->textures->Load("assets/images/ui/fight_ui.png");

	round_snd = App->audio->LoadChunk("assets/sfx/voices/announcer_round.wav");
	one_snd = App->audio->LoadChunk("assets/sfx/voices/announcer_1.wav");
	two_snd = App->audio->LoadChunk("assets/sfx/voices/announcer_2.wav");
	three_snd = App->audio->LoadChunk("assets/sfx/voices/announcer_3.wav");
	final_snd = App->audio->LoadChunk("assets/sfx/voices/announcer_final.wav");
	fight_snd = App->audio->LoadChunk("assets/sfx/voices/announcer_fight.wav");
	you_snd = App->audio->LoadChunk("assets/sfx/voices/announcer_you.wav");
	win_snd = App->audio->LoadChunk("assets/sfx/voices/announcer_win.wav");
	lose_snd = App->audio->LoadChunk("assets/sfx/voices/announcer_lose.wav");
	perfect_snd = App->audio->LoadChunk("assets/sfx/voices/announcer_perfect.wav");
	scoreUp_snd = App->audio->LoadChunk("assets/sfx/effects/score_up.wav");

	timerStarted = redKoEnabled = starFight = roundSoundPlayed = numberRoundSoundPlayed = fightSoundPlayed = youFinalSound = winLoseFinalSound = perfectSound = gotBonus = App->fight->stopedFight = false;

	intTimeBonusPuntuation = intVitalBonusPuntuation = intBonusPuntuation = totalBonus = 0;

	return ret;
}

// Load assets
bool ModuleUI::CleanUp()
{
	App->audio->UnloadChunk(scoreUp_snd);
	scoreUp_snd = nullptr;
	App->audio->UnloadChunk(perfect_snd);
	perfect_snd = nullptr;
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

	App->textures->Unload(graphics);
	App->fonts->UnLoad(numbers);
	App->fonts->UnLoad(typography1);

	graphics = nullptr;

	return true;
}

// Update: draw background
update_status ModuleUI::PostUpdate()
{
	if (App->input->keyboard[SDL_SCANCODE_KP_PLUS] == KEY_STATE::KEY_DOWN) {
		App->player1->puntuation += 51;
		App->player2->puntuation += 51;
	}

	LifeBarsBlit();
	TimerBlit(numbers);
	RoundsWinnedBlit();
	StartFightBlit();
	EndFight();
	BlitPuntuation();

	if (App->input->keyboard[SDL_SCANCODE_F2] == KEY_STATE::KEY_DOWN) {
		debugGamepads = !debugGamepads;
	}

	if (debugGamepads) {
		BlitGamePadDebug();
	}

	return UPDATE_CONTINUE;
}

void ModuleUI::TimerBlit(int font_id) {

	if (font_id < 0 || font_id >= MAX_FONTS || App->fonts->fonts[font_id].graphic == nullptr) {
		LOG("Unable to render text with bmp font id %d", font_id);
		return;
	}

	int timeRemaining = App->fight->GetTimer();

	if (App->fight->stopedFight) {
		tiempo[0] = (char)48 + (stopedTimer / 10);
		tiempo[1] = (char)48 + (stopedTimer % 10);
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + LIFE_BAR_LENGHT + 42, 36, font_id, tiempo);
	}
	else if (timerStarted && timeRemaining > 0) {
		tiempo[0] = (char)48 + (timeRemaining / 10);
		tiempo[1] = (char)48 + (timeRemaining % 10);
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + LIFE_BAR_LENGHT + 42, 36, font_id, tiempo);
	}
	else {
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + LIFE_BAR_LENGHT + 42, 36, font_id, "99");
		timerStarted = false;
	}
}

void ModuleUI::LifeBarsBlit() {
	App->render->Blit(graphics, -App->render->camera.x / SCREEN_SIZE + 37, 17, &lifeBars, false);
	yelllowBar1.x = yelllowBar1.w - App->player1->life*1.36;
	App->render->Blit(graphics, -App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH / 2 - 154, 21, &yelllowBar1, true);

	yelllowBar2.w = App->player2->life*1.36;
	App->render->Blit(graphics, -App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH / 2 + 18, 21, &yelllowBar2, false);

	App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + 39, 33, typography1, "DHALSIM");
	App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH - 122, 33, typography1, "DHALSIM");

}

void ModuleUI::RoundsWinnedBlit() {

	if (App->fight->player1RoundWinned == 1)
		App->render->Blit(graphics, -App->render->camera.x / SCREEN_SIZE + 0, 13, &iconRoundWinned, false);
	else if (App->fight->player1RoundWinned == 2) {
		App->render->Blit(graphics, -App->render->camera.x / SCREEN_SIZE + 0, 12, &iconRoundWinned, false);
		App->render->Blit(graphics, -App->render->camera.x / SCREEN_SIZE + 18, 12, &iconRoundWinned, false);
	}

	if (App->fight->player2RoundWinned == 1)
		App->render->Blit(graphics, -App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH - iconRoundWinned.w - 2, 12, &iconRoundWinned, false);
	else if (App->fight->player2RoundWinned == 2) {
		App->render->Blit(graphics, -App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH - iconRoundWinned.w - 2, 12, &iconRoundWinned, false);
		App->render->Blit(graphics, -App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH - iconRoundWinned.w - 19, 12, &iconRoundWinned, false);
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
			App->render->Blit(graphics, -App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH / 2 - 45, SCREEN_HEIGHT / 2 - roundRect.h, &roundRect, false);
			if (App->fight->round == 1)
				App->render->Blit(graphics, -App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH / 2 + 35, SCREEN_HEIGHT / 2 - round1Rect.h, &round1Rect, false);
			else if (App->fight->round == 2)
				App->render->Blit(graphics, -App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH / 2 + 35, SCREEN_HEIGHT / 2 - round2Rect.h, &round2Rect, false);
			else if (App->fight->round == 3)
				App->render->Blit(graphics, -App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH / 2 + 35, SCREEN_HEIGHT / 2 - round3Rect.h, &round3Rect, false);
			else if (App->fight->round > 3)
				App->render->Blit(graphics, -App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH / 2 - 45 - roundFRect.w, SCREEN_HEIGHT / 2 - roundFRect.h, &roundFRect, false);
		}
		else if (timeRemaining >= 1) {
			App->render->Blit(graphics, -App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH / 2 - 45, SCREEN_HEIGHT / 2 - fightRect.h, &fightRect, false);
		}
		else {
			starFight = false;
			App->player1->freeze = false;
			App->player2->freeze = false;
			App->fight->roundStarted = true;
			StartTimer();
		}
	}
}

void ModuleUI::StartEndFight(int player, bool timeOut) {
	stopedTimer = App->fight->GetTimer();
	winnerPlayer = player;
}

void ModuleUI::EndFight() {
	if (App->fight->endFightStarted) {
		int timeRemaining = (App->fight->endFightTimer - SDL_GetTicks()) / 1000;

		if (timeRemaining == 12 && !youFinalSound) {
			App->audio->PlayChunk(you_snd);

			if (winnerPlayer == 1) {
				App->player1->inputs.Push(IN_VICTORY);
				App->player2->inputs.Push(IN_LOOSE2);
			}
			else if (winnerPlayer == 2) {
				App->player1->inputs.Push(IN_LOOSE);
				App->player2->inputs.Push(IN_VICTORY2);
			}

			youFinalSound = true;
		}
		else if (timeRemaining == 11 && !winLoseFinalSound) {
			if (winnerPlayer == 1)
				App->audio->PlayChunk(win_snd);
			else if (winnerPlayer == 2)
				App->audio->PlayChunk(lose_snd);
			winLoseFinalSound = true;
		}

		else if (timeRemaining == 10 && !perfectSound) {
			if (App->player1->life == 100 || App->player2->life == 100)
				App->audio->PlayChunk(perfect_snd);
			perfectSound = true;
		}

		if (timeRemaining > 0 && timeRemaining >= 11 && App->frames % 4 != 0)
			if (winnerPlayer == 1)
				App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 - 30, typography1, "PLAYER 1 WIN");
			else if (winnerPlayer == 2)
				App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 - 30, typography1, "PLAYER 2 WIN");

		if (timeRemaining < 11) {
			GetBonusPuntuations(timeRemaining < 9);
			if (timeRemaining < 11) {
				App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH / 2 - 70, 55, typography1, "TIME");
				App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH / 2 + 66 - strlen(timeBonusPuntuation) * 16, 55, numbers, timeBonusPuntuation);
			}

			if (timeRemaining < 10) {
				App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH / 2 - 82, 75, typography1, "VITAL");
				App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH / 2 + 66 - strlen(vitalBonusPuntuation) * 16, 75, numbers, vitalBonusPuntuation);
			}

			if (timeRemaining < 9) {
				App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH / 2 - 82, 100, typography1, "BONUS");
				App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH / 2 + 66 - strlen(bonusPuntuation) * 16, 100, numbers, bonusPuntuation);
			}
		}
	}
}

void ModuleUI::BlitGamePadDebug() {

#pragma region Player 1
	if (App->input->pads[0].a)
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + 10, SCREEN_HEIGHT / 2 - 30, typography1, "A");

	if (App->input->pads[0].x)
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + 10, SCREEN_HEIGHT / 2 - 10, typography1, "X");

	if (App->input->pads[0].y)
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + 10, SCREEN_HEIGHT / 2 + 10, typography1, "Y");

	if (App->input->pads[0].b)
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + 10, SCREEN_HEIGHT / 2 + 30, typography1, "B");

	if (App->input->pads[0].R1)
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + 10, SCREEN_HEIGHT / 2 + 50, typography1, "R1");

	if (App->input->pads[0].R2)
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + 20, SCREEN_HEIGHT / 2 + 50, typography1, "R2");


	if (App->input->pads[0].up)
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + 20, SCREEN_HEIGHT / 2 - 30, typography1, "UP");


	if (App->input->pads[0].down)
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + 20, SCREEN_HEIGHT / 2 - 30, typography1, "DOWN");


	if (App->input->pads[0].left)
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + 50, SCREEN_HEIGHT / 2 - 30, typography1, "LEFT");


	if (App->input->pads[0].right)
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + 50, SCREEN_HEIGHT / 2 - 30, typography1, "RIGHT");
#pragma endregion

#pragma region Player 2
	if (App->input->pads[1].a)
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH - 10, SCREEN_HEIGHT / 2 - 30, typography1, "A");

	if (App->input->pads[1].x)
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH - 10, SCREEN_HEIGHT / 2 - 10, typography1, "X");

	if (App->input->pads[1].y)
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH - 10, SCREEN_HEIGHT / 2 + 10, typography1, "Y");

	if (App->input->pads[1].b)
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH - 10, SCREEN_HEIGHT / 2 + 30, typography1, "B");

	if (App->input->pads[1].R1)
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH - 10, SCREEN_HEIGHT / 2 + 50, typography1, "R1");

	if (App->input->pads[1].R2)
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH - 20, SCREEN_HEIGHT / 2 + 50, typography1, "R2");


	if (App->input->pads[1].up)
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH - 20, SCREEN_HEIGHT / 2 - 30, typography1, "UP");


	if (App->input->pads[1].down)
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH - 20, SCREEN_HEIGHT / 2 - 30, typography1, "DOWN");


	if (App->input->pads[1].left)
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH - 50, SCREEN_HEIGHT / 2 - 30, typography1, "LEFT");


	if (App->input->pads[1].right)
		App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH - 50, SCREEN_HEIGHT / 2 - 30, typography1, "RIGHT");
#pragma endregion	
}

void ModuleUI::GetPuntuations() {
	std::string stPlayer1Puntuation = std::to_string(App->player1->puntuation);
	std::string stPlayer2Puntuation = std::to_string(App->player2->puntuation);

	player1Puntuation = new char[stPlayer1Puntuation.length() + 1];
	strcpy_s(player1Puntuation, stPlayer1Puntuation.length() + 1, stPlayer1Puntuation.c_str());

	player2Puntuation = new char[stPlayer2Puntuation.length() + 1];
	strcpy_s(player2Puntuation, stPlayer2Puntuation.length() + 1, stPlayer2Puntuation.c_str());
}

void ModuleUI::BlitPuntuation() {
	GetPuntuations();
	App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + 10, 0, typography1, "1P");
	App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + 120 - strlen(player1Puntuation) * 12, 0, typography1, player1Puntuation);
	App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + 260, 0, typography1, "2P");
	App->fonts->BlitText(-App->render->camera.x / SCREEN_SIZE + 380 - strlen(player2Puntuation) * 12, 0, typography1, player2Puntuation);
}

void ModuleUI::GetBonusPuntuations(bool sum) {
	if (!gotBonus) {
		intTimeBonusPuntuation = stopedTimer * 100;
		intVitalBonusPuntuation = 30000 * ((float)(winnerPlayer == 1 ? App->player1->life : App->player2->life) / 100);
		totalBonus = intTimeBonusPuntuation + intVitalBonusPuntuation;

		std::string stTimeBonusPuntuation = std::to_string(intTimeBonusPuntuation);
		std::string stVitalBonusPuntuation = std::to_string(intVitalBonusPuntuation);

		timeBonusPuntuation = new char[stTimeBonusPuntuation.length() + 1];
		strcpy_s(timeBonusPuntuation, stTimeBonusPuntuation.length() + 1, stTimeBonusPuntuation.c_str());

		vitalBonusPuntuation = new char[stVitalBonusPuntuation.length() + 1];
		strcpy_s(vitalBonusPuntuation, stVitalBonusPuntuation.length() + 1, stVitalBonusPuntuation.c_str());

		gotBonus = true;
	}

	if (sum && totalBonus > 0) {
		if (App->frames % 8 == 0)
			App->audio->PlayChunk(scoreUp_snd);

		totalBonus -= 100;
		intBonusPuntuation += 100;

		if (winnerPlayer == 1)
			App->player1->puntuation += 100;
		else
			App->player2->puntuation += 100;
	}

	std::string stBonusPuntuation = std::to_string(intBonusPuntuation);

	bonusPuntuation = new char[stBonusPuntuation.length() + 1];
	strcpy_s(bonusPuntuation, stBonusPuntuation.length() + 1, stBonusPuntuation.c_str());
}