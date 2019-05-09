#include "Globals.h"
#include "Application.h"
#include "ModuleSceneSagat.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleRyu.h"
#include "ModuleDhalsim.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModuleAudio.h"
#include "ModuleParticles.h"
#include "ModuleCollision.h"
#include "ModuleUI.h"

// Reference at https://youtu.be/6OlenbCC4WI?t=382

ModuleSceneSagat::ModuleSceneSagat()
{
	// ground
	ground.x = 0;
	ground.y = 973;
	ground.w = 1024;
	ground.h = 49;

	// rocks
	rocks = { 983, 159, 25, 25 };

	// foreground
	foreground = { 0, 655, 692, 195 };

	// Background
	background = { 0, 432, 633, 195 };

	// Here rests the module palmera :(
	palmtree.PushBack({ 699, 212, 325, 317 }, 24, { 0,0 }, 0, {}, {}, {});
	palmtree.PushBack({ 696, 550, 328, 317 }, 24, { 0,0 }, 0, {}, {}, {});

}

ModuleSceneSagat::~ModuleSceneSagat()
{}

// Load assets
bool ModuleSceneSagat::Start()
{
	LOG("Loading background assets");
	bool ret = true;
	graphics = App->textures->Load("assets/images/sprites/stages/KenSagatStage.png");
	music = App->audio->LoadSong("assets/music/thailand_s_1.ogg");

	App->audio->PlaySongDelay(music, -1, 2000);

	App->render->scenelimit = 790;

	App->render->limit1Box.x = 0;
	App->render->limit1Box.y = 0;
	App->render->limit1Box.w = 3;
	App->render->limit1Box.h = SCREEN_HEIGHT;

	App->render->limit2Box.x = SCREEN_WIDTH - 3;
	App->render->limit2Box.y = 0;
	App->render->limit2Box.w = 3;
	App->render->limit2Box.h = SCREEN_HEIGHT;

	App->render->camera.x = App->render->camera.y = 0;

	App->ryu->Enable();
	App->dhalsim->Enable();
	App->particles->Enable();
	App->collisions->Enable();
	App->UI->Enable();

	collider = App->collisions->AddCollider({ 0,0,1,SCREEN_HEIGHT }, COLLIDER_WALL);
	collider2 = App->collisions->AddCollider({ 790,0,1,SCREEN_HEIGHT }, COLLIDER_WALL);

	App->UI->StartFight();

	return ret;
}

// Unload assets
bool ModuleSceneSagat::CleanUp()
{
	LOG("Unloading Sagat stage");

	App->textures->Unload(graphics);
	graphics = nullptr;
	App->audio->UnloadSong(music);
	music = nullptr;

	if (collider != nullptr) {
		collider->to_delete = true;
		collider = nullptr;
	}

	if (collider2 != nullptr) {
		collider2->to_delete = true;
		collider2 = nullptr;
	}

	App->ryu->Disable();
	App->dhalsim->Disable();
	App->particles->Disable();
	App->collisions->Disable();
	App->UI->Disable();

	return true;
}

// Update: draw background
update_status ModuleSceneSagat::Update()
{

	// Draw everything --------------------------------------	
	App->render->Blit(graphics, 0, 0, &background, false, 0.5f);
	App->render->Blit(graphics, 0, -19, &foreground, false, 0.75f); // back of the room

	App->render->Blit(graphics, -243, 175, &ground, false, 0.75f);
	App->render->Blit(graphics, 174, 199, &rocks, false, 0.75f);
	App->render->Blit(graphics, 489, 168, &rocks, false, 0.75f);

	return UPDATE_CONTINUE;
}

update_status ModuleSceneSagat::PostUpdate() {

	App->render->Blit(App->scene_Sagat->graphics, 364, 0, &(App->scene_Sagat->palmtree.GetCurrentFrameBox()), 0.75); // palmtree animation
	return UPDATE_CONTINUE;
}

void ModuleSceneSagat::StopMusic(int time) {
	Mix_FadeOutMusic(time);
}
