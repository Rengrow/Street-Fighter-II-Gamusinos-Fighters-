#ifndef __ModuleSecondPlayer_H__
#define __ModuleSecondPlayer_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "p2Qeue.h"

enum ryu_states2
{
	ST_UNKNOWN2,

	ST_IDLE2,
	ST_WALK_FORWARD2,
	ST_WALK_BACKWARD2,
	ST_JUMP_NEUTRAL2,
	ST_JUMP_FORWARD2,
	ST_JUMP_BACKWARD2,
	ST_CROUCH2,
	ST_CROUCHING2,
	ST_STANDING2,

	L_PUNCH_STANDING2,
	L_PUNCH_NEUTRAL_JUMP2,
	L_PUNCH_FORWARD_JUMP2,
	L_PUNCH_BACKWARD_JUMP2,
	L_PUNCH_CROUCH2,

	L_KIK_STANDING2,
	L_KIK_NEUTRAL_JUMP2,
	L_KIK_FORWARD_JUMP2,
	L_KIK_BACKWARD_JUMP2,
	L_KIK_CROUCH2,

	ST_HEAD_REEL2,
	ST_GUT_REEL2,
	ST_CROUCH_REEL2,

	ST_HADOKEN2
};

enum ryu_inputs2
{
	IN_LEFT_DOWN2,
	IN_LEFT_UP2,
	IN_RIGHT_DOWN2,
	IN_RIGHT_UP2,
	IN_LEFT_AND_RIGHT2,
	IN_JUMP2,
	IN_CROUCH_UP2,
	IN_CROUCH_DOWN2,
	IN_JUMP_AND_CROUCH2,
	IN_L_PUNCH2,
	IN_L_KIK2,
	IN_HADOKEN2,

	IN_HEAD_REEL2,
	IN_GUT_REEL2,
	IN_CROUCH_REEL2,

	IN_CROUCHING_FINISH2,
	IN_STANDING_FINISH2,
	IN_JUMP_FINISH2,
	IN_L_PUNCH_FINISH2,
	IN_L_KIK_FINISH2,
	IN_HADOKEN_FINISH2,
	IN_REEL_FINISH2
};

struct SDL_Texture;

class ModuleSecondPlayer : public Module
{
public:
	ModuleSecondPlayer();
	~ModuleSecondPlayer();

	bool Start();
	update_status Update();
	bool CleanUp();
	void internal_input2(p2Qeue<ryu_inputs2>& inputs);
	bool external_input2(p2Qeue<ryu_inputs2>& inputs);
	ryu_states2 process_fsm2(p2Qeue<ryu_inputs2>& inputs);
	void OnCollision(Collider* c1, Collider* c2);

public:
	Collider* collider2 = nullptr;
	SDL_Texture* graphics2 = nullptr;
	Animation idle2;
	Animation forward2;
	Animation backward2;
	Animation lp2, lk2, clp2, clk2;
	Animation neutralJump2;
	Animation hdk2;
	Animation streel2;
	Animation crouching2, standing2, crouch2;

	p2Point <int> pivot2 = { 250, 220 };		// The pivot is found in the furthest toe from the enemy. The 250 equals to the x = 0 that the character has respect his spritesheet
	iPoint position2;
	p2Qeue<ryu_inputs2> inputs2;

	bool flip = true;
	int framesAtaque = 0;
	int framesJump = 0;
	int mov; //lp, mp, hp, lk, mk, hk

	Uint32 jump_timer2 = 0;
	Uint32 l_punch_timer2 = 0;
	Uint32 l_kik_timer2 = 0;
	Uint32 hadoken_timer2 = 0;
	Uint32 crouching_timer2 = 0;
	Uint32 standing_timer2 = 0;
	Uint32 reel_timer2 = 0;
};

#endif