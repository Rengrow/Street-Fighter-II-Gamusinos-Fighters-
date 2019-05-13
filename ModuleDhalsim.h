#ifndef __ModuleSecondPlayer_H__
#define __ModuleSecondPlayer_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "p2Qeue.h"

#define D_L_STANDING_PUNCH_TIME 28
#define D_L_CROUCHING_PUNCH_TIME 30
#define D_L_D_JUMPING_PUNCH_TIME 30

#define D_L_STANDING_KIK_TIME 50
#define D_L_CROUCHING_KIK_TIME 30
#define D_L_D_JUMPING_KIK_TIME 20

#define D_M_STANDING_PUNCH_TIME 1500
#define D_M_CROUCHING_PUNCH_TIME 700
#define D_M_D_JUMPING_PUNCH_TIME 1300

#define D_H_STANDING_PUNCH_TIME 90
#define D_H_CROUCHING_PUNCH_TIME 700
#define D_H_D_JUMPING_PUNCH_TIME 1300

#define D_M_STANDING_KIK_TIME 1800
#define D_M_CROUCHING_KIK_TIME 800
#define D_M_D_JUMPING_KIK_TIME 1350

#define D_F_STANDING_PUNCH_TIME 1500
#define D_F_CROUCHING_PUNCH_TIME 700
#define D_F_D_JUMPING_PUNCH_TIME 1300

#define D_F_STANDING_KIK_TIME 1800
#define D_F_CROUCHING_KIK_TIME 800
#define D_F_D_JUMPING_KIK_TIME 1350

#define D_HADOKEN_TIME 66
#define D_CROUCHING_TIME 10
#define D_STANDING_TIME 10
#define D_JUMP_TIME 55
#define D_GETTING_UP_TIME 45
#define D_DEFENDING_TIME 20

#define D_HEAD_REEL_TIME 43
#define D_CROUCH_REEL_TIME 20
#define D_GUT_REEL_TIME 25

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
	ST_DEFENDING2,

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

	M_PUNCH_STANDING2,
	M_PUNCH_NEUTRAL_JUMP2,
	M_PUNCH_FORWARD_JUMP2,
	M_PUNCH_BACKWARD_JUMP2,
	M_PUNCH_CROUCH2,

	M_KIK_STANDING2,
	M_KIK_NEUTRAL_JUMP2,
	M_KIK_FORWARD_JUMP2,
	M_KIK_BACKWARD_JUMP2,
	M_KIK_CROUCH2,

	F_PUNCH_STANDING2,
	F_PUNCH_NEUTRAL_JUMP2,
	F_PUNCH_FORWARD_JUMP2,
	F_PUNCH_BACKWARD_JUMP2,
	F_PUNCH_CROUCH2,

	F_KIK_STANDING2,
	F_KIK_NEUTRAL_JUMP2,
	F_KIK_FORWARD_JUMP2,
	F_KIK_BACKWARD_JUMP2,
	F_KIK_CROUCH2,

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
	IN_DEFENDING2,

	IN_L_PUNCH2,
	IN_L_KIK2,
	IN_M_PUNCH2,
	IN_M_KIK2,
	IN_F_PUNCH2,
	IN_F_KIK2,
	IN_HADOKEN2,

	IN_HEAD_REEL2,
	IN_GUT_REEL2,
	IN_CROUCH_REEL2,
	IN_FALLING2,

	IN_CROUCHING_FINISH2,
	IN_STANDING_FINISH2,
	IN_JUMP_FINISH2,
	IN_PUNCH_FINISH2,
	IN_KIK_FINISH2,
	IN_HADOKEN_FINISH2,
	IN_DEFENDING_FINISH2,

	IN_REEL_FINISH2,
	IN_FALLING_FINISH2,
	IN_GETTING_UP_FINISH2,

	IN_VICTORY2,
	IN_LOOSE2,
	IN_END2
};

struct SDL_Texture;

class ModuleDhalsim : public Module
{
public:
	ModuleDhalsim();
	~ModuleDhalsim();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();
	void internal_input(p2Qeue<ryu_inputs2>& inputs);
	bool external_input(p2Qeue<ryu_inputs2>& inputs);
	ryu_states2 process_fsm(p2Qeue<ryu_inputs2>& inputs);
	void OnCollision(Collider* c1, Collider* c2);
	void BlitCharacterAndAddColliders(Animation* current_animation, SDL_Texture *texture);
	void ClearColliders();
	bool IsntOnLeftLimit();
	bool IsntOnRightLimit();

public:
	Collider* colliders[MAX_COLLIDERS_PER_FRAME];
	SDL_Texture* graphics = nullptr;
	SDL_Texture* graphics2 = nullptr;
	SDL_Texture* graphics3 = nullptr;
	Animation idle;
	Animation forward;
	Animation backward;
	Animation lp, lk, clp, clk;
	Animation jlp, jlk, jflp, jflk, jblp, jblk; // (j)umping, (j)umping(f)orward, (j)umping(b)ackward
	Animation mp, hp, mk, hk;
	Animation neutralJump;
	Animation forwardJump;
	Animation backwardJump;
	Animation hdk; //hadouken
	Animation streel; //standing reel
	Animation stgreel; //standing gut reel
	Animation creel; //crouching reel
	Animation airreel;
	Animation getup;
	Animation crouching, standing, crouch;
	Animation win1, win2;
	Animation ground;
	// Animation sweep;
	Mix_Chunk* hdk_voice = nullptr;
	Mix_Chunk* hdk_hit = nullptr;
	Mix_Chunk* low_kick = nullptr;
	Mix_Chunk* low_fist = nullptr;
	Mix_Chunk* high_fist = nullptr;
	Mix_Chunk* high_kick = nullptr;

	iPoint position;
	p2Qeue<ryu_inputs2> inputs;
	ryu_states2 state;

	Uint32 invulnerabilityFrames;
	bool flip = true;
	bool altColor = true;
	bool freeze;
	int victoryExecuted;
	int framesAtaque = 0;
	int framesJump = 0;
	int mov; //lp, mp, hp, lk, mk, hk
	
	int jumpHeight = 0;
	int speed = 1;
	int life;
	
	//light
	Uint32 l_standing_punch_timer = 0;
	Uint32 l_crouching_punch_timer = 0;
	Uint32 l_d_jumping_punch_timer = 0;

	Uint32 l_standing_kik_timer = 0;
	Uint32 l_crouching_kik_timer = 0;
	Uint32 l_d_jumping_kik_timer = 0;

	//medium
	Uint32 m_standing_punch_timer = 0;
	Uint32 m_crouching_punch_timer = 0;
	Uint32 m_d_jumping_punch_timer = 0;

	Uint32 m_standing_kik_timer = 0;
	Uint32 m_crouching_kik_timer = 0;
	Uint32 m_d_jumping_kik_timer = 0;

	//fierce
	Uint32 f_standing_punch_timer = 0;
	Uint32 f_crouching_punch_timer = 0;
	Uint32 f_d_jumping_punch_timer = 0;

	Uint32 f_standing_kik_timer = 0;
	Uint32 f_crouching_kik_timer = 0;
	Uint32 f_d_jumping_kik_timer = 0;

	//others
	Uint32 hadoken_timer = 0;
	Uint32 crouching_timer = 0;
	Uint32 standing_timer = 0;
	Uint32 jump_timer = 0;
	Uint32 getting_up_timer = 0;
	Uint32 defending_timer = 0;

	Uint32 head_reel_timer = 0;
	Uint32 crouch_reel_timer = 0;
	Uint32 gut_reel_timer = 0;
};

#endif