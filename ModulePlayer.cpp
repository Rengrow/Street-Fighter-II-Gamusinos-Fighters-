#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"

ModulePlayer::ModulePlayer()
{
	position.x = 100;
	position.y = 220;

	// idle animation (arcade sprite sheet)
	idle.PushBack({ 348, 3, 61, 95 });
	idle.PushBack({ 408, 3, 60, 95 });
	idle.PushBack({ 468, 3, 58, 95 });
	idle.PushBack({ 526, 3, 55, 95 });
	idle.speed = 0.1f;

	// walk forward animation (arcade sprite sheet)
	forward.PushBack({ 1, 3, 53, 94 });
	forward.PushBack({ 53, 3, 62, 95 });
	forward.PushBack({ 115, 3, 63, 95 });
	forward.PushBack({ 179, 3, 64, 94 });
	forward.PushBack({ 243, 3, 54, 95 });
	forward.PushBack({ 298, 3, 49, 94 });
	forward.speed = 0.1f;

	// walk backward animation (arcade sprite sheet)
	backward.PushBack({ 694, 3, 56, 93 });
	backward.PushBack({ 752, 3, 60, 93 });
	backward.PushBack({ 813, 3, 58, 93 });
	backward.PushBack({ 872, 3, 56, 93 });
	backward.PushBack({ 930, 3, 57, 93 });
	backward.PushBack({ 0, 102, 57, 92 });
	backward.speed = 0.2f;


	// lp
	lp.PushBack({ 1, 3, 53, 94 });
	lp.PushBack({ 59, 101, 63, 93 });
	lp.PushBack({ 124, 101, 91, 93 });
	lp.PushBack({ 217, 101, 73, 93 });
	lp.PushBack({ 59, 101, 63, 93 });
	lp.PushBack({ 1, 3, 53, 94 });
	lp.speed = 0.1f;

	// lk
	lk.PushBack({ 1, 3, 53, 94 });
	lk.PushBack({ 400, 102, 63, 93 });
	lk.PushBack({ 467, 100, 115, 93 });
	lk.PushBack({ 400, 102, 63, 93 });
	lk.PushBack({ 1, 3, 53, 94 });
	lk.speed = 0.1f;

	// Neutral jump
	neutralJump.PushBack({ 195, 512, 55, 105 });
	neutralJump.PushBack({ 252, 528, 50, 89 });
	neutralJump.PushBack({ 303, 540, 54, 77 });
	neutralJump.PushBack({ 358, 547, 48, 70 });
	neutralJump.PushBack({ 407, 528, 48, 89 });
	neutralJump.PushBack({ 407, 528, 48, 89 });
	neutralJump.PushBack({ 407, 528, 48, 89 });
	neutralJump.PushBack({ 195, 512, 55, 105 });
	neutralJump.speed = 0.081f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player textures");
	bool ret = true;
	graphics = App->textures->Load("assets/images/sprites/characters/ryu1.png"); // arcade version
	return ret;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player 1");

	App->textures->Unload(graphics);

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	Animation* current_animation = &idle;

	int speed = 1;

	if ((App->input->keyboard[SDL_SCANCODE_D] == 1) && (atacar == false) && (jump == false))
	{
		current_animation = &forward;
		position.x += speed;
	}
	if ((App->input->keyboard[SDL_SCANCODE_A] == 1) && (atacar == false) && (jump == false))
	{
		current_animation = &backward;
		position.x -= speed;
	}

	if ((App->input->keyboard[SDL_SCANCODE_U] == 1) && (atacar == false) && (jump == false)) {
		atacar = true;
		mov = 1;
	}
	if ((App->input->keyboard[SDL_SCANCODE_J] == 1) && (atacar == false) && (jump == false)) {
		atacar = true;
		mov = 4;
	}

	if ((App->input->keyboard[SDL_SCANCODE_W] == 1) && (atacar == false) && (jump == false)) {
		jump = true;
	}
	//Light punch Ryu
	if (atacar == true && framesAtaque == 0 && mov == 1)
		framesAtaque = 1;

	if (atacar == true && mov == 1)
		current_animation = &lp;

	if (framesAtaque > 60 && mov == 1) {
		atacar = false;
		framesAtaque = 0;
		mov = 0;
	}

	//Light kick Ryu
	if (atacar == true && framesAtaque == 0 && mov == 4)
		framesAtaque = 1;

	if (atacar == true && mov == 4)
		current_animation = &lk;

	if (framesAtaque > 50 && mov == 4) {
		atacar = false;
		framesAtaque = 0;
		mov = 0;
	}


	//Neutral jump Ryu
	if (jump == true && framesJump == 0)
		framesJump = 1;
	if (jump == true)
		current_animation = &neutralJump;

	if (framesJump > 0 && framesJump < 50)
	{
		position.y -= speed+1;
	}

	if (framesJump > 49 && framesJump < 99)
	{
		position.y += speed+1;
	}

	if (framesJump > 98) {
		jump = false;
		framesJump = 0;
	}

	if (framesAtaque > 0)
		framesAtaque++;
	if (framesJump > 0)
		framesJump++;

	// Draw everything --------------------------------------
	SDL_Rect r = current_animation->GetCurrentFrame();

	App->render->Blit(graphics, position.x, position.y - r.h, &r);

	return UPDATE_CONTINUE;
}