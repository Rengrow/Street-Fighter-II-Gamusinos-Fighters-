#include "Globals.h"
#include "Application.h"
#include "ModuleFight.h"
#include "ModulePlayer.h"
#include "ModuleSecondPlayer.h"
#include "ModuleUI.h"
#include "ModuleSceneKen.h"
#include "ModuleParticles.h"
#include "ModuleCollision.h"
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
	App->player->Enable();
	App->player2->Enable();
	App->particles->Enable();
	App->collisions->Enable();
	App->UI->Enable();

	roundStarted = false;

	App->UI->StartFight();

	return true;
}

update_status ModuleFight::Update()
{
	if (roundStarted)
		if (App->player2->life <= 0 || (App->UI->GetTimer() <= 0 && App->player->life > App->player2->life))
			Win(1);
		else if (App->player->life <= 0 || (App->UI->GetTimer() <= 0 && App->player2->life > App->player->life))
			Win(2);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleFight::CleanUp()
{
	App->player->Disable();
	App->player2->Disable();
	App->particles->Disable();
	App->collisions->Disable();
	App->UI->Disable();
	return true;
}

void ModuleFight::Win(int player) {
	App->player->freeze = true;
	App->player2->freeze = true;

	App->UI->StartEndFight(player);
}

