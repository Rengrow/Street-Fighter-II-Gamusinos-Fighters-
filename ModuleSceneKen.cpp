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
	
	// ground
	ground.x = 0;
	ground.y = 878;
	ground.w = 914;
	ground.h = 58;

	ground2.x = 0;
	ground2.y = 878;
	ground2.w = 951;
	ground2.h = 8;
	// big ship
	foreground.x = 0;
	foreground.y = 187;
	foreground.w = 523;
	foreground.h = 184;
	
	//sky
	background.x = 0;
	background.y = 0;
	background.w = 572;
	background.h = 179;
	
	// flag animation
	flag.PushBack({ 578, 6, 38, 27 }, 0.08f);
	flag.PushBack({ 578, 36, 38, 27 }, 0.08f);
	flag.PushBack({ 578, 66, 38, 27 }, 0.08f);

	// Girl Animation
	girl.PushBack({ 714, 6, 32, 48 }, 0.05f);
	girl.PushBack({ 714, 60, 32, 47 }, 0.05f);
	girl.PushBack({ 714, 6, 32, 48 }, 0.05f);
	girl.PushBack({ 714, 115, 32, 47 }, 0.05f);
	
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

	graphics = App->textures->Load("assets/images/sprites/stages/KenSagatStage.png");
	music = App->audio->LoadSong("assets/music/usa_k_1.ogg");
	
	App->player->Enable();
	App->player2->Enable();
	App->particles->Enable();
	App->collisions->Enable();
	App->audio->PlaySongDelay(music, -1, 2000);

	App->render->limit1Box.x = 0;
	App->render->limit1Box.y = 0;
	App->render->limit1Box.w = 3;
	App->render->limit1Box.h = SCREEN_HEIGHT;

	App->render->limit2Box.x = SCREEN_WIDTH - 3;
	App->render->limit2Box.y = 0;
	App->render->limit2Box.w = 3;
	App->render->limit2Box.h = SCREEN_HEIGHT;

	App->render->limit1 = App->collisions->AddCollider(App->render->limit1Box, COLLIDER_WALL);
	App->render->limit2 = App->collisions->AddCollider(App->render->limit2Box, COLLIDER_WALL);
	App->render->scenelimit = -780;

	return true;
}

// UnLoad assets
bool ModuleSceneKen::CleanUp()
{
	LOG("Unloading ken scene");

	App->player->Disable();
	App->player2->Disable();
	App->particles->Disable();
	App->collisions->Disable();

	App->textures->Unload(graphics);
	graphics = nullptr;

	App->audio->UnloadSong(music);
	music = nullptr;
	
	return true;
}

// Update: draw background
update_status ModuleSceneKen::Update()
{
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
	App->render->Blit(graphics, 469, 18, &(flag.GetCurrentFrame()), false, 0.75f); // flag animation

	App->render->Blit(graphics, 0, (int)foreground_pos, &foreground, false, 0.92f);
	App->render->Blit(graphics, 187, 112 + (int)foreground_pos, &(girl.GetCurrentFrame()), false, 0.92f); // girl animation

	App->render->Blit(graphics, 0, 170, &ground, false);

	if (App->input->keyboard[SDL_SCANCODE_SPACE] == 1) {
		Mix_FadeOutMusic(2000);
		App->fade->FadeToBlack(this, (Module*)App->scene_Sagat, 5);
	}

	return UPDATE_CONTINUE;
}