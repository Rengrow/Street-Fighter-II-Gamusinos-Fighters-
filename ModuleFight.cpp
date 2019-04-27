#include "Globals.h"
#include "Application.h"
#include "ModuleFight.h"
#include "ModulePlayer.h"
#include "ModuleSecondPlayer.h"
#include "ModuleUI.h"
#include "ModuleSceneKen.h"
#include "ModuleParticles.h"
#include "ModuleCollision.h"
#include"ModuleFadeToBlack.h"
#include "ModuleInput.h"
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
	App->scene_ken->Enable();

	roundStarted = stopedFight = endFightStarted = false;

	player1RoundWinned = player2RoundWinned = 0;
	round = 1;

	return true;
}

update_status ModuleFight::Update()
{
	if (roundStarted)
		if ((!endFightStarted && (App->player2->life <= 0 || (GetTimer() <= 0 && App->player->life > App->player2->life))) || App->input->keyboard[SDL_SCANCODE_F10] == KEY_STATE::KEY_DOWN)
			Win(1);
		else if ((!endFightStarted && (App->player->life <= 0 || (GetTimer() <= 0 && App->player2->life > App->player->life))) || App->input->keyboard[SDL_SCANCODE_F11] == KEY_STATE::KEY_DOWN)
			Win(2);
		else if ((!endFightStarted && (App->player2->life <= 0 || (GetTimer() <= 0 && App->player->life == App->player2->life))))
			Win(1);

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
		}
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleFight::CleanUp()
{
	App->scene_ken->Disable();
	return true;
}

int ModuleFight::GetTimer() {
	return (timeOutTimer - SDL_GetTicks()) / 1000;
}

void ModuleFight::StartNewRound() {
	endFightStarted = stopedFight = roundStarted = false;
	App->scene_ken->StopMusic(2000);
	App->fade->FadeToBlack((Module*)App->scene_ken, (Module*)App->scene_ken, 2);
}

void ModuleFight::EndFullFight() {
	App->scene_ken->StopMusic(2000);
	App->fade->FadeToBlack(this, (Module*)App->endBattle, 2);
}

void ModuleFight::Win(int player) {
	App->player->freeze = true;
	App->player2->freeze = true;

	if (player == 1) {
		player1RoundWinned++;
		//player -> win
	}
	else if (player == 2) {
		player2RoundWinned++;
		//player2 -> win
	}

	round++;

	endFightTimer = SDL_GetTicks() + 5000; //5 Seconds

	endFightStarted = stopedFight = true;

	App->UI->StartEndFight(player);
}

