#include "Globals.h"
#include "Application.h"
#include "ModuleFight.h"
#include "ModulePlayer1.h"
#include "ModulePlayer2.h"
#include "ModuleUI.h"
#include "ModuleSceneKen.h"
#include "ModuleSceneSagat.h"
#include "ModuleParticles.h"
#include "ModuleCollision.h"
#include"ModuleFadeToBlack.h"
#include "ModuleInput.h"
#include "ModuleSlowdown.h"
#include "SDL/include/SDL.h"


ModuleFight::ModuleFight() : Module()
{
}

// Destructor
ModuleFight::~ModuleFight()
{
}

bool ModuleFight::Start()
{
	//if (SDL_GetTicks() % 2 == 0)
		stage = App->scene_Sagat;
	//else
	//	stage = App->scene_ken;

	stage->Enable();

	roundStarted = stopedFight = endFightStarted = false;
	player1RoundWinned = player2RoundWinned = autoWinLoseTimer = 0;
	round = 1;

	return true;
}

update_status ModuleFight::Update()
{
	if (roundStarted)
		if ((!endFightStarted && (App->player2->life <= 0 || (GetTimer() <= 0 && App->player1->life > App->player2->life))) || App->input->keyboard[SDL_SCANCODE_F10] == KEY_STATE::KEY_DOWN && autoWinLoseTimer < SDL_GetTicks()) {
			if (GetTimer() <= 0)
				App->UI->timeOver = true;
			Win(1);
			autoWinLoseTimer = SDL_GetTicks() + 16000;
		}
		else if ((!endFightStarted && (App->player1->life <= 0 || (GetTimer() <= 0 && App->player2->life > App->player1->life))) || App->input->keyboard[SDL_SCANCODE_F11] == KEY_STATE::KEY_DOWN && autoWinLoseTimer < SDL_GetTicks()) {
			if (GetTimer() <= 0)
				App->UI->timeOver = true;
			Win(2);
			autoWinLoseTimer = SDL_GetTicks() + 16000;
		}
		else if ((!endFightStarted && ((App->player1->life == 0 && App->player2->life == 0) || (GetTimer() <= 0 && App->player1->life == App->player2->life))) || App->input->keyboard[SDL_SCANCODE_F9] == KEY_STATE::KEY_DOWN && autoWinLoseTimer < SDL_GetTicks()) {
			if (GetTimer() > 0 && App->player1->life == 0 && App->player2->life == 0)
				App->UI->doubleKO = true;
			else if (GetTimer() <= 0 && App->player1->life == App->player2->life)
				App->UI->timeOver = true;
			Win(0);
			autoWinLoseTimer = SDL_GetTicks() + 7000;
		}

	CheckFlipPlayers();
	if (endFightStarted) {
		if (((App->fight->endFightTimer - SDL_GetTicks()) / 1000) == 0) {
			if (App->UI->winnerPlayer == 1 && App->fight->player1RoundWinned == 1) {
				StartNewRound();
			}
			else if (App->UI->winnerPlayer == 2 && App->fight->player2RoundWinned == 1) {
				StartNewRound();
			}
			else if (App->UI->winnerPlayer == 1 && App->fight->player1RoundWinned == 2) {
				EndFullFight();
			}
			else if (App->UI->winnerPlayer == 2 && App->fight->player2RoundWinned == 2) {
				EndFullFight();
			}
			else if (round == 5) {
				EndFullFight();
			}
			else if (App->UI->winnerPlayer == 0 && App->fight->player1RoundWinned == 1 && App->fight->player2RoundWinned == 1) {
				StartNewRound();
			}			
			else if (App->UI->winnerPlayer == 0 && App->fight->player1RoundWinned == 2 && App->fight->player2RoundWinned == 2) {
				StartNewRound();
			}
			else if (App->UI->winnerPlayer == 0 && (App->fight->player1RoundWinned == 2 || App->fight->player2RoundWinned == 2)) {
				EndFullFight();
			}
			else
				StartNewRound();
		}
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleFight::CleanUp()
{
	if (stage != nullptr)
		stage->Disable();
	stage = nullptr;

	return true;
}

int ModuleFight::GetTimer() {
	return (timeOutTimer - SDL_GetTicks()) / 1000;
}

void ModuleFight::StartNewRound() {
	endFightStarted = roundStarted = false;
	stage->StopMusic(2000);
	App->fade->FadeToBlack((Module*)stage, (Module*)stage, 2);
}

void ModuleFight::EndFullFight() {
	stage->StopMusic(2000);
	App->fade->FadeToBlack(this, (Module*)App->endBattle, 2);
}

void ModuleFight::Win(int player) {
	App->player1->freeze = true;
	App->player2->freeze = true;
	App->slowdown->StartSlowdown(30, 100);

	if (player == 1)
		player1RoundWinned++;
	else if (player == 2)
		player2RoundWinned++;
	else if (player == 0) {
		if (player1RoundWinned != 1 && player2RoundWinned != 1) {
			player1RoundWinned++;
			player2RoundWinned++;
		}
	}
	round++;

	if (player != 0)
		endFightTimer = SDL_GetTicks() + 13000; //13 Seconds
	else
		endFightTimer = SDL_GetTicks() + 5000; //13 Seconds

	endFightStarted = stopedFight = true;

	App->UI->StartEndFight(player, GetTimer() <= 0);
}

void ModuleFight::CheckFlipPlayers() {
	if (App->player1->position.x > App->player2->position.x && left == false && SDL_GetTicks() && !App->player1->freeze && !App->player2->freeze) {
		App->player1->turn = true;
		App->player2->turn = true;
		right = false;
		left = true;
	}

	if (App->player1->position.x < App->player2->position.x && right == false && SDL_GetTicks() && !App->player1->freeze && !App->player2->freeze) {
		App->player1->turn = true;
		App->player2->turn = true;
		right = true;
		left = false;
	}

	if (App->player1->turn)
	{
		App->player1->inputs.Push(IN_TURNING);
	}

	if (App->player2->turn)
	{
		App->player2->inputs.Push(IN_TURNING2);
	}
}

