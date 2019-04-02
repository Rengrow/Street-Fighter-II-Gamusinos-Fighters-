#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleSecondPlayer.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModuleSecondPlayer::ModuleSecondPlayer()
{
	position.x = 100;
	position.y = 220;

	// idle animation (arcade sprite sheet)
	idle2.PushBack({ 348, 3, 61, 95 });
	idle2.PushBack({ 408, 3, 60, 95 });
	idle2.PushBack({ 468, 3, 58, 95 });
	idle2.PushBack({ 526, 3, 55, 95 });
	idle2.speed = 0.1f;

	// walk forward animation (arcade sprite sheet)
	forward2.PushBack({ 1, 3, 53, 94 });
	forward2.PushBack({ 53, 3, 62, 95 });
	forward2.PushBack({ 115, 3, 63, 95 });
	forward2.PushBack({ 179, 3, 64, 94 });
	forward2.PushBack({ 243, 3, 54, 95 });
	forward2.PushBack({ 298, 3, 49, 94 });
	forward2.speed = 0.1f;
}

ModuleSecondPlayer::~ModuleSecondPlayer()
{}

// Load assets
bool ModuleSecondPlayer::Start()
{
	LOG("Loading player textures");
	bool ret = true;
	graphics2 = App->textures->Load("assets/images/sprites/characters/ryu1.png"); // arcade version
	return ret;
}

// Update: draw background
update_status ModuleSecondPlayer::Update()
{
	Animation* current_animation = &idle2;

	int speed = 1;

	if(App->input->keyboard[SDL_SCANCODE_G] == KEY_STATE::KEY_REPEAT)
	{
		current_animation = &forward2;
		position.x += speed;
	}

	// Draw everything --------------------------------------
	SDL_Rect r2 = current_animation->GetCurrentFrame();

	App->render->Blit(graphics2, position.x, position.y - r2.h, &r2, true);
	
	return UPDATE_CONTINUE;
}