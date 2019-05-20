#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "p2Qeue.h"

#define R_L_STANDING_PUNCH_TIME 28
#define R_L_CROUCHING_PUNCH_TIME 30
#define R_L_D_JUMPING_PUNCH_TIME 30

#define R_L_STANDING_KIK_TIME 50
#define R_L_CROUCHING_KIK_TIME 30
#define R_L_D_JUMPING_KIK_TIME 20

#define R_M_STANDING_PUNCH_TIME 60
#define R_M_CROUCHING_PUNCH_TIME 36
#define R_M_D_JUMPING_PUNCH_TIME 36

#define R_M_STANDING_KIK_TIME 60
#define R_M_CROUCHING_KIK_TIME 36
#define R_M_D_JUMPING_KIK_TIME 36

#define R_F_STANDING_PUNCH_TIME 70
#define R_F_CROUCHING_PUNCH_TIME 40
#define R_F_D_JUMPING_PUNCH_TIME 40

#define R_F_STANDING_KIK_TIME 70
#define R_F_CROUCHING_KIK_TIME 40
#define R_F_D_JUMPING_KIK_TIME 40

#define R_HADOKEN_TIME 66
#define R_CROUCHING_TIME 10
#define R_STANDING_TIME 10
#define R_JUMP_TIME 55
#define R_GETTING_UP_TIME 48

#define R_HEAD_REEL_TIME 43
#define R_CROUCH_REEL_TIME 20
#define R_GUT_REEL_TIME 25



struct Mix_Chunk;

enum ryu_states
{
	ST_UNKNOWN,

	ST_IDLE,
	ST_WALK_FORWARD,
	ST_WALK_BACKWARD,
	ST_JUMP_NEUTRAL,
	ST_JUMP_FORWARD,
	ST_JUMP_BACKWARD,
	ST_CROUCHING,
	ST_CROUCH,
	ST_STANDING,
	ST_DEFENDING,

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

	M_PUNCH_STANDING,
	M_PUNCH_NEUTRAL_JUMP,
	M_PUNCH_FORWARD_JUMP,
	M_PUNCH_BACKWARD_JUMP,
	M_PUNCH_CROUCH,

	M_KIK_STANDING,
	M_KIK_NEUTRAL_JUMP,
	M_KIK_FORWARD_JUMP,
	M_KIK_BACKWARD_JUMP,
	M_KIK_CROUCH,

	F_PUNCH_STANDING,
	F_PUNCH_NEUTRAL_JUMP,
	F_PUNCH_FORWARD_JUMP,
	F_PUNCH_BACKWARD_JUMP,
	F_PUNCH_CROUCH,

	F_KIK_STANDING,
	F_KIK_NEUTRAL_JUMP,
	F_KIK_FORWARD_JUMP,
	F_KIK_BACKWARD_JUMP,
	F_KIK_CROUCH,

	ST_HEAD_REEL,
	ST_GUT_REEL,
	ST_CROUCH_REEL,
	ST_FALLING,

	ST_GETTING_UP,

	ST_HADOKEN,

	VICTORY,
	LOOSE
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
	IN_DEFENDING,
	IN_IDLE,
	IN_LEFT_AND_JUMP,
	IN_LEFT_AND_CROUCH,
	IN_RIGHT_AND_JUMP,
	IN_RIGHT_AND_CROUCH,

	IN_L_PUNCH,
	IN_L_KIK,
	IN_M_PUNCH,
	IN_M_KIK,
	IN_F_PUNCH,
	IN_F_KIK,
	IN_HADOKEN,

	IN_HEAD_REEL,
	IN_GUT_REEL,
	IN_CROUCH_REEL,
	IN_FALLING,
	IN_GETTING_UP,

	IN_CROUCHING_FINISH,
	IN_STANDING_FINISH,
	IN_JUMP_FINISH,
	IN_PUNCH_FINISH,
	IN_KIK_FINISH,
	IN_HADOKEN_FINISH,

	IN_REEL_FINISH,
	IN_FALLING_FINISH,
	IN_GETTING_UP_FINISH,

	IN_VICTORY,
	IN_LOOSE,
	IN_END
};

struct SDL_Texture;


class ModuleRyu : public Module
{
public:
	ModuleRyu();
	~ModuleRyu();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();
	void internal_input(p2Qeue<ryu_inputs>& inputs);
	bool external_input(p2Qeue<ryu_inputs>& inputs);
	ryu_states process_fsm(p2Qeue<ryu_inputs>& inputs);
	void OnCollision(Collider* c1, Collider* c2);
	void BlitCharacterAndAddColliders(Animation* current_animation, SDL_Texture* texture);
	void ClearColliders();
	bool IsntOnLeftLimit();
	bool IsntOnRightLimit();

public:
	Collider* colliders[MAX_COLLIDERS_PER_FRAME];
	SDL_Texture* graphics = nullptr;
	SDL_Texture* graphics2 = nullptr;
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
	ryu_states state;
	p2Qeue<ryu_inputs> inputs;
	iPoint position;

	int victoryExecuted;
	bool freeze;
	bool godmode = false;
	int framesAtaque = 0;
	int framesJump = 0;
	int jumpHeight = 0;
	int speed = 1;
	int life;
	int mov; //lp, mp, hp, lk, mk, hk
	bool flip = false;
	Uint32 invulnerabilityFrames;

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