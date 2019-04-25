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
#include "ModuleUI.h"


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

	//box
	box.x = 854;
	box.y = 6;
	box.w = 155;
	box.h = 99;

	//Little thing of metal
	little.x = 577;
	little.y = 96;
	little.w = 22;
	little.h = 17;

	//Big thing of metal
	big.x = 577;
	big.y = 116;
	big.w = 32;
	big.h = 25;

	// flag animation
	flag.PushBack({ 578, 6, 38, 27 }, 10, { 0,0 }, 0, {}, {}, {});
	flag.PushBack({ 578, 36, 38, 27 }, 10, { 0,0 }, 0, {}, {}, {});
	flag.PushBack({ 578, 66, 38, 27 }, 10, { 0,0 }, 0, {}, {}, {});

	// Girl Animation
	girl.PushBack({ 714, 6, 32, 48 }, 10, { 0,0 }, 0, {}, {}, {});
	girl.PushBack({ 714, 61, 32, 48 }, 10, { 0,0 }, 0, {}, {}, {});
	girl.PushBack({ 714, 6, 32, 48 }, 10, { 0,0 }, 0, {}, {}, {});
	girl.PushBack({ 714, 116, 32, 48 }, 10, { 0,0 }, 0, {}, {}, {});

	// Purple guy animation
	prGuy.PushBack({ 621, 6, 41, 33 }, 10, { 0,0 }, 0, {}, {}, {});
	prGuy.PushBack({ 621, 46, 41, 33 }, 10, { 0,0 }, 0, {}, {}, {});
	prGuy.PushBack({ 621, 86, 41, 33 }, 10, { 0,0 }, 0, {}, {}, {});

	//Hat animation
	greyHat.PushBack({ 667, 144, 17, 13 }, 10, { 0,0 }, 0, {}, {}, {});
	greyHat.PushBack({ 667, 164, 15, 13 }, 10, { 0,0 }, 0, {}, {}, {});

	//Brown Hat
	brownHat.PushBack({ 577, 144, 22, 11 }, 12, { 0,0 }, 0, {}, {}, {});
	brownHat.PushBack({ 577, 158, 25, 11 }, 12, { 0,0 }, 0, {}, {}, {});
	brownHat.PushBack({ 577, 172, 31, 11 }, 12, { 0,0 }, 0, {}, {}, {});

	//Bald guy
	baldGuy.PushBack({ 667, 75, 41, 63 }, 10, { 0,0 }, 0, {}, {}, {});
	baldGuy.PushBack({ 621, 126, 41, 63 }, 10, { 0,0 }, 0, {}, {}, {});
	baldGuy.PushBack({ 667, 6, 41, 63 }, 10, { 0,0 }, 0, {}, {}, {});
	baldGuy.PushBack({ 621, 126, 41, 63 }, 10, { 0,0 }, 0, {}, {}, {});

	//Blue Guy
	blueGuy.PushBack({ 752, 80, 47, 54 }, 10, { 0,0 }, 0, {}, {}, {});
	blueGuy.PushBack({ 752, 136, 47, 54 }, 10, { 0,0 }, 0, {}, {}, {});
	blueGuy.PushBack({ 802, 6, 47, 54 }, 10, { 0,0 }, 0, {}, {}, {});
	blueGuy.PushBack({ 752, 136, 47, 54 }, 10, { 0,0 }, 0, {}, {}, {});

	//Blue guy 2
	blueGuy2.PushBack({ 752, 6, 33, 30 }, 10, { 0,0 }, 0, {}, {}, {});
	blueGuy2.PushBack({ 752, 43, 33, 30 }, 10, { 0,0 }, 0, {}, {}, {});

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
	App->UI->Enable();
	App->audio->PlaySongDelay(music, -1, 2000);

	App->render->limit1Box.x = 0;
	App->render->limit1Box.y = 0;
	App->render->limit1Box.w = 3;
	App->render->limit1Box.h = SCREEN_HEIGHT;

	App->render->limit2Box.x = SCREEN_WIDTH - 3;
	App->render->limit2Box.y = 0;
	App->render->limit2Box.w = 3;
	App->render->limit2Box.h = SCREEN_HEIGHT;

	App->render->scenelimit = 620;


	App->render->camera.x = App->render->camera.y = 0;

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
	App->UI->Disable();

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
	App->render->Blit(graphics, 469, 18, &(flag.GetCurrentFrameBox()), false, 0.75f); // flag animation

	App->render->Blit(graphics, 0, (int)foreground_pos, &foreground, false, 0.92f);
	App->render->Blit(graphics, 187, 112 + (int)foreground_pos, &(girl.GetCurrentFrameBox()), false, 0.92f); // girl animation
	App->render->Blit(graphics, 125, 23 + (int)foreground_pos, &(prGuy.GetCurrentFrameBox()), false, 0.92f); //purple guy animation
	App->render->Blit(graphics, 161, 118 + (int)foreground_pos, &(greyHat.GetCurrentFrameBox()), false, 0.92f); //grey hat
	App->render->Blit(graphics, 91, 45 + (int)foreground_pos, &(brownHat.GetCurrentFrameBox()), false, 0.92f); //brown hat
	App->render->Blit(graphics, 117, 100 + (int)foreground_pos, &(baldGuy.GetCurrentFrameBox()), false, 0.92f); //bald guy
	App->render->Blit(graphics, 286, 100 + (int)foreground_pos, &(blueGuy.GetCurrentFrameBox()), false, 0.92f); //blue guy
	App->render->Blit(graphics, 220, 129 + (int)foreground_pos, &(blueGuy2.GetCurrentFrameBox()), false, 0.92f); //blue guy 2

	

	App->render->Blit(graphics, 0, 170, &ground, false);
	App->render->Blit(graphics, 520, 120, &box, false); //box

	App->render->Blit(graphics, 180, 160, &little, false);
	App->render->Blit(graphics, 390, 160, &little, false);

	
	

	if (App->input->keyboard[SDL_SCANCODE_SPACE] == 1) {
		Mix_FadeOutMusic(2000);
		App->fade->FadeToBlack(this, (Module*)App->scene_Sagat, 2);
	}

	return UPDATE_CONTINUE;
}

update_status ModuleSceneKen::PostUpdate() {

	App->render->Blit(graphics, 177, 200, &big, false);
	App->render->Blit(graphics, 387, 200, &big, false);

	return UPDATE_CONTINUE;
}