#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "p2Qeue.h"

#define JUMP_TIME 2000
#define L_PUNCH_TIME 353
#define L_KIK_TIME 600
#define HADOKEN_TIME 600
#define CROUCHING_TIME 250
#define STANDING_TIME 250

enum ryu_states
{
	ST_UNKNOWN,

	ST_IDLE,
	ST_WALK_FORWARD,
	ST_WALK_BACKWARD,
	ST_JUMP_NEUTRAL,
	ST_JUMP_FORWARD,
	ST_JUMP_BACKWARD,
	ST_CROUCH,
	ST_CROUCHING,
	ST_STANDING,

	L_PUNCH_STANDING,
	L_PUNCH_NEUTRAL_JUMP,
	L_PUNCH_FORWARD_JUMP,
	L_PUNCH_BACKWARD_JUMP,
	L_PUNCH_CROUCH,

	L_KIK_STANDING,
	L_KIK_NEUTRAL_JUMP,
	L_KIK_FORWARD_JUMP,
	L_KIK_BACKWARD_JUMP,
	L_KIK_CROUCH,

	ST_HADOKEN
};

enum ryu_inputs
{
	IN_LEFT_DOWN,
	IN_LEFT_UP,
	IN_RIGHT_DOWN,
	IN_RIGHT_UP,
	IN_LEFT_AND_RIGHT,
	IN_JUMP,
	IN_CROUCH_UP,
	IN_CROUCH_DOWN,
	IN_JUMP_AND_CROUCH,
	IN_L_PUNCH,
	IN_L_KIK,
	IN_HADOKEN,
	IN_CROUCHING_FINISH,
	IN_STANDING_FINISH,
	IN_JUMP_FINISH,
	IN_L_PUNCH_FINISH,
	IN_L_KIK_FINISH,
	IN_HADOKEN_FINISH
};

struct SDL_Texture;

class ModulePlayer : public Module
{
public:
	ModulePlayer();
	~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();
	void internal_input(p2Qeue<ryu_inputs>& inputs);
	bool external_input(p2Qeue<ryu_inputs>& inputs);
	ryu_states process_fsm(p2Qeue<ryu_inputs>& inputs);

public:
	Collider* collider = nullptr;
	SDL_Texture* graphics = nullptr;
	Animation idle;
	Animation forward;
	Animation backward;
	Animation lp, lk, clp, clk;
	Animation neutralJump;
	Animation hdk;
	Animation streel;
	Animation crouching, standing, crouch;
	iPoint position;
	p2Point <int> pivot = { 100, 220 };		// The pivot is found in the furthest toe from the enemy
	void OnCollision(Collider* c1, Collider* c2);

	bool godmode = false;
	int framesAtaque = 0;
	int framesJump = 0;
	int mov; //lp, mp, hp, lk, mk, hk

	Uint32 jump_timer = 0;
	Uint32 l_punch_timer = 0;
	Uint32 l_kik_timer = 0;
	Uint32 hadoken_timer = 0;
	Uint32 crouching_timer = 0;
	Uint32 standing_timer = 0;
};

#endif