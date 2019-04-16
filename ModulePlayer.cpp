#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"
#include "p2Qeue.h"
#include "SDL\include\SDL.h"


ModulePlayer::ModulePlayer()
{
	position.x = 100;
	position.y = 220;

	// idle animation (arcade sprite sheet)
	idle.PushBack({ 348, 3, 61, 95 }, 6, 0, {}, {}, {}, {});
	idle.PushBack({ 408, 3, 60, 95 }, 6, 0, {}, {}, {}, {});
	idle.PushBack({ 468, 3, 58, 95 }, 6, 0, {}, {}, {}, {});
	idle.PushBack({ 526, 3, 55, 95 }, 6, 0, {}, {}, {}, {});

	// walk forward animation (arcade sprite sheet)
	forward.PushBack({ 1, 3, 53, 94 }, 6, 0, {}, {}, {}, {});
	forward.PushBack({ 53, 3, 62, 95 }, 6, 0, {}, {}, {}, {});
	forward.PushBack({ 115, 3, 63, 95 }, 6, 0, {}, {}, {}, {});
	forward.PushBack({ 179, 3, 64, 94 }, 6, 0, {}, {}, {}, {});
	forward.PushBack({ 243, 3, 54, 95 }, 6, 0, {}, {}, {}, {});
	forward.PushBack({ 298, 3, 49, 94 }, 6, 0, {}, {}, {}, {});

	// walk backward animation (arcade sprite sheet)
	backward.PushBack({ 694, 3, 56, 93 }, 6, 0, {}, {}, {}, {});
	backward.PushBack({ 752, 3, 60, 93 }, 6, 0, {}, {}, {}, {});
	backward.PushBack({ 813, 3, 58, 93 }, 6, 0, {}, {}, {}, {});
	backward.PushBack({ 872, 3, 56, 93 }, 6, 0, {}, {}, {}, {});
	backward.PushBack({ 930, 3, 57, 93 }, 6, 0, {}, {}, {}, {});
	backward.PushBack({ 0, 102, 57, 92 }, 6, 0, {}, {}, {}, {});


	// lp
	lp.PushBack({ 59, 101, 63, 93 }, 4, 0, {}, {}, {}, {});
	lp.PushBack({ 124, 101, 91, 93 }, 8, 0, {}, {}, {}, {});
	lp.PushBack({ 59, 101, 63, 93 }, 8, 0, {}, {}, {}, {});
	lp.PushBack({ 409, 3, 58, 93 }, 2, 0, {}, {}, {}, {});

	// lk
	lk.PushBack({ 1, 3, 53, 94 }, 6, 0, {}, {}, {}, {});
	lk.PushBack({ 400, 102, 63, 93 }, 6, 0, {}, {}, {}, {});
	lk.PushBack({ 467, 100, 115, 93 }, 16, 0, {}, {}, {}, {});
	lk.PushBack({ 400, 102, 63, 93 }, 8, 0, {}, {}, {}, {});
	lk.PushBack({ 1, 3, 53, 94 }, 4, 0, {}, {}, {}, {});

	// Neutral jump
	neutralJump.PushBack({ 195, 512, 55, 105 }, 4, 0, {}, {}, {}, {});		// La velocidad es adecuada, pero las animaciones están mal / leen mal el tiempo
	neutralJump.PushBack({ 252, 528, 50, 89 }, 4, 0, {}, {}, {}, {});
	neutralJump.PushBack({ 303, 540, 54, 77 }, 8, 0, {}, {}, {}, {});
	neutralJump.PushBack({ 358, 547, 48, 70 }, 12, 0, {}, {}, {}, {});
	neutralJump.PushBack({ 407, 528, 48, 89 }, 20, 0, {}, {}, {}, {});
	neutralJump.PushBack({ 195, 512, 55, 105 }, 4, 0, {}, {}, {}, {});

	//Hadoken
	hdk.PushBack({ 462, 751, 74, 90 }, 4, 0, {}, {}, {}, {});		// Falta un trozo de animación, cuya durada depende de si es light, medium or hard
	hdk.PushBack({ 537, 757, 85, 84 }, 12, 0, {}, {}, {}, {});
	hdk.PushBack({ 623, 758, 90, 83 }, 2, 0, {}, {}, {}, {});
	hdk.PushBack({ 714, 764, 106, 77 }, 2, 0, {}, {}, {}, {});

	// Standing reel
	streel.PushBack({ 143, 857, 67, 92 }, 6, 0, {}, {}, {}, {});		// Ya que no hay colisiones, no se puede ver...
	streel.PushBack({ 213, 857, 69, 91 }, 6, 0, {}, {}, {}, {});
	streel.PushBack({ 285, 857, 80, 91 }, 6, 0, {}, {}, {}, {});
	streel.PushBack({ 367, 857, 66, 91 }, 6, 0, {}, {}, {}, {});

	//Crouching
	crouching.PushBack({ 0, 317, 57, 70 }, 1, 0, {}, {}, {}, {});

	//Standing
	standing.PushBack({ 0, 317, 57, 70 }, 1, 0, {}, {}, {}, {});

	//Crouch
	crouch.PushBack({ 57, 325, 62, 62 }, 1, 0, {}, {}, {}, {});

	//Crouching l punch
	clp.PushBack({ 226, 325, 70, 61 }, 8, 0, {}, {}, {}, {});
	clp.PushBack({ 296, 325, 96, 61 }, 8, 0, {}, {}, {}, {});
	clp.PushBack({ 392, 324, 65, 61 }, 6, 0, {}, {}, {}, {});

	//Crouching l kik
	clk.PushBack({ 617, 322, 71, 65 }, 14, 0, {}, {}, {}, {});
	clk.PushBack({ 688, 322, 113, 65 }, 14, 0, {}, {}, {}, {});
	clk.PushBack({ 617, 322, 71, 65 }, 12, 0, {}, {}, {}, {});
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player textures");
	bool ret = true;
	graphics = App->textures->Load("assets/images/sprites/characters/ryu1.png"); // arcade version

	Animation* current_animation;

	return ret;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player 1");

	App->textures->Unload(graphics);
	ClearColliders();

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	int speed = 1;
	ryu_states current_state = ST_UNKNOWN;
	Animation* current_animation = &idle;

	App->player->external_input(inputs);
	App->player->internal_input(inputs);
	ryu_states state = process_fsm(inputs);

	if (state != current_state)
	{
		switch (state)
		{
		case ST_IDLE:
			current_animation = &idle;
			break;

		case ST_WALK_FORWARD:
			current_animation = &forward;
			position.x++;
			break;

		case ST_WALK_BACKWARD:
			current_animation = &backward;
			position.x--;
			break;

		case ST_JUMP_NEUTRAL:
			current_animation = &neutralJump;
			if (SDL_GetTicks() - App->player->jump_timer > 1001)
			{
				position.y += speed;
			}
			if (SDL_GetTicks() - App->player->jump_timer < 1000)
			{
				position.y -= speed;
			}

			break;

		case ST_JUMP_FORWARD:
			LOG("JUMPING FORWARD ^^>>\n");
			break;

		case ST_JUMP_BACKWARD:
			LOG("JUMPING BACKWARD ^^<<\n");
			break;

		case ST_CROUCHING:
			current_animation = &crouching;
			break;

		case ST_CROUCH:
			current_animation = &crouch;
			break;

		case ST_STANDING:
			current_animation = &standing;
			break;

		case L_PUNCH_CROUCH:
			current_animation = &clp;
			break;

		case L_PUNCH_STANDING:
			current_animation = &lp;
			break;

		case L_PUNCH_NEUTRAL_JUMP:
			LOG("PUNCH JUMP NEUTRAL ^^++\n");
			break;

		case L_PUNCH_FORWARD_JUMP:
			LOG("PUNCH JUMP FORWARD ^>>+\n");
			break;

		case L_PUNCH_BACKWARD_JUMP:
			LOG("PUNCH JUMP BACKWARD ^<<+\n");
			break;

		case L_KIK_CROUCH:
			current_animation = &clk;
			break;

		case L_KIK_STANDING:
			current_animation = &lk;
			break;

		case L_KIK_NEUTRAL_JUMP:
			LOG("KIK JUMP NEUTRAL ^^++\n");
			break;

		case L_KIK_FORWARD_JUMP:
			LOG("KIK JUMP FORWARD ^>>+\n");
			break;

		case L_KIK_BACKWARD_JUMP:
			LOG("KIK JUMP BACKWARD ^<<+\n");
			break;

		case ST_HEAD_REEL:
			current_animation = &streel;
			break;

		case ST_GUT_REEL:
			break;

		case ST_CROUCH_REEL:
			break;

		case ST_HADOKEN:
			current_animation = &hdk;
			if (SDL_GetTicks() - App->player->hadoken_timer == 550)
			{
				App->particles->AddParticle(App->particles->hdk, flip, position.x + 25, position.y - 70, 0, COLLIDER_PLAYER_SHOT, App->audio->hdk, 200);
			}
			break;
		}
	}
	current_state = state;

	//GOD MODE
	if (App->input->keyboard[SDL_SCANCODE_F5] == KEY_STATE::KEY_DOWN) {
		if (godmode == false)
			godmode = true;
		else
			godmode = false;
	}

	BlitCharacterAndAddColliders(current_animation);

	return UPDATE_CONTINUE;
}

void ModulePlayer::ClearColliders() {
	for (int i = 0; i < MAX_COLLIDERS_PER_FRAME; i++)
	{
		if (colliders[i] != nullptr) {
			colliders[i]->to_delete = true;
			colliders[i] = nullptr;
		}
	}
}

void ModulePlayer::OnCollision(Collider* c1, Collider* c2) {

	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_PLAYER2_SHOT)
	{
		App->audio->PlayChunk(App->audio->hdk_hit);
		inputs.Push(IN_HEAD_REEL);
	}

	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_PLAYER2)
	{
		if (position.x != 0) {
			position.x--;
		}
	}

	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_WALL)
	{
		if (position.x == App->render->limit1Box.x) {
			position.x++;
		}
		if (position.x == (App->render->limit1Box.x + App->render->camera.w)) {
			position.x--;
		}
	}
}

void ModulePlayer::BlitCharacterAndAddColliders(Animation* current_animation) {
	Frame frame = current_animation->GetCurrentFrame();
	SDL_Rect r;

	int hitboxesQnt = frame.GetColliderQnt();

	ClearColliders();

	if (!godmode)
		for (int i = 0; i < hitboxesQnt; i++)
		{
			r = frame.hitBoxeRects[i];
			colliders[i] = App->collisions->AddCollider({ position.x + frame.position[i].x, position.y + frame.position[i].y ,r.w, r.h }, frame.types[i], frame.callbacks[i]);
		}

	r = frame.frame;
	App->render->Blit(graphics, position.x, position.y - r.h, &r, flip);
}

bool ModulePlayer::external_input(p2Qeue<ryu_inputs>& inputs)
{
	static bool left = false;
	static bool right = false;
	static bool down = false;
	static bool up = false;

	//Key UP
	if (App->input->keyboard[SDL_SCANCODE_S] == KEY_STATE::KEY_UP)
	{
		inputs.Push(IN_CROUCH_UP);
		down = false;

	}

	if (App->input->keyboard[SDL_SCANCODE_W] == KEY_STATE::KEY_UP)
	{
		up = false;
	}

	if (App->input->keyboard[SDL_SCANCODE_A] == KEY_STATE::KEY_UP)
	{
		inputs.Push(IN_LEFT_UP);
		left = false;
	}

	if (App->input->keyboard[SDL_SCANCODE_D] == KEY_STATE::KEY_UP)
	{
		inputs.Push(IN_RIGHT_UP);
		right = false;
	}
	//Key down

	if (App->input->keyboard[SDL_SCANCODE_U] == KEY_STATE::KEY_DOWN)
	{
		inputs.Push(IN_L_PUNCH);
	}

	if (App->input->keyboard[SDL_SCANCODE_J] == KEY_STATE::KEY_DOWN)
	{
		inputs.Push(IN_L_KIK);
	}

	if (App->input->keyboard[SDL_SCANCODE_Q] == KEY_STATE::KEY_DOWN)
	{
		inputs.Push(IN_HADOKEN);
	}

	if (App->input->keyboard[SDL_SCANCODE_W] == KEY_STATE::KEY_DOWN || App->input->keyboard[SDL_SCANCODE_W] == KEY_STATE::KEY_REPEAT)
	{
		up = true;
	}

	if (App->input->keyboard[SDL_SCANCODE_S] == KEY_STATE::KEY_DOWN || App->input->keyboard[SDL_SCANCODE_S] == KEY_STATE::KEY_REPEAT)
	{
		down = true;
	}

	if (App->input->keyboard[SDL_SCANCODE_A] == KEY_STATE::KEY_DOWN || App->input->keyboard[SDL_SCANCODE_A] == KEY_STATE::KEY_REPEAT)
	{
		left = true;
	}

	if (App->input->keyboard[SDL_SCANCODE_D] == KEY_STATE::KEY_DOWN || App->input->keyboard[SDL_SCANCODE_D] == KEY_STATE::KEY_REPEAT)
	{
		right = true;
	}


	if (left && right)
		inputs.Push(IN_LEFT_AND_RIGHT);
	{
		if (left)
			inputs.Push(IN_LEFT_DOWN);
		if (right)
			inputs.Push(IN_RIGHT_DOWN);
	}

	if (up && down)
		inputs.Push(IN_JUMP_AND_CROUCH);
	else
	{
		if (down)
			inputs.Push(IN_CROUCH_DOWN);
		else
		{
			inputs.Push(IN_CROUCH_UP);
		}
		if (up)
			inputs.Push(IN_JUMP);
	}

	return true;
}

void ModulePlayer::internal_input(p2Qeue<ryu_inputs>& inputs)
{
	if (App->player->jump_timer > 0)
	{
		if (SDL_GetTicks() - App->player->jump_timer > JUMP_TIME)
		{
			inputs.Push(IN_JUMP_FINISH);
			App->player->jump_timer = 0;
		}
	}

	if (App->player->l_punch_timer > 0)
	{
		if (SDL_GetTicks() - App->player->l_punch_timer > L_PUNCH_TIME)
		{
			inputs.Push(IN_L_PUNCH_FINISH);
			App->player->l_punch_timer = 0;
		}
	}

	if (App->player->l_kik_timer > 0)
	{
		if (SDL_GetTicks() - App->player->l_kik_timer > L_KIK_TIME)
		{
			inputs.Push(IN_L_KIK_FINISH);
			App->player->l_kik_timer = 0;
		}
	}

	if (App->player->hadoken_timer > 0)
	{
		if (SDL_GetTicks() - App->player->hadoken_timer > HADOKEN_TIME)
		{
			inputs.Push(IN_HADOKEN_FINISH);
			App->player->hadoken_timer = 0;
		}
	}

	if (crouching_timer > 0)
	{
		if (SDL_GetTicks() - crouching_timer > CROUCHING_TIME)
		{
			inputs.Push(IN_CROUCHING_FINISH);
			crouching_timer = 0;
		}
	}

	if (standing_timer > 0)
	{
		if (SDL_GetTicks() - standing_timer > STANDING_TIME)
		{
			inputs.Push(IN_STANDING_FINISH);
			standing_timer = 0;
		}
	}

	if (reel_timer > 0)
	{
		if (SDL_GetTicks() - reel_timer > REEL_TIME)
		{
			inputs.Push(IN_REEL_FINISH);
			reel_timer = 0;
		}
	}
}

ryu_states ModulePlayer::process_fsm(p2Qeue<ryu_inputs>& inputs)
{
	static ryu_states state = ST_IDLE;
	ryu_inputs last_input;

	while (inputs.Pop(last_input))
	{
		switch (state)
		{
		case ST_IDLE:
		{
			switch (last_input)
			{
			case IN_RIGHT_DOWN: state = ST_WALK_FORWARD; break;
			case IN_LEFT_DOWN: state = ST_WALK_BACKWARD; break;
			case IN_JUMP: state = ST_JUMP_NEUTRAL; jump_timer = SDL_GetTicks();  break;
			case IN_CROUCH_DOWN: state = ST_CROUCHING; crouching_timer = SDL_GetTicks(); break;
			case IN_L_PUNCH: state = L_PUNCH_STANDING; l_punch_timer = SDL_GetTicks();  break;
			case IN_L_KIK: state = L_KIK_STANDING; l_kik_timer = SDL_GetTicks();  break;
			case IN_HADOKEN: state = ST_HADOKEN; hadoken_timer = SDL_GetTicks(); break;
			case IN_HEAD_REEL: state = ST_HEAD_REEL; reel_timer = SDL_GetTicks(); break;
			case IN_GUT_REEL: state = ST_GUT_REEL; reel_timer = SDL_GetTicks(); break;
			}
		}
		break;

		case ST_WALK_FORWARD:
		{
			switch (last_input)
			{
			case IN_RIGHT_UP: state = ST_IDLE; break;
			case IN_LEFT_AND_RIGHT: state = ST_IDLE; break;
			case IN_JUMP: state = ST_JUMP_FORWARD; jump_timer = SDL_GetTicks();  break;
			case IN_CROUCH_DOWN: state = ST_CROUCHING; crouching_timer = SDL_GetTicks(); break;
			case IN_L_PUNCH: state = L_PUNCH_STANDING; l_punch_timer = SDL_GetTicks();  break;
			case IN_L_KIK: state = L_KIK_STANDING; l_kik_timer = SDL_GetTicks();  break;
			case IN_HEAD_REEL: state = ST_HEAD_REEL; reel_timer = SDL_GetTicks(); break;
			case IN_GUT_REEL: state = ST_GUT_REEL; reel_timer = SDL_GetTicks(); break;
			}
		}
		break;

		case ST_WALK_BACKWARD:
		{
			switch (last_input)
			{
			case IN_LEFT_UP: state = ST_IDLE; break;
			case IN_LEFT_AND_RIGHT: state = ST_IDLE; break;
			case IN_JUMP: state = ST_JUMP_BACKWARD; jump_timer = SDL_GetTicks();  break;
			case IN_CROUCH_DOWN: state = ST_CROUCHING; crouching_timer = SDL_GetTicks(); break;
			case IN_L_PUNCH: state = L_PUNCH_STANDING; l_punch_timer = SDL_GetTicks();  break;
			case IN_L_KIK: state = L_KIK_STANDING; l_kik_timer = SDL_GetTicks();  break;
			case IN_HEAD_REEL: state = ST_HEAD_REEL; reel_timer = SDL_GetTicks(); break;
			case IN_GUT_REEL: state = ST_GUT_REEL; reel_timer = SDL_GetTicks(); break;
			}
		}
		break;

		case ST_JUMP_NEUTRAL:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH: state = ST_IDLE; break;
			case IN_L_PUNCH: state = L_PUNCH_NEUTRAL_JUMP; l_punch_timer = SDL_GetTicks(); break;
			case IN_L_KIK: state = L_KIK_NEUTRAL_JUMP; l_kik_timer = SDL_GetTicks(); break;
			}
		}
		break;

		case ST_JUMP_FORWARD:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH: state = ST_IDLE; break;
			case IN_L_PUNCH: state = L_PUNCH_FORWARD_JUMP; l_punch_timer = SDL_GetTicks(); break;
			case IN_L_KIK: state = L_KIK_FORWARD_JUMP; l_kik_timer = SDL_GetTicks(); break;
			}
		}
		break;

		case ST_JUMP_BACKWARD:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH: state = ST_IDLE; break;
			case IN_L_PUNCH: state = L_PUNCH_BACKWARD_JUMP; l_punch_timer = SDL_GetTicks(); break;
			case IN_L_KIK: state = L_KIK_BACKWARD_JUMP; l_kik_timer = SDL_GetTicks(); break;
			}
		}
		break;

		case L_PUNCH_NEUTRAL_JUMP:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH: state = ST_IDLE; break;
			}
		}
		break;

		case ST_HADOKEN:
		{
			switch (last_input)
			{
			case IN_HADOKEN_FINISH: state = ST_IDLE; break;
			case IN_HEAD_REEL: state = ST_HEAD_REEL; reel_timer = SDL_GetTicks(); break;
			case IN_GUT_REEL: state = ST_GUT_REEL; reel_timer = SDL_GetTicks(); break;
			}
		}
		break;

		case L_KIK_NEUTRAL_JUMP:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH: state = ST_IDLE; break;
			}
		}
		break;

		case L_PUNCH_FORWARD_JUMP:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH: state = ST_IDLE; break;
			case IN_L_PUNCH_FINISH: state = ST_JUMP_FORWARD; break;
			}
		}
		break;

		case L_KIK_FORWARD_JUMP:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH: state = ST_IDLE; break;
			case IN_L_KIK_FINISH: state = ST_JUMP_FORWARD; break;
			}
		}
		break;

		case L_PUNCH_BACKWARD_JUMP:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH: state = ST_IDLE; break;
			case IN_L_PUNCH_FINISH: state = ST_JUMP_BACKWARD; break;
			}
		}
		break;

		case L_KIK_BACKWARD_JUMP:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH: state = ST_IDLE; break;
			case IN_L_KIK_FINISH: state = ST_JUMP_BACKWARD; break;
			}
		}
		break;

		case L_PUNCH_STANDING:
		{
			switch (last_input)
			{
			case IN_L_PUNCH_FINISH: state = ST_IDLE; break;
			case IN_HEAD_REEL: state = ST_HEAD_REEL; reel_timer = SDL_GetTicks(); break;
			case IN_GUT_REEL: state = ST_GUT_REEL; reel_timer = SDL_GetTicks(); break;
			}
		}
		break;

		case L_KIK_STANDING:
		{
			switch (last_input)
			{
			case IN_L_KIK_FINISH: state = ST_IDLE; break;
			case IN_HEAD_REEL: state = ST_HEAD_REEL; reel_timer = SDL_GetTicks(); break;
			case IN_GUT_REEL: state = ST_GUT_REEL; reel_timer = SDL_GetTicks(); break;
			}
		}
		break;

		case ST_CROUCHING:
		{
			switch (last_input)
			{
			case IN_CROUCHING_FINISH: state = ST_CROUCH; break;
			case IN_CROUCH_REEL: state = ST_CROUCH_REEL; reel_timer = SDL_GetTicks(); break;
			}
		}
		break;

		case ST_STANDING:
		{
			switch (last_input)
			{
			case IN_STANDING_FINISH: state = ST_IDLE; break;
			case IN_CROUCH_REEL: state = ST_CROUCH_REEL; reel_timer = SDL_GetTicks(); break;
			}
		}
		break;

		case ST_CROUCH:
		{
			switch (last_input)
			{
			case IN_CROUCH_UP: state = ST_STANDING; standing_timer = SDL_GetTicks(); break;
			case IN_L_PUNCH: state = L_PUNCH_CROUCH; l_punch_timer = SDL_GetTicks(); break;
			case IN_L_KIK: state = L_KIK_CROUCH; l_kik_timer = SDL_GetTicks(); break;
			case IN_CROUCH_REEL: state = ST_CROUCH_REEL; reel_timer = SDL_GetTicks(); break;
			}
		}
		break;

		case L_PUNCH_CROUCH:
		{
			switch (last_input)
			{
			case IN_L_PUNCH_FINISH: state = ST_CROUCH; break;
			case IN_CROUCH_UP && IN_L_PUNCH_FINISH: state = ST_STANDING; standing_timer = SDL_GetTicks(); break;
			case IN_CROUCH_REEL: state = ST_CROUCH_REEL; reel_timer = SDL_GetTicks(); break;
			}
		}
		break;

		case L_KIK_CROUCH:
		{
			switch (last_input)
			{
			case IN_L_KIK_FINISH: state = ST_CROUCH; break;
			case IN_CROUCH_UP && IN_L_KIK_FINISH: state = ST_STANDING; standing_timer = SDL_GetTicks(); break;
			case IN_CROUCH_REEL: state = ST_CROUCH_REEL; reel_timer = SDL_GetTicks(); break;
			}
		}
		break;


		case ST_HEAD_REEL:
		{
			switch (last_input)
			{
			case IN_REEL_FINISH: state = ST_IDLE; break;
			}
		}
		break;

		case ST_GUT_REEL:
		{
			switch (last_input)
			{
			case IN_REEL_FINISH: state = ST_IDLE; break;
			}
		}
		break;

		case ST_CROUCH_REEL:
		{
			switch (last_input)
			{
			case IN_REEL_FINISH: state = ST_CROUCH; break;
			}
		}
		break;
		}

	}

	return state;
}