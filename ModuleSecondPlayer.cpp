#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleSecondPlayer.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"
#include "ModulePlayer.h"
#include "p2Qeue.h"
#include "SDL\include\SDL.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModuleSecondPlayer::ModuleSecondPlayer()
{
	position2.x = 250;
	position2.y = 220;

	// idle animation (arcade sprite sheet)
	idle2.PushBack({ 348, 3, 61, 95 }, 6, 0, {}, {}, {}, {});
	idle2.PushBack({ 408, 3, 60, 95 }, 6, 0, {}, {}, {}, {});
	idle2.PushBack({ 468, 3, 58, 95 }, 6, 0, {}, {}, {}, {});
	idle2.PushBack({ 526, 3, 55, 95 }, 6, 0, {}, {}, {}, {});

	// walk forward animation (arcade sprite sheet)
	forward2.PushBack({ 1, 3, 53, 94 }, 6, 0, {}, {}, {}, {});
	forward2.PushBack({ 53, 3, 62, 95 }, 6, 0, {}, {}, {}, {});
	forward2.PushBack({ 115, 3, 63, 95 }, 6, 0, {}, {}, {}, {});
	forward2.PushBack({ 179, 3, 64, 94 }, 6, 0, {}, {}, {}, {});
	forward2.PushBack({ 243, 3, 54, 95 }, 6, 0, {}, {}, {}, {});
	forward2.PushBack({ 298, 3, 49, 94 }, 6, 0, {}, {}, {}, {});

	// walk backward animation (arcade sprite sheet)
	backward2.PushBack({ 694, 3, 56, 93 }, 6, 0, {}, {}, {}, {});
	backward2.PushBack({ 752, 3, 60, 93 }, 6, 0, {}, {}, {}, {});
	backward2.PushBack({ 813, 3, 58, 93 }, 6, 0, {}, {}, {}, {});
	backward2.PushBack({ 872, 3, 56, 93 }, 6, 0, {}, {}, {}, {});
	backward2.PushBack({ 930, 3, 57, 93 }, 6, 0, {}, {}, {}, {});
	backward2.PushBack({ 0, 102, 57, 92 }, 6, 0, {}, {}, {}, {});


	// lp
	lp2.PushBack({ 59, 101, 63, 93 }, 4, 0, {}, {}, {}, {});
	lp2.PushBack({ 124, 101, 91, 93 }, 8, 0, {}, {}, {}, {});
	lp2.PushBack({ 59, 101, 63, 93 }, 8, 0, {}, {}, {}, {});
	lp2.PushBack({ 409, 3, 58, 93 }, 2, 0, {}, {}, {}, {});

	// lk
	lk2.PushBack({ 1, 3, 53, 94 }, 6, 0, {}, {}, {}, {});
	lk2.PushBack({ 400, 102, 63, 93 }, 6, 0, {}, {}, {}, {});
	lk2.PushBack({ 467, 100, 115, 93 }, 16, 0, {}, {}, {}, {});
	lk2.PushBack({ 400, 102, 63, 93 }, 8, 0, {}, {}, {}, {});
	lk2.PushBack({ 1, 3, 53, 94 }, 4, 0, {}, {}, {}, {});

	// Neutral jump
	neutralJump2.PushBack({ 195, 512, 55, 105 }, 4, 0, {}, {}, {}, {});		// La velocidad es adecuada, pero las animaciones están mal / leen mal el tiempo
	neutralJump2.PushBack({ 252, 528, 50, 89 }, 4, 0, {}, {}, {}, {});
	neutralJump2.PushBack({ 303, 540, 54, 77 }, 8, 0, {}, {}, {}, {});
	neutralJump2.PushBack({ 358, 547, 48, 70 }, 12, 0, {}, {}, {}, {});
	neutralJump2.PushBack({ 407, 528, 48, 89 }, 20, 0, {}, {}, {}, {});
	neutralJump2.PushBack({ 195, 512, 55, 105 }, 4, 0, {}, {}, {}, {});

	//Hadoken
	hdk2.PushBack({ 462, 751, 74, 90 }, 4, 0, {}, {}, {}, {});		// Falta un trozo de animación, cuya durada depende de si es light, medium or hard
	hdk2.PushBack({ 537, 757, 85, 84 }, 12, 0, {}, {}, {}, {});
	hdk2.PushBack({ 623, 758, 90, 83 }, 2, 0, {}, {}, {}, {});
	hdk2.PushBack({ 714, 764, 106, 77 }, 2, 0, {}, {}, {}, {});

	// Standing reel
	streel2.PushBack({ 143, 857, 67, 92 }, 6, 0, {}, {}, {}, {});		// Ya que no hay colisiones, no se puede ver...
	streel2.PushBack({ 213, 857, 69, 91 }, 6, 0, {}, {}, {}, {});
	streel2.PushBack({ 285, 857, 80, 91 }, 6, 0, {}, {}, {}, {});
	streel2.PushBack({ 367, 857, 66, 91 }, 6, 0, {}, {}, {}, {});

	//Crouching
	crouching2.PushBack({ 0, 317, 57, 70 }, 1, 0, {}, {}, {}, {});

	//Standing
	standing2.PushBack({ 0, 317, 57, 70 }, 1, 0, {}, {}, {}, {});

	//Crouch
	crouch2.PushBack({ 57, 325, 62, 62 }, 1, 0, {}, {}, {}, {});

	//Crouching l punch
	clp2.PushBack({ 226, 325, 70, 61 }, 8, 0, {}, {}, {}, {});
	clp2.PushBack({ 296, 325, 96, 61 }, 8, 0, {}, {}, {}, {});
	clp2.PushBack({ 392, 324, 65, 61 }, 6, 0, {}, {}, {}, {});

	//Crouching l kik
	clk2.PushBack({ 617, 322, 71, 65 }, 14, 0, {}, {}, {}, {});
	clk2.PushBack({ 688, 322, 113, 65 }, 14, 0, {}, {}, {}, {});
	clk2.PushBack({ 617, 322, 71, 65 }, 12, 0, {}, {}, {}, {});
}

ModuleSecondPlayer::~ModuleSecondPlayer()
{}

// Load assets
bool ModuleSecondPlayer::Start()
{
	LOG("Loading player textures");
	bool ret = true;
	graphics2 = App->textures->Load("assets/images/sprites/characters/ryu1.png"); // arcade version

	collider2 = App->collisions->AddCollider(idle2.GetCurrentFrame(), COLLIDER_PLAYER2, this);

	Animation* current_animation;

	return ret;
}

bool ModuleSecondPlayer::CleanUp()
{
	LOG("Unloading player 2");

	App->textures->Unload(graphics2);
	if (collider2 != nullptr) {
		collider2->to_delete = true;
		collider2 = nullptr;
	}

	return true;
}

// Update: draw background
update_status ModuleSecondPlayer::Update()
{
	int speed = 1;

	
	ryu_states2 current_state = ST_UNKNOWN2;
	Animation* current_animation = &idle2;

	App->player2->external_input2(inputs2);
	App->player2->internal_input2(inputs2);
	ryu_states2 state = process_fsm2(inputs2);

	if (state != current_state)
	{
		switch (state)
		{
		case ST_IDLE2:
			current_animation = &idle2;
			break;

		case ST_WALK_FORWARD2:
			current_animation = &forward2;
			position2.x--;
			break;

		case ST_WALK_BACKWARD2:
			current_animation = &backward2;
			position2.x++;
			break;

		case ST_JUMP_NEUTRAL2:
			current_animation = &neutralJump2;
			if (SDL_GetTicks() - App->player2->jump_timer2 > 1001)
			{
				position2.y += speed;
			}
			if (SDL_GetTicks() - App->player2->jump_timer2 < 1000)
			{
				position2.y -= speed;
			}
			break;

		case ST_JUMP_FORWARD2:
			LOG("JUMPING FORWARD ^^>>\n");
			break;

		case ST_JUMP_BACKWARD2:
			LOG("JUMPING BACKWARD ^^<<\n");
			break;

		case ST_CROUCHING2:
			current_animation = &crouching2;
			break;

		case ST_CROUCH2:
			current_animation = &crouch2;
			break;

		case ST_STANDING2:
			current_animation = &standing2;
			break;

		case L_PUNCH_CROUCH2:
			current_animation = &clp2;
			break;

		case L_PUNCH_STANDING2:
			current_animation = &lp2;
			break;

		case L_PUNCH_NEUTRAL_JUMP2:
			LOG("PUNCH JUMP NEUTRAL ^^++\n");
			break;

		case L_PUNCH_FORWARD_JUMP2:
			LOG("PUNCH JUMP FORWARD ^>>+\n");
			break;

		case L_PUNCH_BACKWARD_JUMP2:
			LOG("PUNCH JUMP BACKWARD ^<<+\n");
			break;

		case L_KIK_CROUCH2:
			current_animation = &clk2;
			break;

		case L_KIK_STANDING2:
			current_animation = &lk2;
			break;

		case L_KIK_NEUTRAL_JUMP2:
			LOG("KIK JUMP NEUTRAL ^^++\n");
			break;

		case L_KIK_FORWARD_JUMP2:
			LOG("KIK JUMP FORWARD ^>>+\n");
			break;

		case L_KIK_BACKWARD_JUMP2:
			LOG("KIK JUMP BACKWARD ^<<+\n");
			break;

		case ST_HEAD_REEL2:
			current_animation = &streel2;
			break;

		case ST_GUT_REEL2:
			break;

		case ST_CROUCH_REEL2:
			break;

		case ST_HADOKEN2:
			current_animation = &hdk2;
			if (SDL_GetTicks() - App->player2->hadoken_timer2 == 350)
			{
				App->particles->AddParticle(App->particles->hdk, position2.x + 25, position2.y - 70, 0, COLLIDER_PLAYER2_SHOT, App->audio->hdk, 200);
			}
			break;
		}
	}
	current_state = state;

	if (collider2 != nullptr) {
		collider2->SetPos(position2.x, position2.y - 95);
	}
	// Draw everything --------------------------------------
	SDL_Rect r2 = current_animation->GetCurrentFrame();

	App->render->Blit(graphics2, position2.x, position2.y - r2.h, &r2, flip);

	return UPDATE_CONTINUE;
}

void ModuleSecondPlayer::OnCollision(Collider* c1, Collider* c2) {
	if (c1->type == COLLIDER_PLAYER2 && c2->type == COLLIDER_PLAYER_SHOT)
	{
		App->audio->PlayChunk(App->audio->hdk_hit);
		inputs2.Push(IN_HEAD_REEL2);
	}

	if (c1->type == COLLIDER_PLAYER2 && c2->type == COLLIDER_PLAYER)
	{
		if ((position2.x+60) != (App->render->camera.x+App->render->camera.w)) {
			position2.x = (App->player->position.x + 63);
		}
		else { App->player->position.x--; }
	}

	if (c1->type == COLLIDER_PLAYER2 && c2->type == COLLIDER_WALL)
	{
		if (position2.x == App->render->limit1Box.x) {
			position2.x++;
		}
		if (position2.x + 60 == (App->render->limit1Box.x+App->render->camera.w)) {
			position2.x--;
		}
	}
}

bool ModuleSecondPlayer::external_input2(p2Qeue<ryu_inputs2>& inputs)
{
	static bool left = false;
	static bool right = false;
	static bool down = false;
	static bool up = false;

	//Key UP
	if (App->input->keyboard[SDL_SCANCODE_V] == KEY_STATE::KEY_UP)
	{
		inputs.Push(IN_CROUCH_UP2);
		down = false;
	}

	if (App->input->keyboard[SDL_SCANCODE_F] == KEY_STATE::KEY_UP)
	{
		up = false;
	}

	if (App->input->keyboard[SDL_SCANCODE_C] == KEY_STATE::KEY_UP)
	{
		inputs.Push(IN_RIGHT_UP2);
		right = false;
	}

	if (App->input->keyboard[SDL_SCANCODE_B] == KEY_STATE::KEY_UP)
	{
		inputs.Push(IN_LEFT_UP2);
		left = false;
		
	}
	//Key down

	if (App->input->keyboard[SDL_SCANCODE_1] == KEY_STATE::KEY_DOWN)
	{
		inputs.Push(IN_L_PUNCH2);
	}

	if (App->input->keyboard[SDL_SCANCODE_2] == KEY_STATE::KEY_DOWN)
	{
		inputs.Push(IN_L_KIK2);
	}

	if (App->input->keyboard[SDL_SCANCODE_3] == KEY_STATE::KEY_DOWN)
	{
		inputs.Push(IN_HADOKEN2);
	}

	if (App->input->keyboard[SDL_SCANCODE_F] == KEY_STATE::KEY_DOWN || App->input->keyboard[SDL_SCANCODE_F] == KEY_STATE::KEY_REPEAT)
	{
		up = true;
	}

	if (App->input->keyboard[SDL_SCANCODE_V] == KEY_STATE::KEY_DOWN || App->input->keyboard[SDL_SCANCODE_V] == KEY_STATE::KEY_REPEAT)
	{
		down = true;
	}

	if (App->input->keyboard[SDL_SCANCODE_B] == KEY_STATE::KEY_DOWN || App->input->keyboard[SDL_SCANCODE_B] == KEY_STATE::KEY_REPEAT)
	{
		
		left = true;
	}

	if (App->input->keyboard[SDL_SCANCODE_C] == KEY_STATE::KEY_DOWN || App->input->keyboard[SDL_SCANCODE_C] == KEY_STATE::KEY_REPEAT)
	{
		right = true;
	}

	


	if (left && right)
		inputs.Push(IN_LEFT_AND_RIGHT2);
	{
		if (left)
			inputs.Push(IN_LEFT_DOWN2);
		if (right)
			inputs.Push(IN_RIGHT_DOWN2);
	}

	if (up && down)
		inputs.Push(IN_JUMP_AND_CROUCH2);
	else
	{
		if (down)
			inputs.Push(IN_CROUCH_DOWN2);
		else
		{
			inputs.Push(IN_CROUCH_UP2);
		}
		if (up)
			inputs.Push(IN_JUMP2);
	}

	return true;
}

void ModuleSecondPlayer::internal_input2(p2Qeue<ryu_inputs2>& inputs)
{
	if (App->player2->jump_timer2 > 0)
	{
		if (SDL_GetTicks() - App->player2->jump_timer2 > JUMP_TIME)
		{
			inputs.Push(IN_JUMP_FINISH2);
			App->player2->jump_timer2 = 0;
		}
	}

	if (App->player2->l_punch_timer2 > 0)
	{
		if (SDL_GetTicks() - App->player2->l_punch_timer2 > L_PUNCH_TIME)
		{
			inputs.Push(IN_L_PUNCH_FINISH2);
			App->player2->l_punch_timer2 = 0;
		}
	}

	if (App->player2->l_kik_timer2 > 0)
	{
		if (SDL_GetTicks() - App->player2->l_kik_timer2 > L_KIK_TIME)
		{
			inputs2.Push(IN_L_KIK_FINISH2);
			App->player2->l_kik_timer2 = 0;
		}
	}

	if (App->player2->hadoken_timer2 > 0)
	{
		if (SDL_GetTicks() - App->player2->hadoken_timer2 > HADOKEN_TIME)
		{
			inputs2.Push(IN_HADOKEN_FINISH2);
			App->player2->hadoken_timer2 = 0;
		}
	}

	if (crouching_timer2 > 0)
	{
		if (SDL_GetTicks() - crouching_timer2 > CROUCHING_TIME)
		{
			inputs2.Push(IN_CROUCHING_FINISH2);
			crouching_timer2 = 0;
		}
	}

	if (standing_timer2 > 0)
	{
		if (SDL_GetTicks() - standing_timer2 > STANDING_TIME)
		{
			inputs2.Push(IN_STANDING_FINISH2);
			standing_timer2 = 0;
		}
	}

	if (reel_timer2 > 0)
	{
		if (SDL_GetTicks() - reel_timer2 > REEL_TIME)
		{
			inputs2.Push(IN_REEL_FINISH2);
			reel_timer2 = 0;
		}
	}
}

ryu_states2 ModuleSecondPlayer::process_fsm2(p2Qeue<ryu_inputs2>& inputs)
{
	static ryu_states2 state = ST_IDLE2;
	ryu_inputs2 last_input;

	while (inputs.Pop(last_input))
	{
		switch (state)
		{
		case ST_IDLE2:
		{
			switch (last_input)
			{
			case IN_RIGHT_DOWN2: state = ST_WALK_FORWARD2; break;
			case IN_LEFT_DOWN2: state = ST_WALK_BACKWARD2; break;
			case IN_JUMP2: state = ST_JUMP_NEUTRAL2; jump_timer2 = SDL_GetTicks();  break;
			case IN_CROUCH_DOWN2: state = ST_CROUCHING2; crouching_timer2 = SDL_GetTicks(); break;
			case IN_L_PUNCH2: state = L_PUNCH_STANDING2; l_punch_timer2 = SDL_GetTicks();  break;
			case IN_L_KIK2: state = L_KIK_STANDING2; l_kik_timer2 = SDL_GetTicks();  break;
			case IN_HADOKEN2: state = ST_HADOKEN2; hadoken_timer2 = SDL_GetTicks(); break;
			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; reel_timer2 = SDL_GetTicks(); break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; reel_timer2 = SDL_GetTicks(); break;
			}
		}
		break;

		case ST_WALK_FORWARD2:
		{
			switch (last_input)
			{
			case IN_RIGHT_UP2: state = ST_IDLE2; break;
			case IN_LEFT_AND_RIGHT2: state = ST_IDLE2; break;
			case IN_JUMP2: state = ST_JUMP_FORWARD2; jump_timer2 = SDL_GetTicks();  break;
			case IN_CROUCH_DOWN2: state = ST_CROUCHING2; crouching_timer2 = SDL_GetTicks(); break;
			case IN_L_PUNCH2: state = L_PUNCH_STANDING2; l_punch_timer2 = SDL_GetTicks();  break;
			case IN_L_KIK2: state = L_KIK_STANDING2; l_kik_timer2 = SDL_GetTicks();  break;
			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; reel_timer2 = SDL_GetTicks(); break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; reel_timer2 = SDL_GetTicks(); break;
			}
		}
		break;

		case ST_WALK_BACKWARD2:
		{
			switch (last_input)
			{
			case IN_LEFT_UP2: state = ST_IDLE2; break;
			case IN_LEFT_AND_RIGHT2: state = ST_IDLE2; break;
			case IN_JUMP2: state = ST_JUMP_BACKWARD2; jump_timer2 = SDL_GetTicks();  break;
			case IN_CROUCH_DOWN2: state = ST_CROUCHING2; crouching_timer2 = SDL_GetTicks(); break;
			case IN_L_PUNCH2: state = L_PUNCH_STANDING2; l_punch_timer2 = SDL_GetTicks();  break;
			case IN_L_KIK2: state = L_KIK_STANDING2; l_kik_timer2 = SDL_GetTicks();  break;
			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; reel_timer2 = SDL_GetTicks(); break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; reel_timer2 = SDL_GetTicks(); break;
			}
		}
		break;

		case ST_JUMP_NEUTRAL2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;
			case IN_L_PUNCH2: state = L_PUNCH_NEUTRAL_JUMP2; l_punch_timer2 = SDL_GetTicks(); break;
			case IN_L_KIK2: state = L_KIK_NEUTRAL_JUMP2; l_kik_timer2 = SDL_GetTicks(); break;
			}
		}
		break;

		case ST_JUMP_FORWARD2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;
			case IN_L_PUNCH2: state = L_PUNCH_FORWARD_JUMP2; l_punch_timer2 = SDL_GetTicks(); break;
			case IN_L_KIK2: state = L_KIK_FORWARD_JUMP2; l_kik_timer2 = SDL_GetTicks(); break;
			}
		}
		break;

		case ST_JUMP_BACKWARD2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;
			case IN_L_PUNCH2: state = L_PUNCH_BACKWARD_JUMP2; l_punch_timer2 = SDL_GetTicks(); break;
			case IN_L_KIK2: state = L_KIK_BACKWARD_JUMP2; l_kik_timer2 = SDL_GetTicks(); break;
			}
		}
		break;

		case L_PUNCH_NEUTRAL_JUMP2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;
			}
		}
		break;

		case ST_HADOKEN2:
		{
			switch (last_input)
			{
			case IN_HADOKEN_FINISH2: state = ST_IDLE2; break;
			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; reel_timer2 = SDL_GetTicks(); break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; reel_timer2 = SDL_GetTicks(); break;
			}
		}
		break;

		case L_KIK_NEUTRAL_JUMP2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;
			}
		}
		break;

		case L_PUNCH_FORWARD_JUMP2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;
			case IN_L_PUNCH_FINISH2: state = ST_JUMP_FORWARD2; break;
			}
		}
		break;

		case L_KIK_FORWARD_JUMP2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;
			case IN_L_KIK_FINISH2: state = ST_JUMP_FORWARD2; break;
			}
		}
		break;

		case L_PUNCH_BACKWARD_JUMP2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;
			case IN_L_PUNCH_FINISH2: state = ST_JUMP_BACKWARD2; break;
			}
		}
		break;

		case L_KIK_BACKWARD_JUMP2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;
			case IN_L_KIK_FINISH2: state = ST_JUMP_BACKWARD2; break;
			}
		}
		break;

		case L_PUNCH_STANDING2:
		{
			switch (last_input)
			{
			case IN_L_PUNCH_FINISH2: state = ST_IDLE2; break;
			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; reel_timer2 = SDL_GetTicks(); break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; reel_timer2 = SDL_GetTicks(); break;
			}
		}
		break;

		case L_KIK_STANDING2:
		{
			switch (last_input)
			{
			case IN_L_KIK_FINISH2: state = ST_IDLE2; break;
			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; reel_timer2 = SDL_GetTicks(); break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; reel_timer2 = SDL_GetTicks(); break;
			}
		}
		break;
		case ST_CROUCHING2:
		{
			switch (last_input)
			{
			case IN_CROUCHING_FINISH2: state = ST_CROUCH2; break;
			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; reel_timer2 = SDL_GetTicks(); break;
			}
		}
		break;

		case ST_STANDING2:
		{
			switch (last_input)
			{
			case IN_STANDING_FINISH2: state = ST_IDLE2; break;
			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; reel_timer2 = SDL_GetTicks(); break;
			}
		}
		break;


		case ST_CROUCH2:
		{
			switch (last_input)
			{
			case IN_CROUCH_UP2: state = ST_STANDING2; standing_timer2 = SDL_GetTicks(); break;
			case IN_L_PUNCH2: state = L_PUNCH_CROUCH2; l_punch_timer2 = SDL_GetTicks(); break;
			case IN_L_KIK2: state = L_KIK_CROUCH2; l_kik_timer2 = SDL_GetTicks(); break;
			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; reel_timer2 = SDL_GetTicks(); break;
			}
		}
		break;

		case L_PUNCH_CROUCH2:
		{
			switch (last_input)
			{
			case IN_L_PUNCH_FINISH2: state = ST_CROUCH2; break;
			case IN_CROUCH_UP2 && IN_L_PUNCH_FINISH2: state = ST_STANDING2; standing_timer2 = SDL_GetTicks(); break;
			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; reel_timer2 = SDL_GetTicks(); break;
			}
		}
		break;

		case L_KIK_CROUCH2:
		{
			switch (last_input)
			{
			case IN_L_KIK_FINISH2: state = ST_CROUCH2; break;
			case IN_CROUCH_UP2 && IN_L_KIK_FINISH2: state = ST_STANDING2; standing_timer2 = SDL_GetTicks(); break;
			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; reel_timer2 = SDL_GetTicks(); break;
			}
		}
		break;

		case ST_HEAD_REEL2:
		{
			switch (last_input)
			{
			case IN_REEL_FINISH2: state = ST_IDLE2; break;
			}
		}
		break;

		case ST_GUT_REEL2:
		{
			switch (last_input)
			{
			case IN_REEL_FINISH2: state = ST_IDLE2; break;
			}
		}
		break;

		case ST_CROUCH_REEL2:
		{
			switch (last_input)
			{
			case IN_REEL_FINISH2: state = ST_CROUCH2; break;
			}
		}
		break;
		}
	}

	return state;
}
