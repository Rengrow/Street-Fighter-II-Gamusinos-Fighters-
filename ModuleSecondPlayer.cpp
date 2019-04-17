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
	position.x = 250;
	position.y = 220;

	// idle animation (arcade sprite sheet)
	idle.PushBack({ 348, 3, 61, 95 }, 6, 0, {}, {}, {});
	idle.PushBack({ 408, 3, 60, 95 }, 6, 0, {}, {}, {});
	idle.PushBack({ 468, 3, 58, 95 }, 6, 0, {}, {}, {});
	idle.PushBack({ 526, 3, 55, 95 }, 6, 0, {}, {}, {});

	// walk forward animation (arcade sprite sheet)
	forward.PushBack({ 1, 3, 53, 94 }, 6, 0, {}, {}, {});
	forward.PushBack({ 53, 3, 62, 95 }, 6, 0, {}, {}, {});
	forward.PushBack({ 115, 3, 63, 95 }, 6, 0, {}, {}, {});
	forward.PushBack({ 179, 3, 64, 94 }, 6, 0, {}, {}, {});
	forward.PushBack({ 243, 3, 54, 95 }, 6, 0, {}, {}, {});
	forward.PushBack({ 298, 3, 49, 94 }, 6, 0, {}, {}, {});

	// walk backward animation (arcade sprite sheet)
	backward.PushBack({ 694, 3, 56, 93 }, 6, 0, {}, {}, {});
	backward.PushBack({ 752, 3, 60, 93 }, 6, 0, {}, {}, {});
	backward.PushBack({ 813, 3, 58, 93 }, 6, 0, {}, {}, {});
	backward.PushBack({ 872, 3, 56, 93 }, 6, 0, {}, {}, {});
	backward.PushBack({ 930, 3, 57, 93 }, 6, 0, {}, {}, {});
	backward.PushBack({ 0, 102, 57, 92 }, 6, 0, {}, {}, {});


	// lp
	lp.PushBack({ 59, 101, 63, 93 }, 4, 0, {}, {}, {});
	lp.PushBack({ 124, 101, 91, 93 }, 8, 0, {}, {}, {});
	lp.PushBack({ 59, 101, 63, 93 }, 8, 0, {}, {}, {});
	lp.PushBack({ 409, 3, 58, 93 }, 2, 0, {}, {}, {});

	// lk
	lk.PushBack({ 1, 3, 53, 94 }, 6, 0, {}, {}, {});
	lk.PushBack({ 400, 102, 63, 93 }, 6, 0, {}, {}, {});
	lk.PushBack({ 467, 100, 115, 93 }, 16, 0, {}, {}, {});
	lk.PushBack({ 400, 102, 63, 93 }, 8, 0, {}, {}, {});
	lk.PushBack({ 1, 3, 53, 94 }, 4, 0, {}, {}, {});

	// Neutral jump
	neutralJump.PushBack({ 195, 512, 55, 105 }, 4, 0, {}, {}, {});		// La velocidad es adecuada, pero las animaciones están mal / leen mal el tiempo
	neutralJump.PushBack({ 252, 528, 50, 89 }, 4, 0, {}, {}, {});
	neutralJump.PushBack({ 303, 540, 54, 77 }, 8, 0, {}, {}, {});
	neutralJump.PushBack({ 358, 547, 48, 70 }, 12, 0, {}, {}, {});
	neutralJump.PushBack({ 407, 528, 48, 89 }, 20, 0, {}, {}, {});
	neutralJump.PushBack({ 195, 512, 55, 105 }, 4, 0, {}, {}, {});

	//Hadoken
	hdk.PushBack({ 462, 751, 74, 90 }, 8, 0, {}, {}, {});		// Falta un trozo de animación, cuya durada depende de si es light, medium or hard
	hdk.PushBack({ 537, 757, 85, 84 }, 12, 0, {}, {}, {});
	hdk.PushBack({ 623, 758, 90, 83 }, 8, 0, {}, {}, {});
	hdk.PushBack({ 714, 764, 106, 77 }, 40, 0, {}, {}, {});

	// Standing reel
	streel.PushBack({ 143, 857, 67, 92 }, 6, 0, {}, {}, {});		// Ya que no hay colisiones, no se puede ver...
	streel.PushBack({ 213, 857, 69, 91 }, 6, 0, {}, {}, {});
	streel.PushBack({ 285, 857, 80, 91 }, 6, 0, {}, {}, {});
	streel.PushBack({ 367, 857, 66, 91 }, 6, 0, {}, {}, {});

	//Crouching
	crouching.PushBack({ 0, 317, 57, 70 }, 1, 0, {}, {}, {});

	//Standing
	standing.PushBack({ 0, 317, 57, 70 }, 1, 0, {}, {}, {});

	//Crouch
	crouch.PushBack({ 57, 325, 62, 62 }, 1, 0, {}, {}, {});

	//Crouching l punch
	clp.PushBack({ 226, 325, 70, 61 }, 8, 0, {}, {}, {});
	clp.PushBack({ 296, 325, 96, 61 }, 8, 0, {}, {}, {});
	clp.PushBack({ 392, 324, 65, 61 }, 6, 0, {}, {}, {});

	//Crouching l kik
	clk.PushBack({ 617, 322, 71, 65 }, 14, 0, {}, {}, {});
	clk.PushBack({ 688, 322, 113, 65 }, 14, 0, {}, {}, {});
	clk.PushBack({ 617, 322, 71, 65 }, 12, 0, {}, {}, {});
}

ModuleSecondPlayer::~ModuleSecondPlayer()
{}

// Load assets
bool ModuleSecondPlayer::Start()
{
	LOG("Loading player textures");
	bool ret = true;
	graphics = App->textures->Load("assets/images/sprites/characters/ryu1.png"); // arcade version

	Animation* current_animation;

	return ret;
}

bool ModuleSecondPlayer::CleanUp()
{
	LOG("Unloading player 2");

	App->textures->Unload(graphics);
	ClearColliders();

	return true;
}

// Update: draw background
update_status ModuleSecondPlayer::Update()
{
	int speed = 1;


	ryu_states2 current_state = ST_UNKNOWN2;
	Animation* current_animation = &idle;

	external_input(inputs);
	internal_input(inputs);
	ryu_states2 state = process_fsm(inputs);

	if (state != current_state)
	{
		switch (state)
		{
		case ST_IDLE2:
			current_animation = &idle;
			break;

		case ST_WALK_FORWARD2:
			current_animation = &forward;
			position.x--;
			break;

		case ST_WALK_BACKWARD2:
			current_animation = &backward;
			position.x++;
			break;

		case ST_JUMP_NEUTRAL2:
			current_animation = &neutralJump;
			
			if (App->frames - jump_timer > 27 && (App->frames - jump_timer <= JUMP_TIME))
			{
				jumpHeight += speed + 1;
			}
			if (App->frames - jump_timer < 28 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 1;
			}
			break;

		case ST_JUMP_FORWARD2:
			LOG("JUMPING FORWARD ^^>>\n");
			break;

		case ST_JUMP_BACKWARD2:
			LOG("JUMPING BACKWARD ^^<<\n");
			break;

		case ST_CROUCHING2:
			current_animation = &crouching;
			break;

		case ST_CROUCH2:
			current_animation = &crouch;
			break;

		case ST_STANDING2:
			current_animation = &standing;
			break;

		case L_PUNCH_CROUCH2:
			current_animation = &clp;
			break;

		case L_PUNCH_STANDING2:
			current_animation = &lp;
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
			current_animation = &clk;
			break;

		case L_KIK_STANDING2:
			current_animation = &lk;
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
			current_animation = &streel;
			break;

		case ST_GUT_REEL2:
			break;

		case ST_CROUCH_REEL2:
			break;

		case ST_HADOKEN2:
			current_animation = &hdk;
			if (App->frames - hadoken_timer == 42)
			{
				App->particles->AddParticle(App->particles->hdk, flip, position.x + 25, position.y - 70, 0, COLLIDER_PLAYER2_SHOT, App->audio->hdk, 200);
			}
			break;
		}
	}
	current_state = state;

	// Draw everything --------------------------------------	

	BlitCharacterAndAddColliders(current_animation);

	return UPDATE_CONTINUE;
}

void ModuleSecondPlayer::ClearColliders() {
	for (int i = 0; i < MAX_COLLIDERS_PER_FRAME; i++)
	{
		if (colliders[i] != nullptr) {
			colliders[i]->to_delete = true;
			colliders[i] = nullptr;
		}
	}
}

void ModuleSecondPlayer::OnCollision(Collider* c1, Collider* c2) {
	if (c1->type == COLLIDER_PLAYER2 && c2->type == COLLIDER_PLAYER_SHOT)
	{
		App->audio->PlayChunk(App->audio->hdk_hit);
		inputs.Push(IN_HEAD_REEL2);
	}

	if (c1->type == COLLIDER_PLAYER2 && c2->type == COLLIDER_PLAYER)
	{
		if ((position.x + 60) != (App->render->camera.x + App->render->camera.w)) {
			position.x = (App->player->position.x + 63);
		}
		else { App->player->position.x--; }
	}

	if (c1->type == COLLIDER_PLAYER2 && c2->type == COLLIDER_WALL)
	{
		if (position.x == App->render->limit1Box.x) {
			position.x++;
		}
		if (position.x + 60 == (App->render->limit1Box.x + App->render->camera.w)) {
			position.x--;
		}
	}
}

void ModuleSecondPlayer::BlitCharacterAndAddColliders(Animation* current_animation) {
	Frame frame = current_animation->GetCurrentFrame();
	SDL_Rect r;

	int hitboxesQnt = frame.GetColliderQnt();

	ClearColliders();

	for (int i = 0; i < hitboxesQnt; i++)
	{
		r = frame.hitBoxeRects[i];
		colliders[i] = App->collisions->AddCollider({ position.x + r.x, position.y + r.y ,r.w, r.h }, frame.types[i], frame.callbacks[i]);
	}

	r = frame.frame;
	App->render->Blit(graphics, position.x, position.y - r.h + jumpHeight, &r, flip);
}

bool ModuleSecondPlayer::external_input(p2Qeue<ryu_inputs2>& inputs)
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

void ModuleSecondPlayer::internal_input(p2Qeue<ryu_inputs2>& inputs)
{
	if (jump_timer > 0)
	{
		if (App->frames - jump_timer > JUMP_TIME)
		{
			inputs.Push(IN_JUMP_FINISH2);
			jump_timer = 0;
		}
	}

	if (l_punch_timer > 0)
	{
		if (App->frames - l_punch_timer > L_PUNCH_TIME)
		{
			inputs.Push(IN_L_PUNCH_FINISH2);
			l_punch_timer = 0;
		}
	}

	if (l_kik_timer > 0)
	{
		if (App->frames - l_kik_timer > L_KIK_TIME)
		{
			inputs.Push(IN_L_KIK_FINISH2);
			l_kik_timer = 0;
		}
	}

	if (hadoken_timer > 0)
	{
		if (App->frames - hadoken_timer > HADOKEN_TIME)
		{
			inputs.Push(IN_HADOKEN_FINISH2);
			hadoken_timer = 0;
		}
	}

	if (crouching_timer > 0)
	{
		if (App->frames - crouching_timer > CROUCHING_TIME)
		{
			inputs.Push(IN_CROUCHING_FINISH2);
			crouching_timer = 0;
		}
	}

	if (standing_timer > 0)
	{
		if (App->frames - standing_timer > STANDING_TIME)
		{
			inputs.Push(IN_STANDING_FINISH2);
			standing_timer = 0;
		}
	}

	if (reel_timer > 0)
	{
		if (App->frames - reel_timer > REEL_TIME)
		{
			inputs.Push(IN_REEL_FINISH2);
			reel_timer = 0;
		}
	}
}

ryu_states2 ModuleSecondPlayer::process_fsm(p2Qeue<ryu_inputs2>& inputs)
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
			case IN_JUMP2: state = ST_JUMP_NEUTRAL2; jump_timer = App->frames;  break;
			case IN_CROUCH_DOWN2: state = ST_CROUCHING2; crouching_timer = App->frames; break;
			case IN_L_PUNCH2: state = L_PUNCH_STANDING2; l_punch_timer = App->frames;  break;
			case IN_L_KIK2: state = L_KIK_STANDING2; l_kik_timer = App->frames;  break;
			case IN_HADOKEN2: state = ST_HADOKEN2; hadoken_timer = App->frames; break;
			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; reel_timer = App->frames; break;
			}
		}
		break;

		case ST_WALK_FORWARD2:
		{
			switch (last_input)
			{
			case IN_RIGHT_UP2: state = ST_IDLE2; break;
			case IN_LEFT_AND_RIGHT2: state = ST_IDLE2; break;
			case IN_JUMP2: state = ST_JUMP_FORWARD2; jump_timer = App->frames;  break;
			case IN_CROUCH_DOWN2: state = ST_CROUCHING2; crouching_timer = App->frames; break;
			case IN_L_PUNCH2: state = L_PUNCH_STANDING2; l_punch_timer = App->frames;  break;
			case IN_L_KIK2: state = L_KIK_STANDING2; l_kik_timer = App->frames;  break;
			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; reel_timer = App->frames; break;
			}
		}
		break;

		case ST_WALK_BACKWARD2:
		{
			switch (last_input)
			{
			case IN_LEFT_UP2: state = ST_IDLE2; break;
			case IN_LEFT_AND_RIGHT2: state = ST_IDLE2; break;
			case IN_JUMP2: state = ST_JUMP_BACKWARD2; jump_timer = App->frames;  break;
			case IN_CROUCH_DOWN2: state = ST_CROUCHING2; crouching_timer = App->frames; break;
			case IN_L_PUNCH2: state = L_PUNCH_STANDING2; l_punch_timer = App->frames;  break;
			case IN_L_KIK2: state = L_KIK_STANDING2; l_kik_timer = App->frames;  break;
			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; reel_timer = App->frames; break;
			}
		}
		break;

		case ST_JUMP_NEUTRAL2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;
			case IN_L_PUNCH2: state = L_PUNCH_NEUTRAL_JUMP2; l_punch_timer = App->frames; break;
			case IN_L_KIK2: state = L_KIK_NEUTRAL_JUMP2; l_kik_timer = App->frames; break;
			}
		}
		break;

		case ST_JUMP_FORWARD2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;
			case IN_L_PUNCH2: state = L_PUNCH_FORWARD_JUMP2; l_punch_timer = App->frames; break;
			case IN_L_KIK2: state = L_KIK_FORWARD_JUMP2; l_kik_timer = App->frames; break;
			}
		}
		break;

		case ST_JUMP_BACKWARD2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;
			case IN_L_PUNCH2: state = L_PUNCH_BACKWARD_JUMP2; l_punch_timer = App->frames; break;
			case IN_L_KIK2: state = L_KIK_BACKWARD_JUMP2; l_kik_timer = App->frames; break;
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
			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; reel_timer = App->frames; break;
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
			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; reel_timer = App->frames; break;
			}
		}
		break;

		case L_KIK_STANDING2:
		{
			switch (last_input)
			{
			case IN_L_KIK_FINISH2: state = ST_IDLE2; break;
			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; reel_timer = App->frames; break;
			}
		}
		break;
		case ST_CROUCHING2:
		{
			switch (last_input)
			{
			case IN_CROUCHING_FINISH2: state = ST_CROUCH2; break;
			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; reel_timer = App->frames; break;
			}
		}
		break;

		case ST_STANDING2:
		{
			switch (last_input)
			{
			case IN_STANDING_FINISH2: state = ST_IDLE2; break;
			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; reel_timer = App->frames; break;
			}
		}
		break;


		case ST_CROUCH2:
		{
			switch (last_input)
			{
			case IN_CROUCH_UP2: state = ST_STANDING2; standing_timer = App->frames; break;
			case IN_L_PUNCH2: state = L_PUNCH_CROUCH2; l_punch_timer = App->frames; break;
			case IN_L_KIK2: state = L_KIK_CROUCH2; l_kik_timer = App->frames; break;
			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; reel_timer = App->frames; break;
			}
		}
		break;

		case L_PUNCH_CROUCH2:
		{
			switch (last_input)
			{
			case IN_L_PUNCH_FINISH2: state = ST_CROUCH2; break;
			case IN_CROUCH_UP2 && IN_L_PUNCH_FINISH2: state = ST_STANDING2; standing_timer = App->frames; break;
			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; reel_timer = App->frames; break;
			}
		}
		break;

		case L_KIK_CROUCH2:
		{
			switch (last_input)
			{
			case IN_L_KIK_FINISH2: state = ST_CROUCH2; break;
			case IN_CROUCH_UP2 && IN_L_KIK_FINISH2: state = ST_STANDING2; standing_timer = App->frames; break;
			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; reel_timer = App->frames; break;
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
