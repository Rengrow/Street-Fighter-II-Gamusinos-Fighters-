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
	idle.PushBack({ 348, 3, 61, 95 }, 6);
	idle.PushBack({ 408, 3, 60, 95 }, 6);
	idle.PushBack({ 468, 3, 58, 95 }, 6);
	idle.PushBack({ 526, 3, 55, 95 }, 6);

	// walk forward animation (arcade sprite sheet)
	forward.PushBack({ 1, 3, 53, 94 }, 6);
	forward.PushBack({ 53, 3, 62, 95 }, 6);
	forward.PushBack({ 115, 3, 63, 95 }, 6);
	forward.PushBack({ 179, 3, 64, 94 }, 6);
	forward.PushBack({ 243, 3, 54, 95 }, 6);
	forward.PushBack({ 298, 3, 49, 94 }, 6);

	// walk backward animation (arcade sprite sheet)
	backward.PushBack({ 694, 3, 56, 93 }, 6);
	backward.PushBack({ 752, 3, 60, 93 }, 6);
	backward.PushBack({ 813, 3, 58, 93 }, 6);
	backward.PushBack({ 872, 3, 56, 93 }, 6);
	backward.PushBack({ 930, 3, 57, 93 }, 6);
	backward.PushBack({ 0, 102, 57, 92 }, 6);


	// lp
	lp.PushBack({ 59, 101, 63, 93 }, 4);
	lp.PushBack({ 124, 101, 91, 93 }, 8);
	lp.PushBack({ 59, 101, 63, 93 }, 8);
	lp.PushBack({ 409, 3, 58, 93 }, 2);

	// lk
	lk.PushBack({ 1, 3, 53, 94 }, 6);
	lk.PushBack({ 400, 102, 63, 93 }, 6);
	lk.PushBack({ 467, 100, 115, 93 }, 16);
	lk.PushBack({ 400, 102, 63, 93 }, 8);
	lk.PushBack({ 1, 3, 53, 94 }, 4);

	// Neutral jump
	neutralJump.PushBack({ 195, 512, 55, 105 }, 1);		// La velocidad es adecuada, pero las animaciones están mal / leen mal el tiempo
	neutralJump.PushBack({ 252, 528, 50, 89 }, 1);
	neutralJump.PushBack({ 303, 540, 54, 77 }, 2);
	neutralJump.PushBack({ 358, 547, 48, 70 }, 3);
	neutralJump.PushBack({ 407, 528, 48, 89 }, 2);
	neutralJump.PushBack({ 407, 528, 48, 89 }, 2);
	neutralJump.PushBack({ 407, 528, 48, 89 }, 1);
	neutralJump.PushBack({ 195, 512, 55, 105 }, 1);

	//Hadoken
	hdk.PushBack({ 462, 751, 74, 90 }, 4);		// Falta un trozo de animación, cuya durada depende de si es light, medium or hard
	hdk.PushBack({ 537, 757, 85, 84 }, 12);
	hdk.PushBack({ 623, 758, 90, 83 }, 2);
	hdk.PushBack({ 714, 764, 106, 77 }, 2);

	// Standing reel
	streel.PushBack({ 143, 857, 67, 92 }, 6);		// Ya que no hay colisiones, no se puede ver...
	streel.PushBack({ 213, 857, 69, 91 }, 6);
	streel.PushBack({ 285, 857, 80, 91 }, 6);
	streel.PushBack({ 367, 857, 66, 91 }, 6);
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
	punch_collider = App->collisions->AddCollider(lp.GetCurrentFrame(), COLLIDER_PLAYER_HIT, this);
	
	Animation* current_animation;
	
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

	if ((App->input->keyboard[SDL_SCANCODE_D] == KEY_STATE::KEY_REPEAT) && (atacar == false) && (jump == false) && (avanzar == true))
	{
		current_animation = &forward;
		position.x += speed;
	}

	if ((App->input->keyboard[SDL_SCANCODE_A] == KEY_STATE::KEY_REPEAT) && (atacar == false) && (jump == false) && (retroceder == true))
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

	if (collider != nullptr) {
		collider->SetPos(position.x, position.y - 95);
	}


	//Light punch Ryu
	if (atacar == true && framesAtaque == 0 && mov == 1)
		framesAtaque = 1;

	if (atacar == true && mov == 1)
		current_animation = &lp;

	if (framesAtaque > 22 && mov == 1) {
		atacar = false;
		framesAtaque = 0;
		mov = 0;
	}

	//Light kick Ryu
	if (atacar == true && framesAtaque == 0 && mov == 4)
		framesAtaque = 1;

	if (atacar == true && mov == 4)
		current_animation = &lk;

	if (framesAtaque > 37 && mov == 4) {
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

	if (atacar == true && mov == 7 && framesAtaque == 20)
		App->particles->AddParticle(App->particles->hdk, position.x + 25, position.y - 70, 0, COLLIDER_PLAYER_SHOT, App->audio->hdk, 200);

	if (framesAtaque > 32 && mov == 7) {
		atacar = false;
		framesAtaque = 0;
		mov = 0;
	}

	//Standing reel
	if (atacar == true && framesAtaque == 0 && mov == 8) {
		framesAtaque = 1;
	}

	if (atacar == true && mov == 8)
		current_animation = &streel;

	if (framesAtaque > 24 && mov == 8) {
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
	avanzar = true;
	retroceder = true;

	// Draw everything --------------------------------------
	SDL_Rect r = current_animation->GetCurrentFrame();

	App->render->Blit(graphics, position.x, position.y - r.h, &r, false);

	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(Collider* c1, Collider* c2) {
	
	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_PLAYER2_SHOT)
	{
		mov = 8;
		atacar = true;
		App->audio->PlayChunk(App->audio->hdk_hit);
	}

	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_PLAYER2)
	{
		avanzar = false;
	}
	
	
	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_WALL)
	{
		// the condition checks at which side of the screen the player is, and makes it impossible to move further away from the center

		// when characters automatically face each other, such condition doesnt make sense, since moving out of scene is only possible by walking backwards (walking forwards will collide with player)

		//probably a bool "player at wall" will be necessary in the future, in order to provent characters from exiting camera if beign pushed back because an enemy attack.
		//when the movement is triggered, it should check the bool state. If true, should not move in x axis. bool becomes true in this collision, resets in "Contadores"
		
		if (position.x-App->render->camera.w/2 < 0){
		
		 retroceder = false;
		}
		if (position.x - App->render->camera.w/2 > 0){

		 avanzar = false;
		}
	}
}
