#ifndef __ModulePlayer2_H__
#define __ModulePlayer2_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "p2Qeue.h"

// All defines have an added +3 frames, unless said otherwise

#define D_L_STANDING_PUNCH_TIME 16//+3
#define D_L_CROUCHING_PUNCH_TIME 23//+5
#define D_L_D_JUMPING_PUNCH_TIME 3003

#define D_L_STANDING_KIK_TIME 22//+4
#define D_L_CROUCHING_KIK_TIME 21//+3
#define D_L_D_JUMPING_KIK_TIME 3003

#define D_M_STANDING_PUNCH_TIME 26//+4
#define D_M_CROUCHING_PUNCH_TIME 23//+5
#define D_M_D_JUMPING_PUNCH_TIME 3003

#define D_M_STANDING_KIK_TIME 22//+4
#define D_M_CROUCHING_KIK_TIME 28//+3
#define D_M_D_JUMPING_KIK_TIME 3003

#define D_F_STANDING_PUNCH_TIME 36//+6
#define D_F_CROUCHING_PUNCH_TIME 36//+8
#define D_F_D_JUMPING_PUNCH_TIME 3003

#define D_F_STANDING_KIK_TIME 35//+7
#define D_F_CROUCHING_KIK_TIME 46//+3
#define D_F_D_JUMPING_KIK_TIME 3003

#define D_DIZZI_TIME 120


#define D_GRABBING_TIME 2

#define D_M_GRAB_TIME 135//+15
#define D_F_GRAB_TIME 40//+3
#define D_M_GRABBED_TIME 135
#define D_F_GRABBED_TIME 80

#define D_HADOKEN_TIME 32//+6 //not altered, pendent of yoga fire
#define D_CROUCHING_TIME 4 //used to be 5
#define D_STANDING_TIME 4 //used to be 5
#define D_JUMP_TIME 55 //ok

#define D_SWEEP_TIME 72

#define D_GETTING_UP_TIME 30//+2 //ok
#define D_DEFENDING_TIME 20 //ok

#define D_HEAD_REEL_TIME 35 //+3
#define D_CROUCH_REEL_TIME 13 //+1
#define D_GUT_REEL_TIME 27 //+3

#define D_LP_YOGA_FIRE 32//+6
#define D_MP_YOGA_FIRE 32//+6
#define D_HP_YOGA_FIRE 32//+6

#define D_LP_YOGA_FLAME 72//+33
#define D_MP_YOGA_FLAME 89//+41
#define D_HP_YOGA_FLAME 109//+50

#define D_YOGA_DRILL 1000
#define D_YOGA_MUMMY 1000

#define D_L_CLOSE_STANDING_KIK_TIME 16//+3
#define D_M_CLOSE_STANDING_KIK_TIME 16//+3
#define D_F_CLOSE_STANDING_KIK_TIME 24//+3
#define D_L_CLOSE_CROUCHING_KIK_TIME 17//+2
#define D_M_CLOSE_CROUCHING_KIK_TIME 14//+2


// following defines made by Adria, not used in State Matrix
#define D_L_CLOSE_STANDING_PUNCH_TIME 16//+3
#define D_M_CLOSE_STANDING_PUNCH_TIME 16//+3
#define D_F_CLOSE_STANDING_PUNCH_TIME 33//+5
#define D_L_CLOSE_CROUCHING_PUNCH_TIME 11//+2
#define D_M_CLOSE_CROUCHING_PUNCH_TIME 15//+2


#define D_TURNING_TIME 6//+3
#define D_CROUCHING_TURNING_TIME 6//+2



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

	ST_CROUCH_DEFENDING2,
	ST_CROUCH_DEFENDING_READY2,
	ST_DEFENDING2,

	M_GRABBING2,
	F_GRABBING2,

	M_GRAB2,
	F_GRAB2,

	M_GRABBED2,
	F_GRABBED2,

	//testing adding states in order to debug animations of close attacks -Adri�
	L_PUNCH_CLOSE2,
	M_PUNCH_CLOSE2,
	F_PUNCH_CLOSE2,

	L_KIK_CLOSE2,
	M_KIK_CLOSE2,
	F_KIK_CLOSE2,

	L_PUNCH_CROUCHCLOSE2,
	M_PUNCH_CROUCHCLOSE2,
	L_KIK_CROUCHCLOSE2,
	M_KIK_CROUCHCLOSE2,
	//end of test


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
	ST_DIZZI2,
	SWEEP2,
	BURNING2,

	ST_GETTING_UP2,

	L_YFIRE2,
	M_YFIRE2,
	F_YFIRE2,

	L_YFLAME2,
	M_YFLAME2,
	F_YFLAME2,

	YMUMMY2,
	YDRILL2,

	TURNING2,
	CROUCH_TURNING2,

	VICTORY2,
	LOOSE2
};

enum ryu_inputs2
{
	IN_LEFT_DOWN2,
	IN_LEFT_UP2,

	IN_LEFT_AND_CROUCH2,
	IN_LEFT_AND_JUMP2,

	IN_RIGHT_DOWN2,
	IN_RIGHT_UP2,

	IN_RIGHT_AND_CROUCH2,
	IN_RIGHT_AND_JUMP2,

	IN_JUMP2,

	IN_CROUCH_DOWN2,
	IN_CROUCH_UP2,

	IN_JUMP_AND_CROUCH2,

	IN_IDLE2,

	IN_DEFENDING2,
	IN_CROUCH_DEFENDING2,

	IN_L_PUNCH2,
	IN_L_KIK2,
	IN_M_PUNCH2,
	IN_M_KIK2,
	IN_F_PUNCH2,
	IN_F_KIK2,

	IN_GRAB2,
	IN_GRABBING_FINISH2,
	IN_M_GRAB_FINISH2,
	IN_F_GRAB_FINISH2,

	IN_HEAD_REEL2,
	IN_GUT_REEL2,
	IN_CROUCH_REEL2,
	IN_FALLING2,
	IN_SWEEP2,
	IN_BURNING2,

	IN_CROUCHING_FINISH2,
	IN_STANDING_FINISH2,
	IN_JUMP_FINISH2,
	IN_PUNCH_FINISH2,
	IN_KIK_FINISH2,
	IN_DEFENDING_FINISH2,

	IN_REEL_FINISH2,
	IN_FALLING_FINISH2,
	IN_GETTING_UP_FINISH2,
	IN_DIZZI_FINISH2,
	IN_SWEEP_FINISH2,
	IN_BURNING_FINISH2,

	IN_L_YFIRE2,
	IN_M_YFIRE2,
	IN_F_YFIRE2,

	IN_L_YFLAME2,
	IN_M_YFLAME2,
	IN_F_YFLAME2,

	IN_YFIRE_FINISH2,
	IN_YFLAME_FINISH2,
	IN_YDRILL_FINISH2,
	IN_YMUMMY_FINISH2,

	IN_M_GRABBED2,
	IN_F_GRABBED2,
	IN_GRABBED_FINISH2,

	IN_TURNING2,
	IN_TURNING_FINISH2,

	IN_VICTORY2,
	IN_LOOSE2,
	IN_END2
};

struct SDL_Texture;

class ModulePlayer2 : public Module
{
public:
	ModulePlayer2();
	~ModulePlayer2();

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
	void IsClose();

public:
	Collider* colliders[MAX_COLLIDERS_PER_FRAME];
	SDL_Texture* graphics = nullptr;
	SDL_Texture* graphics2 = nullptr;
	SDL_Texture* graphics3 = nullptr;
	SDL_Texture* graphics6 = nullptr;
	SDL_Texture* shadow = nullptr;
	Animation idle, forward, backward;
	Animation lp, lk, clp, clk, cmp, cmk, chp, chk;
	Animation close_lp, close_lk, close_clp, close_clk, close_cmp, close_cmk, close_chp, close_chk, close_firstframe_lk_mk;
	Animation jlp, jlk, jmp, jmk, jhp, jhk; // (j)umping, (j)umping(f)orward, (j)umping(b)ackward. Nonexistent variations: jfmp, jbmp,jfmk, jbmk,jfhp, jbhp, jfhk, jbhk
	Animation mp, hp, mk, hk;
	Animation close_mp, close_hp, close_mk, close_hk;
	Animation neutralJump, forwardJump, backwardJump;
	Animation yoga_fire_lp, yoga_fire_mp, yoga_fire_hp, yoga_drill, yoga_mummy;
	Animation yoga_flame_lp, yoga_flame_mp, yoga_flame_hp, burning;
	Animation streel; //standing reel
	Animation stgreel; //standing gut reel
	Animation creel; //crouching reel
	Animation airreel, sweep;
	Animation getup, cdefending, defending, grab, grab2, stun;
	Animation crouching, standing, crouch;
	Animation win1, win2, lose;
	Animation ground;
	Animation grabbing, grabbed, thrown;
	Animation turn_anim, cturn_anim;
	// Animation sweep;
	Mix_Chunk* hdk_hit = nullptr;
	Mix_Chunk* low_kick = nullptr;
	Mix_Chunk* low_fist = nullptr;
	Mix_Chunk* high_fist = nullptr;
	Mix_Chunk* high_kick = nullptr;
	Mix_Chunk* block = nullptr;
	Mix_Chunk* yoga_snd = nullptr;
	Mix_Chunk* fire_snd = nullptr;
	Mix_Chunk* flame_snd = nullptr;

	iPoint position;
	p2Qeue<ryu_inputs2> inputs;
	ryu_states2 state;

	Uint32 invulnerabilityFrames;
	bool wining;
	bool falling;
	bool turn;
	bool flip;
	bool altColor = true;
	bool colliding;
	bool freeze;
	bool yoga_check;
	bool yoga_sound;
	bool flame_sound;
	int victoryExecuted;
	int pushbacktimerhit;
	int pushbacktimerprojectile;
	int pushbackspeed;
	int typeofattack; // 1 = light, 2 = medium, 3 = hard
	int dizzydamage; // 1 =light, 2 = medium, 3 = hard, 4 = special, 5 = throw
	int framesAtaque;
	int framesJump;
	int mov; //lp, mp, hp, lk, mk, hk
	int levitationtimer;
	int dizzylvl;
	Uint32 lasttimedamaged;
	Uint32 timeUpdated;
	Uint32 timeStoped;

	bool dizzi;

	int sprite_change_timer;
	bool close;
	int jumpHeight;
	int speed;
	int life;
	int puntuation;

	//light
	Uint32 l_standing_punch_timer = 0;
	Uint32 l_close_standing_punch_timer = 0;
	Uint32 l_crouching_punch_timer = 0;
	Uint32 l_close_crouching_punch_timer = 0;
	Uint32 l_d_jumping_punch_timer = 0;

	Uint32 l_standing_kik_timer = 0;
	Uint32 l_close_standing_kik_timer = 0;
	Uint32 l_crouching_kik_timer = 0;
	Uint32 l_close_crouching_kik_timer = 0;
	Uint32 l_d_jumping_kik_timer = 0;

	//medium
	Uint32 m_standing_punch_timer = 0;
	Uint32 m_close_standing_punch_timer = 0;
	Uint32 m_crouching_punch_timer = 0;
	Uint32 m_close_crouching_punch_timer = 0;
	Uint32 m_d_jumping_punch_timer = 0;

	Uint32 m_standing_kik_timer = 0;
	Uint32 m_close_standing_kik_timer = 0;
	Uint32 m_crouching_kik_timer = 0;
	Uint32 m_close_crouching_kik_timer = 0;
	Uint32 m_d_jumping_kik_timer = 0;

	Uint32 m_grab_timer = 0;

	//fierce
	Uint32 f_standing_punch_timer = 0;
	Uint32 f_close_standing_punch_timer = 0;
	Uint32 f_crouching_punch_timer = 0;
	Uint32 f_close_crouching_punch_timer = 0;
	Uint32 f_d_jumping_punch_timer = 0;

	Uint32 f_standing_kik_timer = 0;
	Uint32 f_close_standing_kik_timer = 0;
	Uint32 f_crouching_kik_timer = 0;
	Uint32 f_close_crouching_kik_timer = 0;
	Uint32 f_d_jumping_kik_timer = 0;

	Uint32 f_grab_timer = 0;

	//others

	Uint32 grabbing_timer = 0;
	Uint32 m_grabbed_timer = 0;
	Uint32 f_grabbed_timer = 0;

	Uint32 hadoken_timer = 0;
	Uint32 l_yflame_timer = 0;
	Uint32 m_yflame_timer = 0;
	Uint32 f_yflame_timer = 0;
	Uint32 burning_timer = 0;

	Uint32 crouching_timer = 0;
	Uint32 standing_timer = 0;
	Uint32 dizzi_timer = 0;
	Uint32 jump_timer = 0;
	Uint32 getting_up_timer = 0;
	Uint32 defending_timer = 0;
	Uint32 crouch_defending_timer = 0;

	Uint32 turning_timer = 0;
	Uint32 lose_timer = 0;

	Uint32 head_reel_timer = 0;
	Uint32 crouch_reel_timer = 0;
	Uint32 gut_reel_timer = 0;
	Uint32 sweep_timer = 0;
};

#endif