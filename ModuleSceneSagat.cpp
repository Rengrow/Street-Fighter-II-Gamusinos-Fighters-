#include "Globals.h"
#include "Application.h"
#include "ModuleSceneSagat.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleSecondPlayer.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModuleAudio.h"
#include "ModuleParticles.h"
#include "ModuleCollision.h"
// Reference at https://youtu.be/6OlenbCC4WI?t=382

ModuleSceneSagat::ModuleSceneSagat()
{
	// ground
	ground = {8, 376, 848, 64};

	// roof
	roof = {91, 7, 765, 49};

	// foreground
	foreground = {164, 66, 336, 51};

	// Background / sky
	background = {760, 1443, 508, 145};

	// flag animation
	water.PushBack({8, 447, 283, 9}, 0.02f);
	water.PushBack({296, 447, 283, 12}, 0.02f);
	water.PushBack({588, 447, 283, 18}, 0.02f);
}

ModuleSceneSagat::~ModuleSceneSagat()
{}

// Load assets
bool ModuleSceneSagat::Start()
{
	LOG("Loading background assets");
	bool ret = true;
	graphics = App->textures->Load("/assets/images/sprites/stages/stages_sagat.png");
	music = App->audio->LoadSong("assets/music/thailand_s_1.ogg");

	App->player->Enable();
	App->player2->Enable();
	App->particles->Enable();
	App->collisions->Enable();
	App->audio->PlaySongDelay(music, -1, 2000);

	return ret;
}

// Unload assets
bool ModuleSceneSagat::CleanUp()
{
	LOG("Unloading Sagat stage");

	App->player->Disable();
	App->player2->Disable();

	App->textures->Unload(graphics);

	App->audio->UnloadSong(music);

	music = nullptr;

	return true;
}

// Update: draw background
update_status ModuleSceneSagat::Update()
{
	// Draw everything --------------------------------------	
	App->render->Blit(graphics, 0, 160, &ground, false);
	App->render->Blit(graphics, 50, -15, &background, 0.75f); // back of the room
	
	App->render->Blit(graphics, 280, 125, &foreground, false);
	App->render->Blit(graphics, 305, 136, &(water.GetCurrentFrame()), false); // water animation
	App->render->Blit(graphics, 0, -16, &roof, false, 0.75f);

	if (App->input->keyboard[SDL_SCANCODE_SPACE] == 1) {
		Mix_FadeOutMusic(2000);
		App->fade->FadeToBlack(this, (Module*)App->endBattle, 5);
	}

	return UPDATE_CONTINUE;
}