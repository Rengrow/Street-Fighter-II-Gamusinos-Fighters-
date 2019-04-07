#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleSceneKen.h"
#include "ModulePlayer.h"
#include "ModuleSecondPlayer.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModuleAudio.h"
#include "ModuleParticles.h"
#include "ModuleCollision.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModuleSceneKen::ModuleSceneKen()
{
	//wall (for colliders)

	wall.x = 0;
	wall.y = 0;
	wall.w = 3;
	wall.h = 224;

	//wall2 (for colliders)

	wall2.x = 381;
	wall2.y = 0;
	wall2.w = 3;
	wall2.h = 234;

	// ground
	ground.x = 8;
	ground.y = 391;
	ground.w = 896;
	ground.h = 72;

	// foreground
	foreground.x = 8;
	foreground.y = 24;
	foreground.w = 521;
	foreground.h = 181;

	// Background / sky
	background.x = 72;
	background.y = 208;
	background.w = 768;
	background.h = 176;

	// flag animation
	flag.PushBack({ 848, 208, 40, 40 }, 0.08f);
	flag.PushBack({ 848, 256, 40, 40 }, 0.08f);
	flag.PushBack({ 848, 304, 40, 40 }, 0.08f);

	// Girl Animation
	girl.PushBack({ 624, 16, 32, 56 }, 0.05f);
	girl.PushBack({ 624, 80, 32, 56 }, 0.05f);
	girl.PushBack({ 624, 144, 32, 56 }, 0.05f);

	// for moving the foreground
	foreground_pos = 0;
	forward = true;
}

ModuleSceneKen::~ModuleSceneKen()
{}

// Load assets
bool ModuleSceneKen::Start()
{
	LOG("Loading ken scene");

	graphics = App->textures->Load("ken_stage.png");
	music = App->audio->LoadSong("assets/music/usa_k_1.ogg");
	collider = App->collisions->AddCollider(wall, COLLIDER_WALL, this);
	collider2 = App->collisions->AddCollider(wall2, COLLIDER_WALL, this);
	
	App->player->Enable();
	App->player2->Enable();
	App->particles->Enable();
	App->collisions->Enable();
	App->audio->PlaySongDelay(music, -1, 2000);


	return true;
}

// UnLoad assets
bool ModuleSceneKen::CleanUp()
{
	LOG("Unloading ken scene");

	App->player->Disable();
	App->player2->Disable();

	App->textures->Unload(graphics);

	App->audio->UnloadSong(music);

	music = nullptr;
	
	return true;
}

// Update: draw background
update_status ModuleSceneKen::Update()
{
	if (collider != nullptr) {
		collider->SetPos(wall.x, wall.y);
	}
	if (collider2 != nullptr) {
		collider2->SetPos(wall2.x, wall2.y);
	}
	// Calculate boat Y position -----------------------------
	if (foreground_pos < -6.0f)
		forward = false;
	else if (foreground_pos > 0.0f)
		forward = true;

	if (forward)
		foreground_pos -= 0.02f;
	else
		foreground_pos += 0.02f;

	// Draw everything --------------------------------------
	App->render->Blit(graphics, 0, 0, &background, false, 0.75f); // sea and sky
	App->render->Blit(graphics, 560, 8, &(flag.GetCurrentFrame()), false, 0.75f); // flag animation

	App->render->Blit(graphics, 0, (int)foreground_pos, &foreground, false, 0.92f);
	App->render->Blit(graphics, 192, 104 + (int)foreground_pos, &(girl.GetCurrentFrame()), false, 0.92f); // girl animation

	App->render->Blit(graphics, 0, 170, &ground, false);

	if (App->input->keyboard[SDL_SCANCODE_SPACE] == 1) {
		Mix_FadeOutMusic(2000);
		App->fade->FadeToBlack(this, (Module*)App->scene_honda, 5);
	}

	return UPDATE_CONTINUE;
}