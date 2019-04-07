#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleSecondPlayer.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModuleSecondPlayer::ModuleSecondPlayer()
{
	position.x = 250;
	position.y = 220;

	// idle animation (arcade sprite sheet)
	idle2.PushBack({ 348, 3, 61, 95 }, 0.1f);
	idle2.PushBack({ 408, 3, 60, 95 }, 0.1f);
	idle2.PushBack({ 468, 3, 58, 95 }, 0.1f);
	idle2.PushBack({ 526, 3, 55, 95 }, 0.1f);

	// walk forward animation (arcade sprite sheet)
	forward2.PushBack({ 1, 3, 53, 94 }, 0.1f);
	forward2.PushBack({ 53, 3, 62, 95 }, 0.1f);
	forward2.PushBack({ 115, 3, 63, 95 }, 0.1f);
	forward2.PushBack({ 179, 3, 64, 94 }, 0.1f);
	forward2.PushBack({ 243, 3, 54, 95 }, 0.1f);
	forward2.PushBack({ 298, 3, 49, 94 }, 0.1f);

	// walk backward animation (arcade sprite sheet)
	backward2.PushBack({ 694, 3, 56, 93 }, 0.1f);
	backward2.PushBack({ 752, 3, 60, 93 }, 0.1f);
	backward2.PushBack({ 813, 3, 58, 93 }, 0.1f);
	backward2.PushBack({ 872, 3, 56, 93 }, 0.1f);
	backward2.PushBack({ 930, 3, 57, 93 }, 0.1f);
	backward2.PushBack({ 0, 102, 57, 92 }, 0.1f);

	// lp
	lp2.PushBack({ 1, 3, 53, 94 }, 0.1f);
	lp2.PushBack({ 59, 101, 63, 93 }, 0.1f);
	lp2.PushBack({ 124, 101, 91, 93 }, 0.1f);
	lp2.PushBack({ 217, 101, 73, 93 }, 0.1f);
	lp2.PushBack({ 59, 101, 63, 93 }, 0.1f);
	lp2.PushBack({ 1, 3, 53, 94 }, 0.1f);

	// lk
	lk2.PushBack({ 1, 3, 53, 94 }, 0.1f);
	lk2.PushBack({ 400, 102, 63, 93 }, 0.1f);
	lk2.PushBack({ 467, 100, 115, 93 }, 0.1f);
	lk2.PushBack({ 400, 102, 63, 93 }, 0.1f);
	lk2.PushBack({ 1, 3, 53, 94 }, 0.1f);

	// Neutral jump
	neutralJump2.PushBack({ 195, 512, 55, 105 }, 0.081f);
	neutralJump2.PushBack({ 252, 528, 50, 89 }, 0.081f);
	neutralJump2.PushBack({ 303, 540, 54, 77 }, 0.081f);
	neutralJump2.PushBack({ 358, 547, 48, 70 }, 0.081f);
	neutralJump2.PushBack({ 407, 528, 48, 89 }, 0.081f);
	neutralJump2.PushBack({ 407, 528, 48, 89 }, 0.081f);
	neutralJump2.PushBack({ 407, 528, 48, 89 }, 0.081f);
	neutralJump2.PushBack({ 195, 512, 55, 105 }, 0.081f);

	//Hadoken
	hdk.PushBack({ 462, 751, 74, 90 }, 0.1f);
	hdk.PushBack({ 537, 757, 85, 84 }, 0.1f);
	hdk.PushBack({ 623, 758, 90, 83 }, 0.1f);
	hdk.PushBack({ 714, 764, 106, 77 }, 0.1f);
	hdk.PushBack({ 714, 764, 106, 77 }, 0.1f);
}

ModuleSecondPlayer::~ModuleSecondPlayer()
{}

// Load assets
bool ModuleSecondPlayer::Start()
{
	LOG("Loading player textures");
	bool ret = true;
	graphics2 = App->textures->Load("assets/images/sprites/characters/ryu1.png"); // arcade version
	collider = App->collisions->AddCollider(idle2.GetCurrentFrame(), COLLIDER_PLAYER, this);

	return ret;
}

bool ModuleSecondPlayer::CleanUp()
{
	LOG("Unloading player 2");

	App->textures->Unload(graphics2);
	if (collider != nullptr) {
		collider->to_delete = true;
		collider = nullptr;
	}

	return true;
}

// Update: draw background
update_status ModuleSecondPlayer::Update()
{
	Animation* current_animation = &idle2;

	int speed = 1;

	if(App->input->keyboard[SDL_SCANCODE_C] == KEY_STATE::KEY_REPEAT)
	{
		current_animation = &forward2;
		position.x -= speed;
	}

	if ((App->input->keyboard[SDL_SCANCODE_B] == KEY_STATE::KEY_REPEAT) && (atacar == false) && (jump == false))
	{
		current_animation = &backward2;
		position.x += speed;
	}

	if ((App->input->keyboard[SDL_SCANCODE_1] == KEY_STATE::KEY_DOWN) && (atacar == false) && (jump == false)) {
		atacar = true;
		mov = 1;
	}

	if ((App->input->keyboard[SDL_SCANCODE_2] == KEY_STATE::KEY_DOWN) && (atacar == false) && (jump == false)) {
		atacar = true;
		mov = 4;
	}

	if ((App->input->keyboard[SDL_SCANCODE_F] == KEY_STATE::KEY_DOWN) && (atacar == false) && (jump == false)) {
		jump = true;
	}
	if ((App->input->keyboard[SDL_SCANCODE_3] == KEY_STATE::KEY_DOWN) && (atacar == false) && (jump == false))
	{
		atacar = true;
		mov = 7;
	}


	collider->SetPos(position.x, position.y - 95);

	//Light punch Ryu
	if (atacar == true && framesAtaque == 0 && mov == 1)
		framesAtaque = 1;

	if (atacar == true && mov == 1)
		current_animation = &lp2;

	if (framesAtaque > 60 && mov == 1) {
		atacar = false;
		framesAtaque = 0;
		mov = 0;
	}

	//Light kick Ryu
	if (atacar == true && framesAtaque == 0 && mov == 4)
		framesAtaque = 1;

	if (atacar == true && mov == 4)
		current_animation = &lk2;

	if (framesAtaque > 50 && mov == 4) {
		atacar = false;
		framesAtaque = 0;
		mov = 0;
	}


	//Neutral jump Ryu
	if (jump == true && framesJump == 0)
		framesJump = 1;
	if (jump == true)
		current_animation = &neutralJump2;

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
		App->particles->AddParticle(App->particles->hdk, position.x - 25, position.y - 70, COLLIDER_PLAYER_SHOT, App->audio->hdk, 200);

	if (framesAtaque > 50 && mov == 7) {
		atacar = false;
		framesAtaque = 0;
		mov = 0;
	}

	//Contadores
	if (framesAtaque > 0)
		framesAtaque++;
	if (framesJump > 0)
		framesJump++;

	// Draw everything --------------------------------------
	SDL_Rect r2 = current_animation->GetCurrentFrame();

	App->render->Blit(graphics2, position.x, position.y - r2.h, &r2, flip);
	
	return UPDATE_CONTINUE;
}

