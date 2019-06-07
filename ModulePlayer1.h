#ifndef __ModulePlayer1_H__
#define __ModulePlayer1_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "p2Qeue.h"

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

	ST_CROUCH_DEFENDING,
	ST_CROUCH_DEFENDING_READY,
	ST_DEFENDING,

	M_GRABBING,
	F_GRABBING,

	M_GRAB,
	F_GRAB,

	GRABBED,

	//testing adding states in order to debug animations of close attacks -Adrià
	L_PUNCH_CLOSE,
	M_PUNCH_CLOSE,
	F_PUNCH_CLOSE,

	L_KIK_CLOSE,
	M_KIK_CLOSE,
	F_KIK_CLOSE,

	L_PUNCH_CROUCHCLOSE,
	M_PUNCH_CROUCHCLOSE,
	L_KIK_CROUCHCLOSE,
	M_KIK_CROUCHCLOSE,
	//end of test


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
	ST_DIZZI,
	SWEEP,
	BURNING,

	ST_GETTING_UP,

	L_YFIRE,
	M_YFIRE,
	F_YFIRE,

	L_YFLAME,
	M_YFLAME,
	F_YFLAME,

	YMUMMY,
	YDRILL,

	TURNING,
	CROUCH_TURNING,

	VICTORY,
	LOOSE
};

enum ryu_inputs
{
	IN_LEFT_DOWN,
	IN_LEFT_UP,

	IN_LEFT_AND_CROUCH,
	IN_LEFT_AND_JUMP,

	IN_RIGHT_DOWN,
	IN_RIGHT_UP,

	IN_RIGHT_AND_CROUCH,
	IN_RIGHT_AND_JUMP,

	IN_JUMP,

	IN_CROUCH_DOWN,
	IN_CROUCH_UP,

	IN_JUMP_AND_CROUCH,

	IN_IDLE,

	IN_DEFENDING,
	IN_CROUCH_DEFENDING,

	IN_L_PUNCH,
	IN_L_KIK,
	IN_M_PUNCH,
	IN_M_KIK,
	IN_F_PUNCH,
	IN_F_KIK,

	IN_GRAB,
	IN_GRABBING_FINISH,
	IN_M_GRAB_FINISH,
	IN_F_GRAB_FINISH,

	IN_HEAD_REEL,
	IN_GUT_REEL,
	IN_CROUCH_REEL,
	IN_FALLING,
	IN_SWEEP,
	IN_BURNING,

	IN_CROUCHING_FINISH,
	IN_STANDING_FINISH,
	IN_JUMP_FINISH,
	IN_PUNCH_FINISH,
	IN_KIK_FINISH,
	IN_DEFENDING_FINISH,

	IN_REEL_FINISH,
	IN_FALLING_FINISH,
	IN_GETTING_UP_FINISH,
	IN_DIZZI_FINISH,
	IN_SWEEP_FINISH,
	IN_BURNING_FINISH,

	IN_L_YFIRE,
	IN_M_YFIRE,
	IN_F_YFIRE,

	IN_L_YFLAME,
	IN_M_YFLAME,
	IN_F_YFLAME,

	IN_YFIRE_FINISH,
	IN_YFLAME_FINISH,
	IN_YDRILL_FINISH,
	IN_YMUMMY_FINISH,

	IN_GRABBED,
	IN_GRABBED_FINISH,

	IN_TURNING,
	IN_TURNING_FINISH,

	IN_VICTORY,
	IN_LOOSE,
	IN_END
};

struct SDL_Texture;


class ModulePlayer1 : public Module
{
public:
	ModulePlayer1();
	~ModulePlayer1();

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
	void IsClose();

public:
	Collider* colliders[MAX_COLLIDERS_PER_FRAME];
	SDL_Texture* graphics = nullptr;
	SDL_Texture* graphics2 = nullptr;
	SDL_Texture* graphics3 = nullptr;
	SDL_Texture* graphics4 = nullptr;
	SDL_Texture* graphics5 = nullptr;
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

	ryu_states state;
	p2Qeue<ryu_inputs> inputs;
	iPoint position;

	Uint32 invulnerabilityFrames;
	bool turn;
	bool flip;
	bool altColor = false;
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
	bool godmode = false;

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
	Uint32 grabbed_timer = 0;

	Uint32 hadoken_timer = 0;
	Uint32 l_yflame_timer = 0;
	Uint32 m_yflame_timer = 0;
	Uint32 f_yflame_timer = 0;

	Uint32 crouching_timer = 0;
	Uint32 standing_timer = 0;
	Uint32 dizzi_timer = 0;
	Uint32 jump_timer = 0;
	Uint32 getting_up_timer = 0;
	Uint32 defending_timer = 0;
	Uint32 crouch_defending_timer = 0;

	Uint32 turning_timer = 0;

	Uint32 head_reel_timer = 0;
	Uint32 crouch_reel_timer = 0;
	Uint32 gut_reel_timer = 0;
	Uint32 sweep_timer = 0;
	Uint32 burning_timer = 0;
};

#endif