#include "Globals.h"
#include "Application.h"
#include "ModuleSceneHonda.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"

// Reference at https://youtu.be/6OlenbCC4WI?t=382

ModuleSceneHonda::ModuleSceneHonda()
{
	// ground
	ground = {8, 376, 848, 64};

	// roof
	roof = {91, 7, 765, 49};

	// foreground
	foreground = {164, 66, 336, 51};

	// Background / sky
	background = {120, 128, 671, 199};

	// flag animation
	water.PushBack({8, 447, 283, 9});
	water.PushBack({296, 447, 283, 12});
	water.PushBack({588, 447, 283, 18});
	water.speed = 0.02f;
}

ModuleSceneHonda::~ModuleSceneHonda()
{}

// Load assets
bool ModuleSceneHonda::Start()
{
	LOG("Loading background assets");
	bool ret = true;
	graphics = App->textures->Load("honda_stage2.png");

	// TODO 1: Enable (and properly disable) the player module
	App->player->Enable();

	return ret;
}

// Unload assets
bool ModuleSceneHonda::CleanUp()
{
	LOG("Unloading honda stage");

	App->player->Disable();

	App->textures->Unload(graphics);

	return true;
}

// Update: draw background
update_status ModuleSceneHonda::Update()
{
	// Draw everything --------------------------------------	
	App->render->Blit(graphics, 0, 160, &ground);
	App->render->Blit(graphics, 50, -15, &background, 0.75f); // back of the room
	
	App->render->Blit(graphics, 280, 125, &foreground);
	App->render->Blit(graphics, 305, 136, &(water.GetCurrentFrame())); // water animation
	App->render->Blit(graphics, 0, -16, &roof, 0.75f);

	if (App->input->keyboard[SDL_SCANCODE_SPACE] == 1)
		App->fade->FadeToBlack(this, (Module*)App->endBattle, 5);

	return UPDATE_CONTINUE;
}