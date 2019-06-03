#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePlayer2.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"
#include "ModulePlayer1.h"
#include "ModuleFonts.h"
#include "p2Qeue.h"
#include "ModuleSlowdown.h"
#include "SDL\include\SDL.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModulePlayer2::ModulePlayer2()
{
}

ModulePlayer2::~ModulePlayer2()
{}

// Load assets
bool ModulePlayer2::Start()
{
	LOG("Loading ryu textures");
	bool ret = true;

	if (!altColor) {
		graphics = App->textures->Load("assets/images/sprites/characters/dhalshim1.png"); // arcade version
		graphics2 = App->textures->Load("assets/images/sprites/characters/dhalshim2.png"); // arcade version
		graphics3 = App->textures->Load("assets/images/sprites/characters/dictator-dhalshim.png"); // arcade version
	}
	else {
		graphics = App->textures->Load("assets/images/sprites/characters/colorvar-dhalshim1.png"); // arcade version
		graphics2 = App->textures->Load("assets/images/sprites/characters/colorvar-dhalshim2.png"); // arcade version
		graphics3 = App->textures->Load("assets/images/sprites/characters/colorvar-dictator-dhalshim.png"); // arcade version
	}

	shadow = App->textures->Load("assets/images/sprites/sfx/sfx.png");
	hdk_voice = App->audio->LoadChunk("assets/sfx/voices/ryu_ken_hadouken.wav");
	hdk_hit = App->audio->LoadChunk("assets/sfx/effects/fist_intro.wav");
	low_kick = App->audio->LoadChunk("assets/sfx/effects/low_kick.wav");
	low_fist = App->audio->LoadChunk("assets/sfx/effects/low_fist.wav");
	high_fist = App->audio->LoadChunk("assets/sfx/effects/high_fist.wav");
	high_kick = App->audio->LoadChunk("assets/sfx/effects/high_kick.wav");
	block = App->audio->LoadChunk("assets/sfx/effects/blocked_hadouken.wav");
	yoga_snd = App->audio->LoadChunk("assets/sfx/voices/dhalsim_yoga.wav");
	fire_snd = App->audio->LoadChunk("assets/sfx/voices/dhalsim_fire.wav");
	flame_snd = App->audio->LoadChunk("assets/sfx/voices/dhalsim_flame.wav");

	position.x = 250;
	position.y = 215;

	life = 100;
	freeze = true;
	victoryExecuted = invulnerabilityFrames = dizzylvl = lasttimedamaged = timeUpdated = timeStoped = 0;
	levitationtimer = -100;
	Animation* current_animation;
	// idle animation (arcade sprite sheet)
	const int idlenColliders = 3;
	SDL_Rect idleHitbox[idlenColliders] = { { -25, 76, 24, 16}, { -16, 50, 50, 27}, { -10, 3, 40, 50} };
	COLLIDER_TYPE idleColliderType[idlenColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* idleCallback[idlenColliders] = { {this}, {this}, {this} };

	idle.PushBack({ 848, 438, 72, 99 }, 6, { 33,5 }, idlenColliders, idleHitbox, idleColliderType, idleCallback);
	idle.PushBack({ 922, 438, 67, 99 }, 6, { 33,5 }, idlenColliders, idleHitbox, idleColliderType, idleCallback);
	idle.PushBack({ 694, 448, 76, 89 }, 6, { 33,5 }, idlenColliders, idleHitbox, idleColliderType, idleCallback);
	idle.PushBack({ 772, 444, 74, 93 }, 6, { 33,5 }, idlenColliders, idleHitbox, idleColliderType, idleCallback);

	// Turn (change side animation)
	const int turn_animnColliders = 3;
	SDL_Rect turn_animHitbox[turn_animnColliders] = { { -25, 76, 24, 16}, { -16, 50, 50, 27}, { -10, 3, 40, 50} };
	COLLIDER_TYPE turn_animColliderType[turn_animnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* turn_animCallback[turn_animnColliders] = { {this}, {this}, {this} };

	turn_anim.PushBack({ 374, 657, 61, 94 }, 2, { 33,5 }, turn_animnColliders, turn_animHitbox, turn_animColliderType, turn_animCallback);
	turn_anim.PushBack({ 302, 657, 65, 94 }, 2, { 33,5 }, turn_animnColliders, turn_animHitbox, turn_animColliderType, turn_animCallback);
	turn_anim.PushBack({ 228, 657, 72, 94 }, 2, { 33,5 }, turn_animnColliders, turn_animHitbox, turn_animColliderType, turn_animCallback);

	// walk forward animation (arcade sprite sheet)
	const int forwardnColliders = 3;
	SDL_Rect forwardHitbox[forwardnColliders] = { { -25, 76, 24, 16}, { -16, 50, 50, 27}, { -10, 3, 40, 50} };
	COLLIDER_TYPE forwardColliderType[forwardnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* forwardCallback[forwardnColliders] = { {this}, {this}, {this} };

	forward.PushBack({ 1, 546, 78, 97 }, 4, { 46,5 }, forwardnColliders, forwardHitbox, forwardColliderType, forwardCallback);
	forward.PushBack({ 81, 549, 86, 94 }, 4, { 46,5 }, forwardnColliders, forwardHitbox, forwardColliderType, forwardCallback);
	forward.PushBack({ 172, 544, 72, 99 }, 4, { 46,5 }, forwardnColliders, forwardHitbox, forwardColliderType, forwardCallback);
	forward.PushBack({ 246, 543, 68, 100 }, 4, { 46,5 }, forwardnColliders, forwardHitbox, forwardColliderType, forwardCallback);
	forward.PushBack({ 316, 544, 65, 99 }, 4, { 46,5 }, forwardnColliders, forwardHitbox, forwardColliderType, forwardCallback);
	forward.PushBack({ 383, 546, 68, 97 }, 4, { 46,5 }, forwardnColliders, forwardHitbox, forwardColliderType, forwardCallback);
	forward.PushBack({ 453, 547, 73, 96 }, 4, { 46,5 }, forwardnColliders, forwardHitbox, forwardColliderType, forwardCallback);
	forward.PushBack({ 530, 546, 68, 97 }, 4, { 46,5 }, forwardnColliders, forwardHitbox, forwardColliderType, forwardCallback);
	forward.PushBack({ 600, 544, 65, 99 }, 4, { 46,5 }, forwardnColliders, forwardHitbox, forwardColliderType, forwardCallback);
	forward.PushBack({ 246, 543, 68, 100 }, 4, { 46,5 }, forwardnColliders, forwardHitbox, forwardColliderType, forwardCallback);


	// walk backward animation (arcade sprite sheet)
	const int backwardnColliders = 3;
	SDL_Rect backwardHitbox[backwardnColliders] = { { -18, 79, 24, 16}, { -6, 37, 40, 47}, { -6, 5, 40, 32} };
	COLLIDER_TYPE backwardColliderType[backwardnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* backwardCallback[backwardnColliders] = { {this}, {this}, {this} };

	backward.PushBack({ 737, 542, 71, 101 }, 6, { 45,5 }, backwardnColliders, backwardHitbox, backwardColliderType, backwardCallback);
	backward.PushBack({ 810, 545, 86, 98 }, 6, { 45,5 }, backwardnColliders, backwardHitbox, backwardColliderType, backwardCallback);
	backward.PushBack({ 898, 540, 64, 103 }, 6, { 45,5 }, backwardnColliders, backwardHitbox, backwardColliderType, backwardCallback);
	backward.PushBack({ 667, 539, 68, 104 }, 6, { 45,5 }, backwardnColliders, backwardHitbox, backwardColliderType, backwardCallback);
	backward.PushBack({ 1, 648, 65, 103 }, 6, { 45,5 }, backwardnColliders, backwardHitbox, backwardColliderType, backwardCallback);
	backward.PushBack({ 68, 650, 73, 101 }, 6, { 45,5 }, backwardnColliders, backwardHitbox, backwardColliderType, backwardCallback);
	backward.PushBack({ 143, 651, 83, 100 }, 6, { 45,5 }, backwardnColliders, backwardHitbox, backwardColliderType, backwardCallback);
	backward.PushBack({ 68, 650, 73, 101 }, 6, { 45,5 }, backwardnColliders, backwardHitbox, backwardColliderType, backwardCallback);
	backward.PushBack({ 1, 648, 65, 103 }, 6, { 45,5 }, backwardnColliders, backwardHitbox, backwardColliderType, backwardCallback);
	backward.PushBack({ 667, 539, 68, 104 }, 6, { 45,5 }, backwardnColliders, backwardHitbox, backwardColliderType, backwardCallback);


	// lp

	const int lpnColliders = 3;
	const int lpnColliders2 = 4;
	SDL_Rect lpHitbox[lpnColliders] = { { 0, 0, 0, 0}, { -28, 37, 40, 47}, { -11, 5, 60, 32} };
	SDL_Rect lpHitbox2[lpnColliders2] = { { 0, 0, 0, 0}, { -28, 37, 40, 47}, { -11, 5, 60, 32}, { -66, 55, 60, 15} };
	COLLIDER_TYPE lpColliderType[lpnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	COLLIDER_TYPE lpColliderType2[lpnColliders2] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	Module* lpCallback[lpnColliders] = { {this}, {this}, {this} };
	Module* lpCallback2[lpnColliders2] = { {this}, {this}, {this}, {(Module*)App->player1} };
	lp.PushBack({ 437, 665, 90, 86 }, 2, { 33,5 }, lpnColliders, lpHitbox, lpColliderType, lpCallback);
	lp.PushBack({ 529, 668, 83, 83 }, 3, { 33,5 }, lpnColliders, lpHitbox, lpColliderType, lpCallback);
	lp.PushBack({ 614, 671, 135, 80 }, 6, { 33,5 }, lpnColliders2, lpHitbox2, lpColliderType2, lpCallback2);
	lp.PushBack({ 529, 668, 83, 83 }, 5, { 33,5 }, lpnColliders, lpHitbox, lpColliderType, lpCallback);


	// close lp

	const int close_lpnColliders = 2;
	const int close_lpnColliders2 = 3;
	const int close_lpnColliders3 = 3;
	SDL_Rect close_lpHitbox[close_lpnColliders] = { { -34, 84, 24, 15 },{ -20, 0, 40, 84 } };
	SDL_Rect close_lpHitbox2[close_lpnColliders2] = { { -34, 84, 24, 15 },{ -20, 0, 40, 84 },{ -41, 77, 44, 24 } };
	SDL_Rect close_lpHitbox3[close_lpnColliders3] = { { -34, 84, 24, 15 },{ -20, 0, 40, 84 },{ -41, 77, 70, 12 } };
	COLLIDER_TYPE close_lpColliderType[close_lpnColliders] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 } };
	COLLIDER_TYPE close_lpColliderType2[close_lpnColliders2] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2_HIT } };
	COLLIDER_TYPE close_lpColliderType3[close_lpnColliders3] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2_HIT } };
	Module* close_lpCallback[close_lpnColliders] = { { this },{ this } };
	Module* close_lpCallback2[close_lpnColliders2] = { { this },{ this }, { (Module*)App->player1 } };
	Module* close_lpCallback3[close_lpnColliders3] = { { this },{ this },{ (Module*)App->player1 } };
	close_lp.PushBack({ 70, 24, 73, 110 }, 2, { 33,5 }, close_lpnColliders, close_lpHitbox, close_lpColliderType, close_lpCallback);
	close_lp.PushBack({ 145, 24, 65, 110 }, 3, { 33,5 }, close_lpnColliders2, close_lpHitbox2, close_lpColliderType2, close_lpCallback2);
	close_lp.PushBack({ 212, 57, 106, 77 }, 6, { 33,5 }, close_lpnColliders3, close_lpHitbox3, close_lpColliderType3, close_lpCallback3);
	close_lp.PushBack({ 145, 24, 65, 110 }, 5, { 33,5 }, close_lpnColliders, close_lpHitbox, close_lpColliderType, close_lpCallback);

	//mp
	const int mpnColliders = 3;
	const int mpnColliders2 = 3;
	const int mpnColliders3 = 4;
	const int mpnColliders4 = 4;
	const int mpnColliders5 = 3;

	SDL_Rect mpHitbox[mpnColliders] = { { -50, 75, 20, 16}, { -26, 37, 48, 42}, { -26, 0, 48, 37} };
	SDL_Rect mpHitbox2[mpnColliders2] = { { -65, 59, 24, 20}, { -9, 0, 39, 50}, { -48, 0, 33, 50} };
	SDL_Rect mpHitbox3[mpnColliders3] = { { -75, 32, 24, 20}, { -9, 0, 39, 47}, { -48, 0, 33, 47}, { -110, 32, 50, 20} };
	SDL_Rect mpHitbox4[mpnColliders4] = { { -81, 30, 83, 13}, { -9, 0, 39, 47}, { -48, 0, 33, 47}, {-164, 28, 50, 15} };
	SDL_Rect mpHitbox5[mpnColliders5] = { { -75, 32, 24, 20}, { -9, 0, 39, 47}, { -48, 0, 33, 47} };
	COLLIDER_TYPE mpColliderType[mpnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	COLLIDER_TYPE mpColliderType2[mpnColliders2] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	COLLIDER_TYPE mpColliderType3[mpnColliders3] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	COLLIDER_TYPE mpColliderType4[mpnColliders4] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2},{COLLIDER_PLAYER2_HIT} };
	COLLIDER_TYPE mpColliderType5[mpnColliders5] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* mpCallback[mpnColliders] = { {this}, {this}, {this} };
	Module* mpCallback2[mpnColliders2] = { {this}, {this}, {this} };
	Module* mpCallback3[mpnColliders3] = { {this}, {this}, {this}, {(Module*)App->player1} };
	Module* mpCallback4[mpnColliders4] = { {this}, {this}, {this}, {(Module*)App->player1} };
	Module* mpCallback5[mpnColliders5] = { {this}, {this}, {this} };


	mp.PushBack({ 751, 652, 74, 99 }, 3, { 33,5 }, mpnColliders, mpHitbox, mpColliderType, mpCallback);
	mp.PushBack({ 827, 674, 86, 77 }, 4, { 33,5 }, mpnColliders2, mpHitbox2, mpColliderType2, mpCallback2);
	mp.PushBack({ 0, 816, 150, 46 }, 4, { 33,5 }, mpnColliders3, mpHitbox3, mpColliderType3, mpCallback3);
	mp.PushBack({ 152, 816, 214, 46 }, 6, { 33,5 }, mpnColliders4, mpHitbox4, mpColliderType4, mpCallback4);
	mp.PushBack({ 0, 816, 150, 46 }, 4, { 33,5 }, mpnColliders5, mpHitbox5, mpColliderType5, mpCallback5);
	mp.PushBack({ 827, 674, 86, 77 }, 4, { 33,5 }, mpnColliders2, mpHitbox2, mpColliderType2, mpCallback2);
	//mp.PushBack({ 751, 652, 74, 99 }, 1, { 33,5 }, mpnColliders, mpHitbox, mpColliderType, mpCallback);


	// close mp
	const int close_mpnColliders = 2;
	const int close_mpnColliders2 = 3;
	const int close_mpnColliders3 = 3;
	SDL_Rect close_mpHitbox[close_mpnColliders] = { { -44, 74, 26, 15 },{ -9, 0, 63, 74 } };
	SDL_Rect close_mpHitbox2[close_mpnColliders2] = { { -44, 74, 26, 15 },{ -9, 0, 63, 74 }, { -53, 37, 34, 30} };
	SDL_Rect close_mpHitbox3[close_mpnColliders3] = { { -44, 74, 26, 15 },{ -9, 0, 63, 74 }, { -62, 43, 32, 30} };
	COLLIDER_TYPE close_mpColliderType[close_mpnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	COLLIDER_TYPE close_mpColliderType2[close_mpnColliders2] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	COLLIDER_TYPE close_mpColliderType3[close_mpnColliders3] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	Module* close_mpCallback[close_mpnColliders] = { {this}, {this} };
	Module* close_mpCallback2[close_mpnColliders2] = { {this}, {this}, {this} };
	Module* close_mpCallback3[close_mpnColliders3] = { {this}, {this}, {(Module*)App->player1} };

	close_mp.PushBack({ 320, 50, 80, 84 }, 2, { 33,5 }, close_mpnColliders, close_mpHitbox, close_mpColliderType, close_mpCallback);
	close_mp.PushBack({ 402, 40, 80, 94 }, 3, { 33,5 }, close_mpnColliders2, close_mpHitbox2, close_mpColliderType2, close_mpCallback2);
	close_mp.PushBack({ 484, 1, 80, 133 }, 6, { 33,5 }, close_mpnColliders3, close_mpHitbox3, close_mpColliderType3, close_mpCallback3);
	close_mp.PushBack({ 402, 40, 80, 94 }, 4, { 33,5 }, close_mpnColliders, close_mpHitbox, close_mpColliderType, close_mpCallback);
	close_mp.PushBack({ 320, 50, 80, 84 }, 1, { 33,5 }, close_mpnColliders, close_mpHitbox, close_mpColliderType, close_mpCallback);


	// hp

	const int hpnColliders = 3;
	const int hpnColliders2 = 3;
	const int hpnColliders3 = 3;
	const int hpnColliders4 = 3;
	const int hpnColliders5 = 3;
	SDL_Rect hpHitbox[hpnColliders] = { { -50, 75, 20, 16 },{ -26, 37, 48, 42 },{ -26, 0, 48, 37 } };
	SDL_Rect hpHitbox2[hpnColliders2] = { { -65, 59, 24, 20 },{ -9, 0, 39, 50 },{ -48, 0, 33, 50 } };
	SDL_Rect hpHitbox3[hpnColliders3] = { { -41, 15, 74, 30 },{ -9, 0, 69, 53 },{ -115, 15, 33, 30 } };
	SDL_Rect hpHitbox4[hpnColliders4] = { { -78, 15, 85, 30 },{ -9, 0, 69, 53 },{ -163, 15, 83, 30 } };
	SDL_Rect hpHitbox5[hpnColliders5] = { { -79, 15, 54, 30 }, { -9, 0, 69, 53 }, { 0, 0, 0, 0 } };
	COLLIDER_TYPE hpColliderType[hpnColliders] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 } };
	COLLIDER_TYPE hpColliderType2[hpnColliders2] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 } };
	COLLIDER_TYPE hpColliderType3[hpnColliders3] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2_HIT } };
	COLLIDER_TYPE hpColliderType4[hpnColliders4] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2_HIT } };
	COLLIDER_TYPE hpColliderType5[hpnColliders5] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 } };
	Module* hpCallback[hpnColliders] = { { this },{ this },{ this } };
	Module* hpCallback2[hpnColliders2] = { { this },{ this },{ this } };
	Module* hpCallback3[hpnColliders3] = { { this },{ this },{ (Module*)App->player1 } };
	Module* hpCallback4[hpnColliders4] = { { this },{ this },{ (Module*)App->player1 } };
	Module* hpCallback5[hpnColliders5] = { { this },{ this }, { this } };
	hp.PushBack({ 751, 652, 74, 99 }, 3, { 33,5 }, hpnColliders, hpHitbox, hpColliderType, hpCallback);
	hp.PushBack({ 827, 674, 86, 77 }, 4, { 33,5 }, hpnColliders2, hpHitbox2, hpColliderType2, hpCallback2);
	hp.PushBack({ 368, 816, 151, 46 }, 4, { 33,5 }, hpnColliders3, hpHitbox3, hpColliderType3, hpCallback3);
	hp.PushBack({ 521, 816, 247, 46 }, 6, { 33,5 }, hpnColliders4, hpHitbox4, hpColliderType4, hpCallback4);
	hp.PushBack({ 368, 816, 151, 46 }, 10, { 33,5 }, hpnColliders5, hpHitbox5, hpColliderType5, hpCallback5);
	hp.PushBack({ 827, 674, 86, 77 }, 8, { 33,5 }, hpnColliders2, hpHitbox2, hpColliderType2, hpCallback2);
	hp.PushBack({ 751, 652, 74, 99 }, 1, { 33,5 }, hpnColliders, hpHitbox, hpColliderType, hpCallback);


	// close hp
	const int close_hpnColliders = 3;
	const int close_hpnColliders2 = 3;
	const int close_hpnColliders3 = 3;
	const int close_hpnColliders4 = 3;
	SDL_Rect close_hpHitbox[close_hpnColliders] = { { 0, 0, 0, 0 },{ -30, 71, 24, 20 },{ -13, 1, 46, 76 } };
	SDL_Rect close_hpHitbox2[close_hpnColliders2] = { { -59, 53, 22, 24 },{ -4, 1, 46, 77 },{ -39, 45, 58, 34 } };
	SDL_Rect close_hpHitbox3[close_hpnColliders3] = { { -59, 53, 22, 24 },{ -4, 1, 46, 77 },{ -40, 42, 70, 34 } };
	SDL_Rect close_hpHitbox4[close_hpnColliders4] = { { -59, 53, 22, 24 },{ -4, 1, 46, 77 },{ 0, 0, 0, 0 } };
	COLLIDER_TYPE close_hpColliderType[close_hpnColliders] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 } };
	COLLIDER_TYPE close_hpColliderType2[close_hpnColliders2] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2_HIT } };
	COLLIDER_TYPE close_hpColliderType3[close_hpnColliders3] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2_HIT } };
	COLLIDER_TYPE close_hpColliderType4[close_hpnColliders4] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 } };
	Module* close_hpCallback[close_hpnColliders] = { { this },{ this },{ this } };
	Module* close_hpCallback2[close_hpnColliders2] = { { this },{ this },{ this } };
	Module* close_hpCallback3[close_hpnColliders3] = { { this },{ this },{ (Module*)App->player1 } };
	Module* close_hpCallback4[close_hpnColliders4] = { { this },{ this },{ (Module*)App->player1 } };
	close_hp.PushBack({ 124, 596, 72, 94 }, 2, { 33,5 }, close_hpnColliders, close_hpHitbox, close_hpColliderType, close_hpCallback);
	close_hp.PushBack({ 566, 38, 53, 96 }, 3, { 33,5 }, close_hpnColliders, close_hpHitbox, close_hpColliderType, close_hpCallback);
	close_hp.PushBack({ 621, 46, 80, 88 }, 3, { 33,5 }, close_hpnColliders2, close_hpHitbox2, close_hpColliderType2, close_hpCallback2);
	close_hp.PushBack({ 703, 42, 84, 92 }, 8, { 33,5 }, close_hpnColliders3, close_hpHitbox3, close_hpColliderType3, close_hpCallback3);
	close_hp.PushBack({ 621, 46, 80, 88 }, 10, { 33,5 }, close_hpnColliders4, close_hpHitbox4, close_hpColliderType4, close_hpCallback4);
	close_hp.PushBack({ 566, 38, 53, 96 }, 7, { 33,5 }, close_hpnColliders, close_hpHitbox, close_hpColliderType, close_hpCallback);

	// lk
	const int lknColliders = 3;
	const int lknColliders3 = 4;
	SDL_Rect lkHitbox[lknColliders] = { { -15, 79, 44, 16}, { -6, 37, 40, 47}, { -6, 0, 40, 37} };
	SDL_Rect lkHitbox3[lknColliders3] = { { 3, 84, 50, 27}, { -13, 57, 100, 27}, { -6, 0, 40, 57}, { -100, 77, 67, 26} };
	COLLIDER_TYPE lkColliderType[lknColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	COLLIDER_TYPE lkColliderType3[lknColliders3] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	Module* lkCallback[lknColliders] = { {this}, {this}, {this} };
	Module* lkCallback3[lknColliders3] = { {this}, {this}, {this}, {(Module*)App->player1} };
	lk.PushBack({ 770, 770, 58, 92 }, 2, { 32,5 }, lknColliders, lkHitbox, lkColliderType, lkCallback);
	lk.PushBack({ 842, 753, 118, 109 }, 3, { 33,5 }, lknColliders, lkHitbox, lkColliderType, lkCallback);
	lk.PushBack({ 0, 896, 165, 109 }, 8, { 33,5 }, lknColliders3, lkHitbox3, lkColliderType3, lkCallback3);
	lk.PushBack({ 842, 753, 118, 109 }, 4, { 33,5 }, lknColliders, lkHitbox, lkColliderType, lkCallback);
	lk.PushBack({ 770, 770, 58, 92 }, 5, { 33,5 }, lknColliders, lkHitbox, lkColliderType, lkCallback);


	// close_firstframe_lk_mk
	const int close_firstframe_lk_mknColliders = 2;
	SDL_Rect close_firstframe_lk_mkHitbox[close_firstframe_lk_mknColliders] = { { -38, 69, 22, 20}, { -11, 0, 50, 74} };
	COLLIDER_TYPE close_firstframe_lk_mkColliderType[close_firstframe_lk_mknColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* close_firstframe_lk_mkCallback[close_firstframe_lk_mknColliders] = { {this}, {this} };
	close_firstframe_lk_mk.PushBack({ 770, 770, 58, 92 }, 3, { 33,5 }, close_firstframe_lk_mknColliders, close_firstframe_lk_mkHitbox, close_firstframe_lk_mkColliderType, close_firstframe_lk_mkCallback);


	// close_lk
	const int close_lknColliders = 3;
	SDL_Rect close_lkHitbox[close_lknColliders] = { { -54, 69, 22, 20}, { -27, 0, 50, 74}, { -65, 71, 80, 28} };
	COLLIDER_TYPE close_lkColliderType[close_lknColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	Module* close_lkCallback[close_lknColliders] = { {this}, {this}, {(Module*)App->player1} };
	close_lk.PushBack({ 789, 41, 123, 93 }, 8, { 33,5 }, close_lknColliders, close_lkHitbox, close_lkColliderType, close_lkCallback);


	// mk
	const int mknColliders1 = 3;
	const int mknColliders2 = 2;
	const int mknColliders3 = 3;
	const int mknColliders4 = 3;
	SDL_Rect mkHitbox1[mknColliders1] = { { -15, 79, 44, 16 },{ -6, 37, 40, 47 },{ -6, 0, 40, 37 } };
	SDL_Rect mkHitbox2[mknColliders2] = { { -10, 69, 104, 26 },{ -6, 0, 40, 84 } };
	SDL_Rect mkHitbox3[mknColliders3] = { { -18, 57, 100, 37 },{ 0, 0, 40, 77 },{ -110, 84, 67, 26 } };
	SDL_Rect mkHitbox4[mknColliders4] = { { -15, 79, 94, 16 },{ -6, 37, 40, 47 },{ -6, 0, 40, 37 } };
	COLLIDER_TYPE mkColliderType1[mknColliders1] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 } };
	COLLIDER_TYPE mkColliderType2[mknColliders2] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 } };
	COLLIDER_TYPE mkColliderType3[mknColliders3] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2_HIT } };
	COLLIDER_TYPE mkColliderType4[mknColliders4] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 } };
	Module* mkCallback1[mknColliders1] = { { this },{ this },{ this } };
	Module* mkCallback2[mknColliders2] = { { this },{ this } };
	Module* mkCallback3[mknColliders3] = { { this },{ this },{ (Module*)App->player1 } };
	Module* mkCallback4[mknColliders4] = { { this },{ this }, {this} };
	mk.PushBack({ 770, 770, 58, 92 }, 2, { 32,5 }, mknColliders1, mkHitbox1, mkColliderType1, mkCallback1);
	mk.PushBack({ 167, 910, 115, 96 }, 3, { 33,5 }, mknColliders2, mkHitbox2, mkColliderType2, mkCallback2);
	mk.PushBack({ 284, 896, 177, 109 }, 8, { 33,5 }, mknColliders3, mkHitbox3, mkColliderType3, mkCallback3);
	mk.PushBack({ 770, 770, 58, 92 }, 4, { 33,5 }, mknColliders4, mkHitbox4, mkColliderType4, mkCallback4);
	mk.PushBack({ 770, 770, 58, 92 }, 5, { 33,5 }, mknColliders1, mkHitbox1, mkColliderType1, mkCallback1);


	// close_mk
	const int close_mknColliders = 3;
	SDL_Rect close_mkHitbox[close_mknColliders] = { { -13, 69, 22, 20}, { 14, 0, 50, 74}, { -20, 70, 84, 26} };
	COLLIDER_TYPE close_mkColliderType[close_mknColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	Module* close_mkCallback[close_mknColliders] = { {this}, {this}, {(Module*)App->player1} };
	close_mk.PushBack({ 914, 32, 103, 102 }, 8, { 33,5 }, close_mknColliders, close_mkHitbox, close_mkColliderType, close_mkCallback);


	//hk
	const int hknColliders = 3;
	const int hknColliders2 = 4;
	SDL_Rect hkHitbox[hknColliders] = { { -25, 76, 24, 16}, { -6, 37, 40, 47}, { -6, 0, 40, 37} };
	SDL_Rect hkHitbox2[hknColliders2] = { {-3, 75, 100, 27},{-23, 55, 100, 27}, { -6, 0, 40, 57}, { -117, 75, 67, 30} };
	COLLIDER_TYPE hkColliderType[hknColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	COLLIDER_TYPE hkColliderType2[hknColliders2] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	Module* hkCallback[hknColliders] = { {this}, {this}, {this} };
	Module* hkCallback2[hknColliders2] = { {this}, {this}, {this}, {(Module*)App->player1} };


	hk.PushBack({ 463, 909, 73, 96 }, 1, { 32,5 }, hknColliders, hkHitbox, hkColliderType, hkCallback);
	hk.PushBack({ 538, 899, 54, 106 }, 2, { 33,5 }, hknColliders, hkHitbox, hkColliderType, hkCallback);
	hk.PushBack({ 595, 901, 50, 104 }, 3, { 33,5 }, hknColliders, hkHitbox, hkColliderType, hkCallback);
	hk.PushBack({ 647, 903, 103, 102 }, 4, { 33,5 }, hknColliders, hkHitbox, hkColliderType, hkCallback);
	hk.PushBack({ 752, 900, 184, 105 }, 6, { 33,5 }, hknColliders2, hkHitbox2, hkColliderType2, hkCallback2);
	hk.PushBack({ 647, 903, 103, 102 }, 10, { 33,5 }, hknColliders, hkHitbox, hkColliderType, hkCallback);
	hk.PushBack({ 595, 901, 50, 104 }, 8, { 33,5 }, hknColliders, hkHitbox, hkColliderType, hkCallback);
	hk.PushBack({ 538, 899, 54, 106 }, 1, { 33,5 }, hknColliders, hkHitbox, hkColliderType, hkCallback);


	// close_hk
	const int close_hknColliders = 3;
	const int close_hknColliders2 = 4;
	SDL_Rect close_hkHitbox[close_hknColliders] = { { 0, 0, 0, 0}, { -9, 1, 46, 76}, { -26, 71, 24, 20} };
	SDL_Rect close_hkHitbox2[close_hknColliders2] = { { 0, 0, 0, 0}, { -9, 1, 46, 76}, { -26, 71, 24, 20}, { -24, 39, 74, 40} };
	COLLIDER_TYPE close_hkColliderType[close_hknColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	COLLIDER_TYPE close_hkColliderType2[close_hknColliders2] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	Module* close_hkCallback[close_hknColliders] = { {this}, {this}, {this} };
	Module* close_hkCallback2[close_hknColliders2] = { {this}, {this}, {this}, {(Module*)App->player1} };


	close_hk.PushBack({ 209, 158, 63, 87 }, 3, { 32,5 }, close_hknColliders, close_hkHitbox, close_hkColliderType, close_hkCallback);
	close_hk.PushBack({ 0, 145, 67, 100 }, 3, { 33,5 }, close_hknColliders, close_hkHitbox, close_hkColliderType, close_hkCallback);
	close_hk.PushBack({ 70, 151, 55, 94 }, 2, { 33,5 }, close_hknColliders, close_hkHitbox, close_hkColliderType, close_hkCallback);
	close_hk.PushBack({ 131, 139, 76, 106 }, 8, { 33,5 }, close_hknColliders2, close_hkHitbox2, close_hkColliderType2, close_hkCallback2);
	close_hk.PushBack({ 70, 151, 55, 94 }, 3, { 33,5 }, close_hknColliders, close_hkHitbox, close_hkColliderType, close_hkCallback);
	close_hk.PushBack({ 0, 145, 67, 100 }, 4, { 33,5 }, close_hknColliders, close_hkHitbox, close_hkColliderType, close_hkCallback);
	close_hk.PushBack({ 209, 158, 63, 87 }, 1, { 33,5 }, close_hknColliders, close_hkHitbox, close_hkColliderType, close_hkCallback);


	// Neutral jump 50
	const int neutraljumpnColliders = 3;
	const int neutraljumpnColliders2 = 3;
	SDL_Rect neutraljumpHitbox[neutraljumpnColliders] = { { -5, 81, 42, 36}, { -6, 37, 40, 47}, { -6, 5, 40, 32} };
	SDL_Rect neutraljumpHitbox2[neutraljumpnColliders2] = { { 0, 0, 0, 0}, { -6, 37, 40, 47}, { -6, 5, 40, 32} };
	COLLIDER_TYPE neutraljumpColliderType[neutraljumpnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	COLLIDER_TYPE neutraljumpColliderType2[neutraljumpnColliders2] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* neutraljumpCallback[neutraljumpnColliders] = { {this}, {this}, {this} };
	Module* neutraljumpCallback2[neutraljumpnColliders2] = { {this}, {this}, {this} };
	neutralJump.PushBack({ 1, 364, 57, 122 }, 28, { 33,5 }, neutraljumpnColliders, neutraljumpHitbox, neutraljumpColliderType, neutraljumpCallback);		// La velocidad es adecuada, pero las animaciones están mal / leen mal el tiempo
	neutralJump.PushBack({ 60, 398, 57, 88 }, 14, { 33,5 }, neutraljumpnColliders2, neutraljumpHitbox2, neutraljumpColliderType2, neutraljumpCallback2);
	neutralJump.PushBack({ 119, 417, 58, 69 }, 14, { 33,5 }, neutraljumpnColliders2, neutraljumpHitbox2, neutraljumpColliderType2, neutraljumpCallback2);
	neutralJump.PushBack({ 60, 398, 57, 88 }, 14, { 33,5 }, neutraljumpnColliders2, neutraljumpHitbox2, neutraljumpColliderType2, neutraljumpCallback2);
	neutralJump.PushBack({ 1, 364, 57, 122 }, 28, { 33,5 }, neutraljumpnColliders, neutraljumpHitbox, neutraljumpColliderType, neutraljumpCallback);		// La velocidad es adecuada, pero las animaciones están mal / leen mal el tiempo


	// forward jump 50
	const int forwardjumpnColliders = 3;
	SDL_Rect forwardjumpHitbox[forwardjumpnColliders] = { { -5, 99, 24, 16}, { -6, 37, 40, 67}, { -6, 5, 40, 32} };
	SDL_Rect forwardjumpHitbox2[forwardjumpnColliders] = { { 0, 0, 0, 0}, { -6, 37, 55, 55}, { -6, 5, 40, 32} };
	SDL_Rect forwardjumpHitbox3[forwardjumpnColliders] = { { 0, 0, 0, 0}, { -6, 0, 50, 60}, { 0, 0, 0, 0} };
	SDL_Rect forwardjumpHitbox4[forwardjumpnColliders] = { { 0, 0, 0, 0}, { -5, 40, 80, 37}, { -65, 0, 32, 40} };
	COLLIDER_TYPE forwardjumpColliderType[forwardjumpnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* forwardjumpCallback[forwardjumpnColliders] = { {this}, {this}, {this} };

	forwardJump.PushBack({ 714, 247, 56, 114 }, 8, { 33,5 }, forwardjumpnColliders, forwardjumpHitbox, forwardjumpColliderType, forwardjumpCallback);		// La velocidad es adecuada, pero las animaciones están mal / leen mal el tiempo
	forwardJump.PushBack({ 772, 272, 66, 89 }, 6, { 33,5 }, forwardjumpnColliders, forwardjumpHitbox2, forwardjumpColliderType, forwardjumpCallback);
	forwardJump.PushBack({ 840, 294, 59, 67 }, 7, { 33,5 }, forwardjumpnColliders, forwardjumpHitbox3, forwardjumpColliderType, forwardjumpCallback);
	forwardJump.PushBack({ 901, 271, 94, 90 }, 4, { 33,5 }, forwardjumpnColliders, forwardjumpHitbox4, forwardjumpColliderType, forwardjumpCallback);
	forwardJump.PushBack({ 714, 247, 56, 114 }, 58, { 33,5 }, forwardjumpnColliders, forwardjumpHitbox, forwardjumpColliderType, forwardjumpCallback);		// La velocidad es adecuada, pero las animaciones están mal / leen mal el tiempo


	// backward jump 50
	const int backwardjumpnColliders = 3;
	SDL_Rect backwardjumpHitbox[forwardjumpnColliders] = { { -5, 99, 24, 16}, { -6, 37, 40, 67}, { -6, 5, 40, 32} };
	SDL_Rect backwardjumpHitbox2[forwardjumpnColliders] = { { 0, 0, 0, 0}, { -6, 37, 55, 55}, { -6, 5, 40, 32} };
	SDL_Rect backwardjumpHitbox3[forwardjumpnColliders] = { { 0, 0, 0, 0}, { -6, 0, 50, 60}, { 0, 0, 0, 0} };
	SDL_Rect backwardjumpHitbox4[forwardjumpnColliders] = { { 0, 0, 0, 0}, { -5, 40, 80, 37}, { -65, 0, 32, 40} };
	COLLIDER_TYPE backwardjumpColliderType[backwardjumpnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* backwardjumpCallback[backwardjumpnColliders] = { {this}, {this}, {this} };

	backwardJump.PushBack({ 714, 247, 56, 114 }, 8, { 33,5 }, backwardjumpnColliders, backwardjumpHitbox, backwardjumpColliderType, backwardjumpCallback);		// La velocidad es adecuada, pero las animaciones están mal / leen mal el tiempo
	backwardJump.PushBack({ 901, 271, 94, 90 }, 4, { 33,5 }, backwardjumpnColliders, backwardjumpHitbox4, backwardjumpColliderType, backwardjumpCallback);
	backwardJump.PushBack({ 840, 294, 59, 67 }, 7, { 33,5 }, backwardjumpnColliders, backwardjumpHitbox3, backwardjumpColliderType, backwardjumpCallback);
	backwardJump.PushBack({ 772, 272, 66, 89 }, 6, { 33,5 }, backwardjumpnColliders, backwardjumpHitbox2, backwardjumpColliderType, backwardjumpCallback);
	backwardJump.PushBack({ 714, 247, 56, 114 }, 58, { 33,5 }, backwardjumpnColliders, backwardjumpHitbox, backwardjumpColliderType, backwardjumpCallback);		// La velocidad es adecuada, pero las animaciones están mal / leen mal el tiempo


	// Jumping neutral / diagonal lp
	const int jlpnColliders = 3;
	const int jlpnColliders2 = 5;
	SDL_Rect jlpHitbox[jlpnColliders] = { { 0, 0, 0, 0}, { 0, 27, 48, 51}, { 0, 0, 48, 27} };
	SDL_Rect jlpHitbox2[jlpnColliders2] = { { -103, 21, 34, 26}, { -63, 41, 34, 26}, { 0, 27, 68, 51}, { 0, 0, 48, 27}, {-138, 0, 35, 30} };
	COLLIDER_TYPE jlpColliderType[jlpnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	COLLIDER_TYPE jlpColliderType2[jlpnColliders2] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	Module*jlpCallback[jlpnColliders] = { {this}, {this}, {this} };
	Module*jlpCallback2[jlpnColliders2] = { {this}, {this}, {this}, {this}, {(Module*)App->player1} };

	jlp.PushBack({ 179, 398, 58, 88 }, 3, { 33,5 }, { jlpnColliders }, { jlpHitbox }, { jlpColliderType }, { jlpCallback });
	jlp.PushBack({ 239, 410, 111, 76 }, 4, { 33,5 }, { jlpnColliders }, { jlpHitbox }, { jlpColliderType }, { jlpCallback });
	jlp.PushBack({ 352, 401, 173, 85 }, 16, { 33,5 }, { jlpnColliders2 }, { jlpHitbox2 }, { jlpColliderType2 }, { jlpCallback2 });
	jlp.PushBack({ 239, 410, 111, 76 }, 4, { 33,5 }, { jlpnColliders }, { jlpHitbox }, { jlpColliderType }, { jlpCallback });
	jlp.PushBack({ 179, 398, 58, 88 }, 2973, { 33,5 }, { jlpnColliders }, { jlpHitbox }, { jlpColliderType }, { jlpCallback });


	// Jumping neutral / diagonal lk
	const int jlknColliders = 3;
	const int jlknColliders2 = 5;
	SDL_Rect jlkHitbox[jlknColliders] = { { 0, 0, 0, 0}, { -10, 27, 48, 51}, { -10, 0, 48, 27} };
	SDL_Rect jlkHitbox2[jlknColliders2] = { { -120, 0, 34, 26}, { -73, 17, 34, 26}, { -10, 27, 61, 51}, { -10, 0, 48, 27}, {-163, 0, 35, 24} };
	COLLIDER_TYPE jlkColliderType[jlknColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	COLLIDER_TYPE jlkColliderType2[jlknColliders2] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	Module*jlkCallback[jlknColliders] = { {this}, {this}, {this} };
	Module*jlkCallback2[jlknColliders2] = { {this}, {this}, {this}, {this}, {(Module*)App->player1} };

	jlk.PushBack({ 527, 396, 76, 90 }, 4, { 33,5 }, { jlknColliders }, { jlkHitbox }, { jlkColliderType }, { jlkCallback });
	jlk.PushBack({ 605, 415, 133, 71 }, 6, { 33,5 }, { jlknColliders }, { jlkHitbox }, { jlkColliderType }, { jlkCallback });
	jlk.PushBack({ 740, 404, 202, 82 }, 18, { 33,5 }, { jlknColliders2 }, { jlkHitbox2 }, { jlkColliderType2 }, { jlkCallback2 });
	jlk.PushBack({ 605, 415, 133, 71 }, 4, { 33,5 }, { jlknColliders }, { jlkHitbox }, { jlkColliderType }, { jlkCallback });
	jlk.PushBack({ 527, 396, 76, 90 }, 50, { 33,5 }, { jlknColliders }, { jlkHitbox }, { jlkColliderType }, { jlkCallback });


	//jumping neutral / diagonal mp
	const int jmpnColliders = 3;
	const int jmpnColliders2 = 5;
	SDL_Rect jmpHitbox[jmpnColliders] = { { 0, 0, 0, 0 },{ 0, 27, 48, 51 },{ 0, 0, 48, 27 } };
	SDL_Rect jmpHitbox2[jmpnColliders2] = { { -103, 21, 34, 26 },{ -63, 41, 34, 26 },{ 0, 27, 68, 51 },{ 0, 0, 48, 27 },{ -138, 0, 35, 30 } };
	COLLIDER_TYPE jmpColliderType[jmpnColliders] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 } };
	COLLIDER_TYPE jmpColliderType2[jmpnColliders2] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2_HIT } };
	Module*jmpCallback[jmpnColliders] = { { this },{ this },{ this } };
	Module*jmpCallback2[jmpnColliders2] = { { this },{ this },{ this },{ this },{ (Module*)App->player1 } };

	jmp.PushBack({ 179, 398, 58, 88 }, 3, { 33,5 }, { jmpnColliders }, { jmpHitbox }, { jmpColliderType }, { jmpCallback });
	jmp.PushBack({ 239, 410, 111, 76 }, 4, { 33,5 }, { jmpnColliders }, { jmpHitbox }, { jmpColliderType }, { jmpCallback });
	jmp.PushBack({ 352, 401, 173, 85 }, 10, { 33,5 }, { jmpnColliders2 }, { jmpHitbox2 }, { jmpColliderType2 }, { jmpCallback2 });
	jmp.PushBack({ 239, 410, 111, 76 }, 4, { 33,5 }, { jmpnColliders }, { jmpHitbox }, { jmpColliderType }, { jmpCallback });
	jmp.PushBack({ 179, 398, 58, 88 }, 2979, { 33,5 }, { jmpnColliders }, { jmpHitbox }, { jmpColliderType }, { jmpCallback });

	//jumping neutral / diagonal mk
	const int jmknColliders = 3;
	const int jmknColliders2 = 5;
	SDL_Rect jmkHitbox[jmknColliders] = { { 0, 0, 0, 0 },{ -10, 27, 48, 51 },{ -10, 0, 48, 27 } };
	SDL_Rect jmkHitbox2[jmknColliders2] = { { -120, 0, 34, 26 },{ -73, 17, 34, 26 },{ -10, 27, 61, 51 },{ -10, 0, 48, 27 },{ -163, 0, 35, 24 } };
	COLLIDER_TYPE jmkColliderType[jmknColliders] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 } };
	COLLIDER_TYPE jmkColliderType2[jmknColliders2] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2_HIT } };
	Module*jmkCallback[jmknColliders] = { { this },{ this },{ this } };
	Module*jmkCallback2[jmknColliders2] = { { this },{ this },{ this },{ this },{ (Module*)App->player1 } };

	jmk.PushBack({ 527, 396, 76, 90 }, 4, { 33,5 }, { jmknColliders }, { jmkHitbox }, { jmkColliderType }, { jmkCallback });
	jmk.PushBack({ 605, 415, 133, 71 }, 6, { 33,5 }, { jmknColliders }, { jmkHitbox }, { jmkColliderType }, { jmkCallback });
	jmk.PushBack({ 740, 404, 202, 82 }, 12, { 33,5 }, { jmknColliders2 }, { jmkHitbox2 }, { jmkColliderType2 }, { jmkCallback2 });
	jmk.PushBack({ 605, 415, 133, 71 }, 4, { 33,5 }, { jmknColliders }, { jmkHitbox }, { jmkColliderType }, { jmkCallback });
	jmk.PushBack({ 527, 396, 76, 90 }, 2974, { 33,5 }, { jmknColliders }, { jmkHitbox }, { jmkColliderType }, { jmkCallback });


	//jumping neutral / diagonal hp
	const int jhpnColliders = 3;
	const int jhpnColliders2 = 5;
	SDL_Rect jhpHitbox[jhpnColliders] = { { 0, 0, 0, 0 },{ 0, 27, 48, 51 },{ 0, 0, 48, 27 } };
	SDL_Rect jhpHitbox2[jhpnColliders2] = { { -103, 21, 34, 26 },{ -63, 41, 34, 26 },{ 0, 27, 68, 51 },{ 0, 0, 48, 27 },{ -138, 0, 35, 30 } };
	COLLIDER_TYPE jhpColliderType[jhpnColliders] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 } };
	COLLIDER_TYPE jhpColliderType2[jhpnColliders2] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2_HIT } };
	Module*jhpCallback[jhpnColliders] = { { this },{ this },{ this } };
	Module*jhpCallback2[jhpnColliders2] = { { this },{ this },{ this },{ this },{ (Module*)App->player1 } };

	jhp.PushBack({ 179, 398, 58, 88 }, 3, { 33,5 }, { jhpnColliders }, { jhpHitbox }, { jhpColliderType }, { jhpCallback });
	jhp.PushBack({ 239, 410, 111, 76 }, 4, { 33,5 }, { jhpnColliders }, { jhpHitbox }, { jhpColliderType }, { jhpCallback });
	jhp.PushBack({ 352, 401, 173, 85 }, 4, { 33,5 }, { jhpnColliders2 }, { jhpHitbox2 }, { jhpColliderType2 }, { jhpCallback2 });
	jhp.PushBack({ 239, 410, 111, 76 }, 4, { 33,5 }, { jhpnColliders }, { jhpHitbox }, { jhpColliderType }, { jhpCallback });
	jhp.PushBack({ 179, 398, 58, 88 }, 2985, { 33,5 }, { jhpnColliders }, { jhpHitbox }, { jhpColliderType }, { jhpCallback });

	//jumping neutral / diagonal hk
	const int jhknColliders = 3;
	const int jhknColliders2 = 4;
	SDL_Rect jhkHitbox[jhknColliders] = { { 0, 0, 0, 0 },{ -29, 86, 24, 20 },{ -28, 44, 50, 46 } };
	SDL_Rect jhkHitbox2[jhknColliders2] = { { -5, 43, 57, 29 },{ -47, 59, 64, 24 }, { -111, 72, 54, 24 }, { -111, 60, 72, 46} };
	COLLIDER_TYPE jhkColliderType[jhknColliders] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 } };
	COLLIDER_TYPE jhkColliderType2[jhknColliders2] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{COLLIDER_PLAYER2},{ COLLIDER_PLAYER2_HIT } };
	Module*jhkCallback[jhknColliders] = { { this },{ this },{ this } };
	Module*jhkCallback2[jhknColliders2] = { { this },{ this }, {this}, { (Module*)App->player1 } };

	jhk.PushBack({ 1, 414, 73, 96 }, 1, { 33,5 }, { jhknColliders }, { jhkHitbox }, { jhkColliderType }, { jhkCallback });
	jhk.PushBack({ 76, 404, 54, 106 }, 2, { 33,5 }, { jhknColliders }, { jhkHitbox }, { jhkColliderType }, { jhkCallback });
	jhk.PushBack({ 132, 406, 51, 104 }, 3, { 33,5 }, { jhknColliders }, { jhkHitbox }, { jhkColliderType }, { jhkCallback });
	jhk.PushBack({ 185, 408, 103, 102 }, 4, { 33,5 }, { jhknColliders }, { jhkHitbox }, { jhkColliderType }, { jhkCallback });
	jhk.PushBack({ 293, 408, 184, 102 }, 6, { 33,5 }, { jhknColliders2 }, { jhkHitbox2 }, { jhkColliderType2 }, { jhkCallback2 });
	jhk.PushBack({ 185, 408, 103, 102 }, 4, { 33,5 }, { jhknColliders }, { jhkHitbox }, { jhkColliderType }, { jhkCallback });
	jhk.PushBack({ 132, 406, 51, 104 }, 3, { 33,5 }, { jhknColliders }, { jhkHitbox }, { jhkColliderType }, { jhkCallback });
	jhk.PushBack({ 438, 110, 57, 122 }, 2977, { 33,5 }, { jhknColliders }, { jhkHitbox }, { jhkColliderType }, { jhkCallback });


	//Lp yoga fire
	const int yoga_fire_lpnColliders = 3;
	SDL_Rect yoga_fire_lpHitbox[yoga_fire_lpnColliders] = { { 0, 0, 0, 0}, { -30, 71, 24, 20 }, { -13, 1, 46, 76 } };
	SDL_Rect yoga_fire_lpHitbox2[yoga_fire_lpnColliders] = { { -2, 80, 26, 20}, { 3, 41, 50, 44}, { -30, 1, 46, 38} };
	SDL_Rect yoga_fire_lpHitbox3[yoga_fire_lpnColliders] = { { 0, 0, 0, 0}, { -36, 78, 26, 20}, { -4, 1, 46, 76} };
	SDL_Rect yoga_fire_lpHitbox4[yoga_fire_lpnColliders] = { { -60, 61, 26, 20}, { -15, 44, 46, 42}, { -4, 1, 46, 38} };
	SDL_Rect yoga_fire_lpHitbox5[yoga_fire_lpnColliders] = { { -72, 49, 26, 20}, { -15, 44, 46, 42}, { -4, 1, 46, 38} };
	SDL_Rect yoga_fire_lpHitbox6[yoga_fire_lpnColliders] = { { -18, 80, 36, 20}, { -13, 41, 50, 44}, { -46, 1, 46, 38} };
	COLLIDER_TYPE yoga_fire_lpColliderType[yoga_fire_lpnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* yoga_fire_lpCallback[yoga_fire_lpnColliders] = { {this}, {this}, {this} };
	yoga_fire_lp.PushBack({ 124, 596, 72, 94 }, 2, { 29,5 }, yoga_fire_lpnColliders, yoga_fire_lpHitbox, yoga_fire_lpColliderType, yoga_fire_lpCallback);		// Falta un trozo de animación, cuya durada depende de si es light, medium or hard
	yoga_fire_lp.PushBack({ 198, 595, 72, 96 }, 2, { 29,5 }, yoga_fire_lpnColliders, yoga_fire_lpHitbox2, yoga_fire_lpColliderType, yoga_fire_lpCallback);
	yoga_fire_lp.PushBack({ 272, 595, 86, 96 }, 7, { 29,5 }, yoga_fire_lpnColliders, yoga_fire_lpHitbox6, yoga_fire_lpColliderType, yoga_fire_lpCallback);
	yoga_fire_lp.PushBack({ 360, 596, 58, 94 }, 3, { 29,5 }, yoga_fire_lpnColliders, yoga_fire_lpHitbox3, yoga_fire_lpColliderType, yoga_fire_lpCallback);
	yoga_fire_lp.PushBack({ 420, 602, 81, 88 }, 3, { 29,5 }, yoga_fire_lpnColliders, yoga_fire_lpHitbox4, yoga_fire_lpColliderType, yoga_fire_lpCallback);
	//yoga_fire_lp.PushBack({ 617, 598, 94, 92 }, 6, { 29,5 }, yoga_fire_lpnColliders, yoga_fire_lpHitbox5, yoga_fire_lpColliderType, yoga_fire_lpCallback);
	//yoga_fire_lp.PushBack({ 420, 602, 81, 88 }, 9, { 29,5 }, yoga_fire_lpnColliders, yoga_fire_lpHitbox4, yoga_fire_lpColliderType, yoga_fire_lpCallback);


	//Mp yoga fire
	const int yoga_fire_mpnColliders = 3;
	SDL_Rect yoga_fire_mpHitbox[yoga_fire_mpnColliders] = { { 0, 0, 0, 0}, { -30, 71, 24, 20 }, { -13, 1, 46, 76 } };
	SDL_Rect yoga_fire_mpHitbox2[yoga_fire_mpnColliders] = { { -2, 80, 26, 20}, { 3, 41, 50, 44}, { -30, 1, 46, 38} };
	SDL_Rect yoga_fire_mpHitbox3[yoga_fire_mpnColliders] = { { 0, 0, 0, 0}, { -36, 78, 26, 20}, { -4, 1, 46, 76} };
	SDL_Rect yoga_fire_mpHitbox4[yoga_fire_mpnColliders] = { { -60, 61, 26, 20}, { -15, 44, 46, 42}, { -4, 1, 46, 38} };
	SDL_Rect yoga_fire_mpHitbox5[yoga_fire_mpnColliders] = { { -72, 49, 26, 20}, { -15, 44, 46, 42}, { -4, 1, 46, 38} };
	SDL_Rect yoga_fire_mpHitbox6[yoga_fire_mpnColliders] = { { -18, 80, 36, 20}, { -13, 41, 50, 44}, { -46, 1, 46, 38} };
	COLLIDER_TYPE yoga_fire_mpColliderType[yoga_fire_mpnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* yoga_fire_mpCallback[yoga_fire_mpnColliders] = { {this}, {this}, {this} };
	yoga_fire_mp.PushBack({ 124, 596, 72, 94 }, 2, { 29,5 }, yoga_fire_mpnColliders, yoga_fire_mpHitbox, yoga_fire_mpColliderType, yoga_fire_mpCallback);
	yoga_fire_mp.PushBack({ 198, 595, 72, 96 }, 2, { 29,5 }, yoga_fire_mpnColliders, yoga_fire_mpHitbox2, yoga_fire_mpColliderType, yoga_fire_mpCallback);
	yoga_fire_mp.PushBack({ 272, 595, 86, 96 }, 7, { 29,5 }, yoga_fire_mpnColliders, yoga_fire_mpHitbox6, yoga_fire_mpColliderType, yoga_fire_mpCallback);
	yoga_fire_mp.PushBack({ 360, 596, 58, 94 }, 3, { 29,5 }, yoga_fire_mpnColliders, yoga_fire_mpHitbox3, yoga_fire_mpColliderType, yoga_fire_mpCallback);
	yoga_fire_mp.PushBack({ 420, 602, 81, 88 }, 3, { 29,5 }, yoga_fire_mpnColliders, yoga_fire_mpHitbox4, yoga_fire_mpColliderType, yoga_fire_mpCallback);
	yoga_fire_mp.PushBack({ 617, 598, 94, 92 }, 6, { 29,5 }, yoga_fire_mpnColliders, yoga_fire_mpHitbox5, yoga_fire_mpColliderType, yoga_fire_mpCallback);
	yoga_fire_mp.PushBack({ 420, 602, 81, 88 }, 9, { 29,5 }, yoga_fire_mpnColliders, yoga_fire_mpHitbox4, yoga_fire_mpColliderType, yoga_fire_mpCallback);

	//Hp yoga fire
	const int yoga_fire_hpnColliders = 3;
	SDL_Rect yoga_fire_hpHitbox[yoga_fire_hpnColliders] = { { 0, 0, 0, 0}, { -30, 71, 24, 20 }, { -13, 1, 46, 76 } };
	SDL_Rect yoga_fire_hpHitbox2[yoga_fire_hpnColliders] = { { -2, 80, 26, 20}, { 3, 41, 50, 44}, { -30, 1, 46, 38} };
	SDL_Rect yoga_fire_hpHitbox3[yoga_fire_hpnColliders] = { { 0, 0, 0, 0}, { -36, 78, 26, 20}, { -4, 1, 46, 76} };
	SDL_Rect yoga_fire_hpHitbox4[yoga_fire_hpnColliders] = { { -60, 61, 26, 20}, { -15, 44, 46, 42}, { -4, 1, 46, 38} };
	SDL_Rect yoga_fire_hpHitbox5[yoga_fire_hpnColliders] = { { -72, 49, 26, 20}, { -15, 44, 46, 42}, { -4, 1, 46, 38} };
	SDL_Rect yoga_fire_hpHitbox6[yoga_fire_hpnColliders] = { { -18, 80, 36, 20}, { -13, 41, 50, 44}, { -46, 1, 46, 38} };
	COLLIDER_TYPE yoga_fire_hpColliderType[yoga_fire_hpnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* yoga_fire_hpCallback[yoga_fire_hpnColliders] = { {this}, {this}, {this} };
	yoga_fire_hp.PushBack({ 124, 596, 72, 94 }, 2, { 29,5 }, yoga_fire_hpnColliders, yoga_fire_hpHitbox, yoga_fire_hpColliderType, yoga_fire_hpCallback);
	yoga_fire_hp.PushBack({ 198, 595, 72, 96 }, 2, { 29,5 }, yoga_fire_hpnColliders, yoga_fire_hpHitbox2, yoga_fire_hpColliderType, yoga_fire_hpCallback);
	yoga_fire_hp.PushBack({ 272, 595, 86, 96 }, 7, { 29,5 }, yoga_fire_hpnColliders, yoga_fire_hpHitbox6, yoga_fire_hpColliderType, yoga_fire_hpCallback);
	yoga_fire_hp.PushBack({ 360, 596, 58, 94 }, 3, { 29,5 }, yoga_fire_hpnColliders, yoga_fire_hpHitbox3, yoga_fire_hpColliderType, yoga_fire_hpCallback);
	yoga_fire_hp.PushBack({ 420, 602, 81, 88 }, 3, { 29,5 }, yoga_fire_hpnColliders, yoga_fire_hpHitbox4, yoga_fire_hpColliderType, yoga_fire_hpCallback);
	yoga_fire_hp.PushBack({ 617, 598, 94, 92 }, 6, { 29,5 }, yoga_fire_hpnColliders, yoga_fire_hpHitbox5, yoga_fire_hpColliderType, yoga_fire_hpCallback);
	yoga_fire_hp.PushBack({ 420, 602, 81, 88 }, 9, { 29,5 }, yoga_fire_hpnColliders, yoga_fire_hpHitbox4, yoga_fire_hpColliderType, yoga_fire_hpCallback);


	// Lp yoga flame

	const int yoga_flame_lpnColliders = 3;
	const int yoga_flame_lpnColliders2 = 4;
	const int yoga_flame_lpnColliders3 = 4;
	const int yoga_flame_lpnColliders4 = 4;
	const int yoga_flame_lpnColliders5 = 3;
	SDL_Rect yoga_flame_lpHitbox[yoga_flame_lpnColliders] = { { 0, 0, 0, 0}, { -30, 71, 24, 20 }, { -13, 1, 46, 76 } };
	SDL_Rect yoga_flame_lpHitbox2[yoga_flame_lpnColliders] = { { -2, 80, 26, 20}, { 3, 41, 50, 44}, { -30, 1, 46, 38} };
	SDL_Rect yoga_flame_lpHitbox3[yoga_flame_lpnColliders] = { { 0, 0, 0, 0}, { -36, 78, 26, 20}, { -4, 1, 46, 76} };
	SDL_Rect yoga_flame_lpHitbox4[yoga_flame_lpnColliders] = { { -60, 61, 26, 20}, { -15, 44, 46, 42}, { -4, 1, 46, 38} };
	SDL_Rect yoga_flame_lpHitbox5[yoga_flame_lpnColliders] = { { -72, 49, 26, 20}, { -15, 44, 46, 42}, { -4, 1, 46, 38} };
	SDL_Rect yoga_flame_lpHitbox6[yoga_flame_lpnColliders] = { { -18, 80, 36, 20}, { -13, 41, 50, 44}, { -46, 1, 46, 38} };
	SDL_Rect yoga_flame_lpHitbox7[yoga_flame_lpnColliders2] = { { -62, 49, 26, 20}, { -15, 41, 50, 44}, { -4, 1, 46, 38}, { -95, 37, 56, 32} };
	SDL_Rect yoga_flame_lpHitbox8[yoga_flame_lpnColliders2] = { { -62, 49, 26, 20}, { -15, 41, 50, 44}, { -4, 1, 46, 38}, { -88, 40, 80, 48} };
	SDL_Rect yoga_flame_lpHitbox9[yoga_flame_lpnColliders3] = { { -62, 49, 26, 20}, { -15, 41, 50, 44}, { -4, 1, 46, 38}, { -88, 40, 80, 48} };
	SDL_Rect yoga_flame_lpHitbox10[yoga_flame_lpnColliders4] = { { -62, 49, 26, 20}, { -15, 41, 50, 44}, { -4, 1, 46, 38}, { -88, 40, 80, 48} };
	SDL_Rect yoga_flame_lpHitbox11[yoga_flame_lpnColliders5] = { { -62, 49, 26, 20}, { -15, 41, 50, 44}, { -4, 1, 46, 38} };
	COLLIDER_TYPE yoga_flame_lpColliderType[yoga_flame_lpnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	COLLIDER_TYPE yoga_flame_lpColliderType2[yoga_flame_lpnColliders2] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	COLLIDER_TYPE yoga_flame_lpColliderType3[yoga_flame_lpnColliders3] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	COLLIDER_TYPE yoga_flame_lpColliderType4[yoga_flame_lpnColliders4] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	COLLIDER_TYPE yoga_flame_lpColliderType5[yoga_flame_lpnColliders5] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* yoga_flame_lpCallback[yoga_flame_lpnColliders] = { {this}, {this}, {this} };
	Module* yoga_flame_lpCallback2[yoga_flame_lpnColliders2] = { {this}, {this}, {this}, {(Module*)App->player1} };
	Module* yoga_flame_lpCallback3[yoga_flame_lpnColliders3] = { {this}, {this}, {this}, {(Module*)App->player1} };
	Module* yoga_flame_lpCallback4[yoga_flame_lpnColliders4] = { {this}, {this}, {this}, {(Module*)App->player1} };
	Module* yoga_flame_lpCallback5[yoga_flame_lpnColliders5] = { {this}, {this}, {this} };
	yoga_flame_lp.PushBack({ 124, 596, 72, 94 }, 2, { 29,5 }, yoga_flame_lpnColliders, yoga_flame_lpHitbox, yoga_flame_lpColliderType, yoga_flame_lpCallback);
	yoga_flame_lp.PushBack({ 198, 595, 72, 96 }, 2, { 29,5 }, yoga_flame_lpnColliders, yoga_flame_lpHitbox2, yoga_flame_lpColliderType, yoga_flame_lpCallback);
	yoga_flame_lp.PushBack({ 272, 595, 86, 96 }, 3, { 29,5 }, yoga_flame_lpnColliders, yoga_flame_lpHitbox6, yoga_flame_lpColliderType, yoga_flame_lpCallback);
	yoga_flame_lp.PushBack({ 198, 595, 72, 96 }, 2, { 29,5 }, yoga_flame_lpnColliders, yoga_flame_lpHitbox2, yoga_flame_lpColliderType, yoga_flame_lpCallback);
	yoga_flame_lp.PushBack({ 272, 595, 86, 96 }, 2, { 29,5 }, yoga_flame_lpnColliders, yoga_flame_lpHitbox6, yoga_flame_lpColliderType, yoga_flame_lpCallback);
	yoga_flame_lp.PushBack({ 360, 596, 58, 94 }, 3, { 29,5 }, yoga_flame_lpnColliders, yoga_flame_lpHitbox3, yoga_flame_lpColliderType, yoga_flame_lpCallback);
	yoga_flame_lp.PushBack({ 503, 602, 112, 88 }, 2, { 29,5 }, yoga_flame_lpnColliders, yoga_flame_lpHitbox4, yoga_flame_lpColliderType, yoga_flame_lpCallback);
	yoga_flame_lp.PushBack({ 420, 602, 81, 88 }, 1, { 29,5 }, yoga_flame_lpnColliders, yoga_flame_lpHitbox4, yoga_flame_lpColliderType, yoga_flame_lpCallback);
	yoga_flame_lp.PushBack({ 617, 598, 144, 92 }, 3, { 29,5 }, yoga_flame_lpnColliders, yoga_flame_lpHitbox5, yoga_flame_lpColliderType, yoga_flame_lpCallback);
	yoga_flame_lp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_lpnColliders, yoga_flame_lpHitbox5, yoga_flame_lpColliderType, yoga_flame_lpCallback);
	yoga_flame_lp.PushBack({ 617, 598, 144, 92 }, 3, { 29,5 }, yoga_flame_lpnColliders2, yoga_flame_lpHitbox7, yoga_flame_lpColliderType2, yoga_flame_lpCallback2);
	yoga_flame_lp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_lpnColliders2, yoga_flame_lpHitbox7, yoga_flame_lpColliderType2, yoga_flame_lpCallback2);
	yoga_flame_lp.PushBack({ 763, 598, 157, 92 }, 3, { 29,5 }, yoga_flame_lpnColliders2, yoga_flame_lpHitbox7, yoga_flame_lpColliderType2, yoga_flame_lpCallback2);
	yoga_flame_lp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_lpnColliders2, yoga_flame_lpHitbox7, yoga_flame_lpColliderType2, yoga_flame_lpCallback2);
	yoga_flame_lp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_lpnColliders2, yoga_flame_lpHitbox8, yoga_flame_lpColliderType2, yoga_flame_lpCallback2);
	yoga_flame_lp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_lpnColliders2, yoga_flame_lpHitbox8, yoga_flame_lpColliderType2, yoga_flame_lpCallback2);
	yoga_flame_lp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_lpnColliders2, yoga_flame_lpHitbox8, yoga_flame_lpColliderType2, yoga_flame_lpCallback2);
	yoga_flame_lp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_lpnColliders2, yoga_flame_lpHitbox8, yoga_flame_lpColliderType2, yoga_flame_lpCallback2);
	yoga_flame_lp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_lpnColliders3, yoga_flame_lpHitbox9, yoga_flame_lpColliderType3, yoga_flame_lpCallback3);
	yoga_flame_lp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_lpnColliders3, yoga_flame_lpHitbox9, yoga_flame_lpColliderType3, yoga_flame_lpCallback3);
	yoga_flame_lp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_lpnColliders3, yoga_flame_lpHitbox9, yoga_flame_lpColliderType3, yoga_flame_lpCallback3);
	yoga_flame_lp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_lpnColliders3, yoga_flame_lpHitbox9, yoga_flame_lpColliderType3, yoga_flame_lpCallback3);
	yoga_flame_lp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_lpnColliders3, yoga_flame_lpHitbox9, yoga_flame_lpColliderType3, yoga_flame_lpCallback3);
	yoga_flame_lp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_lpnColliders3, yoga_flame_lpHitbox9, yoga_flame_lpColliderType3, yoga_flame_lpCallback3);
	yoga_flame_lp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_lpnColliders3, yoga_flame_lpHitbox9, yoga_flame_lpColliderType3, yoga_flame_lpCallback3);
	yoga_flame_lp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_lpnColliders3, yoga_flame_lpHitbox9, yoga_flame_lpColliderType3, yoga_flame_lpCallback3);
	yoga_flame_lp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_lpnColliders4, yoga_flame_lpHitbox10, yoga_flame_lpColliderType4, yoga_flame_lpCallback4);
	yoga_flame_lp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_lpnColliders5, yoga_flame_lpHitbox11, yoga_flame_lpColliderType5, yoga_flame_lpCallback5);
	yoga_flame_lp.PushBack({ 176, 710, 156, 92 }, 3, { 29,5 }, yoga_flame_lpnColliders5, yoga_flame_lpHitbox11, yoga_flame_lpColliderType5, yoga_flame_lpCallback5);
	yoga_flame_lp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_lpnColliders5, yoga_flame_lpHitbox11, yoga_flame_lpColliderType5, yoga_flame_lpCallback5);
	yoga_flame_lp.PushBack({ 176, 710, 156, 92 }, 3, { 29,5 }, yoga_flame_lpnColliders5, yoga_flame_lpHitbox11, yoga_flame_lpColliderType5, yoga_flame_lpCallback5);
	yoga_flame_lp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_lpnColliders5, yoga_flame_lpHitbox11, yoga_flame_lpColliderType5, yoga_flame_lpCallback5);
	yoga_flame_lp.PushBack({ 176, 710, 156, 92 }, 3, { 29,5 }, yoga_flame_lpnColliders5, yoga_flame_lpHitbox11, yoga_flame_lpColliderType5, yoga_flame_lpCallback5);
	yoga_flame_lp.PushBack({ 617, 598, 94, 92 }, 4, { 29,5 }, yoga_flame_lpnColliders5, yoga_flame_lpHitbox11, yoga_flame_lpColliderType5, yoga_flame_lpCallback5);


	//Mp yoga flame
	const int yoga_flame_mpnColliders = 3;
	const int yoga_flame_mpnColliders2 = 4;
	const int yoga_flame_mpnColliders3 = 4;
	const int yoga_flame_mpnColliders4 = 4;
	const int yoga_flame_mpnColliders5 = 3;
	SDL_Rect yoga_flame_mpHitbox[yoga_flame_mpnColliders] = { { 0, 0, 0, 0}, { -30, 71, 24, 20 }, { -13, 1, 46, 76 } };
	SDL_Rect yoga_flame_mpHitbox2[yoga_flame_mpnColliders] = { { -2, 80, 26, 20}, { 3, 41, 50, 44}, { -30, 1, 46, 38} };
	SDL_Rect yoga_flame_mpHitbox3[yoga_flame_mpnColliders] = { { 0, 0, 0, 0}, { -36, 78, 26, 20}, { -4, 1, 46, 76} };
	SDL_Rect yoga_flame_mpHitbox4[yoga_flame_mpnColliders] = { { -60, 61, 26, 20}, { -15, 44, 46, 42}, { -4, 1, 46, 38} };
	SDL_Rect yoga_flame_mpHitbox5[yoga_flame_mpnColliders] = { { -72, 49, 26, 20}, { -15, 44, 46, 42}, { -4, 1, 46, 38} };
	SDL_Rect yoga_flame_mpHitbox6[yoga_flame_mpnColliders] = { { -18, 80, 36, 20}, { -13, 41, 50, 44}, { -46, 1, 46, 38} };
	SDL_Rect yoga_flame_mpHitbox7[yoga_flame_mpnColliders2] = { { -62, 49, 26, 20}, { -15, 41, 50, 44}, { -4, 1, 46, 38}, { -95, 37, 56, 32} };
	SDL_Rect yoga_flame_mpHitbox8[yoga_flame_mpnColliders2] = { { -62, 49, 26, 20}, { -15, 41, 50, 44}, { -4, 1, 46, 38}, { -88, 40, 80, 48} };
	SDL_Rect yoga_flame_mpHitbox9[yoga_flame_lpnColliders3] = { { -62, 49, 26, 20}, { -15, 41, 50, 44}, { -4, 1, 46, 38}, { -88, 40, 80, 48} };
	SDL_Rect yoga_flame_mpHitbox10[yoga_flame_lpnColliders4] = { { -62, 49, 26, 20}, { -15, 41, 50, 44}, { -4, 1, 46, 38}, { -88, 40, 80, 48} };
	SDL_Rect yoga_flame_mpHitbox11[yoga_flame_lpnColliders5] = { { -62, 49, 26, 20}, { -15, 41, 50, 44}, { -4, 1, 46, 38} };
	COLLIDER_TYPE yoga_flame_mpColliderType[yoga_flame_mpnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	COLLIDER_TYPE yoga_flame_mpColliderType2[yoga_flame_mpnColliders2] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	COLLIDER_TYPE yoga_flame_mpColliderType3[yoga_flame_lpnColliders3] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	COLLIDER_TYPE yoga_flame_mpColliderType4[yoga_flame_lpnColliders4] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	COLLIDER_TYPE yoga_flame_mpColliderType5[yoga_flame_lpnColliders5] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* yoga_flame_mpCallback[yoga_flame_mpnColliders] = { {this}, {this}, {this} };
	Module* yoga_flame_mpCallback2[yoga_flame_mpnColliders2] = { {this}, {this}, {this}, {(Module*)App->player1} };
	Module* yoga_flame_mpCallback3[yoga_flame_lpnColliders3] = { {this}, {this}, {this}, {(Module*)App->player1} };
	Module* yoga_flame_mpCallback4[yoga_flame_lpnColliders4] = { {this}, {this}, {this}, {(Module*)App->player1} };
	Module* yoga_flame_mpCallback5[yoga_flame_lpnColliders5] = { {this}, {this}, {this} };
	yoga_flame_mp.PushBack({ 124, 596, 72, 94 }, 2, { 29,5 }, yoga_flame_mpnColliders, yoga_flame_mpHitbox, yoga_flame_mpColliderType, yoga_flame_mpCallback);
	yoga_flame_mp.PushBack({ 198, 595, 72, 96 }, 2, { 29,5 }, yoga_flame_mpnColliders, yoga_flame_mpHitbox2, yoga_flame_mpColliderType, yoga_flame_mpCallback);
	yoga_flame_mp.PushBack({ 272, 595, 86, 96 }, 3, { 29,5 }, yoga_flame_mpnColliders, yoga_flame_mpHitbox6, yoga_flame_mpColliderType, yoga_flame_mpCallback);
	yoga_flame_mp.PushBack({ 198, 595, 72, 96 }, 2, { 29,5 }, yoga_flame_mpnColliders, yoga_flame_mpHitbox2, yoga_flame_mpColliderType, yoga_flame_mpCallback);
	yoga_flame_mp.PushBack({ 272, 595, 86, 96 }, 3, { 29,5 }, yoga_flame_mpnColliders, yoga_flame_mpHitbox6, yoga_flame_mpColliderType, yoga_flame_mpCallback);
	yoga_flame_mp.PushBack({ 360, 596, 58, 94 }, 3, { 29,5 }, yoga_flame_mpnColliders, yoga_flame_mpHitbox3, yoga_flame_mpColliderType, yoga_flame_mpCallback);
	yoga_flame_mp.PushBack({ 503, 602, 112, 88 }, 2, { 29,5 }, yoga_flame_mpnColliders, yoga_flame_mpHitbox4, yoga_flame_mpColliderType, yoga_flame_mpCallback);
	yoga_flame_mp.PushBack({ 420, 602, 81, 88 }, 1, { 29,5 }, yoga_flame_mpnColliders, yoga_flame_mpHitbox4, yoga_flame_mpColliderType, yoga_flame_mpCallback);
	yoga_flame_mp.PushBack({ 617, 598, 144, 92 }, 3, { 29,5 }, yoga_flame_mpnColliders, yoga_flame_mpHitbox5, yoga_flame_mpColliderType, yoga_flame_mpCallback);
	yoga_flame_mp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_mpnColliders, yoga_flame_mpHitbox5, yoga_flame_mpColliderType, yoga_flame_mpCallback);
	yoga_flame_mp.PushBack({ 617, 598, 144, 92 }, 3, { 29,5 }, yoga_flame_mpnColliders2, yoga_flame_mpHitbox7, yoga_flame_mpColliderType2, yoga_flame_mpCallback2);
	yoga_flame_mp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_mpnColliders2, yoga_flame_mpHitbox7, yoga_flame_mpColliderType2, yoga_flame_mpCallback2);
	yoga_flame_mp.PushBack({ 763, 598, 157, 92 }, 3, { 29,5 }, yoga_flame_mpnColliders2, yoga_flame_mpHitbox7, yoga_flame_mpColliderType2, yoga_flame_mpCallback2);
	yoga_flame_mp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_mpnColliders2, yoga_flame_mpHitbox7, yoga_flame_mpColliderType2, yoga_flame_mpCallback2);
	yoga_flame_mp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_mpnColliders2, yoga_flame_mpHitbox8, yoga_flame_mpColliderType2, yoga_flame_mpCallback2);
	yoga_flame_mp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_mpnColliders2, yoga_flame_mpHitbox8, yoga_flame_mpColliderType2, yoga_flame_mpCallback2);
	yoga_flame_mp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_mpnColliders2, yoga_flame_mpHitbox8, yoga_flame_mpColliderType2, yoga_flame_mpCallback2);
	yoga_flame_mp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_mpnColliders2, yoga_flame_mpHitbox8, yoga_flame_mpColliderType2, yoga_flame_mpCallback2);
	yoga_flame_mp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_mpnColliders3, yoga_flame_mpHitbox9, yoga_flame_mpColliderType3, yoga_flame_mpCallback3);
	yoga_flame_mp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_mpnColliders3, yoga_flame_mpHitbox9, yoga_flame_mpColliderType3, yoga_flame_mpCallback3);
	yoga_flame_mp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_mpnColliders3, yoga_flame_mpHitbox9, yoga_flame_mpColliderType3, yoga_flame_mpCallback3);
	yoga_flame_mp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_mpnColliders3, yoga_flame_mpHitbox9, yoga_flame_mpColliderType3, yoga_flame_mpCallback3);
	yoga_flame_mp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_mpnColliders3, yoga_flame_mpHitbox9, yoga_flame_mpColliderType3, yoga_flame_mpCallback3);
	yoga_flame_mp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_mpnColliders3, yoga_flame_mpHitbox9, yoga_flame_mpColliderType3, yoga_flame_mpCallback3);
	yoga_flame_mp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_mpnColliders3, yoga_flame_mpHitbox9, yoga_flame_mpColliderType3, yoga_flame_mpCallback3);
	yoga_flame_mp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_mpnColliders3, yoga_flame_mpHitbox9, yoga_flame_mpColliderType3, yoga_flame_mpCallback3);
	yoga_flame_mp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_mpnColliders3, yoga_flame_mpHitbox9, yoga_flame_mpColliderType3, yoga_flame_mpCallback3);
	yoga_flame_mp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_mpnColliders3, yoga_flame_mpHitbox9, yoga_flame_mpColliderType3, yoga_flame_mpCallback3);
	yoga_flame_mp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_mpnColliders3, yoga_flame_mpHitbox9, yoga_flame_mpColliderType3, yoga_flame_mpCallback3);
	yoga_flame_mp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_mpnColliders3, yoga_flame_mpHitbox9, yoga_flame_mpColliderType3, yoga_flame_mpCallback3);
	yoga_flame_mp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_mpnColliders3, yoga_flame_mpHitbox9, yoga_flame_mpColliderType3, yoga_flame_mpCallback3);
	yoga_flame_mp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_mpnColliders3, yoga_flame_mpHitbox9, yoga_flame_mpColliderType3, yoga_flame_mpCallback3);
	yoga_flame_mp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_mpnColliders3, yoga_flame_mpHitbox9, yoga_flame_mpColliderType3, yoga_flame_mpCallback3);
	yoga_flame_mp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_mpnColliders3, yoga_flame_mpHitbox9, yoga_flame_mpColliderType3, yoga_flame_mpCallback3);
	yoga_flame_mp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_mpnColliders4, yoga_flame_mpHitbox10, yoga_flame_mpColliderType4, yoga_flame_mpCallback4);
	yoga_flame_mp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_mpnColliders5, yoga_flame_mpHitbox11, yoga_flame_mpColliderType5, yoga_flame_mpCallback5);
	yoga_flame_mp.PushBack({ 176, 710, 156, 92 }, 3, { 29,5 }, yoga_flame_mpnColliders5, yoga_flame_mpHitbox11, yoga_flame_mpColliderType5, yoga_flame_mpCallback5);
	yoga_flame_mp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_mpnColliders5, yoga_flame_mpHitbox11, yoga_flame_mpColliderType5, yoga_flame_mpCallback5);
	yoga_flame_mp.PushBack({ 176, 710, 156, 92 }, 3, { 29,5 }, yoga_flame_mpnColliders5, yoga_flame_mpHitbox11, yoga_flame_mpColliderType5, yoga_flame_mpCallback5);
	yoga_flame_mp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_mpnColliders5, yoga_flame_mpHitbox11, yoga_flame_mpColliderType5, yoga_flame_mpCallback5);
	yoga_flame_mp.PushBack({ 176, 710, 156, 92 }, 3, { 29,5 }, yoga_flame_mpnColliders5, yoga_flame_mpHitbox11, yoga_flame_mpColliderType5, yoga_flame_mpCallback5);
	yoga_flame_mp.PushBack({ 617, 598, 94, 92 }, 4, { 29,5 }, yoga_flame_mpnColliders5, yoga_flame_mpHitbox11, yoga_flame_mpColliderType5, yoga_flame_mpCallback5);


	//Hp yoga flame
	const int yoga_flame_hpnColliders = 3;
	const int yoga_flame_hpnColliders2 = 4;
	const int yoga_flame_hpnColliders3 = 4;
	const int yoga_flame_hpnColliders4 = 4;
	const int yoga_flame_hpnColliders5 = 3;
	SDL_Rect yoga_flame_hpHitbox[yoga_flame_hpnColliders] = { { 0, 0, 0, 0}, { -30, 71, 24, 20 }, { -13, 1, 46, 76 } };
	SDL_Rect yoga_flame_hpHitbox2[yoga_flame_hpnColliders] = { { -2, 80, 26, 20}, { 3, 41, 50, 44}, { -30, 1, 46, 38} };
	SDL_Rect yoga_flame_hpHitbox3[yoga_flame_hpnColliders] = { { 0, 0, 0, 0}, { -36, 78, 26, 20}, { -4, 1, 46, 76} };
	SDL_Rect yoga_flame_hpHitbox4[yoga_flame_hpnColliders] = { { -60, 61, 26, 20}, { -15, 44, 46, 42}, { -4, 1, 46, 38} };
	SDL_Rect yoga_flame_hpHitbox5[yoga_flame_hpnColliders] = { { -72, 49, 26, 20}, { -15, 44, 46, 42}, { -4, 1, 46, 38} };
	SDL_Rect yoga_flame_hpHitbox6[yoga_flame_hpnColliders] = { { -18, 80, 36, 20}, { -13, 41, 50, 44}, { -46, 1, 46, 38} };
	SDL_Rect yoga_flame_hpHitbox7[yoga_flame_hpnColliders2] = { { -62, 49, 26, 20}, { -15, 41, 50, 44}, { -4, 1, 46, 38}, { -95, 37, 56, 32} };
	SDL_Rect yoga_flame_hpHitbox8[yoga_flame_hpnColliders2] = { { -62, 49, 26, 20}, { -15, 41, 50, 44}, { -4, 1, 46, 38}, { -88, 40, 80, 48} };
	SDL_Rect yoga_flame_hpHitbox9[yoga_flame_lpnColliders3] = { { -62, 49, 26, 20}, { -15, 41, 50, 44}, { -4, 1, 46, 38}, { -88, 40, 80, 48} };
	SDL_Rect yoga_flame_hpHitbox10[yoga_flame_hpnColliders4] = { { -62, 49, 26, 20}, { -15, 41, 50, 44}, { -4, 1, 46, 38}, { -88, 40, 80, 48} };
	SDL_Rect yoga_flame_hpHitbox11[yoga_flame_hpnColliders5] = { { -62, 49, 26, 20}, { -15, 41, 50, 44}, { -4, 1, 46, 38} };
	COLLIDER_TYPE yoga_flame_hpColliderType[yoga_flame_hpnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	COLLIDER_TYPE yoga_flame_hpColliderType2[yoga_flame_hpnColliders2] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	COLLIDER_TYPE yoga_flame_hpColliderType3[yoga_flame_lpnColliders3] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	COLLIDER_TYPE yoga_flame_hpColliderType4[yoga_flame_hpnColliders4] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	COLLIDER_TYPE yoga_flame_hpColliderType5[yoga_flame_hpnColliders5] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* yoga_flame_hpCallback[yoga_flame_hpnColliders] = { {this}, {this}, {this} };
	Module* yoga_flame_hpCallback2[yoga_flame_hpnColliders2] = { {this}, {this}, {this}, {(Module*)App->player1} };
	Module* yoga_flame_hpCallback3[yoga_flame_lpnColliders3] = { {this}, {this}, {this}, {(Module*)App->player1} };
	Module* yoga_flame_hpCallback4[yoga_flame_hpnColliders4] = { {this}, {this}, {this}, {(Module*)App->player1} };
	Module* yoga_flame_hpCallback5[yoga_flame_hpnColliders5] = { {this}, {this}, {this} };
	yoga_flame_hp.PushBack({ 124, 596, 72, 94 }, 2, { 29,5 }, yoga_flame_hpnColliders, yoga_flame_hpHitbox, yoga_flame_hpColliderType, yoga_flame_hpCallback);
	yoga_flame_hp.PushBack({ 198, 595, 72, 96 }, 2, { 29,5 }, yoga_flame_hpnColliders, yoga_flame_hpHitbox2, yoga_flame_hpColliderType, yoga_flame_hpCallback);
	yoga_flame_hp.PushBack({ 272, 595, 86, 96 }, 4, { 29,5 }, yoga_flame_hpnColliders, yoga_flame_hpHitbox6, yoga_flame_hpColliderType, yoga_flame_hpCallback);
	yoga_flame_hp.PushBack({ 198, 595, 72, 96 }, 2, { 29,5 }, yoga_flame_hpnColliders, yoga_flame_hpHitbox2, yoga_flame_hpColliderType, yoga_flame_hpCallback);
	yoga_flame_hp.PushBack({ 272, 595, 86, 96 }, 4, { 29,5 }, yoga_flame_hpnColliders, yoga_flame_hpHitbox6, yoga_flame_hpColliderType, yoga_flame_hpCallback);
	yoga_flame_hp.PushBack({ 360, 596, 58, 94 }, 3, { 29,5 }, yoga_flame_hpnColliders, yoga_flame_hpHitbox3, yoga_flame_hpColliderType, yoga_flame_hpCallback);
	yoga_flame_hp.PushBack({ 503, 602, 112, 88 }, 2, { 29,5 }, yoga_flame_hpnColliders, yoga_flame_hpHitbox4, yoga_flame_hpColliderType, yoga_flame_hpCallback);
	yoga_flame_hp.PushBack({ 420, 602, 81, 88 }, 1, { 29,5 }, yoga_flame_hpnColliders, yoga_flame_hpHitbox4, yoga_flame_hpColliderType, yoga_flame_hpCallback);
	yoga_flame_hp.PushBack({ 617, 598, 144, 92 }, 3, { 29,5 }, yoga_flame_hpnColliders, yoga_flame_hpHitbox5, yoga_flame_hpColliderType, yoga_flame_hpCallback);
	yoga_flame_hp.PushBack({ 617, 598, 94, 92 }, 2, { 29,5 }, yoga_flame_hpnColliders, yoga_flame_hpHitbox5, yoga_flame_hpColliderType, yoga_flame_hpCallback);
	yoga_flame_hp.PushBack({ 617, 598, 144, 92 }, 3, { 29,5 }, yoga_flame_hpnColliders2, yoga_flame_hpHitbox7, yoga_flame_hpColliderType2, yoga_flame_hpCallback2);
	yoga_flame_hp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_hpnColliders2, yoga_flame_hpHitbox7, yoga_flame_hpColliderType2, yoga_flame_hpCallback2);
	yoga_flame_hp.PushBack({ 763, 598, 157, 92 }, 3, { 29,5 }, yoga_flame_hpnColliders2, yoga_flame_hpHitbox7, yoga_flame_hpColliderType2, yoga_flame_hpCallback2);
	yoga_flame_hp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_hpnColliders2, yoga_flame_hpHitbox7, yoga_flame_hpColliderType2, yoga_flame_hpCallback2);
	yoga_flame_hp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_hpnColliders2, yoga_flame_hpHitbox8, yoga_flame_hpColliderType2, yoga_flame_hpCallback2);
	yoga_flame_hp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_hpnColliders2, yoga_flame_hpHitbox8, yoga_flame_hpColliderType2, yoga_flame_hpCallback2);
	yoga_flame_hp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_hpnColliders2, yoga_flame_hpHitbox8, yoga_flame_hpColliderType2, yoga_flame_hpCallback2);
	yoga_flame_hp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_hpnColliders2, yoga_flame_hpHitbox8, yoga_flame_hpColliderType2, yoga_flame_hpCallback2);
	yoga_flame_hp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_hpnColliders3, yoga_flame_hpHitbox9, yoga_flame_hpColliderType3, yoga_flame_hpCallback3);
	yoga_flame_hp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_hpnColliders3, yoga_flame_hpHitbox9, yoga_flame_hpColliderType3, yoga_flame_hpCallback3);
	yoga_flame_hp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_hpnColliders3, yoga_flame_hpHitbox9, yoga_flame_hpColliderType3, yoga_flame_hpCallback3);
	yoga_flame_hp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_hpnColliders3, yoga_flame_hpHitbox9, yoga_flame_hpColliderType3, yoga_flame_hpCallback3);
	yoga_flame_hp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_hpnColliders3, yoga_flame_hpHitbox9, yoga_flame_hpColliderType3, yoga_flame_hpCallback3);
	yoga_flame_hp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_hpnColliders3, yoga_flame_hpHitbox9, yoga_flame_hpColliderType3, yoga_flame_hpCallback3);
	yoga_flame_hp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_hpnColliders3, yoga_flame_hpHitbox9, yoga_flame_hpColliderType3, yoga_flame_hpCallback3);
	yoga_flame_hp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_hpnColliders3, yoga_flame_hpHitbox9, yoga_flame_hpColliderType3, yoga_flame_hpCallback3);
	yoga_flame_hp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_hpnColliders3, yoga_flame_hpHitbox9, yoga_flame_hpColliderType3, yoga_flame_hpCallback3);
	yoga_flame_hp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_hpnColliders3, yoga_flame_hpHitbox9, yoga_flame_hpColliderType3, yoga_flame_hpCallback3);
	yoga_flame_hp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_hpnColliders3, yoga_flame_hpHitbox9, yoga_flame_hpColliderType3, yoga_flame_hpCallback3);
	yoga_flame_hp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_hpnColliders3, yoga_flame_hpHitbox9, yoga_flame_hpColliderType3, yoga_flame_hpCallback3);
	yoga_flame_hp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_hpnColliders3, yoga_flame_hpHitbox9, yoga_flame_hpColliderType3, yoga_flame_hpCallback3);
	yoga_flame_hp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_hpnColliders3, yoga_flame_hpHitbox9, yoga_flame_hpColliderType3, yoga_flame_hpCallback3);
	yoga_flame_hp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_hpnColliders3, yoga_flame_hpHitbox9, yoga_flame_hpColliderType3, yoga_flame_hpCallback3);
	yoga_flame_hp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_hpnColliders3, yoga_flame_hpHitbox9, yoga_flame_hpColliderType3, yoga_flame_hpCallback3);
	yoga_flame_hp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_hpnColliders3, yoga_flame_hpHitbox9, yoga_flame_hpColliderType3, yoga_flame_hpCallback3);
	yoga_flame_hp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_hpnColliders3, yoga_flame_hpHitbox9, yoga_flame_hpColliderType3, yoga_flame_hpCallback3);
	yoga_flame_hp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_hpnColliders3, yoga_flame_hpHitbox9, yoga_flame_hpColliderType3, yoga_flame_hpCallback3);
	yoga_flame_hp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_hpnColliders3, yoga_flame_hpHitbox9, yoga_flame_hpColliderType3, yoga_flame_hpCallback3);
	yoga_flame_hp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_hpnColliders3, yoga_flame_hpHitbox9, yoga_flame_hpColliderType3, yoga_flame_hpCallback3);
	yoga_flame_hp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_hpnColliders3, yoga_flame_hpHitbox9, yoga_flame_hpColliderType3, yoga_flame_hpCallback3);
	yoga_flame_hp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_hpnColliders3, yoga_flame_hpHitbox9, yoga_flame_hpColliderType3, yoga_flame_hpCallback3);
	yoga_flame_hp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_hpnColliders3, yoga_flame_hpHitbox9, yoga_flame_hpColliderType3, yoga_flame_hpCallback3);
	yoga_flame_hp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_hpnColliders5, yoga_flame_hpHitbox11, yoga_flame_hpColliderType5, yoga_flame_hpCallback5);
	yoga_flame_hp.PushBack({ 2, 710, 172, 92 }, 3, { 29,5 }, yoga_flame_hpnColliders4, yoga_flame_hpHitbox10, yoga_flame_hpColliderType4, yoga_flame_hpCallback4);
	yoga_flame_hp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_hpnColliders5, yoga_flame_hpHitbox11, yoga_flame_hpColliderType5, yoga_flame_hpCallback5);
	yoga_flame_hp.PushBack({ 176, 710, 156, 92 }, 3, { 29,5 }, yoga_flame_hpnColliders5, yoga_flame_hpHitbox11, yoga_flame_hpColliderType5, yoga_flame_hpCallback5);
	yoga_flame_hp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_hpnColliders5, yoga_flame_hpHitbox11, yoga_flame_hpColliderType5, yoga_flame_hpCallback5);
	yoga_flame_hp.PushBack({ 176, 710, 156, 92 }, 3, { 29,5 }, yoga_flame_hpnColliders5, yoga_flame_hpHitbox11, yoga_flame_hpColliderType5, yoga_flame_hpCallback5);
	yoga_flame_hp.PushBack({ 617, 598, 94, 92 }, 1, { 29,5 }, yoga_flame_hpnColliders5, yoga_flame_hpHitbox11, yoga_flame_hpColliderType5, yoga_flame_hpCallback5);
	yoga_flame_hp.PushBack({ 176, 710, 156, 92 }, 3, { 29,5 }, yoga_flame_hpnColliders5, yoga_flame_hpHitbox11, yoga_flame_hpColliderType5, yoga_flame_hpCallback5);
	yoga_flame_hp.PushBack({ 617, 598, 94, 92 }, 4, { 29,5 }, yoga_flame_hpnColliders5, yoga_flame_hpHitbox11, yoga_flame_hpColliderType5, yoga_flame_hpCallback5);


	// Yoga drill
	const int yoga_drillnColliders = 3;
	const int yoga_drillnColliders3 = 4;
	SDL_Rect yoga_drillHitbox[yoga_drillnColliders] = { { 0, 0, 0, 0}, { 2, 52, 20, 20}, { 17, 45, 50, 48} };
	SDL_Rect yoga_drillHitbox3[yoga_drillnColliders3] = { { 0, 50, 24, 24}, { -19, 25, 58, 36}, { -44, 14, 54, 22}, { -58, -9, 64, 40} };
	COLLIDER_TYPE yoga_drillColliderType[yoga_drillnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	COLLIDER_TYPE yoga_drillColliderType3[yoga_drillnColliders3] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	Module* yoga_drillCallback[yoga_drillnColliders] = { {this}, {this}, {this} };
	Module* yoga_drillCallback3[yoga_drillnColliders3] = { {this}, {this}, {this}, {(Module*)App->player1} };
	yoga_drill.PushBack({ 901, 271, 94, 90 }, 7, { 32,5 }, yoga_drillnColliders, yoga_drillHitbox, yoga_drillColliderType, yoga_drillCallback);
	yoga_drill.PushBack({ 602, 522, 125, 67 }, 7, { 33,5 }, yoga_drillnColliders3, yoga_drillHitbox3, yoga_drillColliderType3, yoga_drillCallback3);
	yoga_drill.PushBack({ 729, 508, 120, 81 }, 7, { 33,5 }, yoga_drillnColliders3, yoga_drillHitbox3, yoga_drillColliderType3, yoga_drillCallback3);
	yoga_drill.PushBack({ 851, 514, 120, 75 }, 7, { 33,5 }, yoga_drillnColliders3, yoga_drillHitbox3, yoga_drillColliderType3, yoga_drillCallback3);
	yoga_drill.PushBack({ 0, 609, 122, 81 }, 7, { 33,5 }, yoga_drillnColliders3, yoga_drillHitbox3, yoga_drillColliderType3, yoga_drillCallback3);


	// Yoga mummy
	const int yoga_mummynColliders = 3;
	const int yoga_mummynColliders3 = 4;
	SDL_Rect yoga_mummyHitbox[yoga_mummynColliders] = { { 0, 0, 0, 0}, { -13, 33, 24, 20}, { -14, 48, 50, 48} };
	SDL_Rect yoga_mummyHitbox3[yoga_mummynColliders3] = { { 0, 0, 0, 0}, { -23, -7, 112, 44}, { -127, 4, 24, 22}, { -93, -4, 58, 38} };
	COLLIDER_TYPE yoga_mummyColliderType[yoga_mummynColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	COLLIDER_TYPE yoga_mummyColliderType3[yoga_mummynColliders3] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	Module* yoga_mummyCallback[yoga_mummynColliders] = { {this}, {this}, {this} };
	Module* yoga_mummyCallback3[yoga_mummynColliders3] = { {this}, {this}, {this}, {(Module*)App->player1} };
	yoga_mummy.PushBack({ 772, 272, 66, 89 }, 7, { 32,5 }, yoga_mummynColliders, yoga_mummyHitbox, yoga_drillColliderType, yoga_mummyCallback);
	yoga_mummy.PushBack({ 1, 562, 149, 27 }, 7, { 33,5 }, yoga_mummynColliders3, yoga_mummyHitbox3, yoga_mummyColliderType3, yoga_mummyCallback3);
	yoga_mummy.PushBack({ 151, 548, 148, 41 }, 7, { 33,5 }, yoga_mummynColliders3, yoga_mummyHitbox3, yoga_mummyColliderType3, yoga_mummyCallback3);
	yoga_mummy.PushBack({ 301, 562, 149, 27 }, 7, { 33,5 }, yoga_mummynColliders3, yoga_mummyHitbox3, yoga_mummyColliderType3, yoga_mummyCallback3);
	yoga_mummy.PushBack({ 452, 548, 148, 41 }, 7, { 33,5 }, yoga_mummynColliders3, yoga_mummyHitbox3, yoga_mummyColliderType3, yoga_mummyCallback3);


	// Standing reel
	const int streelnColliders = 3;

	SDL_Rect streelHitbox1[streelnColliders] = { { -11, 78, 24, 16}, { -6, 26, 40, 47}, { -6, 0, 40, 45} };
	COLLIDER_TYPE streelColliderType[streelnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* streelCallback[streelnColliders] = { {this}, {this}, {this} };

	streel.PushBack({ 70, 810, 72, 92 }, 8, { 30,5 }, streelnColliders, streelHitbox1, streelColliderType, streelCallback);
	streel.PushBack({ 144, 811, 69, 93 }, 8, { 31,5 }, streelnColliders, streelHitbox1, streelColliderType, streelCallback);
	streel.PushBack({ 215, 808, 72, 96 }, 8, { 47,5 }, streelnColliders, streelHitbox1, streelColliderType, streelCallback);
	streel.PushBack({ 289, 808, 86, 96 }, 8, { 63,5 }, streelnColliders, streelHitbox1, streelColliderType, streelCallback);


	// Standing gut reel
	const int stgreelnColliders = 3;

	SDL_Rect stgreelHitbox2[stgreelnColliders] = { { -5, 78, 44, 11}, { -13, 37, 40, 37}, { -13, 0, 45, 37} };
	COLLIDER_TYPE stgreelColliderType[stgreelnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* stgreelCallback[stgreelnColliders] = { {this}, {this}, {this} };

	stgreel.PushBack({ 377, 813, 67, 91 }, 6, { 29,5 }, stgreelnColliders, stgreelHitbox2, stgreelColliderType, stgreelCallback);
	stgreel.PushBack({ 446, 816, 80, 88 }, 6, { 42,5 }, stgreelnColliders, stgreelHitbox2, stgreelColliderType, stgreelCallback);
	stgreel.PushBack({ 528, 818, 71, 86 }, 6, { 44,5 }, stgreelnColliders, stgreelHitbox2, stgreelColliderType, stgreelCallback);
	stgreel.PushBack({ 601, 825, 76, 79 }, 6, { 45,5 }, stgreelnColliders, stgreelHitbox2, stgreelColliderType, stgreelCallback);


	// Crouching reel
	const int creelnColliders = 3;
	SDL_Rect creelHitbox1[creelnColliders] = { { -7, 44, 44, 26}, { -6, 27, 53, 21}, { -6, 0, 53, 27} };
	COLLIDER_TYPE creelColliderType[creelnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* creelCallback[creelnColliders] = { {this}, {this}, {this} };
	creel.PushBack({ 679, 833, 57, 71 }, 6, { 25,5 }, creelnColliders, { creelHitbox1 }, { creelColliderType }, { creelCallback });
	creel.PushBack({ 738, 835, 61, 69 }, 6, { 31,5 }, creelnColliders, { creelHitbox1 }, { creelColliderType }, { creelCallback });


	//Standing
	const int standingnColliders = 3;
	SDL_Rect standingHitbox[standingnColliders] = { { -32, 47, 26, 22}, { -6, 27, 46, 27}, { -6, 0, 46, 27} };
	COLLIDER_TYPE standingColliderType[standingnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* standingCallback[standingnColliders] = { {this}, {this}, {this} };

	standing.PushBack({ 274, 169, 56, 77 }, 1, { 33,5 }, standingnColliders, standingHitbox, standingColliderType, standingCallback);


	//Crouching
	const int crouchingnColliders = 3;
	SDL_Rect crouchingHitbox[crouchingnColliders] = { { -32, 47, 26, 22}, { -6, 27, 46, 27}, { -6, 0, 46, 27} };
	COLLIDER_TYPE crouchingColliderType[crouchingnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* crouchingCallback[crouchingnColliders] = { {this}, {this}, {this} };

	crouching.PushBack({ 274, 169, 56, 77 }, 1, { 33,5 }, crouchingnColliders, crouchingHitbox, crouchingColliderType, crouchingCallback);


	//Crouch
	const int crouchnColliders = 3;
	SDL_Rect crouchHitbox[crouchnColliders] = { { -32, 47, 26, 22}, { -6, 27, 46, 27}, { -6, 0, 46, 27} };
	COLLIDER_TYPE crouchColliderType[crouchnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* crouchCallback[crouchnColliders] = { {this}, {this}, {this} };

	crouch.PushBack({ 331, 181, 61, 66 }, 1, { 33,5 }, crouchnColliders, crouchHitbox, crouchColliderType, crouchCallback);

	//Crouch turn
	const int cturn_animnColliders = 3;
	SDL_Rect cturn_animHitbox[cturn_animnColliders] = { { -32, 47, 26, 22}, { -6, 27, 46, 27}, { -6, 0, 46, 27} };
	COLLIDER_TYPE cturn_animColliderType[cturn_animnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* cturn_animCallback[cturn_animnColliders] = { {this}, {this}, {this} };

	cturn_anim.PushBack({ 393, 175, 57, 70 }, 3, { 33,5 }, cturn_animnColliders, cturn_animHitbox, cturn_animColliderType, cturn_animCallback);
	cturn_anim.PushBack({ 452, 175, 58, 70 }, 3, { 33,5 }, cturn_animnColliders, cturn_animHitbox, cturn_animColliderType, cturn_animCallback);

	//Crouching l punch
	const int clpnColliders = 3;
	const int clpnColliders2 = 4;
	SDL_Rect clpHitbox1[clpnColliders] = { { -32, 47, 26, 22}, { -6, 27, 46, 27}, { -6, 0, 46, 27} };
	SDL_Rect clpHitbox[clpnColliders] = { { -75, 52, 24, 16}, { -6, 0, 40, 53}, { -46, 0, 37, 53} };
	SDL_Rect clpHitbox2[clpnColliders2] = { { -103, 0, 65, 33}, { -6, 0, 40, 53}, { -46, 0, 57, 53}, {-163, 5, 35, 25} };
	COLLIDER_TYPE clpColliderType[clpnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	COLLIDER_TYPE clpColliderType2[clpnColliders2] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	Module*clpCallback[clpnColliders] = { {this}, {this}, {this} };
	Module*clpCallback2[clpnColliders2] = { {this}, {this}, {this}, {(Module*)App->player1} };
	clp.PushBack({ 511, 170, 83, 75 }, 2, { 46,5 }, { clpnColliders }, { clpHitbox1 }, { clpColliderType }, { clpCallback });
	clp.PushBack({ 596, 193, 83, 52 }, 3, { 33,5 }, { clpnColliders }, { clpHitbox }, { clpColliderType }, { clpCallback });
	clp.PushBack({ 681, 211, 135, 34 }, 3, { 33,5 }, { clpnColliders }, { clpHitbox }, { clpColliderType }, { clpCallback });
	clp.PushBack({ 817, 211, 199, 34 }, 6, { 33,5 }, { clpnColliders2 }, { clpHitbox2 }, { clpColliderType2 }, { clpCallback2 });
	clp.PushBack({ 681, 211, 135, 34 }, 4, { 33,5 }, { clpnColliders }, { clpHitbox }, { clpColliderType }, { clpCallback });
	clp.PushBack({ 596, 193, 83, 52 }, 5, { 33,5 }, { clpnColliders }, { clpHitbox }, { clpColliderType }, { clpCallback });


	//Crouching close_l punch
	const int close_clpnColliders = 3;
	const int close_clpnColliders2 = 4;
	SDL_Rect close_clpHitbox[close_clpnColliders] = { { 0, 0, 0, 0}, { -6, 1, 46, 51}, { -31, 46, 24, 20} };
	SDL_Rect close_clpHitbox2[close_clpnColliders2] = { { 0, 0, 0, 0}, { -6, 1, 46, 51}, { -31, 46, 24, 20}, {-54, 39, 66, 26} };
	COLLIDER_TYPE close_clpColliderType[close_clpnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	COLLIDER_TYPE close_clpColliderType2[close_clpnColliders2] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	Module*close_clpCallback[close_clpnColliders] = { {this}, {this}, {this} };
	Module*close_clpCallback2[close_clpnColliders2] = { {this}, {this}, {this}, {(Module*)App->player1} };
	close_clp.PushBack({ 2, 299, 66, 62 }, 2, { 46,5 }, { close_clpnColliders }, { close_clpHitbox }, { close_clpColliderType }, { close_clpCallback });
	close_clp.PushBack({ 70, 300, 119, 61 }, 4, { 33,5 }, { close_clpnColliders2 }, { close_clpHitbox2 }, { close_clpColliderType2 }, { close_clpCallback2 });
	close_clp.PushBack({ 2, 299, 66, 62 }, 5, { 33,5 }, { close_clpnColliders }, { close_clpHitbox }, { close_clpColliderType }, { close_clpCallback });


	//Crouching m punch
	const int cmpnColliders = 3;
	const int cmpnColliders2 = 4;
	SDL_Rect cmpHitbox1[cmpnColliders] = { { -32, 47, 26, 22 },{ -6, 27, 46, 27 },{ -6, 0, 46, 27 } };
	SDL_Rect cmpHitbox[cmpnColliders] = { { -75, 52, 24, 16 },{ -6, 0, 40, 53 },{ -46, 0, 37, 53 } };
	SDL_Rect cmpHitbox2[cmpnColliders2] = { { -103, 0, 65, 33 },{ -6, 0, 40, 53 },{ -46, 0, 57, 53 },{ -178, 5, 50, 25 } };
	COLLIDER_TYPE cmpColliderType[cmpnColliders] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 } };
	COLLIDER_TYPE cmpColliderType2[cmpnColliders2] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2_HIT } };
	Module*cmpCallback[cmpnColliders] = { { this },{ this },{ this } };
	Module*cmpCallback2[cmpnColliders2] = { { this },{ this },{ this },{ (Module*)App->player1 } };
	cmp.PushBack({ 511, 170, 83, 75 }, 2, { 46,5 }, { cmpnColliders }, { cmpHitbox1 }, { cmpColliderType }, { cmpCallback });
	cmp.PushBack({ 596, 193, 83, 52 }, 3, { 33,5 }, { cmpnColliders }, { cmpHitbox }, { cmpColliderType }, { cmpCallback });
	cmp.PushBack({ 681, 211, 135, 34 }, 3, { 33,5 }, { cmpnColliders }, { cmpHitbox }, { cmpColliderType }, { cmpCallback });
	cmp.PushBack({ 817, 211, 199, 34 }, 6, { 33,5 }, { cmpnColliders2 }, { cmpHitbox2 }, { cmpColliderType2 }, { cmpCallback2 });
	cmp.PushBack({ 681, 211, 135, 34 }, 4, { 33,5 }, { cmpnColliders }, { cmpHitbox }, { cmpColliderType }, { cmpCallback });
	cmp.PushBack({ 596, 193, 83, 52 }, 5, { 33,5 }, { cmpnColliders }, { cmpHitbox }, { cmpColliderType }, { cmpCallback });


	//Crouching close_m punch
	const int close_cmpnColliders = 3;
	const int close_cmpnColliders2 = 4;
	SDL_Rect close_cmpHitbox[close_cmpnColliders] = { { 0, 0, 0, 0}, { -6, 1, 46, 51}, { -31, 46, 24, 20} };
	SDL_Rect close_cmpHitbox2[close_cmpnColliders2] = { { 0, 0, 0, 0}, { -6, 1, 46, 51}, { -31, 46, 24, 20}, {-63, 45, 55, 25} };
	COLLIDER_TYPE close_cmpColliderType[close_cmpnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	COLLIDER_TYPE close_cmpColliderType2[close_cmpnColliders2] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	Module*close_cmpCallback[close_cmpnColliders] = { {this}, {this}, {this} };
	Module*close_cmpCallback2[close_cmpnColliders2] = { {this}, {this}, {this}, {(Module*)App->player1} };
	close_cmp.PushBack({ 2, 299, 66, 62 }, 3, { 46,5 }, { close_cmpnColliders }, { close_cmpHitbox }, { close_cmpColliderType }, { close_cmpCallback });
	close_cmp.PushBack({ 70, 300, 119, 61 }, 5, { 33,5 }, { close_cmpnColliders2 }, { close_cmpHitbox2 }, { close_cmpColliderType2 }, { close_cmpCallback2 });
	close_cmp.PushBack({ 2, 299, 66, 62 }, 7, { 33,5 }, { close_cmpnColliders }, { close_cmpHitbox }, { close_cmpColliderType }, { close_cmpCallback });


	//Crouching h punch
	const int chpnColliders = 3;
	const int chpnColliders2 = 4;
	SDL_Rect chpHitbox1[chpnColliders] = { { -32, 47, 26, 22 },{ -6, 27, 46, 27 },{ -6, 0, 46, 27 } };
	SDL_Rect chpHitbox[chpnColliders] = { { -75, 52, 24, 16 },{ -6, 0, 40, 53 },{ -46, 0, 37, 53 } };
	SDL_Rect chpHitbox2[chpnColliders2] = { { -103, 0, 65, 33 },{ -6, 0, 40, 53 },{ -46, 0, 57, 53 },{ -188, 0, 60, 53 } };
	COLLIDER_TYPE chpColliderType[chpnColliders] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 } };
	COLLIDER_TYPE chpColliderType2[chpnColliders2] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2_HIT } };
	Module*chpCallback[chpnColliders] = { { this },{ this },{ this } };
	Module*chpCallback2[chpnColliders2] = { { this },{ this },{ this },{ (Module*)App->player1 } };
	chp.PushBack({ 511, 170, 83, 75 }, 3, { 46,5 }, { chpnColliders }, { chpHitbox1 }, { chpColliderType }, { chpCallback });
	chp.PushBack({ 596, 193, 83, 52 }, 4, { 33,5 }, { chpnColliders }, { chpHitbox }, { chpColliderType }, { chpCallback });
	chp.PushBack({ 681, 211, 135, 34 }, 4, { 33,5 }, { chpnColliders }, { chpHitbox }, { chpColliderType }, { chpCallback });
	chp.PushBack({ 817, 211, 199, 34 }, 6, { 33,5 }, { chpnColliders2 }, { chpHitbox2 }, { chpColliderType2 }, { chpCallback2 });
	chp.PushBack({ 681, 211, 135, 34 }, 10, { 33,5 }, { chpnColliders }, { chpHitbox }, { chpColliderType }, { chpCallback });
	chp.PushBack({ 596, 193, 83, 52 }, 9, { 33,5 }, { chpnColliders }, { chpHitbox }, { chpColliderType }, { chpCallback });


	//Crouching l kik
	const int clknColliders = 3;
	const int clknColliders2 = 4;
	SDL_Rect clkHitbox[clknColliders] = { { -32, 47, 26, 22}, { -6, 27, 46, 27}, { -6, 0, 46, 27} };
	SDL_Rect clkHitbox2[clknColliders2] = { { -15, 30, 24, 16}, { -6, 0, 48, 43}, { -54, 0, 88, 43}, { -62, 5, 88, 17} };
	COLLIDER_TYPE clkColliderType[clknColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	COLLIDER_TYPE clkColliderType2[clknColliders2] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	Module*clkCallback[clknColliders] = { {this}, {this}, {this} };
	Module*clkCallback2[clknColliders2] = { {this}, {this}, {this}, {(Module*)App->player1} };


	clk.PushBack({ 331, 181, 61, 66 }, 1, { 33,5 }, { clknColliders }, { clkHitbox }, { clkColliderType }, { clkCallback });
	clk.PushBack({ 190, 298, 65, 64 }, 1, { 33,5 }, { clknColliders }, { clkHitbox }, { clkColliderType }, { clkCallback });
	clk.PushBack({ 257, 318, 150, 43 }, 14, { 76,5 }, { clknColliders2 }, { clkHitbox2 }, { clkColliderType2 }, { clkCallback2 });
	clk.PushBack({ 190, 298, 65, 64 }, 5, { 33,5 }, { clknColliders }, { clkHitbox }, { clkColliderType }, { clkCallback });


	//Crouching close_l kick
	const int close_clknColliders = 3;
	const int close_clknColliders2 = 4;
	SDL_Rect close_clkHitbox[close_clknColliders] = { { 0, 0, 0, 0}, { -6, 1, 46, 51}, { -31, 46, 24, 20} };
	SDL_Rect close_clkHitbox2[close_clknColliders2] = { { 0, 0, 0, 0}, { -6, 1, 46, 51}, { -31, 46, 24, 20}, {-31, 3, 90, 26} };
	COLLIDER_TYPE close_clkColliderType[close_clknColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	COLLIDER_TYPE close_clkColliderType2[close_clknColliders2] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	Module*close_clkCallback[close_clknColliders] = { {this}, {this}, {this} };
	Module*close_clkCallback2[close_clknColliders2] = { {this}, {this}, {this}, {(Module*)App->player1} };
	close_clk.PushBack({ 408, 301, 66, 60 }, 4, { 46,5 }, { close_clknColliders }, { close_clkHitbox }, { close_clkColliderType }, { close_clkCallback });
	close_clk.PushBack({ 476, 305, 117, 56 }, 8, { 33,5 }, { close_clknColliders2 }, { close_clkHitbox2 }, { close_clkColliderType2 }, { close_clkCallback2 });
	close_clk.PushBack({ 408, 301, 66, 60 }, 5, { 33,5 }, { close_clknColliders }, { close_clkHitbox }, { close_clkColliderType }, { close_clkCallback });


	//Crouching m kik
	const int cmknColliders = 3;
	const int cmknColliders2 = 4;
	SDL_Rect cmkHitbox[cmknColliders] = { { -32, 47, 26, 22 },{ -6, 27, 46, 27 },{ -6, 0, 46, 27 } };
	SDL_Rect cmkHitbox2[cmknColliders2] = { { -15, 30, 24, 16 },{ -6, 0, 48, 43 },{ -54, 0, 88, 43 },{ -62, 5, 88, 17 } };
	COLLIDER_TYPE cmkColliderType[cmknColliders] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 } };
	COLLIDER_TYPE cmkColliderType2[cmknColliders2] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2_HIT } };
	Module*cmkCallback[cmknColliders] = { { this },{ this },{ this } };
	Module*cmkCallback2[cmknColliders2] = { { this },{ this },{ this },{ (Module*)App->player1 } };


	cmk.PushBack({ 331, 181, 61, 66 }, 3, { 33,5 }, { cmknColliders }, { cmkHitbox }, { cmkColliderType }, { cmkCallback });
	cmk.PushBack({ 190, 298, 65, 64 }, 4, { 33,5 }, { cmknColliders }, { cmkHitbox }, { cmkColliderType }, { cmkCallback });
	cmk.PushBack({ 257, 318, 150, 43 }, 16, { 76,5 }, { cmknColliders2 }, { cmkHitbox2 }, { cmkColliderType2 }, { cmkCallback2 });
	cmk.PushBack({ 190, 298, 65, 64 }, 5, { 33,5 }, { cmknColliders }, { cmkHitbox }, { cmkColliderType }, { cmkCallback });


	//Crouching close_m kick
	const int close_cmknColliders = 3;
	const int close_cmknColliders2 = 4;
	SDL_Rect close_cmkHitbox[close_cmknColliders] = { { 0, 0, 0, 0}, { -12, 1, 46, 51}, { -37, 47, 24, 20} };
	SDL_Rect close_cmkHitbox2[close_cmknColliders2] = { { 0, 0, 0, 0}, { -12, 1, 46, 51}, { -37, 47, 24, 20}, {-37, 3, 90, 26} };
	COLLIDER_TYPE close_cmkColliderType[close_cmknColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	COLLIDER_TYPE close_cmkColliderType2[close_cmknColliders2] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	Module*close_cmkCallback[close_cmknColliders] = { {this}, {this}, {this} };
	Module*close_cmkCallback2[close_cmknColliders2] = { {this}, {this}, {this}, {(Module*)App->player1} };
	close_cmk.PushBack({ 191, 299, 64, 62 }, 1, { 46,5 }, { close_cmknColliders }, { close_cmkHitbox }, { close_cmkColliderType }, { close_cmkCallback });
	close_cmk.PushBack({ 595, 299, 117, 62 }, 8, { 33,5 }, { close_cmknColliders2 }, { close_cmkHitbox2 }, { close_cmkColliderType2 }, { close_cmkCallback2 });
	close_cmk.PushBack({ 191, 299, 64, 62 }, 5, { 33,5 }, { close_cmknColliders }, { close_cmkHitbox }, { close_cmkColliderType }, { close_cmkCallback });


	//Crouching h kik
	const int chknColliders = 3;
	const int chknColliders2 = 4;
	SDL_Rect chkHitbox[chknColliders] = { { -32, 47, 26, 22 },{ -6, 27, 46, 27 },{ -6, 0, 46, 27 } };
	SDL_Rect chkHitbox2[chknColliders2] = { { -15, 30, 24, 16 },{ -6, 0, 48, 43 },{ -54, 0, 88, 43 },{ -62, 5, 88, 17 } };
	COLLIDER_TYPE chkColliderType[chknColliders] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 } };
	COLLIDER_TYPE chkColliderType2[chknColliders2] = { { COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2 },{ COLLIDER_PLAYER2_HIT } };
	Module*chkCallback[clknColliders] = { { this },{ this },{ this } };
	Module*chkCallback2[chknColliders2] = { { this },{ this },{ this },{ (Module*)App->player1 } };


	chk.PushBack({ 331, 181, 61, 66 }, 3, { 33,5 }, { chknColliders }, { chkHitbox }, { chkColliderType }, { chkCallback });
	chk.PushBack({ 190, 298, 65, 64 }, 4, { 33,5 }, { chknColliders }, { chkHitbox }, { chkColliderType }, { chkCallback });
	chk.PushBack({ 257, 318, 150, 43 }, 18, { 76,5 }, { chknColliders2 }, { chkHitbox2 }, { chkColliderType2 }, { chkCallback2 });
	chk.PushBack({ 190, 298, 65, 64 }, 21, { 33,5 }, { chknColliders }, { chkHitbox }, { chkColliderType }, { chkCallback });


	// Win1

	const int winnColliders = 3;
	SDL_Rect winHitbox1[winnColliders] = { { 0, 0, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0} };
	COLLIDER_TYPE winColliderType[winnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* winCallback[winnColliders] = { {this}, {this}, {this} };

	win1.PushBack({ 1, 2, 101, 105 }, 5, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });
	win1.PushBack({ 105, 2, 102, 105 }, 10, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });
	win1.PushBack({ 209, 16, 120, 93 }, 10, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });
	win1.PushBack({ 331, 16, 120, 93 }, 10, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });
	win1.PushBack({ 2, 150, 132, 82 }, 10, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });
	win1.PushBack({ 136, 150, 132, 82 }, 10, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });
	win1.PushBack({ 270, 125, 101, 107 }, 10, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });
	win1.loop = false;

	// Win2
	const int win2nColliders = 3;
	SDL_Rect win2Hitbox1[win2nColliders] = { { 0, 0, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0} };
	COLLIDER_TYPE win2ColliderType[win2nColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* win2Callback[win2nColliders] = { {this}, {this}, {this} };

	win2.PushBack({ 373, 145, 63, 87 }, 5, { 33,5 }, { win2nColliders }, { win2Hitbox1 }, { win2ColliderType }, { win2Callback });
	win2.PushBack({ 438, 110, 57, 122 }, 10, { 33,5 }, { win2nColliders }, { win2Hitbox1 }, { win2ColliderType }, { win2Callback });
	win2.PushBack({ 13, 246, 58, 69 }, 10, { 33,5 }, { win2nColliders }, { win2Hitbox1 }, { win2ColliderType }, { win2Callback });
	win2.PushBack({ 74, 241, 73, 74 }, 10, { 33,5 }, { win2nColliders }, { win2Hitbox1 }, { win2ColliderType }, { win2Callback });
	win2.loop = false;

	// Air falling
	const int airreelnColliders = 3;
	SDL_Rect airreelHitbox1[airreelnColliders] = { { 0, 0, 0, 0}, { 0, 11, 73, 37}, { -71, 5, 50, 45} };
	COLLIDER_TYPE airreelColliderType[airreelnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* airreelCallback[airreelnColliders] = { {this}, {this}, {this} };

	airreel.PushBack({ 639, 750, 125, 52 }, 1000, { 33,5 }, { airreelnColliders }, { airreelHitbox1 }, { airreelColliderType }, { airreelCallback });


	// Falling
	const int sweepnColliders = 3;
	SDL_Rect sweepHitbox1[sweepnColliders] = { { 0, 0, 0, 0}, { -15, 11, 53, 70}, { -61, 5, 50, 35} };
	SDL_Rect sweepHitbox2[sweepnColliders] = { { 0, 0, 0, 0}, { 0, 5, 93, 37}, { 0, 0, 0, 0} };
	COLLIDER_TYPE sweepColliderType[sweepnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	COLLIDER_TYPE sweepColliderType2[sweepnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* sweepCallback[sweepnColliders] = { {this}, {this}, {this} };
	Module* sweepCallback2[sweepnColliders] = { {this}, {this}, {this} };

	sweep.PushBack({ 801, 820, 94, 84 }, 30, { 33,5 }, { sweepnColliders }, { sweepHitbox1 }, { sweepColliderType }, { airreelCallback });
	sweep.PushBack({ 897, 848, 97, 56 }, 10, { 33,5 }, { sweepnColliders }, { sweepHitbox2 }, { sweepColliderType2 }, { sweepCallback2 });
	sweep.PushBack({ 1, 978, 96, 48 }, 10, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });
	sweep.PushBack({ 99, 989, 96, 31 }, 20, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });


	//Get up

	getup.PushBack({ 197, 989, 131, 31 }, 10, { 33,5 }, { win2nColliders }, { winHitbox1 }, { winColliderType }, { winCallback });
	getup.PushBack({ 330, 974, 122, 46 }, 10, { 33,5 }, { win2nColliders }, { winHitbox1 }, { winColliderType }, { winCallback });
	getup.PushBack({ 455, 920, 83, 100 }, 10, { 33,5 }, { win2nColliders }, { winHitbox1 }, { winColliderType }, { winCallback });


	// Defending
	const int defendingnColliders = 3;
	SDL_Rect defendingHitbox1[defendingnColliders] = { { 0, 0, 0, 0}, { 0, 0, 0, 0}, { -10, 0, 30, 20} };
	COLLIDER_TYPE defendingColliderType[defendingnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* defendingCallback[defendingnColliders] = { {this}, {this}, {this} };

	defending.PushBack({ 872, 708, 61, 94 }, 10, { 33,5 }, { defendingnColliders }, { defendingHitbox1 }, { defendingColliderType }, { defendingCallback });


	// Crouch defending
	const int cdefendingnColliders = 3;
	SDL_Rect cdefendingHitbox1[cdefendingnColliders] = { { 0, 0, 0, 0}, { 0, 0, 0, 0}, { 0, 30, 30, 40} };
	COLLIDER_TYPE cdefendingColliderType[cdefendingnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* cdefendingCallback[cdefendingnColliders] = { {this}, {this}, {this} };

	cdefending.PushBack({ 935, 735, 60, 67 }, 5, { 33,5 }, { cdefendingnColliders }, { cdefendingHitbox1 }, { cdefendingColliderType }, { cdefendingCallback });
	cdefending.PushBack({ 5, 838, 63, 66 }, 50, { 33,5 }, { cdefendingnColliders }, { cdefendingHitbox1 }, { cdefendingColliderType }, { cdefendingCallback });


	// Grab
	grab.PushBack({ 540, 932, 116, 88 }, 10, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });
	grab.PushBack({ 658, 933, 80, 87 }, 10, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });
	grab.PushBack({ 741, 927, 72, 93 }, 10, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });
	grab.PushBack({ 815, 956, 134, 64 }, 10, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });


	// Grab2
	grab2.PushBack({ 150, 242, 77, 116 }, 5, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });
	grab2.PushBack({ 228, 244, 71, 114 }, 5, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });
	grab2.PushBack({ 300, 244, 70, 114 }, 5, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });
	grab2.PushBack({ 371, 244, 70, 114 }, 10, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });
	grab2.PushBack({ 228, 244, 71, 114 }, 5, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });
	grab2.PushBack({ 371, 244, 70, 114 }, 10, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });
	grab2.PushBack({ 228, 244, 71, 114 }, 5, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });
	grab2.PushBack({ 371, 244, 70, 114 }, 10, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });
	grab2.PushBack({ 228, 244, 71, 114 }, 5, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });
	grab2.PushBack({ 371, 244, 70, 114 }, 10, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });
	grab2.PushBack({ 228, 244, 71, 114 }, 5, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });
	grab2.PushBack({ 371, 244, 70, 114 }, 10, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });
	grab2.PushBack({ 228, 244, 71, 114 }, 5, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });
	grab2.PushBack({ 371, 244, 70, 114 }, 10, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });
	grab2.PushBack({ 228, 244, 71, 114 }, 5, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });

	// Grabbing
	const int grabbingnColliders = 4;
	SDL_Rect grabbingHitbox1[grabbingnColliders] = { { -25, 76, 24, 16}, { -16, 50, 50, 27}, { -10, 3, 40, 50}, { -30, 50, 70, 27} };
	COLLIDER_TYPE grabbingColliderType[grabbingnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_GRAB} };
	Module* grabbingCallback[grabbingnColliders] = { {this}, {this}, {this}, {this} };

	grabbing.PushBack({ 694, 448, 76, 89 }, 1, { 33,5 }, { grabbingnColliders }, { grabbingHitbox1 }, { grabbingColliderType }, { grabbingCallback });


	// Lose animation
	lose.PushBack({ 99, 989, 96, 31 }, 20, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });


	// Lose animation on ground
	ground.PushBack({ 1, 978, 96, 48 }, 10, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });
	ground.PushBack({ 99, 989, 96, 31 }, 20, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });

	// Stun animation
	const int stunnColliders = 3;
	SDL_Rect stunHitbox[stunnColliders] = { { -25, 76, 24, 16}, { -16, 50, 50, 27}, { -10, 3, 40, 50} };
	COLLIDER_TYPE stunColliderType[stunnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* stunCallback[stunnColliders] = { {this}, {this}, {this} };

	stun.PushBack({ 430, 709, 64, 93 }, 10, { 33,5 }, stunnColliders, stunHitbox, stunColliderType, stunCallback);
	stun.PushBack({ 496, 713, 67, 89 }, 10, { 33,5 }, stunnColliders, stunHitbox, stunColliderType, stunCallback);
	stun.PushBack({ 565, 717, 72, 85 }, 10, { 33,5 }, stunnColliders, stunHitbox, stunColliderType, stunCallback);
	stun.loop = true;

	inputs.Push(IN_END2);

	return ret;
}

bool ModulePlayer2::CleanUp()
{
	LOG("Unloading ryu 2");

	App->audio->UnloadChunk(hdk_voice);
	hdk_voice = nullptr;
	App->audio->UnloadChunk(hdk_hit);
	hdk_hit = nullptr;
	App->audio->UnloadChunk(low_kick);
	low_kick = nullptr;
	App->audio->UnloadChunk(low_fist);
	low_fist = nullptr;
	App->audio->UnloadChunk(high_fist);
	high_fist = nullptr;
	App->audio->UnloadChunk(high_kick);
	high_kick = nullptr;
	App->audio->UnloadChunk(block);
	block = nullptr;
	App->audio->UnloadChunk(yoga_snd);
	yoga_snd = nullptr;
	App->audio->UnloadChunk(fire_snd);
	fire_snd = nullptr;
	App->audio->UnloadChunk(flame_snd);
	flame_snd = nullptr;

	App->textures->Unload(graphics);
	App->textures->Unload(graphics2);
	App->textures->Unload(graphics3);
	App->textures->Unload(shadow);
	ClearColliders();
	idle = forward = backward = Animation();
	lp = lk = clp = clk = cmp = cmk = chp = chk = mp = hp = mk = hk = close_lp = close_lk = close_clp = close_clk = close_cmp = close_cmk = close_chp = close_chk = Animation();
	jlp = jlk = jmp = jmk = jhp = jhk = close_mp = close_hp = close_mk = close_hk = close_firstframe_lk_mk = Animation();
	neutralJump = forwardJump = backwardJump = yoga_drill = yoga_mummy = Animation();
	yoga_fire_lp = yoga_fire_mp = yoga_fire_hp = yoga_flame_lp = yoga_flame_hp = yoga_flame_mp = Animation();
	streel = stgreel = creel = turn_anim = cturn_anim = Animation();
	airreel = sweep = getup = Animation();
	crouching = standing = crouch = defending = cdefending = grab = grab2 = stun = Animation();
	win1 = win2 = lose = Animation();
	ground = Animation();
	return true;
}

update_status ModulePlayer2::PreUpdate() {
	ClearColliders();
	IsClose();
	return UPDATE_CONTINUE;
}

// Update: draw background
update_status ModulePlayer2::Update()
{
	ryu_states2 current_state = ST_UNKNOWN2;
	Animation* current_animation = &idle;
	SDL_Texture *texture = graphics;
	int hdk_spawn; 

	if (flip == false)
	{
		hdk_spawn = 25;
	}

	if (flip == true)
	{
		hdk_spawn = -45;
	}

	if (state == L_YFLAME2 || state == M_YFLAME2 || state == F_YFLAME2 || state == L_YFIRE2 || state == M_YFIRE2 || state == F_YFIRE2) {
		timeUpdated = App->frames;
	}
	else {
		yoga_check = true;
		yoga_sound = false;
		flame_sound = false;
	}

	external_input(inputs);
	internal_input(inputs);
	state = process_fsm(inputs);

	if (state != current_state)
	{
		switch (state)
		{
		case ST_IDLE2:
			texture = graphics3;
			current_animation = &idle;
			typeofattack = 0;
			dizzydamage = 0;
			break;

		case ST_WALK_FORWARD2:
			texture = graphics3;
			current_animation = (flip ? &forward : &backward);
			if (IsntOnLeftLimit())
				position.x--;
			break;

		case ST_WALK_BACKWARD2:
			texture = graphics3;
			current_animation = (flip ? &backward : &forward);
			if (IsntOnRightLimit())
				position.x++;
			break;

		case ST_JUMP_NEUTRAL2:
			texture = graphics;
			current_animation = &neutralJump;
			if (App->frames - jump_timer < 24 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}

			if (App->frames - jump_timer < 28 && (App->frames - jump_timer > 23))
			{
				jumpHeight -= speed;
			}

			if (App->frames - jump_timer < 32 && (App->frames - jump_timer > 27))
			{
				jumpHeight += speed;
			}

			if (App->frames - jump_timer > 31 && (App->frames - jump_timer <= D_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}
			break;

		case ST_JUMP_FORWARD2:
			current_animation = &forwardJump;
			if (App->frames - jump_timer < 24 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}

			if (App->frames - jump_timer < 28 && (App->frames - jump_timer > 23))
			{
				jumpHeight -= speed;
			}

			if (App->frames - jump_timer < 32 && (App->frames - jump_timer > 27))
			{
				jumpHeight += speed;
			}

			if (App->frames - jump_timer > 31 && (App->frames - jump_timer <= D_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}

			if (IsntOnLeftLimit())
				position.x -= 2;
			break;

		case ST_JUMP_BACKWARD2:
			current_animation = &backwardJump;
			if (App->frames - jump_timer < 24 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}

			if (App->frames - jump_timer < 28 && (App->frames - jump_timer > 23))
			{
				jumpHeight -= speed;
			}

			if (App->frames - jump_timer < 32 && (App->frames - jump_timer > 27))
			{
				jumpHeight += speed;
			}

			if (App->frames - jump_timer > 31 && (App->frames - jump_timer <= D_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}

			if (IsntOnRightLimit()){
				position.x += 2;
		}
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
			typeofattack = 1;
			dizzydamage = 1;
			break;

		case M_PUNCH_CROUCH2:
			current_animation = &cmp;
			typeofattack = 2;
			dizzydamage = 2;
			break;

		case F_PUNCH_CROUCH2:
			current_animation = &chp;
			typeofattack = 3;
			dizzydamage = 3;
			break;

		case L_PUNCH_STANDING2:
			texture = graphics3;
			current_animation = &lp;
			typeofattack = 1;
			dizzydamage = 1;
			break;

		case M_PUNCH_STANDING2:
			texture = graphics3;
			current_animation = &mp;
			typeofattack = 2;
			dizzydamage = 2;
			break;

		case F_PUNCH_STANDING2:
			texture = graphics3;
			current_animation = &hp;
			typeofattack = 3;
			dizzydamage = 3;
			break;

		case L_PUNCH_NEUTRAL_JUMP2:
			current_animation = &jlp;
			if (App->frames - jump_timer < 24 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}

			if (App->frames - jump_timer < 28 && (App->frames - jump_timer > 23))
			{
				jumpHeight -= speed;
			}

			if (App->frames - jump_timer < 32 && (App->frames - jump_timer > 27))
			{
				jumpHeight += speed;
			}

			if (App->frames - jump_timer > 31 && (App->frames - jump_timer <= D_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}
			typeofattack = 1;
			dizzydamage = 1;
			break;

		case M_PUNCH_NEUTRAL_JUMP2:
			current_animation = &jmp;
			if (App->frames - jump_timer < 24 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}

			if (App->frames - jump_timer < 28 && (App->frames - jump_timer > 23))
			{
				jumpHeight -= speed;
			}

			if (App->frames - jump_timer < 32 && (App->frames - jump_timer > 27))
			{
				jumpHeight += speed;
			}

			if (App->frames - jump_timer > 31 && (App->frames - jump_timer <= D_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}
			typeofattack = 2;
			dizzydamage = 2;
			break;

		case F_PUNCH_NEUTRAL_JUMP2:
			current_animation = &jhp;
			if (App->frames - jump_timer < 24 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}

			if (App->frames - jump_timer < 28 && (App->frames - jump_timer > 23))
			{
				jumpHeight -= speed;
			}

			if (App->frames - jump_timer < 32 && (App->frames - jump_timer > 27))
			{
				jumpHeight += speed;
			}

			if (App->frames - jump_timer > 31 && (App->frames - jump_timer <= D_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}
			typeofattack = 3;
			dizzydamage = 3;
			break;

		case L_PUNCH_FORWARD_JUMP2:
			current_animation = &jlp;
			if (App->frames - jump_timer < 24 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}

			if (App->frames - jump_timer < 28 && (App->frames - jump_timer > 23))
			{
				jumpHeight -= speed;
			}

			if (App->frames - jump_timer < 32 && (App->frames - jump_timer > 27))
			{
				jumpHeight += speed;
			}

			if (App->frames - jump_timer > 31 && (App->frames - jump_timer <= D_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}

			if (IsntOnLeftLimit())
				position.x -= 2;
			typeofattack = 1;
			dizzydamage = 1;

			break;

		case M_PUNCH_FORWARD_JUMP2:
			current_animation = &jmp;
			if (App->frames - jump_timer < 24 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}

			if (App->frames - jump_timer < 28 && (App->frames - jump_timer > 23))
			{
				jumpHeight -= speed;
			}

			if (App->frames - jump_timer < 32 && (App->frames - jump_timer > 27))
			{
				jumpHeight += speed;
			}

			if (App->frames - jump_timer > 31 && (App->frames - jump_timer <= D_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}

			if (IsntOnLeftLimit())
				position.x -= 2;
			typeofattack = 2;
			dizzydamage = 2;
			break;

		case F_PUNCH_FORWARD_JUMP2:
			current_animation = &jhp;
			if (App->frames - jump_timer < 24 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}

			if (App->frames - jump_timer < 28 && (App->frames - jump_timer > 23))
			{
				jumpHeight -= speed;
			}

			if (App->frames - jump_timer < 32 && (App->frames - jump_timer > 27))
			{
				jumpHeight += speed;
			}

			if (App->frames - jump_timer > 31 && (App->frames - jump_timer <= D_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}

			if (IsntOnLeftLimit())
				position.x -= 2;
			typeofattack = 3;
			dizzydamage = 3;

			break;

		case L_PUNCH_BACKWARD_JUMP2:

			current_animation = &jlp;

			if (App->frames - jump_timer < 24 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}

			if (App->frames - jump_timer < 28 && (App->frames - jump_timer > 23))
			{
				jumpHeight -= speed;
			}

			if (App->frames - jump_timer < 32 && (App->frames - jump_timer > 27))
			{
				jumpHeight += speed;
			}

			if (App->frames - jump_timer > 31 && (App->frames - jump_timer <= D_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}

			if (IsntOnRightLimit())
				position.x += 2;
			typeofattack = 1;
			dizzydamage = 1;
			break;

		case M_PUNCH_BACKWARD_JUMP2:
			current_animation = &jmp;

			if (App->frames - jump_timer < 24 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}

			if (App->frames - jump_timer < 28 && (App->frames - jump_timer > 23))
			{
				jumpHeight -= speed;
			}

			if (App->frames - jump_timer < 32 && (App->frames - jump_timer > 27))
			{
				jumpHeight += speed;
			}

			if (App->frames - jump_timer > 31 && (App->frames - jump_timer <= D_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}

			if (IsntOnRightLimit())
				position.x += 2;
			typeofattack = 2;
			dizzydamage = 2;
			break;

		case F_PUNCH_BACKWARD_JUMP2:
			current_animation = &jhp;

			if (App->frames - jump_timer < 24 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}

			if (App->frames - jump_timer < 28 && (App->frames - jump_timer > 23))
			{
				jumpHeight -= speed;
			}

			if (App->frames - jump_timer < 32 && (App->frames - jump_timer > 27))
			{
				jumpHeight += speed;
			}

			if (App->frames - jump_timer > 31 && (App->frames - jump_timer <= D_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}

			if (IsntOnRightLimit())
				position.x += 2;
			typeofattack = 3;
			dizzydamage = 3;
			break;

		case L_KIK_CROUCH2:
			current_animation = &clk;
			if ((App->frames - l_crouching_kik_timer < 14 && App->frames - l_crouching_kik_timer > 2) && (colliding == false)) {
				if (flip == true) {
					position.x -= speed * 3;
				}
				else position.x += speed * 3;
			}
			typeofattack = 1;
			dizzydamage = 1;
			colliding = false;
			break;

		case M_KIK_CROUCH2:
			current_animation = &cmk;
			if ((App->frames - m_crouching_kik_timer < 19 && App->frames - m_crouching_kik_timer > 7) && (colliding == false)) {
				if (flip == true) {
					position.x -= speed * 4;
				}
				else position.x += speed * 4;
			}
			typeofattack = 2;
			dizzydamage = 2;
			colliding = false;
			break;

		case F_KIK_CROUCH2:
			current_animation = &chk;
			if ((App->frames - f_crouching_kik_timer < 18 && App->frames - f_crouching_kik_timer > 7) && (colliding == false)) {
				if (flip == true) {
					position.x -= speed * 5;
				}
				else position.x += speed * 5;
			}
			typeofattack = 3;
			dizzydamage = 3;
			colliding = false;
			break;

		case L_KIK_STANDING2:
			texture = graphics3;
			current_animation = &lk;
			typeofattack = 1;
			dizzydamage = 1;
			break;

		case M_KIK_STANDING2:
			texture = graphics3;
			current_animation = &mk;
			typeofattack = 2;
			break;

		case F_KIK_STANDING2:
			texture = graphics3;
			current_animation = &hk;
			typeofattack = 3;
			dizzydamage = 3;
			break;

		case L_KIK_NEUTRAL_JUMP2:
			current_animation = &jlk;
			if (App->frames - jump_timer < 24 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}

			if (App->frames - jump_timer < 28 && (App->frames - jump_timer > 23))
			{
				jumpHeight -= speed;
			}

			if (App->frames - jump_timer < 32 && (App->frames - jump_timer > 27))
			{
				jumpHeight += speed;
			}

			if (App->frames - jump_timer > 31 && (App->frames - jump_timer <= D_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}
			typeofattack = 1;
			dizzydamage = 1;
			break;

		case M_KIK_NEUTRAL_JUMP2:
			current_animation = &jmk;
			
			if (App->frames - jump_timer < 24 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}

			if (App->frames - jump_timer < 28 && (App->frames - jump_timer > 23))
			{
				jumpHeight -= speed;
			}

			if (App->frames - jump_timer < 32 && (App->frames - jump_timer > 27))
			{
				jumpHeight += speed;
			}

			if (App->frames - jump_timer > 31 && (App->frames - jump_timer <= D_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}
			typeofattack = 2;
			dizzydamage = 2;
			break;

		case F_KIK_NEUTRAL_JUMP2:
			texture = graphics2;
			current_animation = &jhk;
			
			if (App->frames - jump_timer < 24 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}

			if (App->frames - jump_timer < 28 && (App->frames - jump_timer > 23))
			{
				jumpHeight -= speed;
			}

			if (App->frames - jump_timer < 32 && (App->frames - jump_timer > 27))
			{
				jumpHeight += speed;
			}

			if (App->frames - jump_timer > 31 && (App->frames - jump_timer <= D_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}
			typeofattack = 3;
			dizzydamage = 3;
			break;

		case L_KIK_FORWARD_JUMP2:
			current_animation = &jlk;
			if (App->frames - jump_timer < 24 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}

			if (App->frames - jump_timer < 28 && (App->frames - jump_timer > 23))
			{
				jumpHeight -= speed;
			}

			if (App->frames - jump_timer < 32 && (App->frames - jump_timer > 27))
			{
				jumpHeight += speed;
			}

			if (App->frames - jump_timer > 31 && (App->frames - jump_timer <= D_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}

			if (IsntOnLeftLimit())
				position.x -= 2;
			typeofattack = 1;
			dizzydamage = 1;
			break;

		case M_KIK_FORWARD_JUMP2:
			current_animation = &jmk;
			if (App->frames - jump_timer < 24 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}

			if (App->frames - jump_timer < 28 && (App->frames - jump_timer > 23))
			{
				jumpHeight -= speed;
			}

			if (App->frames - jump_timer < 32 && (App->frames - jump_timer > 27))
			{
				jumpHeight += speed;
			}

			if (App->frames - jump_timer > 31 && (App->frames - jump_timer <= D_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}

			if (IsntOnLeftLimit())
				position.x -= 2;
			typeofattack = 2;
			dizzydamage = 2;
			break;

		case F_KIK_FORWARD_JUMP2:
			texture = graphics2;
			current_animation = &jhk;
			if (App->frames - jump_timer < 24 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}

			if (App->frames - jump_timer < 28 && (App->frames - jump_timer > 23))
			{
				jumpHeight -= speed;
			}

			if (App->frames - jump_timer < 32 && (App->frames - jump_timer > 27))
			{
				jumpHeight += speed;
			}

			if (App->frames - jump_timer > 31 && (App->frames - jump_timer <= D_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}
			typeofattack = 3;
			if (IsntOnLeftLimit())
				position.x -= 2;
			dizzydamage = 3;

			break;

		case L_KIK_BACKWARD_JUMP2:
			current_animation = &jlk;

			if (App->frames - jump_timer < 24 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}

			if (App->frames - jump_timer < 28 && (App->frames - jump_timer > 23))
			{
				jumpHeight -= speed;
			}

			if (App->frames - jump_timer < 32 && (App->frames - jump_timer > 27))
			{
				jumpHeight += speed;
			}

			if (App->frames - jump_timer > 31 && (App->frames - jump_timer <= D_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}

			if (IsntOnRightLimit())
				position.x += 2;
			typeofattack = 1;
			dizzydamage = 1;
			break;

		case M_KIK_BACKWARD_JUMP2:
			current_animation = &jmk;
			if (App->frames - jump_timer < 24 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}

			if (App->frames - jump_timer < 28 && (App->frames - jump_timer > 23))
			{
				jumpHeight -= speed;
			}

			if (App->frames - jump_timer < 32 && (App->frames - jump_timer > 27))
			{
				jumpHeight += speed;
			}

			if (App->frames - jump_timer > 31 && (App->frames - jump_timer <= D_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}

				if (IsntOnRightLimit())
					position.x += 2;
				typeofattack = 2;
				dizzydamage = 2;
			break;

		case F_KIK_BACKWARD_JUMP2:
			texture = graphics2;
			current_animation = &jhk;
			if (App->frames - jump_timer < 24 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}

			if (App->frames - jump_timer < 28 && (App->frames - jump_timer > 23))
			{
				jumpHeight -= speed;
			}

			if (App->frames - jump_timer < 32 && (App->frames - jump_timer > 27))
			{
				jumpHeight += speed;
			}

				if (App->frames - jump_timer > 31 && (App->frames - jump_timer <= D_JUMP_TIME))
				{
					jumpHeight += speed + 3;
				}
				if (IsntOnRightLimit())
					position.x += 2;
				typeofattack = 3;
				dizzydamage = 3;
			break;

		case ST_DEFENDING2:
			current_animation = &defending;
			if (App->frames - defending_timer == 1) {
				App->particles->AddParticle(App->particles->ground_dust, !flip, position.x, position.y - 10, 3, 0, 0, COLLIDER_WALL, 0, 0);
			}

			//Pushback start
			if (pushbacktimerhit != 0) {
				--pushbacktimerhit;
				if (IsntOnLeftLimit() && IsntOnRightLimit())
				{
					if (flip == true) {
						position.x += pushbackspeed;
					}
					else position.x -= pushbackspeed;
				}

				else
				{
					if (flip == true) {
						App->player1->position.x -= pushbackspeed;
					}
					else App->player1->position.x += pushbackspeed;
				}
			}
			break;

		case ST_CROUCH_DEFENDING_READY2:
			current_animation = &crouch;
			break;

		case ST_CROUCH_DEFENDING2:
			current_animation = &cdefending;
			if (App->frames - defending_timer == 1) {
				App->particles->AddParticle(App->particles->ground_dust, !flip, position.x, position.y - 10, 3, 0, 0, COLLIDER_WALL, 0, 0);
			}
			if (pushbacktimerhit != 0) {
				--pushbacktimerhit;
				if (IsntOnLeftLimit() && IsntOnRightLimit())
				{
					if (flip == true) {
						position.x += pushbackspeed;
					}
					else position.x -= pushbackspeed;
				}

				else
				{
					if (flip == true) {
						App->player1->position.x -= pushbackspeed;
					}
					else App->player1->position.x += pushbackspeed;
				}
			}
			break;

		case ST_HEAD_REEL2:
			current_animation = &streel;
			if (App->frames - defending_timer == 1) {
				App->particles->AddParticle(App->particles->ground_dust, !flip, position.x, position.y - 10, 3, 0, 0, COLLIDER_WALL, 0, 0);
			}

			//Pushback start
			if (pushbacktimerhit != 0) {
				--pushbacktimerhit;
				if (IsntOnLeftLimit() && IsntOnRightLimit())
				{
					if (flip == true) {
						position.x += pushbackspeed;
					}
					else position.x -= pushbackspeed;
				}

				else
				{
					if (flip == true) {
						App->player1->position.x -= pushbackspeed;
					}
					else App->player1->position.x += pushbackspeed;
				}
			}
			//Pushback end
			break;

		case ST_GUT_REEL2:
			texture = graphics;
			current_animation = &stgreel;
			if (App->frames - defending_timer == 1) {
				App->particles->AddParticle(App->particles->ground_dust, !flip, position.x, position.y - 10, 3, 0, 0, COLLIDER_WALL, 0, 0);
			}
			break;

		case ST_CROUCH_REEL2:
			current_animation = &creel;
			if (App->frames - defending_timer == 1) {
				App->particles->AddParticle(App->particles->ground_dust, !flip, position.x, position.y - 10, 3, 0, 0, COLLIDER_WALL, 0, 0);
			}
			if (pushbacktimerhit != 0) {
				--pushbacktimerhit;
				if (IsntOnLeftLimit() && IsntOnRightLimit())
				{
					if (flip == true) {
						position.x += pushbackspeed;
					}
					else position.x -= pushbackspeed;
				}

				else
				{
					if (flip == true) {
						App->player1->position.x -= pushbackspeed;
					}
					else App->player1->position.x += pushbackspeed;
				}
			}
			break;

		case L_YFIRE2:
			current_animation = &yoga_fire_lp;
			if (yoga_check == true) {
				timeStoped = App->frames;
				yoga_check = false;
			}
			if (timeUpdated - timeStoped == 5 && !yoga_sound) {
				App->audio->PlayChunk(yoga_snd);
				yoga_sound = true;
			}
			else if (timeUpdated - timeStoped == 25 && !flame_sound) {
				App->audio->PlayChunk(fire_snd);
				flame_sound = true;
			}
			if (timeUpdated - hadoken_timer == 13)
			{
				App->particles->AddParticle(App->particles->hdk, flip, position.x + hdk_spawn, position.y - 70, 3, 0, 0, COLLIDER_PLAYER2_SHOT, 0, 200);
			}
			typeofattack = 1;
			dizzydamage = 4;
			break;

		case M_YFIRE2:
			current_animation = &yoga_fire_mp;
			if (yoga_check == true) {
				timeStoped = App->frames;
				yoga_check = false;
			}
			if (timeUpdated - timeStoped == 5 && !yoga_sound) {
				App->audio->PlayChunk(yoga_snd);
				yoga_sound = true;
			}
			else if (timeUpdated - timeStoped == 25 && !flame_sound) {
				App->audio->PlayChunk(fire_snd);
				flame_sound = true;
			}
			if (timeUpdated - hadoken_timer == 13)
			{
				App->particles->AddParticle(App->particles->hdk, flip, position.x + hdk_spawn, position.y - 70, 3, 0, 0, COLLIDER_PLAYER2_SHOT, 0, 200);
			}
			typeofattack = 1;
			dizzydamage = 4;
			break;

		case F_YFIRE2:
			current_animation = &yoga_fire_mp;
			if (yoga_check == true) {
				timeStoped = App->frames;
				yoga_check = false;
			}
			if (timeUpdated - timeStoped == 5 && !yoga_sound) {
				App->audio->PlayChunk(yoga_snd);
				yoga_sound = true;
			}
			else if (timeUpdated - timeStoped == 25 && !flame_sound) {
				App->audio->PlayChunk(fire_snd);
				flame_sound = true;
			}
			if (timeUpdated - hadoken_timer == 13)
			{
				App->particles->AddParticle(App->particles->hdk, flip, position.x + hdk_spawn, position.y - 70, 3, 0, 0, COLLIDER_PLAYER2_SHOT, 0, 200);
			}
			typeofattack = 1;
			dizzydamage = 4;
			break;

		case L_YFLAME2:
			current_animation = &yoga_flame_lp;
			if (yoga_check == true) {
				timeStoped = App->frames;
				yoga_check = false;
			}
			if (timeUpdated - timeStoped == 5 && !yoga_sound) {
				App->audio->PlayChunk(yoga_snd);
				yoga_sound = true;
			}
			else if (timeUpdated - timeStoped == 35 && !flame_sound) {
				App->audio->PlayChunk(flame_snd);
				flame_sound = true;
			}
			typeofattack = 1;
			dizzydamage = 4;
			break;

		case M_YFLAME2:
			current_animation = &yoga_flame_mp;
			if (yoga_check == true) {
				timeStoped = App->frames;
				yoga_check = false;
			}
			if (timeUpdated - timeStoped == 5 && !yoga_sound) {
				App->audio->PlayChunk(yoga_snd);
				yoga_sound = true;
			}
			else if (timeUpdated - timeStoped == 35 && !flame_sound) {
				App->audio->PlayChunk(flame_snd);
				flame_sound = true;
			}
			typeofattack = 1;
			dizzydamage = 4;
			break;

		case F_YFLAME2:
			current_animation = &yoga_flame_hp;
			if (yoga_check == true) {
				timeStoped = App->frames;
				yoga_check = false;
			}
			if (timeUpdated - timeStoped == 5 && !yoga_sound) {
				App->audio->PlayChunk(yoga_snd);
				yoga_sound = true;
			}
			else if (timeUpdated - timeStoped == 35 && !flame_sound) {
				App->audio->PlayChunk(flame_snd);
				flame_sound = true;
			}
			dizzydamage = 4;
			break;

		case ST_FALLING2:
			current_animation = &airreel;
			jumpHeight += speed + 3;
			if (jumpHeight == 0)
			{
				inputs.Push(IN_FALLING_FINISH2);
			}
			break;

		case YDRILL2:
			current_animation = &yoga_drill;
			typeofattack = 3;
			jumpHeight += speed + 2;
			dizzydamage = 4;
			
			if ((!flip) && (colliding == false)) position.x += speed + 2;
			
			if ((flip) && (colliding == false))  position.x -= speed + 2;

			if (jumpHeight >= 0)
			{
				position.y = 215;
				yoga_drill.ResetAnimation();
				inputs.Push(IN_YDRILL_FINISH2);
			}
			colliding = false;
			break;

		case YMUMMY2:
			
			current_animation = &yoga_mummy;
			jumpHeight += speed + 2;
			typeofattack = 3;
			dizzydamage = 4;

			if ((!flip) && (colliding == false)) position.x += speed + 3;

			if ((flip) && (colliding == false))  position.x -= speed + 3;

			if (jumpHeight >= 0)
			{
				position.y = 215;
				yoga_mummy.ResetAnimation();
				inputs.Push(IN_YMUMMY_FINISH2);
			}
			colliding = false;
			break;

		case ST_GETTING_UP2:
			current_animation = &getup;

			if ((App->frames - getting_up_timer == 1) && (turn == true))
			{
				turn = false;
				flip = !flip;
			}
			break;

		case LOOSE2:
			current_animation = &lose;
			break;


			//HERE IS VICTORY
		case VICTORY2:
			if (victoryExecuted == 1 || (victoryExecuted == 0 && App->frames % 2 == 0)) {
				texture = graphics2;
				current_animation = &win1;
				victoryExecuted = 1;

			}
			else {

				texture = graphics2;
				current_animation = &win2;
				victoryExecuted = 2;

				if (levitationtimer == -100) {
					position.y = 160;
					levitationtimer = 1;
				}
				if (levitationtimer > 1) {
					position.y--;
					levitationtimer--;
				}
				if (levitationtimer < -1) {
					position.y++;
					levitationtimer++;
				}
				if ((levitationtimer == 1) || (levitationtimer == -1)) {
					levitationtimer *= -10;
				}
			}
			break;


			//trying adding cases in order to try out animations

		case L_PUNCH_CLOSE2:
			current_animation = &close_lp;
			typeofattack = 1;
			dizzydamage = 1;
			break;

		case M_PUNCH_CLOSE2:
			current_animation = &close_mp;
			typeofattack = 2;
			dizzydamage = 2;
			break;

		case F_PUNCH_CLOSE2:
			current_animation = &close_hp;
			typeofattack = 3;
			dizzydamage = 3;
			break;

		case L_KIK_CLOSE2:
			if (sprite_change_timer <= 3) {
				texture = graphics3;
				current_animation = &close_firstframe_lk_mk;
				sprite_change_timer++;
			}
			else if (sprite_change_timer > 3 && sprite_change_timer <= 11) {
				texture = graphics;
				current_animation = &close_lk;
				sprite_change_timer++;
			}
			else if (sprite_change_timer > 11 && sprite_change_timer <= D_L_CLOSE_STANDING_KIK_TIME) {
				texture = graphics3;
				current_animation = &close_firstframe_lk_mk;
				if (sprite_change_timer == D_L_CLOSE_STANDING_KIK_TIME) { sprite_change_timer = 0; }
				else { sprite_change_timer++; }
			}
			typeofattack = 1;
			dizzydamage = 1;
			break;

		case M_KIK_CLOSE2:
			if (sprite_change_timer <= 3) {
				texture = graphics3;
				current_animation = &close_firstframe_lk_mk;
				sprite_change_timer++;
			}
			else if (sprite_change_timer > 3 && sprite_change_timer <= 11) {
				texture = graphics;
				current_animation = &close_mk;
				sprite_change_timer++;
			}
			else if (sprite_change_timer > 11 && sprite_change_timer <= D_M_CLOSE_STANDING_KIK_TIME) {
				texture = graphics3;
				current_animation = &close_firstframe_lk_mk;
				if (sprite_change_timer == D_M_CLOSE_STANDING_KIK_TIME) { sprite_change_timer = 0; }
				else { sprite_change_timer++; }
			}
			typeofattack = 2;
			dizzydamage = 2;
			break;



		case F_KIK_CLOSE2:
			current_animation = &close_hk;
			typeofattack = 3;
			dizzydamage = 3;
			break;

		case L_PUNCH_CROUCHCLOSE2:
			current_animation = &close_clp;
			typeofattack = 1;
			dizzydamage = 1;
			break;

		case M_PUNCH_CROUCHCLOSE2:
			current_animation = &close_cmp;
			typeofattack = 2;
			dizzydamage = 2;
			break;

		case L_KIK_CROUCHCLOSE2:
			current_animation = &close_clk;
			typeofattack = 1;
			dizzydamage = 1;
			break;

		case M_KIK_CROUCHCLOSE2:
			current_animation = &close_cmk;
			typeofattack = 2;
			dizzydamage = 2;
			break;

		case M_GRABBING2:
			texture = graphics3;
			current_animation = &grabbing;
			dizzydamage = 5;
			break;

		case F_GRABBING2:
			texture = graphics3;
			current_animation = &grabbing;
			dizzydamage = 5;
			break;

		case M_GRAB2:
			texture = graphics2;
			current_animation = &grab2;
			dizzydamage = 5;
			break;

		case F_GRAB2:
			current_animation = &grab;
			dizzydamage = 5;
			break;

		case GRABBED2:
			current_animation = &airreel;
			break;

		case ST_DIZZI2:
			current_animation = &stun;
			dizzylvl = 0;
			break;

		case TURNING2:
			texture = graphics3;
			current_animation = &turn_anim;
			turn = false;
			break;

		case CROUCH_TURNING2:
			texture = graphics3;
			current_animation = &cturn_anim;
			turn = false;
			break;

		case SWEEP2:
			current_animation = &sweep;
			break;

		//end of test
		}
	}
	current_state = state;

	// Draw everything --------------------------------------	

	BlitCharacterAndAddColliders(current_animation, texture);

	return UPDATE_CONTINUE;
}

void ModulePlayer2::ClearColliders() {
	for (int i = 0; i < MAX_COLLIDERS_PER_FRAME; i++)
	{
		if (colliders[i] != nullptr) {
			colliders[i]->to_delete = true;
			colliders[i] = nullptr;
		}
	}
}

bool ModulePlayer2::IsntOnLeftLimit() {
	return position.x - 42 > -App->render->camera.x / SCREEN_SIZE;
}

bool ModulePlayer2::IsntOnRightLimit() {
	return position.x + 42 < -App->render->camera.x / SCREEN_SIZE + App->render->camera.w;
}

void ModulePlayer2::IsClose() {
	if ((App->player1->position.x - this->position.x <= 90 && App->player1->position.x - this->position.x > 0) || (this->position.x - App->player1->position.x <= 90 && this->position.x - App->player1->position.x > 0))
		close = true;

	else
		close = false;
}

void ModulePlayer2::OnCollision(Collider* c1, Collider* c2) {

	//PUSHBACK CHECK
	if (c1->type == COLLIDER_PLAYER2 && c2->type == COLLIDER_PLAYER_HIT) {
		if (App->player1->typeofattack == 1) { pushbacktimerhit = 10; pushbackspeed = 2; }
		if (App->player1->typeofattack == 2) { pushbacktimerhit = 15; pushbackspeed = 2; }
		if (App->player1->typeofattack == 3) { pushbacktimerhit = 20; pushbackspeed = 2; }
		//		App->particles->AddParticle(App->particles->ground_dust, flip, position.x, position.y, 0, COLLIDER_WALL, 0, 200);
	}

	if (c1->type == COLLIDER_PLAYER2 && c2->type == COLLIDER_PLAYER_SHOT) {
		if (App->player1->typeofattack == 1) { pushbacktimerprojectile = 20; pushbackspeed = 2; }
		if (App->player1->typeofattack == 2) { pushbacktimerprojectile = 15; pushbackspeed = 2; }
		if (App->player1->typeofattack == 3) { pushbacktimerprojectile = 10; pushbackspeed = 2; }
	}


	if (c1->type == COLLIDER_PLAYER2_GRAB && c2->type == COLLIDER_PLAYER)
	{
		inputs.Push(IN_GRAB2);
		App->player1->inputs.Push(IN_GRABBED);
	}

	if (invulnerabilityFrames < App->frames) {

		if (c1->type == COLLIDER_PLAYER2 && c2->type == COLLIDER_PLAYER_HIT) {
			Uint32 lasttimedamagedaux = App->GetFrame();
			if (lasttimedamagedaux - lasttimedamaged > 240) {
				dizzylvl = 0;
			}
			if (App->player1->dizzydamage == 1) { dizzylvl += 1 + (SDL_GetTicks() % 7 - 1); }
			if (App->player1->dizzydamage == 2) { dizzylvl += 5 + (SDL_GetTicks() % 11 - 5); }
			if (App->player1->dizzydamage == 3) { dizzylvl += 11 + (SDL_GetTicks() % 17 - 11); }
			if (App->player1->dizzydamage == 4) { dizzylvl += 13 + (SDL_GetTicks() % 19 - 13); }
			if (App->player1->dizzydamage == 5) { dizzylvl += 7 + (SDL_GetTicks() % 13 - 7); }

			int ponderatedmodifier = (SDL_GetTicks() % 100);
			if ((ponderatedmodifier >= 0) && (ponderatedmodifier < 4)) { dizzylvl -= 3; }
			if ((ponderatedmodifier >= 4) && (ponderatedmodifier < 16)) { dizzylvl -= 2; }
			if ((ponderatedmodifier >= 16) && (ponderatedmodifier < 31)) { dizzylvl -= 1; }
			if ((ponderatedmodifier >= 31) && (ponderatedmodifier < 69)) { dizzylvl += 0; }
			if ((ponderatedmodifier >= 69) && (ponderatedmodifier < 81)) { dizzylvl += +1; }
			if ((ponderatedmodifier >= 81) && (ponderatedmodifier < 93)) { dizzylvl += +2; }
			if ((ponderatedmodifier >= 83) && (ponderatedmodifier < 101)) { dizzylvl += +3; }
			lasttimedamaged = lasttimedamagedaux;
		}

		if (c1->type == COLLIDER_PLAYER2 && c2->type == COLLIDER_PLAYER_SHOT) {
			Uint32 lasttimedamagedaux = App->GetFrame();
			if (lasttimedamagedaux - lasttimedamaged > 240) {
				dizzylvl = 0;
			}
			dizzylvl += 13 + (SDL_GetTicks() % 19);
			int ponderatedmodifier = (SDL_GetTicks() % 100);
			if ((ponderatedmodifier >= 0) && (ponderatedmodifier < 4)) { dizzylvl -= 3; }
			if ((ponderatedmodifier >= 4) && (ponderatedmodifier < 16)) { dizzylvl -= 2; }
			if ((ponderatedmodifier >= 16) && (ponderatedmodifier < 31)) { dizzylvl -= 1; }
			if ((ponderatedmodifier >= 31) && (ponderatedmodifier < 69)) { dizzylvl += 0; }
			if ((ponderatedmodifier >= 69) && (ponderatedmodifier < 81)) { dizzylvl -= +1; }
			if ((ponderatedmodifier >= 81) && (ponderatedmodifier < 93)) { dizzylvl -= +2; }
			if ((ponderatedmodifier >= 83) && (ponderatedmodifier < 101)) { dizzylvl -= +3; }
			lasttimedamaged = lasttimedamagedaux;
		}

		if (dizzylvl >= 32) {
			dizzi = true;
			dizzylvl = 0;
		}

		if (c1->type == COLLIDER_PLAYER2 && c2->type == COLLIDER_PLAYER_SHOT && (state != ST_JUMP_NEUTRAL2 && state != ST_JUMP_FORWARD2 && state != ST_JUMP_BACKWARD2 && state != L_PUNCH_NEUTRAL_JUMP2 && state != L_PUNCH_FORWARD_JUMP2 && state != L_PUNCH_BACKWARD_JUMP2 && state != L_KIK_NEUTRAL_JUMP2 && state != L_KIK_FORWARD_JUMP2 && state != L_KIK_BACKWARD_JUMP2))
		{


			invulnerabilityFrames = 25 + App->frames;
			App->audio->PlayChunk(hdk_hit);

			if ((state == ST_WALK_BACKWARD2 && flip == true) || (state == ST_WALK_FORWARD && flip == false))
			{
				App->audio->PlayChunk(block);
				inputs.Push(IN_DEFENDING2);
				life -= 5;
			}

			else if (state == ST_CROUCHING2 || state == ST_CROUCH2 || state == ST_STANDING2 || state == L_PUNCH_CROUCH2 || state == L_KIK_CROUCH2)
			{
				inputs.Push(IN_CROUCH_REEL2);
				life -= 12;
			}

			else
			{
				inputs.Push(IN_HEAD_REEL2);
				life -= 12;
			}
		}

		if (c1->type == COLLIDER_PLAYER2 && c2->type == COLLIDER_PLAYER_HIT && (state != ST_JUMP_NEUTRAL && state != ST_JUMP_FORWARD && state != ST_JUMP_BACKWARD &&
			state != L_PUNCH_NEUTRAL_JUMP && state != L_PUNCH_FORWARD_JUMP && state != L_PUNCH_BACKWARD_JUMP && state != L_KIK_NEUTRAL_JUMP && state != L_KIK_FORWARD_JUMP && state != L_KIK_BACKWARD_JUMP))
		{
			life -= 7;
			invulnerabilityFrames = 25 + App->frames;

			if (App->player1->state == L_KIK_STANDING2 || App->player1->state == L_KIK_NEUTRAL_JUMP2 || App->player1->state == L_KIK_FORWARD_JUMP2 || App->player1->state == L_KIK_BACKWARD_JUMP2 || App->player1->state == M_KIK_STANDING2 || App->player1->state == M_KIK_NEUTRAL_JUMP2 || App->player1->state == M_KIK_FORWARD_JUMP2 || App->player1->state == M_KIK_BACKWARD_JUMP2
				|| App->player1->state == F_KIK_STANDING2 || App->player1->state == F_KIK_NEUTRAL_JUMP2 || App->player1->state == F_KIK_FORWARD_JUMP2 || App->player1->state == F_KIK_BACKWARD_JUMP2 || App->player1->state == L_KIK_CLOSE2 || App->player1->state == M_KIK_CLOSE2 || App->player1->state == F_KIK_CLOSE2 || App->player2->state == YMUMMY2)
				App->audio->PlayChunk(high_kick);
			else if (App->player1->state == L_KIK_CROUCH2 || App->player1->state == M_KIK_CROUCH2 || App->player1->state == F_KIK_CROUCH2 || App->player1->state == L_KIK_CROUCHCLOSE2 || App->player1->state == M_KIK_CROUCHCLOSE2)
				App->audio->PlayChunk(low_kick);
			else if (App->player1->state == L_PUNCH_STANDING2 || App->player1->state == L_PUNCH_NEUTRAL_JUMP2 || App->player1->state == L_PUNCH_FORWARD_JUMP2 || App->player1->state == L_PUNCH_BACKWARD_JUMP2 || App->player1->state == M_PUNCH_STANDING2 || App->player1->state == M_PUNCH_NEUTRAL_JUMP2 || App->player1->state == M_PUNCH_FORWARD_JUMP2 || App->player1->state == M_PUNCH_BACKWARD_JUMP2
				|| App->player1->state == F_PUNCH_STANDING2 || App->player1->state == F_PUNCH_NEUTRAL_JUMP2 || App->player1->state == F_PUNCH_FORWARD_JUMP2 || App->player1->state == F_PUNCH_BACKWARD_JUMP2 || App->player1->state == L_PUNCH_CLOSE2 || App->player1->state == M_PUNCH_CLOSE2 || App->player1->state == F_PUNCH_CLOSE2 || App->player2->state == YDRILL2)
				App->audio->PlayChunk(high_fist);
			else if (App->player1->state == L_PUNCH_CROUCH2 || App->player1->state == M_PUNCH_CROUCH2 || App->player1->state == F_PUNCH_CROUCH2 || App->player1->state == L_PUNCH_CROUCHCLOSE2 || App->player1->state == M_PUNCH_CROUCHCLOSE2) {
				App->audio->PlayChunk(low_fist);
			}

			if ((state == ST_WALK_BACKWARD2 && flip == true) || (state == ST_WALK_FORWARD && flip == false) || (state == ST_CROUCH_DEFENDING_READY2))
			{
				App->audio->PlayChunk(block);
				inputs.Push(IN_DEFENDING2);
			}

			

			else if (state == ST_CROUCHING2 || state == ST_CROUCH2 || state == ST_STANDING2 || state == L_PUNCH_CROUCH2 || state == L_KIK_CROUCH2)
			{
				inputs.Push(IN_CROUCH_REEL2);
				life -= 7;
			}

			else
			{
				inputs.Push(IN_HEAD_REEL2);
				life -= 7;
			}
		}

		if (c1->type == COLLIDER_PLAYER2 && c2->type == COLLIDER_PLAYER_SHOT && (state == ST_JUMP_NEUTRAL2 || state == ST_JUMP_FORWARD2 || state == ST_JUMP_BACKWARD2 || state == L_PUNCH_NEUTRAL_JUMP2 || state == L_PUNCH_FORWARD_JUMP2 || state == L_PUNCH_BACKWARD_JUMP2 || state == L_KIK_NEUTRAL_JUMP2 || state == L_KIK_FORWARD_JUMP2 || state == L_KIK_BACKWARD_JUMP2))
		{
			life -= 12;
			invulnerabilityFrames = 25 + App->frames;
			App->audio->PlayChunk(hdk_hit);
			inputs.Push(IN_FALLING2);
		}

		if (c1->type == COLLIDER_PLAYER2 && c2->type == COLLIDER_PLAYER_HIT && (state == ST_JUMP_NEUTRAL || state == ST_JUMP_FORWARD || state == ST_JUMP_BACKWARD || state == L_PUNCH_NEUTRAL_JUMP || state == L_PUNCH_FORWARD_JUMP || state == L_PUNCH_BACKWARD_JUMP || state == L_KIK_NEUTRAL_JUMP || state == L_KIK_FORWARD_JUMP || state == L_KIK_BACKWARD_JUMP))
		{
			life -= 7;
			invulnerabilityFrames = 25 + App->frames;

			if (App->player1->state == L_KIK_STANDING2 || App->player1->state == L_KIK_NEUTRAL_JUMP2 || App->player1->state == L_KIK_FORWARD_JUMP2 || App->player1->state == L_KIK_BACKWARD_JUMP2 || App->player1->state == M_KIK_STANDING2 || App->player1->state == M_KIK_NEUTRAL_JUMP2 || App->player1->state == M_KIK_FORWARD_JUMP2 || App->player1->state == M_KIK_BACKWARD_JUMP2
				|| App->player1->state == F_KIK_STANDING2 || App->player1->state == F_KIK_NEUTRAL_JUMP2 || App->player1->state == F_KIK_FORWARD_JUMP2 || App->player1->state == F_KIK_BACKWARD_JUMP2 || App->player1->state == L_KIK_CLOSE2 || App->player1->state == M_KIK_CLOSE2 || App->player1->state == F_KIK_CLOSE2)
				App->audio->PlayChunk(high_kick);
			else if (App->player1->state == L_KIK_CROUCH2 || App->player1->state == M_KIK_CROUCH2 || App->player1->state == F_KIK_CROUCH2 || App->player1->state == L_KIK_CROUCHCLOSE2 || App->player1->state == M_KIK_CROUCHCLOSE2)
				App->audio->PlayChunk(low_kick);
			else if (App->player1->state == L_PUNCH_STANDING2 || App->player1->state == L_PUNCH_NEUTRAL_JUMP2 || App->player1->state == L_PUNCH_FORWARD_JUMP2 || App->player1->state == L_PUNCH_BACKWARD_JUMP2 || App->player1->state == M_PUNCH_STANDING2 || App->player1->state == M_PUNCH_NEUTRAL_JUMP2 || App->player1->state == M_PUNCH_FORWARD_JUMP2 || App->player1->state == M_PUNCH_BACKWARD_JUMP2
				|| App->player1->state == F_PUNCH_STANDING2 || App->player1->state == F_PUNCH_NEUTRAL_JUMP2 || App->player1->state == F_PUNCH_FORWARD_JUMP2 || App->player1->state == F_PUNCH_BACKWARD_JUMP2 || App->player1->state == L_PUNCH_CLOSE2 || App->player1->state == M_PUNCH_CLOSE2 || App->player1->state == F_PUNCH_CLOSE2)
				App->audio->PlayChunk(high_fist);
			else if (App->player1->state == L_PUNCH_CROUCH2 || App->player1->state == M_PUNCH_CROUCH2 || App->player1->state == F_PUNCH_CROUCH2 || App->player1->state == L_PUNCH_CROUCHCLOSE2 || App->player1->state == M_PUNCH_CROUCHCLOSE2)
				App->audio->PlayChunk(low_fist);

			inputs.Push(IN_FALLING2);
		}
	}
}



void ModulePlayer2::BlitCharacterAndAddColliders(Animation* current_animation, SDL_Texture *texture) {
	Frame frame = current_animation->GetCurrentFrame();
	SDL_Rect r;
	SDL_Rect r2;
	int hitboxesQnt = frame.GetColliderQnt();

	for (int i = 0; i < hitboxesQnt; i++)
	{
		r = frame.hitBoxeRects[i];
		if (flip)
			colliders[i] = App->collisions->AddCollider({ position.x - (r.w - frame.pivotPosition.x) + r.x, position.y - r.h + frame.pivotPosition.y + jumpHeight - r.y ,r.w, r.h }, frame.types[i], frame.callbacks[i]);
		else
			colliders[i] = App->collisions->AddCollider({ position.x - frame.pivotPosition.x - r.x, position.y - r.h + frame.pivotPosition.y + jumpHeight - r.y ,r.w, r.h }, frame.types[i], frame.callbacks[i]);
	}

	r = frame.frame;
	r2.x = 4;
	r2.y = 355;
	r2.w = 69;
	r2.h = 11;
	if (jump_timer == 0)
		App->render->Blit(shadow, position.x - (idle.frames->pivotPosition.x), 210, &r2, flip);
	else
		r2.y = 367; App->render->Blit(shadow, position.x - (idle.frames->pivotPosition.x), 210, &r2, flip);

	if (flip)
		App->render->Blit(texture, position.x - (r.w - frame.pivotPosition.x), position.y - r.h + frame.pivotPosition.y + jumpHeight, &r, flip);
	else
		App->render->Blit(texture, position.x - frame.pivotPosition.x, position.y - r.h + frame.pivotPosition.y + jumpHeight, &r, flip);

}

bool ModulePlayer2::external_input(p2Qeue<ryu_inputs2>& inputs)
{
	bool crouch = false;

	if (!freeze) {
		//Key UP
		/*
		if (App->input->keyboard[SDL_SCANCODE_H] == KEY_STATE::KEY_UP)
		{
			inputs.Push(IN_CROUCH_UP2);
			down = false;
		}

		if (App->input->keyboard[SDL_SCANCODE_Y] == KEY_STATE::KEY_UP)
		{
			up = false;
		}

		if (App->input->keyboard[SDL_SCANCODE_G] == KEY_STATE::KEY_UP)
		{
			inputs.Push(IN_RIGHT_UP2);
			right = false;
		}

		if (App->input->keyboard[SDL_SCANCODE_J] == KEY_STATE::KEY_UP)
		{
			inputs.Push(IN_LEFT_UP2);
			left = false;

		}*/
		//Key down
		// Using B as debug tool



		if (App->input->keyboard[SDL_SCANCODE_B] == KEY_STATE::KEY_DOWN)
		{
			inputs.Push(IN_L_PUNCH2);
		}

		if (App->input->pads[1].x == true)
		{
			if (App->input->CheckYogaFlame(250, 0, flip) == true)
			{
				inputs.Push(IN_L_YFLAME2);
			}

			else if (App->input->CheckYogaFire(200, 0, flip) == true)
			{
				inputs.Push(IN_L_YFIRE2);
			}

			else
				inputs.Push(IN_L_PUNCH2);

		}

		if (App->input->pads[1].a == true)
		{
			inputs.Push(IN_L_KIK2);
		}

		if (App->input->pads[1].y == true)
		{

			if (App->input->CheckYogaFlame(250, 0, flip) == true)
			{
				inputs.Push(IN_M_YFLAME2);
			}

			else if (App->input->CheckYogaFire(200, 0, flip) == true)
			{
				inputs.Push(IN_M_YFIRE2);
			}

			else
				inputs.Push(IN_M_PUNCH2);
		}
		
		if (App->input->pads[1].b == true)
		{
			inputs.Push(IN_M_KIK2);
		}

		if (App->input->pads[1].R1 == true)
		{
			if (App->input->CheckYogaFlame(250, 0, flip) == true)
			{
				inputs.Push(IN_F_YFLAME2);
			}

			else if (App->input->CheckYogaFire(200, 0, flip) == true)
			{
				inputs.Push(IN_F_YFIRE2);
			}

			else
				inputs.Push(IN_F_PUNCH2);
		}

		if (App->input->pads[1].R2 == true)
		{
			inputs.Push(IN_F_KIK2);
		}

		if (App->input->pads[1].right)
		{
			if (App->input->pads[1].up)
				inputs.Push(IN_LEFT_AND_JUMP2);

			if (App->input->pads[1].down) {
				inputs.Push(IN_LEFT_AND_CROUCH2);
				crouch = true;
			}

			else
				inputs.Push(IN_LEFT_DOWN2);
		}
		else
			inputs.Push(IN_LEFT_UP2);


		if (App->input->pads[1].left)
		{
			if (App->input->pads[1].up)
				inputs.Push(IN_RIGHT_AND_JUMP2);

			if (App->input->pads[1].down) {
				inputs.Push(IN_RIGHT_AND_CROUCH2);
				crouch = true;
			}

			else
				inputs.Push(IN_RIGHT_DOWN2);
		}
		else
			inputs.Push(IN_RIGHT_UP2);


		if (App->input->pads[1].up && !App->input->pads[1].right && !App->input->pads[1].left)
		{
			inputs.Push(IN_JUMP2);
		}

		if (App->input->pads[1].down && !App->input->pads[1].right && !App->input->pads[1].left)
		{
			inputs.Push(IN_CROUCH_DOWN2);
			crouch = true;
		}

		if (crouch == false)
		{
			inputs.Push(IN_CROUCH_UP2);
		}

		if (!App->input->pads[1].down && !App->input->pads[1].up && !App->input->pads[1].right && !App->input->pads[1].left)
		{
			inputs.Push(IN_IDLE2);
		}
	}
	else {
		App->input->pads[1].left = false;
		App->input->pads[1].right = false;
		App->input->pads[1].down = false;
		App->input->pads[1].up = false;
	}

	return true;
}

void ModulePlayer2::internal_input(p2Qeue<ryu_inputs2>& inputs)
{
	if (jump_timer > 0)
	{
		if (App->frames - jump_timer > D_JUMP_TIME)
		{
			inputs.Push(IN_JUMP_FINISH2);
			jump_timer = 0;
			backwardJump.ResetAnimation();
			forwardJump.ResetAnimation();
			neutralJump.ResetAnimation();
			jlp.ResetAnimation();
			jmp.ResetAnimation();
			jhp.ResetAnimation();
			jlk.ResetAnimation();
			jmk.ResetAnimation();
			jhk.ResetAnimation();			
			
			airreel.ResetAnimation();
			sweep.ResetAnimation();

			win1.ResetAnimation();
			win2.ResetAnimation();
			lose.ResetAnimation();
			ground.ResetAnimation();
		}
	}

	if (grabbing_timer > 0)
	{
		if (App->frames - grabbing_timer > D_GRABBING_TIME)
		{
			inputs.Push(IN_GRABBING_FINISH2);
			grabbing_timer = 0;

		}
	}

	if (m_grab_timer > 0)
	{
		if (App->frames - m_grab_timer > D_M_GRAB_TIME)
		{
			inputs.Push(IN_M_GRAB_FINISH2);
			grab.ResetAnimation();
			grab2.ResetAnimation();
			m_grab_timer = 0;
		}
	}

	if (f_grab_timer > 0)
	{
		if (App->frames - f_grab_timer > D_F_GRAB_TIME)
		{
			inputs.Push(IN_F_GRAB_FINISH2);
			grab.ResetAnimation();
			grab2.ResetAnimation();
			f_grab_timer = 0;
		}
	}

	if (grabbed_timer > 0)
	{
		if (App->frames - grabbed_timer > D_GRABBED_TIME)
		{
			inputs.Push(IN_GRABBED_FINISH2);
			grabbed_timer = 0;
		}
	}

	//light
	if (l_standing_punch_timer > 0)
	{
		if (App->frames - l_standing_punch_timer > D_L_STANDING_PUNCH_TIME)
		{
			inputs.Push(IN_PUNCH_FINISH2);
			lp.ResetAnimation();
			l_standing_punch_timer = 0;
		}
	}

	if (l_close_standing_punch_timer > 0)
	{
		if (App->frames - l_close_standing_punch_timer > D_L_CLOSE_STANDING_PUNCH_TIME)
		{
			inputs.Push(IN_PUNCH_FINISH2);
			close_lp.ResetAnimation();
			l_close_standing_punch_timer = 0;

		}
	}

	if (l_crouching_punch_timer > 0)
	{
		if (App->frames - l_crouching_punch_timer > D_L_CROUCHING_PUNCH_TIME)
		{
			inputs.Push(IN_PUNCH_FINISH2);
			clp.ResetAnimation();
			l_crouching_punch_timer = 0;

		}
	}

	if (l_close_crouching_punch_timer > 0)
	{
		if (App->frames - l_close_crouching_punch_timer > D_L_CLOSE_CROUCHING_PUNCH_TIME)
		{
			inputs.Push(IN_PUNCH_FINISH2);
			close_clp.ResetAnimation();
			l_close_crouching_punch_timer = 0;

		}
	}

	if (l_d_jumping_punch_timer > 0)
	{
		if (App->frames - l_d_jumping_punch_timer > D_L_D_JUMPING_PUNCH_TIME)
		{
			inputs.Push(IN_PUNCH_FINISH2);
			l_d_jumping_punch_timer = 0;
		}
	}

	if (l_standing_kik_timer > 0)
	{
		if (App->frames - l_standing_kik_timer > D_L_STANDING_KIK_TIME)
		{
			inputs.Push(IN_KIK_FINISH2);
			lk.ResetAnimation();
			l_standing_kik_timer = 0;
		}
	}

	if (l_close_standing_kik_timer > 0)
	{
		if (App->frames - l_close_standing_kik_timer > D_L_CLOSE_STANDING_KIK_TIME)
		{
			inputs.Push(IN_KIK_FINISH2);
			close_lk.ResetAnimation();
			l_close_standing_kik_timer = 0;
		}
	}

	if (l_crouching_kik_timer > 0)
	{
		if (App->frames - l_crouching_kik_timer > D_L_CROUCHING_KIK_TIME)
		{
			inputs.Push(IN_KIK_FINISH2);
			clk.ResetAnimation();
			l_crouching_kik_timer = 0;
		}
		
	}

	if (l_close_crouching_kik_timer > 0)
	{
		if (App->frames - l_close_crouching_kik_timer > D_L_CLOSE_CROUCHING_KIK_TIME)
		{
			inputs.Push(IN_KIK_FINISH2);
			close_clk.ResetAnimation();
			l_close_crouching_kik_timer = 0;
		}
		
	}


	//medium
	if (m_standing_punch_timer > 0)
	{
		if (App->frames - m_standing_punch_timer > D_M_STANDING_PUNCH_TIME)
		{
			inputs.Push(IN_PUNCH_FINISH2);
			mp.ResetAnimation();
			m_standing_punch_timer = 0;
		}
	}

	if (m_close_standing_punch_timer > 0)
	{
		if (App->frames - m_close_standing_punch_timer > D_M_CLOSE_STANDING_PUNCH_TIME)
		{
			inputs.Push(IN_PUNCH_FINISH2);
			close_mp.ResetAnimation();
			m_close_standing_punch_timer = 0;
		}
	}

	if (m_crouching_punch_timer > 0)
	{
		if (App->frames - m_crouching_punch_timer > D_M_CROUCHING_PUNCH_TIME)
		{
			inputs.Push(IN_PUNCH_FINISH2);
			cmp.ResetAnimation();
			m_crouching_punch_timer = 0;
		}
	}

	if (m_close_crouching_punch_timer > 0)
	{
		if (App->frames - m_close_crouching_punch_timer > D_M_CLOSE_CROUCHING_PUNCH_TIME)
		{
			inputs.Push(IN_PUNCH_FINISH2);
			close_cmp.ResetAnimation();
			m_close_crouching_punch_timer = 0;
		}
	}

	if (m_d_jumping_punch_timer > 0)
	{
		if (App->frames - m_d_jumping_punch_timer > D_M_D_JUMPING_PUNCH_TIME)
		{
			inputs.Push(IN_PUNCH_FINISH2);
			m_d_jumping_punch_timer = 0;
		}
	}

	if (m_standing_kik_timer > 0)
	{
		if (App->frames - m_standing_kik_timer > D_M_STANDING_KIK_TIME)
		{
			inputs.Push(IN_KIK_FINISH2);
			mk.ResetAnimation();
			m_standing_kik_timer = 0;
		}
	}

	if (m_close_standing_kik_timer > 0)
	{
		if (App->frames - m_close_standing_kik_timer > D_M_CLOSE_STANDING_KIK_TIME)
		{
			inputs.Push(IN_KIK_FINISH2);
			close_mk.ResetAnimation();
			m_close_standing_kik_timer = 0;
		}
	}

	if (m_crouching_kik_timer > 0)
	{
		if (App->frames - m_crouching_kik_timer > D_M_CROUCHING_KIK_TIME)
		{
			inputs.Push(IN_KIK_FINISH2);
			cmk.ResetAnimation();
			m_crouching_kik_timer = 0;
		}
		
	}

	if (m_close_crouching_kik_timer > 0)
	{
		if (App->frames - m_close_crouching_kik_timer > D_M_CLOSE_CROUCHING_KIK_TIME)
		{
			inputs.Push(IN_KIK_FINISH2);
			close_cmk.ResetAnimation();
			m_close_crouching_kik_timer = 0;
		}
		
	}

	//fierce
	if (f_standing_punch_timer > 0)
	{
		if (App->frames - f_standing_punch_timer > D_F_STANDING_PUNCH_TIME)
		{
			inputs.Push(IN_PUNCH_FINISH2);
			hp.ResetAnimation();
			f_standing_punch_timer = 0;
		}
	}

	if (f_close_standing_punch_timer > 0)
	{
		if (App->frames - f_close_standing_punch_timer > D_F_CLOSE_STANDING_PUNCH_TIME)
		{
			inputs.Push(IN_PUNCH_FINISH2);
			close_hp.ResetAnimation();
			f_close_standing_punch_timer = 0;
		}
	}

	if (f_crouching_punch_timer > 0)
	{
		if (App->frames - f_crouching_punch_timer > D_F_CROUCHING_PUNCH_TIME)
		{
			inputs.Push(IN_PUNCH_FINISH2);
			f_crouching_punch_timer = 0;
		}
	}

	if (f_d_jumping_punch_timer > 0)
	{
		if (App->frames - f_d_jumping_punch_timer > D_F_D_JUMPING_PUNCH_TIME)
		{
			inputs.Push(IN_PUNCH_FINISH2);
			jhp.ResetAnimation();
			f_d_jumping_punch_timer = 0;
		}
	}

	if (f_standing_kik_timer > 0)
	{
		if (App->frames - f_standing_kik_timer > D_F_STANDING_KIK_TIME)
		{
			inputs.Push(IN_KIK_FINISH2);
			hk.ResetAnimation();
			f_standing_kik_timer = 0;
		}
	}

	if (f_close_standing_kik_timer > 0)
	{
		if (App->frames - f_close_standing_kik_timer > D_F_CLOSE_STANDING_KIK_TIME)
		{
			inputs.Push(IN_KIK_FINISH2);
			close_hk.ResetAnimation();
			f_close_standing_kik_timer = 0;
		}
	}

	if (f_crouching_kik_timer > 0)
	{
		if (App->frames - f_crouching_kik_timer > D_F_CROUCHING_KIK_TIME)
		{
			inputs.Push(IN_KIK_FINISH2);
			chk.ResetAnimation();
			f_crouching_kik_timer = 0;
		}
		
	}

	if (hadoken_timer > 0)
	{
		if (App->frames - hadoken_timer > D_HADOKEN_TIME)
		{
			inputs.Push(IN_YFIRE_FINISH2);
			yoga_fire_lp.ResetAnimation();
			yoga_fire_mp.ResetAnimation();
			yoga_fire_hp.ResetAnimation();
			hadoken_timer = 0;
		}
	}

	if (l_yflame_timer > 0)
	{
		if (App->frames - l_yflame_timer > D_LP_YOGA_FLAME) // cambiar
		{
			inputs.Push(IN_YFLAME_FINISH2);
			yoga_flame_lp.ResetAnimation();
			l_yflame_timer = 0;
		}
	}

	if (m_yflame_timer > 0)
	{
		if (App->frames - m_yflame_timer > D_MP_YOGA_FLAME) // cambiar
		{
			inputs.Push(IN_YFLAME_FINISH2);
			yoga_flame_mp.ResetAnimation();
			m_yflame_timer = 0;
		}
	}

	if (f_yflame_timer > 0)
	{
		if (App->frames - f_yflame_timer > D_HP_YOGA_FLAME)
		{
			inputs.Push(IN_YFLAME_FINISH2);
			yoga_flame_hp.ResetAnimation();
			f_yflame_timer = 0;
		}
	}

	if (crouching_timer > 0)
	{
		if (App->frames - crouching_timer > D_CROUCHING_TIME)
		{
			inputs.Push(IN_CROUCHING_FINISH2);
			crouching.ResetAnimation();
			crouching_timer = 0;
		}
	}

	if (standing_timer > 0)
	{
		if (App->frames - standing_timer > D_STANDING_TIME)
		{
			inputs.Push(IN_STANDING_FINISH2);
			standing.ResetAnimation();
			standing_timer = 0;
		}
	}

	if (head_reel_timer > 0)
	{
		if (App->frames - head_reel_timer > D_HEAD_REEL_TIME)
		{
			inputs.Push(IN_REEL_FINISH2);
			streel.ResetAnimation();
			head_reel_timer = 0;
		}
	}

	if (gut_reel_timer > 0)
	{
		if (App->frames - gut_reel_timer > D_GUT_REEL_TIME)
		{
			inputs.Push(IN_REEL_FINISH2);
			stgreel.ResetAnimation();
			gut_reel_timer = 0;
		}
	}

	if (crouch_reel_timer > 0)
	{
		if (App->frames - crouch_reel_timer > D_CROUCH_REEL_TIME)
		{
			inputs.Push(IN_REEL_FINISH2);
			creel.ResetAnimation();
			crouch_reel_timer = 0;
		}
	}

	if (getting_up_timer > 0)
	{
		if (App->frames - getting_up_timer > D_GETTING_UP_TIME)
		{
			inputs.Push(IN_GETTING_UP_FINISH2);
			getup.ResetAnimation();
			getting_up_timer = 0;
		}
	}

	if (defending_timer > 0)
	{
		if (App->frames - defending_timer > D_DEFENDING_TIME)
		{
			inputs.Push(IN_DEFENDING_FINISH2);
			defending.ResetAnimation();
			defending_timer = 0;
		}
	}

	if (crouch_defending_timer > 0)
	{
		if (App->frames - crouch_defending_timer > D_DEFENDING_TIME)
		{
			inputs.Push(IN_DEFENDING_FINISH2);
			cdefending.ResetAnimation();
			crouch_defending_timer = 0;
		}
	}

	if (dizzi_timer > 0)
	{
		if (App->frames - dizzi_timer > D_DIZZI_TIME)
		{
			inputs.Push(IN_DIZZI_FINISH2);
			stun.ResetAnimation();
			dizzi_timer = 0;
		}
	}

	if (turning_timer > 0)
	{
		if (App->frames - turning_timer > D_TURNING_TIME)
		{
			inputs.Push(IN_TURNING_FINISH2);
			turning_timer = 0;

			flip = !flip;
		}
	}

	if (sweep_timer > 0)
	{
		if (App->frames - sweep_timer > D_SWEEP_TIME)
		{
			inputs.Push(IN_SWEEP_FINISH2);
			sweep_timer = 0;
			sweep.ResetAnimation();
		}
	}
}

ryu_states2 ModulePlayer2::process_fsm(p2Qeue<ryu_inputs2>& inputs)
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

			case IN_TURNING2: state = TURNING2; turning_timer = App->frames;  break;

			case IN_JUMP2: state = ST_JUMP_NEUTRAL2; jump_timer = App->frames;  break;
			case IN_LEFT_AND_JUMP2: state = ST_JUMP_BACKWARD2; jump_timer = App->frames; break;
			case IN_RIGHT_AND_JUMP2: state = ST_JUMP_FORWARD2; jump_timer = App->frames; break;

			case IN_CROUCH_DOWN2: state = ST_CROUCHING2; crouching_timer = App->frames; break;
			case IN_LEFT_AND_CROUCH2: state = ST_CROUCHING2; crouching_timer = App->frames; break;
			case IN_RIGHT_AND_CROUCH2: state = ST_CROUCHING2; crouching_timer = App->frames; break;

			case IN_L_PUNCH2: {
				if (!close) { state = L_PUNCH_STANDING2; l_standing_punch_timer = App->frames; }
				else if (close) { state = L_PUNCH_CLOSE2; l_close_standing_punch_timer = App->frames; }
			} break;

			case IN_L_KIK2: {
				if (!close) { state = L_KIK_STANDING2; l_standing_kik_timer = App->frames; }
				else if (close) { state = L_KIK_CLOSE2; l_close_standing_kik_timer = App->frames; }
			}break;

			case IN_M_PUNCH2: {
				if (!close) { state = M_PUNCH_STANDING2; m_standing_punch_timer = App->frames; }
				else if (close) { state = M_PUNCH_CLOSE2; m_close_standing_punch_timer = App->frames; }
			}break;

			case IN_M_KIK2: {
				if (!close) { state = M_KIK_STANDING2; m_standing_kik_timer = App->frames; }
				else if (close) { state = M_KIK_CLOSE2; m_close_standing_kik_timer = App->frames; }
			}break;

			case IN_F_PUNCH2: {
				if (!close) { state = F_PUNCH_STANDING2; f_standing_punch_timer = App->frames; }
				else if (close) { state = F_PUNCH_CLOSE2; f_close_standing_punch_timer = App->frames; }
			}break;

			case IN_F_KIK2: {
				if (!close) { state = F_KIK_STANDING2; f_standing_kik_timer = App->frames; }
				else if (close) { state = F_KIK_CLOSE2; f_close_standing_kik_timer = App->frames; }
			}break;

			case IN_L_YFIRE2: state = L_YFIRE2; hadoken_timer = App->frames; break;
			case IN_M_YFIRE2: state = M_YFIRE2; hadoken_timer = App->frames; break;
			case IN_F_YFIRE2: state = F_YFIRE2; hadoken_timer = App->frames; break;

			case IN_L_YFLAME2: state = L_YFLAME2; l_yflame_timer = App->frames; break;
			case IN_M_YFLAME2: state = M_YFLAME2; m_yflame_timer = App->frames; break;
			case IN_F_YFLAME2: state = F_YFLAME2; f_yflame_timer = App->frames; break;

			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;

			case IN_VICTORY2: state = VICTORY2; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case ST_WALK_FORWARD2:
		{
			switch (last_input)
			{
			case IN_RIGHT_UP2: state = ST_IDLE2; break;
			case IN_IDLE2: state = ST_IDLE2; break;

			case IN_TURNING2: state = TURNING2; turning_timer = App->frames;  break;

			case IN_L_YFIRE2: state = L_YFIRE2; hadoken_timer = App->frames; break;
			case IN_M_YFIRE2: state = M_YFIRE2; hadoken_timer = App->frames; break;
			case IN_F_YFIRE2: state = F_YFIRE2; hadoken_timer = App->frames; break;

			case IN_L_YFLAME2: state = L_YFLAME2; l_yflame_timer = App->frames; break;
			case IN_M_YFLAME2: state = M_YFLAME2; m_yflame_timer = App->frames; break;
			case IN_F_YFLAME2: state = F_YFLAME2; f_yflame_timer = App->frames; break;

			case IN_RIGHT_AND_JUMP2: state = ST_JUMP_FORWARD2; jump_timer = App->frames;  break;
			case IN_RIGHT_AND_CROUCH2: state = ST_CROUCHING2; crouching_timer = App->frames; break;

			case IN_L_PUNCH2: {
				if (!close) { state = L_PUNCH_STANDING2; l_standing_punch_timer = App->frames; }
				else if (close) { state = L_PUNCH_CLOSE2; l_close_standing_punch_timer = App->frames; }
			} break;

			case IN_L_KIK2: {
				if (!close) { state = L_KIK_STANDING2; l_standing_kik_timer = App->frames; }
				else if (close) { state = L_KIK_CLOSE2; l_close_standing_kik_timer = App->frames; }
			}break;

			case IN_M_PUNCH2: {
				if (!close) { state = M_PUNCH_STANDING2; m_standing_punch_timer = App->frames; }
				else if (close) { state = M_GRABBING2; grabbing_timer = App->frames; }
			}break;

			case IN_M_KIK2: {
				if (!close) { state = M_KIK_STANDING2; m_standing_kik_timer = App->frames; }
				else if (close) { state = M_KIK_CLOSE2; m_close_standing_kik_timer = App->frames; }
			}break;

			case IN_F_PUNCH2: {
				if (!close) { state = F_PUNCH_STANDING2; f_standing_punch_timer = App->frames; }
				else if (close) { state = F_GRABBING2; grabbing_timer = App->frames; }
			}break;

			case IN_F_KIK2: {
				if (!close) { state = F_KIK_STANDING2; f_standing_kik_timer = App->frames; }
				else if (close) { state = F_KIK_CLOSE2; f_close_standing_kik_timer = App->frames; }
			}break;

			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;

			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_DEFENDING2: state = ST_DEFENDING2; defending_timer = App->frames; break;

			case IN_VICTORY2: state = VICTORY2; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case ST_WALK_BACKWARD2:
		{
			switch (last_input)
			{
			case IN_LEFT_UP2: state = ST_IDLE2; break;
			case IN_IDLE2: state = ST_IDLE2; break;

			case IN_TURNING2: state = TURNING2; turning_timer = App->frames;  break;

			case IN_L_YFIRE2: state = L_YFIRE2; hadoken_timer = App->frames; break;
			case IN_M_YFIRE2: state = M_YFIRE2; hadoken_timer = App->frames; break;
			case IN_F_YFIRE2: state = F_YFIRE2; hadoken_timer = App->frames; break;

			case IN_L_YFLAME2: state = L_YFLAME2; l_yflame_timer = App->frames; break;
			case IN_M_YFLAME2: state = M_YFLAME2; m_yflame_timer = App->frames; break;
			case IN_F_YFLAME2: state = F_YFLAME2; f_yflame_timer = App->frames; break;

			case IN_LEFT_AND_JUMP2: state = ST_JUMP_BACKWARD2; jump_timer = App->frames;  break;
			case IN_LEFT_AND_CROUCH2: state = ST_CROUCHING2; crouching_timer = App->frames; break;

			case IN_L_PUNCH2: {
				if (!close) { state = L_PUNCH_STANDING2; l_standing_punch_timer = App->frames; }
				else if (close) { state = L_PUNCH_CLOSE2; l_close_standing_punch_timer = App->frames; }
			} break;

			case IN_L_KIK2: {
				if (!close) { state = L_KIK_STANDING2; l_standing_kik_timer = App->frames; }
				else if (close) { state = L_KIK_CLOSE2; l_close_standing_kik_timer = App->frames; }
			}break;

			case IN_M_PUNCH2: {
				if (!close) { state = M_PUNCH_STANDING2; m_standing_punch_timer = App->frames; }
				else if (close) { state = M_GRABBING2; grabbing_timer = App->frames; }
			}break;

			case IN_M_KIK2: {
				if (!close) { state = M_KIK_STANDING2; m_standing_kik_timer = App->frames; }
				else if (close) { state = M_KIK_CLOSE2; m_close_standing_kik_timer = App->frames; }
			}break;

			case IN_F_PUNCH2: {
				if (!close) { state = F_PUNCH_STANDING2; f_standing_punch_timer = App->frames; }
				else if (close) { state = F_GRABBING2; grabbing_timer = App->frames; }
			}break;

			case IN_F_KIK2: {
				if (!close) { state = F_KIK_STANDING2; f_standing_kik_timer = App->frames; }
				else if (close) { state = F_KIK_CLOSE2; f_close_standing_kik_timer = App->frames; }
			}break;

			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;

			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;
			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_DEFENDING2: state = ST_DEFENDING2; defending_timer = App->frames; break;

			case IN_VICTORY2: state = VICTORY2; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case ST_JUMP_NEUTRAL2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;

			case IN_L_PUNCH2: state = L_PUNCH_NEUTRAL_JUMP2; break;
			case IN_L_KIK2: state = L_KIK_NEUTRAL_JUMP2; break;

			case IN_M_PUNCH2: state = M_PUNCH_NEUTRAL_JUMP2; break;
			case IN_M_KIK2: state = M_KIK_NEUTRAL_JUMP2; break;

			case IN_F_PUNCH2: {
				if ((App->frames - jump_timer) > 23 && (App->frames - jump_timer) < 37) { state = YMUMMY2; }
				else { state = F_PUNCH_NEUTRAL_JUMP2; }
			}break;

			case IN_F_KIK2: {
				if ((App->frames - jump_timer) > 23 && (App->frames - jump_timer) < 37) { state = YDRILL2; }
				else { state = F_KIK_NEUTRAL_JUMP2; }
			}break;

			case IN_FALLING2: state = ST_FALLING2; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case ST_JUMP_FORWARD2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;

			case IN_L_PUNCH2: state = L_PUNCH_FORWARD_JUMP2; l_d_jumping_punch_timer = App->frames; break;
			case IN_L_KIK2: state = L_KIK_FORWARD_JUMP2; l_d_jumping_kik_timer = App->frames; break;

			case IN_M_PUNCH2: state = M_PUNCH_FORWARD_JUMP2; m_d_jumping_punch_timer = App->frames; break;
			case IN_M_KIK2: state = M_KIK_FORWARD_JUMP2; m_d_jumping_kik_timer = App->frames; break;

			case IN_F_PUNCH2: {
				if ((App->frames - jump_timer) > 23 && (App->frames - jump_timer) < 37) { state = YMUMMY2; }
				else { state = F_PUNCH_FORWARD_JUMP2; f_d_jumping_punch_timer = App->frames; }
			}break;

			case IN_F_KIK2: {
				if ((App->frames - jump_timer) > 23 && (App->frames - jump_timer) < 37) { state = YDRILL2; }
				else { state = F_KIK_FORWARD_JUMP2; f_d_jumping_kik_timer = App->frames; }
			}break;

			case IN_FALLING2: state = ST_FALLING2; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case ST_JUMP_BACKWARD2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;

			case IN_L_PUNCH2: state = L_PUNCH_BACKWARD_JUMP2; l_d_jumping_punch_timer = App->frames; break;
			case IN_L_KIK2: state = L_KIK_BACKWARD_JUMP2; l_d_jumping_kik_timer = App->frames; break;

			case IN_M_PUNCH2: state = M_PUNCH_BACKWARD_JUMP2; m_d_jumping_punch_timer = App->frames; break;
			case IN_M_KIK2: state = M_KIK_BACKWARD_JUMP2; m_d_jumping_kik_timer = App->frames; break;

			case IN_F_PUNCH2: {
				if ((App->frames - jump_timer) > 23 && (App->frames - jump_timer) < 37) { state = YMUMMY2; }
				else { state = F_PUNCH_BACKWARD_JUMP2; f_d_jumping_punch_timer = App->frames; }
			}break;

			case IN_F_KIK2: {
				if ((App->frames - jump_timer) > 23 && (App->frames - jump_timer) < 37) { state = YDRILL2; }
				else { state = F_KIK_BACKWARD_JUMP2; f_d_jumping_kik_timer = App->frames; }
			}break;

			case IN_FALLING2: state = ST_FALLING2; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case L_PUNCH_NEUTRAL_JUMP2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;
			case IN_FALLING2: state = ST_FALLING2; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case M_PUNCH_NEUTRAL_JUMP2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;
			case IN_FALLING2: state = ST_FALLING2; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case F_PUNCH_NEUTRAL_JUMP2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;
			case IN_FALLING2: state = ST_FALLING2; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;


		case L_KIK_NEUTRAL_JUMP2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;
			case IN_FALLING2: state = ST_FALLING2; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case M_KIK_NEUTRAL_JUMP2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;
			case IN_FALLING2: state = ST_FALLING2; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case F_KIK_NEUTRAL_JUMP2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;
			case IN_FALLING2: state = ST_FALLING2; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case L_YFIRE2:
		{
			switch (last_input)
			{
			case IN_YFIRE_FINISH2: state = ST_IDLE2; break;

			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;

			case IN_VICTORY2: state = VICTORY2; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case M_YFIRE2:
		{
			switch (last_input)
			{
			case IN_YFIRE_FINISH2: state = ST_IDLE2; break;

			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;

			case IN_VICTORY2: state = VICTORY2; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case F_YFIRE2:
		{
			switch (last_input)
			{
			case IN_YFIRE_FINISH2: state = ST_IDLE2; break;

			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;

			case IN_VICTORY2: state = VICTORY2; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case L_YFLAME2:
		{
			switch (last_input)
			{
			case IN_YFLAME_FINISH2: state = ST_IDLE2; break;

			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;

			case IN_VICTORY2: state = VICTORY2; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case M_YFLAME2:
		{
			switch (last_input)
			{
			case IN_YFLAME_FINISH2: state = ST_IDLE2; break;

			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;

			case IN_VICTORY2: state = VICTORY2; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case F_YFLAME2:
		{
			switch (last_input)
			{
			case IN_YFLAME_FINISH2: state = ST_IDLE2; break;

			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;

			case IN_VICTORY2: state = VICTORY2; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case YDRILL2:
		{
			switch (last_input)
			{
			case IN_YDRILL_FINISH2: state = ST_IDLE2; break;
			case IN_FALLING2: state = ST_FALLING2; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case YMUMMY2:
		{
			switch (last_input)
			{
			case IN_YMUMMY_FINISH2: state = ST_IDLE2; break;
			case IN_FALLING2: state = ST_FALLING2; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case L_PUNCH_FORWARD_JUMP2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;
			case IN_FALLING2: state = ST_FALLING2; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case L_KIK_FORWARD_JUMP2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;
			case IN_FALLING2: state = ST_FALLING2; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;


		case M_PUNCH_FORWARD_JUMP2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;
			case IN_FALLING2: state = ST_FALLING2; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case M_KIK_FORWARD_JUMP2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;
			case IN_FALLING2: state = ST_FALLING2; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case F_PUNCH_FORWARD_JUMP2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;
			case IN_FALLING2: state = ST_FALLING2; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case F_KIK_FORWARD_JUMP2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;
			case IN_FALLING2: state = ST_FALLING2; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;


		case L_PUNCH_BACKWARD_JUMP2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;
			case IN_FALLING2: state = ST_FALLING2; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case L_KIK_BACKWARD_JUMP2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;
			case IN_FALLING2: state = ST_FALLING2; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;


		case M_PUNCH_BACKWARD_JUMP2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;
			case IN_FALLING2: state = ST_FALLING2; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case M_KIK_BACKWARD_JUMP2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;
			case IN_FALLING2: state = ST_FALLING2; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case F_PUNCH_BACKWARD_JUMP2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;
			case IN_FALLING2: state = ST_FALLING2; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case F_KIK_BACKWARD_JUMP2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;
			case IN_FALLING2: state = ST_FALLING2; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;


		case L_PUNCH_STANDING2:
		{
			switch (last_input)
			{
			case IN_PUNCH_FINISH2: state = ST_IDLE2; break;

			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;
			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_VICTORY2: state = VICTORY2; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case L_PUNCH_CLOSE2:
		{
			switch (last_input)
			{
			case IN_PUNCH_FINISH2: state = ST_IDLE2; break;

			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;
			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_VICTORY2: state = VICTORY2; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case L_KIK_STANDING2:
		{
			switch (last_input)
			{
			case IN_KIK_FINISH2: state = ST_IDLE2; break;

			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;
			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_VICTORY2: state = VICTORY2; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case L_KIK_CLOSE2:
		{
			switch (last_input)
			{
			case IN_KIK_FINISH2: state = ST_IDLE2; break;

			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;
			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_VICTORY2: state = VICTORY2; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case M_PUNCH_STANDING2:
		{
			switch (last_input)
			{
			case IN_PUNCH_FINISH2: state = ST_IDLE2; break;

			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;
			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_VICTORY2: state = VICTORY2; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case M_PUNCH_CLOSE2:
		{
			switch (last_input)
			{
			case IN_PUNCH_FINISH2: state = ST_IDLE2; break;

			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;
			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_VICTORY2: state = VICTORY2; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case M_KIK_STANDING2:
		{
			switch (last_input)
			{
			case IN_KIK_FINISH2: state = ST_IDLE2; break;

			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;
			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_VICTORY2: state = VICTORY2; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case M_KIK_CLOSE2:
		{
			switch (last_input)
			{
			case IN_KIK_FINISH2: state = ST_IDLE2; break;

			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;
			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_VICTORY2: state = VICTORY2; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case F_PUNCH_STANDING2:
		{
			switch (last_input)
			{
			case IN_PUNCH_FINISH2: state = ST_IDLE2; break;

			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;
			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_VICTORY2: state = VICTORY2; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case F_PUNCH_CLOSE2:
		{
			switch (last_input)
			{
			case IN_PUNCH_FINISH2: state = ST_IDLE2; break;

			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;
			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_VICTORY2: state = VICTORY2; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case F_KIK_STANDING2:
		{
			switch (last_input)
			{
			case IN_KIK_FINISH2: state = ST_IDLE2; break;

			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;
			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_VICTORY2: state = VICTORY2; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case F_KIK_CLOSE2:
		{
			switch (last_input)
			{
			case IN_KIK_FINISH2: state = ST_IDLE2; break;

			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;
			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_VICTORY2: state = VICTORY2; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case ST_CROUCHING2:
		{
			switch (last_input)
			{
			case IN_CROUCHING_FINISH2: state = ST_CROUCH2; break;

			case IN_TURNING2: state = CROUCH_TURNING2; turning_timer = App->frames;  break;

			case IN_L_YFIRE2: state = L_YFIRE2; hadoken_timer = App->frames; break;
			case IN_M_YFIRE2: state = M_YFIRE2; hadoken_timer = App->frames; break;
			case IN_F_YFIRE2: state = F_YFIRE2; hadoken_timer = App->frames; break;

			case IN_L_YFLAME2: state = L_YFLAME2; l_yflame_timer = App->frames; break;
			case IN_M_YFLAME2: state = M_YFLAME2; m_yflame_timer = App->frames; break;
			case IN_F_YFLAME2: state = F_YFLAME2; f_yflame_timer = App->frames; break;

			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;

			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; crouch_reel_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case ST_STANDING2:
		{
			switch (last_input)
			{
			case IN_STANDING_FINISH2: state = ST_IDLE2; break;

			case IN_TURNING2: state = CROUCH_TURNING2; turning_timer = App->frames;  break;

			case IN_L_YFIRE2: state = L_YFIRE2; hadoken_timer = App->frames; break;
			case IN_M_YFIRE2: state = M_YFIRE2; hadoken_timer = App->frames; break;
			case IN_F_YFIRE2: state = F_YFIRE2; hadoken_timer = App->frames; break;

			case IN_L_YFLAME2: state = L_YFLAME2; l_yflame_timer = App->frames; break;
			case IN_M_YFLAME2: state = M_YFLAME2; m_yflame_timer = App->frames; break;
			case IN_F_YFLAME2: state = F_YFLAME2; f_yflame_timer = App->frames; break;

			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;

			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; crouch_reel_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case ST_CROUCH2:
		{
			switch (last_input)
			{
			case IN_CROUCH_UP2: state = ST_STANDING2; standing_timer = App->frames; break;

			case IN_TURNING2: state = CROUCH_TURNING2; turning_timer = App->frames;  break;

			case IN_LEFT_AND_CROUCH2: {
				if (flip == true)
					state = ST_CROUCH_DEFENDING_READY2;
				else
					state = ST_CROUCH2;
			}break;

			case IN_RIGHT_AND_CROUCH2: {
				if (flip == false)
					state = ST_CROUCH_DEFENDING_READY2;
				else
					state = ST_CROUCH2;
			}break;

			case IN_L_PUNCH2: {
				if (!close) { state = L_PUNCH_CROUCH2; l_crouching_punch_timer = App->frames; }
				else if (close) { state = L_PUNCH_CROUCHCLOSE2; l_close_crouching_punch_timer = App->frames; }
			}break;

			case IN_L_KIK2: {
				if (!close) { state = L_KIK_CROUCH2; l_crouching_kik_timer = App->frames; }
				else if (close) { state = L_KIK_CROUCHCLOSE2; l_close_crouching_kik_timer = App->frames; }
			}break;

			case IN_M_PUNCH2: {
				if (!close) { state = M_PUNCH_CROUCH2; m_crouching_punch_timer = App->frames; }
				else if (close) { state = M_PUNCH_CROUCHCLOSE2; m_close_crouching_punch_timer = App->frames; }
			}break;

			case IN_M_KIK2: {
				if (!close) { state = M_KIK_CROUCH2; m_crouching_kik_timer = App->frames; }
				else if (close) { state = M_KIK_CROUCHCLOSE2; m_close_crouching_kik_timer = App->frames; }
			}break;

			case IN_F_PUNCH2: state = F_PUNCH_CROUCH2; f_crouching_punch_timer = App->frames; break;
			case IN_F_KIK2: state = F_KIK_CROUCH2; f_crouching_kik_timer = App->frames; break;

			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;

			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; crouch_reel_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_VICTORY2: state = VICTORY2; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case ST_CROUCH_DEFENDING_READY2:
		{
			switch (last_input)
			{
			case IN_CROUCH_UP2: state = ST_STANDING2; standing_timer = App->frames; break;

			case IN_TURNING2: state = CROUCH_TURNING2; turning_timer = App->frames;  break;

			case IN_LEFT_UP2: {
				if (flip == true)
					state = ST_CROUCH2;
				else
					state = ST_CROUCH_DEFENDING_READY2;
			}break;

			case IN_RIGHT_UP2: {
				if (flip == false)
					state = ST_CROUCH2;
				else
					state = ST_CROUCH_DEFENDING_READY2;
			}break;

			case IN_L_PUNCH2: {
				if (!close) { state = L_PUNCH_CROUCH2; l_crouching_punch_timer = App->frames; }
				else if (close) { state = L_PUNCH_CROUCHCLOSE2; l_close_crouching_punch_timer = App->frames; }
			}break;

			case IN_L_KIK2: {
				if (!close) { state = L_KIK_CROUCH2; l_crouching_kik_timer = App->frames; }
				else if (close) { state = L_KIK_CROUCHCLOSE2; l_close_crouching_kik_timer = App->frames; }
			}break;

			case IN_M_PUNCH2: {
				if (!close) { state = M_PUNCH_CROUCH2; m_crouching_punch_timer = App->frames; }
				else if (close) { state = M_PUNCH_CROUCHCLOSE2; m_close_crouching_punch_timer = App->frames; }
			}break;

			case IN_M_KIK2: {
				if (!close) { state = M_KIK_CROUCH2; m_crouching_kik_timer = App->frames; }
				else if (close) { state = M_KIK_CROUCHCLOSE2; m_close_crouching_kik_timer = App->frames; }
			}break;

			case IN_F_PUNCH2: state = F_PUNCH_CROUCH2; f_crouching_punch_timer = App->frames; break;
			case IN_F_KIK2: state = F_KIK_CROUCH2; f_crouching_kik_timer = App->frames; break;

			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;
			case IN_DEFENDING2: state = ST_CROUCH_DEFENDING2; crouch_defending_timer = App->frames; break;

			case IN_VICTORY2: state = VICTORY2; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case ST_CROUCH_DEFENDING2:
		{
			switch (last_input)
			{
			case IN_DEFENDING_FINISH2: state = ST_CROUCH2; break;

			case IN_VICTORY2: state = VICTORY2; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case L_PUNCH_CROUCH2:
		{
			switch (last_input)
			{
			case IN_PUNCH_FINISH2: state = ST_CROUCH2; break;

			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;
			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; crouch_reel_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_VICTORY2: state = VICTORY2; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case L_PUNCH_CROUCHCLOSE2:
		{
			switch (last_input)
			{
			case IN_PUNCH_FINISH2: state = ST_CROUCH2; break;

			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;
			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; crouch_reel_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case L_KIK_CROUCH2:
		{
			switch (last_input)
			{
			case IN_KIK_FINISH2: state = ST_CROUCH2; break;

			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;
			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; crouch_reel_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case L_KIK_CROUCHCLOSE2:
		{
			switch (last_input)
			{
			case IN_KIK_FINISH2: state = ST_CROUCH2; break;

			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;
			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; crouch_reel_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case M_PUNCH_CROUCH2:
		{
			switch (last_input)
			{
			case IN_PUNCH_FINISH2: state = ST_CROUCH2; break;

			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;
			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; crouch_reel_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case M_PUNCH_CROUCHCLOSE2:
		{
			switch (last_input)
			{
			case IN_PUNCH_FINISH2: state = ST_CROUCH2; break;

			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;
			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; crouch_reel_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case M_KIK_CROUCH2:
		{
			switch (last_input)
			{
			case IN_KIK_FINISH2: state = ST_CROUCH2; break;

			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;
			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; crouch_reel_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case M_KIK_CROUCHCLOSE2:
		{
			switch (last_input)
			{
			case IN_KIK_FINISH2: state = ST_CROUCH2; break;

			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;
			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; crouch_reel_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case F_PUNCH_CROUCH2:
		{
			switch (last_input)
			{
			case IN_PUNCH_FINISH2: state = ST_CROUCH2; break;

			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;
			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; crouch_reel_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case F_KIK_CROUCH2:
		{
			switch (last_input)
			{
			case IN_KIK_FINISH2: state = ST_CROUCH2; break;

			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;
			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; crouch_reel_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case ST_HEAD_REEL2:
		{
			switch (last_input)
			{
			case IN_REEL_FINISH2: {
				if (dizzi) { state = ST_DIZZI2; dizzi_timer = App->frames; }
				else state = ST_IDLE2;
			} break;

			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;
			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case ST_GUT_REEL2:
		{
			switch (last_input)
			{
			case IN_REEL_FINISH2: {
				if (dizzi) { state = ST_DIZZI2; dizzi_timer = App->frames; }
				else state = ST_IDLE2;
			} break;

			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;
			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case ST_CROUCH_REEL2:
		{
			switch (last_input)
			{
			case IN_REEL_FINISH2: {
				if (dizzi) { state = ST_DIZZI2; dizzi_timer = App->frames; }
				else state = ST_IDLE2;
			} break;

			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;
			case IN_HEAD_REEL2: state = ST_CROUCH_REEL2; crouch_reel_timer = App->frames; break;
			case IN_SWEEP2: state = SWEEP2; sweep_timer = App->frames; break;

			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case ST_FALLING2:
		{
			switch (last_input)
			{
			case IN_FALLING_FINISH2:state = ST_GETTING_UP2; getting_up_timer = App->frames; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case ST_GETTING_UP2:
		{
			switch (last_input)
			{
			case IN_GETTING_UP_FINISH2: {
				if (dizzi) { state = ST_DIZZI2; dizzi_timer = App->frames; }
				else state = ST_IDLE2;
			} break;
			}
		}
		break;

		case ST_DIZZI2:
		{
			switch (last_input)
			{
			case IN_DIZZI_FINISH2:state = ST_IDLE2; break;
			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;
			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;

			case IN_VICTORY2: state = VICTORY2; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case M_GRABBING2:
		{
			switch (last_input)
			{
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;
			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;

			case IN_GRAB2:state = M_GRAB2; m_grab_timer = App->frames; break;
			case IN_GRABBING_FINISH2:state = ST_IDLE2; break;
			}
		}
		break;

		case F_GRABBING2:
		{
			switch (last_input)
			{
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;
			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;

			case IN_GRAB2:state = F_GRAB2; f_grab_timer = App->frames; break;
			case IN_GRABBING_FINISH2:state = ST_IDLE2; break;
			}
		}
		break;

		case M_GRAB2:
		{
			switch (last_input)
			{
			case IN_M_GRAB_FINISH2:state = ST_IDLE2; break;
			}
		}
		break;

		case F_GRAB2:
		{
			switch (last_input)
			{
			case IN_F_GRAB_FINISH2:state = ST_IDLE2; break;
			}
		}
		break;

		case GRABBED2:
		{
			switch (last_input)
			{
			case IN_GRABBED_FINISH2:state = ST_FALLING2; break;
			}
		}
		break;

		case ST_DEFENDING2:
		{
			switch (last_input)
			{
			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;
			case IN_DEFENDING_FINISH2:state = ST_IDLE2; break;
			case IN_GUT_REEL:state = ST_GUT_REEL2; break;
			}
		}
		break;

		case TURNING2:
		{
			switch (last_input)
			{
			case IN_TURNING_FINISH2:state = ST_IDLE2; break;

			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;
			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;

			case IN_VICTORY2: state = VICTORY2; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case CROUCH_TURNING2:
		{
			switch (last_input)
			{
			case IN_TURNING_FINISH2:state = ST_CROUCH2; break;

			case IN_GRABBED2: state = GRABBED2; grabbed_timer = App->frames; break;
			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; crouch_reel_timer = App->frames; break;

			case IN_VICTORY2: state = VICTORY2; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case SWEEP2:
		{
			switch (last_input)
			{
			case IN_SWEEP_FINISH2:state = ST_GETTING_UP2; getting_up_timer = App->frames; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}

		case LOOSE2:
		{
			switch (last_input)
			{
			case IN_END2:state = ST_IDLE2; break;
			}
		}
		break;

		case VICTORY2:
		{
			switch (last_input)
			{
			case IN_END2:state = ST_IDLE2; break;
			}
		}
		break;
		}
	}

	return state;
}
