#ifndef __ModuleSecondPlayer_H__
#define __ModuleSecondPlayer_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "p2Qeue.h"

struct Mix_Chunk;

enum ryu_states2
{
	ST_UNKNOWN2,

	ST_IDLE2,
	ST_WALK_FORWARD2,
	ST_WALK_BACKWARD2,
	ST_JUMP_NEUTRAL2,
	ST_JUMP_FORWARD2,
	ST_JUMP_BACKWARD2,
	ST_CROUCHING2,
	ST_CROUCH2,
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
	ST_FALLING2,

	ST_GETTING_UP2,

	ST_HADOKEN2,

	VICTORY2,
	LOOSE2
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
	IN_FALLING2,

	IN_CROUCHING_FINISH2,
	IN_STANDING_FINISH2,
	IN_JUMP_FINISH2,
	IN_L_PUNCH_FINISH2,
	IN_L_KIK_FINISH2,
	IN_HADOKEN_FINISH2,

	IN_REEL_FINISH2,
	IN_FALLING_FINISH2,
	IN_GETTING_UP_FINISH2,

	IN_VICTORY2,
	IN_LOOSE2
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
	void internal_input(p2Qeue<ryu_inputs2>& inputs);
	bool external_input(p2Qeue<ryu_inputs2>& inputs);
	ryu_states2 process_fsm(p2Qeue<ryu_inputs2>& inputs);
	void OnCollision(Collider* c1, Collider* c2);
	void BlitCharacterAndAddColliders(Animation* current_animation);
	void ClearColliders();

public:
	Collider* colliders[MAX_COLLIDERS_PER_FRAME];
	SDL_Texture* graphics = nullptr;
	Animation idle;
	Animation forward;
	Animation backward;
	Animation lp, lk, clp, clk;
	Animation jlp, jlk, jflp, jflk, jblp, jblk; // (j)umping, (j)umping(f)orward, (j)umping(b)ackward
	Animation neutralJump;
	Animation forwardJump;
	Animation backwardJump;
	Animation hdk; //hadouken
	Animation streel; //standing reel
	Animation stgreel; //standing gut reel
	Animation creel; //crouching reel
	Animation crouching, standing, crouch;
	Mix_Chunk* hdk_voice = nullptr;
	Mix_Chunk* hdk_hit = nullptr;

	iPoint position;
	p2Qeue<ryu_inputs2> inputs;

	bool flip = true;
	bool freeze;
	int framesAtaque = 0;
	int framesJump = 0;
	int mov; //lp, mp, hp, lk, mk, hk
	
	int jumpHeight = 0;
	int speed = 1;
	int life;
	
	Uint32 l_standing_punch_timer = 0;
	Uint32 l_crouching_punch_timer = 0;
	Uint32 l_d_jumping_punch_timer = 0;

	Uint32 l_standing_kik_timer = 0;
	Uint32 l_crouching_kik_timer = 0;
	Uint32 l_d_jumping_kik_timer = 0;

	Uint32 hadoken_timer = 0;
	Uint32 crouching_timer = 0;
	Uint32 standing_timer = 0;
	Uint32 jump_timer = 0;
	Uint32 getting_up_timer = 0;

	Uint32 head_reel_timer = 0;
	Uint32 crouch_reel_timer = 0;
	Uint32 gut_reel_timer = 0;
};

#endif