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
	position2.x = 250;
	position2.y = 220;

	// idle animation (arcade sprite sheet)
	idle2.PushBack({ 348, 3, 61, 95 }, 6);
	idle2.PushBack({ 408, 3, 60, 95 }, 6);
	idle2.PushBack({ 468, 3, 58, 95 }, 6);
	idle2.PushBack({ 526, 3, 55, 95 }, 6);

	// walk forward animation (arcade sprite sheet)
	forward2.PushBack({ 1, 3, 53, 94 }, 6);
	forward2.PushBack({ 53, 3, 62, 95 }, 6);
	forward2.PushBack({ 115, 3, 63, 95 }, 6);
	forward2.PushBack({ 179, 3, 64, 94 }, 6);
	forward2.PushBack({ 243, 3, 54, 95 }, 6);
	forward2.PushBack({ 298, 3, 49, 94 }, 6);

	// walk backward animation (arcade sprite sheet)
	backward2.PushBack({ 694, 3, 56, 93 }, 6);
	backward2.PushBack({ 752, 3, 60, 93 }, 6);
	backward2.PushBack({ 813, 3, 58, 93 }, 6);
	backward2.PushBack({ 872, 3, 56, 93 }, 6);
	backward2.PushBack({ 930, 3, 57, 93 }, 6);
	backward2.PushBack({ 0, 102, 57, 92 }, 6);


	// lp
	lp2.PushBack({ 59, 101, 63, 93 }, 4);
	lp2.PushBack({ 124, 101, 91, 93 }, 8);
	lp2.PushBack({ 59, 101, 63, 93 }, 8);
	lp2.PushBack({ 409, 3, 58, 93 }, 2);

	// lk
	lk2.PushBack({ 1, 3, 53, 94 }, 6);
	lk2.PushBack({ 400, 102, 63, 93 }, 6);
	lk2.PushBack({ 467, 100, 115, 93 }, 16);
	lk2.PushBack({ 400, 102, 63, 93 }, 8);
	lk2.PushBack({ 1, 3, 53, 94 }, 4);

	// Neutral jump
	neutralJump2.PushBack({ 195, 512, 55, 105 }, 1);		// La velocidad es adecuada, pero las animaciones están mal / leen mal el tiempo
	neutralJump2.PushBack({ 252, 528, 50, 89 }, 1);
	neutralJump2.PushBack({ 303, 540, 54, 77 }, 2);
	neutralJump2.PushBack({ 358, 547, 48, 70 }, 3);
	neutralJump2.PushBack({ 407, 528, 48, 89 }, 2);
	neutralJump2.PushBack({ 407, 528, 48, 89 }, 2);
	neutralJump2.PushBack({ 407, 528, 48, 89 }, 1);
	neutralJump2.PushBack({ 195, 512, 55, 105 }, 1);

	//Hadoken
	hdk2.PushBack({ 462, 751, 74, 90 }, 4);		// Falta un trozo de animación, cuya durada depende de si es light, medium or hard
	hdk2.PushBack({ 537, 757, 85, 84 }, 12);
	hdk2.PushBack({ 623, 758, 90, 83 }, 2);
	hdk2.PushBack({ 714, 764, 106, 77 }, 2);

	// Standing reel
	streel2.PushBack({ 143, 857, 67, 92 }, 6);		// Ya que no hay colisiones, no se puede ver...
	streel2.PushBack({ 213, 857, 69, 91 }, 6);
	streel2.PushBack({ 285, 857, 80, 91 }, 6);
	streel2.PushBack({ 367, 857, 66, 91 }, 6);
}

ModuleSecondPlayer::~ModuleSecondPlayer()
{}

// Load assets
bool ModuleSecondPlayer::Start()
{
	LOG("Loading player textures");
	bool ret = true;
	graphics2 = App->textures->Load("assets/images/sprites/characters/ryu1.png"); // arcade version
	collider = App->collisions->AddCollider(idle2.GetCurrentFrame(), COLLIDER_PLAYER2, this);

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

	if ((App->input->keyboard[SDL_SCANCODE_C] == KEY_STATE::KEY_REPEAT) && (atacar == false) && (jump == false) && (avanzar == true)){
		current_animation = &forward2;
		position2.x -= speed;
	}

	if ((App->input->keyboard[SDL_SCANCODE_B] == KEY_STATE::KEY_REPEAT) && (atacar == false) && (jump == false) && (retroceder == true)) {
		current_animation = &backward2;
		position2.x += speed;
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


	if (collider != nullptr) {
		collider->SetPos(position2.x, position2.y - 95);
	}

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
		position2.y -= speed + 1;
	}

	if (framesJump > 49 && framesJump < 99)
	{
		position2.y += speed + 1;
	}

	if (framesJump > 98) {
		jump = false;
		framesJump = 0;
	}


	//Hadoken
	if (atacar == true && framesAtaque == 0 && mov == 7)
		framesAtaque = 1;

	if (atacar == true && mov == 7)
		current_animation = &hdk2;

	if (atacar == true && mov == 7 && framesAtaque == 30)
		App->particles->AddParticle(App->particles->hdk, position2.x - 25, position2.y - 70, 1, COLLIDER_PLAYER2_SHOT, App->audio->hdk, 200);

	if (framesAtaque > 50 && mov == 7) {
		atacar = false;
		framesAtaque = 0;
		mov = 0;
	}

	//Standing reel
	if (atacar == true && framesAtaque == 0 && mov == 8) {
		framesAtaque = 1;
	}

	if (atacar == true && mov == 8)
		current_animation = &streel2;

	if (framesAtaque > 24 && mov == 8) {
		atacar = false;
		framesAtaque = 0;
		mov = 0;
	}

	//Contadores
	if (framesAtaque > 0)
		framesAtaque++;
	if (framesJump > 0)
		framesJump++;

	avanzar = true;
	retroceder = true;

	// Draw everything --------------------------------------
	SDL_Rect r2 = current_animation->GetCurrentFrame();

	App->render->Blit(graphics2, position2.x, position2.y - r2.h, &r2, flip);
	
	return UPDATE_CONTINUE;
}

void ModuleSecondPlayer::OnCollision(Collider* c1, Collider* c2) {
	if (c1->type == COLLIDER_PLAYER2 && c2->type == COLLIDER_PLAYER_SHOT)
	{
		mov = 8;
		atacar = true;
		App->audio->PlayChunk(App->audio->hdk_hit);
	}

	if (c1->type == COLLIDER_PLAYER2 && c2->type == COLLIDER_PLAYER)
	{
		avanzar = false;
	}
	if (c1->type == COLLIDER_PLAYER2 && c2->type == COLLIDER_WALL)
	{
		// the condition checks at which side of the screen the player is, and makes it impossible to move further away from the center

		// when characters automatically face each other, such condition doesnt make sense, since moving out of scene is only possible by walking backwards (walking forwards will collide with player)

		//probably a bool "player at wall" will be necessary in the future, in order to provent characters from exiting camera if beign pushed back because an enemy attack.
		//when the movement is triggered, it should check the bool state. If true, should not move in x axis. bool becomes true in this collision, resets in "Contadores"

		if (position2.x - App->render->camera.w / 2 < 0) {

			avanzar = false;
		}
		if (position2.x - App->render->camera.w / 2 > 0) {

			retroceder = false;
		}
	}
}
