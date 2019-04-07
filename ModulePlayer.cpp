#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"

ModulePlayer::ModulePlayer()
{
	position.x = 100;
	position.y = 220;

	// idle animation (arcade sprite sheet)
	idle.PushBack({ 348, 3, 61, 95 }, 0.1f);
	idle.PushBack({ 408, 3, 60, 95 }, 0.1f);
	idle.PushBack({ 468, 3, 58, 95 }, 0.1f);
	idle.PushBack({ 526, 3, 55, 95 }, 0.1f);

	// walk forward animation (arcade sprite sheet)
	forward.PushBack({ 1, 3, 53, 94 }, 0.1f);
	forward.PushBack({ 53, 3, 62, 95 }, 0.1f);
	forward.PushBack({ 115, 3, 63, 95 }, 0.1f);
	forward.PushBack({ 179, 3, 64, 94 }, 0.1f);
	forward.PushBack({ 243, 3, 54, 95 }, 0.1f);
	forward.PushBack({ 298, 3, 49, 94 }, 0.1f);

	// walk backward animation (arcade sprite sheet)
	backward.PushBack({ 694, 3, 56, 93 }, 0.1f);
	backward.PushBack({ 752, 3, 60, 93 }, 0.1f);
	backward.PushBack({ 813, 3, 58, 93 }, 0.1f);
	backward.PushBack({ 872, 3, 56, 93 }, 0.1f);
	backward.PushBack({ 930, 3, 57, 93 }, 0.1f);
	backward.PushBack({ 0, 102, 57, 92 }, 0.1f);


	// lp
	lp.PushBack({ 1, 3, 53, 94 }, 0.1f);
	lp.PushBack({ 59, 101, 63, 93 }, 0.1f);
	lp.PushBack({ 124, 101, 91, 93 }, 0.1f);
	lp.PushBack({ 217, 101, 73, 93 }, 0.1f);
	lp.PushBack({ 59, 101, 63, 93 }, 0.1f);
	lp.PushBack({ 1, 3, 53, 94 }, 0.1f);

	// lk
	lk.PushBack({ 1, 3, 53, 94 }, 0.1f);
	lk.PushBack({ 400, 102, 63, 93 }, 0.1f);
	lk.PushBack({ 467, 100, 115, 93 }, 0.1f);
	lk.PushBack({ 400, 102, 63, 93 }, 0.1f);
	lk.PushBack({ 1, 3, 53, 94 }, 0.1f);

	// Neutral jump
	neutralJump.PushBack({ 195, 512, 55, 105 }, 0.081f);
	neutralJump.PushBack({ 252, 528, 50, 89 }, 0.081f);
	neutralJump.PushBack({ 303, 540, 54, 77 }, 0.081f);
	neutralJump.PushBack({ 358, 547, 48, 70 }, 0.081f);
	neutralJump.PushBack({ 407, 528, 48, 89 }, 0.081f);
	neutralJump.PushBack({ 407, 528, 48, 89 }, 0.081f);
	neutralJump.PushBack({ 407, 528, 48, 89 }, 0.081f);
	neutralJump.PushBack({ 195, 512, 55, 105 }, 0.081f);

	//Hadoken
	hdk.PushBack({ 462, 751, 74, 90 }, 0.1f);
	hdk.PushBack({ 537, 757, 85, 84 }, 0.1f);
	hdk.PushBack({ 623, 758, 90, 83 }, 0.1f);
	hdk.PushBack({ 714, 764, 106, 77 }, 0.1f);
	hdk.PushBack({ 714, 764, 106, 77 }, 0.1f);
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player textures");
	bool ret = true;
	graphics = App->textures->Load("assets/images/sprites/characters/ryu1.png"); // arcade version
	
		collider = App->collisions->AddCollider(idle.GetCurrentFrame(), COLLIDER_PLAYER, this);
	
	return ret;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player 1");

	App->textures->Unload(graphics);

	if (collider != nullptr) {
		collider->to_delete = true;
		collider = nullptr;
	}
	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	Animation* current_animation = &idle;

	int speed = 1;

	if ((App->input->keyboard[SDL_SCANCODE_D] == KEY_STATE::KEY_REPEAT) && (atacar == false) && (jump == false))
	{
		current_animation = &forward;
		position.x += speed;
	}

	if ((App->input->keyboard[SDL_SCANCODE_A] == KEY_STATE::KEY_REPEAT) && (atacar == false) && (jump == false))
	{
		current_animation = &backward;
		position.x -= speed;
	}

	if ((App->input->keyboard[SDL_SCANCODE_U] == KEY_STATE::KEY_DOWN) && (atacar == false) && (jump == false)) {
		atacar = true;
		mov = 1;
	}

	if ((App->input->keyboard[SDL_SCANCODE_J] == KEY_STATE::KEY_DOWN) && (atacar == false) && (jump == false)) {
		atacar = true;
		mov = 4;
	}

	if ((App->input->keyboard[SDL_SCANCODE_W] == KEY_STATE::KEY_DOWN) && (atacar == false) && (jump == false)) {
		jump = true;
	}

	if ((App->input->keyboard[SDL_SCANCODE_Q] == KEY_STATE::KEY_DOWN) && (atacar == false) && (jump == false))
	{
		atacar = true;
		mov = 7;
	}

	if (collider != nullptr){
		collider->SetPos(position.x, position.y - 95);
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
		position.y -= speed + 1;
	}

	if (framesJump > 49 && framesJump < 99)
	{
		position.y += speed + 1;
	}

	if (framesJump > 98) {
		jump = false;
		framesJump = 0;
	}



	//Hadoken
	if (atacar == true && framesAtaque == 0 && mov == 7)
		framesAtaque = 1;

	if (atacar == true && mov == 7)
		current_animation = &hdk;

	if (atacar == true && mov == 7 && framesAtaque == 30)
		App->particles->AddParticle(App->particles->hdk, position.x + 25, position.y - 70, COLLIDER_PLAYER_SHOT, App->audio->hdk, 200);

	if (framesAtaque > 50 && mov == 7) {
		atacar = false;
		framesAtaque = 0;
		mov = 0;
	}

	//GOD MODE


	if (App->input->keyboard[SDL_SCANCODE_F5] == KEY_STATE::KEY_DOWN) {

		if (godmode == false) {
			godmode = true;
			collider->to_delete = true;
			collider = nullptr;
		}

		else {
			godmode = false;

			collider = App->collisions->AddCollider(idle.GetCurrentFrame(), COLLIDER_PLAYER, this);
		}
	}


	//Contadores
	if (framesAtaque > 0)
		framesAtaque++;
	if (framesJump > 0)
		framesJump++;

	// Draw everything --------------------------------------
	SDL_Rect r = current_animation->GetCurrentFrame();

	App->render->Blit(graphics, position.x, position.y - r.h, &r, false);

	return UPDATE_CONTINUE;
}