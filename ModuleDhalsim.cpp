#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleDhalsim.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"
#include "ModuleRyu.h"
#include "ModuleFonts.h"
#include "p2Qeue.h"
#include "SDL\include\SDL.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModuleDhalsim::ModuleDhalsim()
{
}

ModuleDhalsim::~ModuleDhalsim()
{}

// Load assets
bool ModuleDhalsim::Start()
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

	hdk_voice = App->audio->LoadChunk("assets/sfx/voices/ryu_ken_hadouken.wav");
	hdk_hit = App->audio->LoadChunk("assets/sfx/effects/fist_intro.wav");
	low_kick = App->audio->LoadChunk("assets/sfx/effects/low_kick.wav");
	low_fist = App->audio->LoadChunk("assets/sfx/effects/low_fist.wav");
	high_fist = App->audio->LoadChunk("assets/sfx/effects/high_fist.wav");
	high_kick = App->audio->LoadChunk("assets/sfx/effects/high_kick.wav");

	position.x = 250;
	position.y = 215;

	life = 100;
	freeze = true;
	victoryExecuted = invulnerabilityFrames = 0;

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
	Module* lpCallback2[lpnColliders2] = { {this}, {this}, {this}, {(Module*)App->ryu} };
	lp.PushBack({ 437, 665, 90, 86 }, 5, { 33,5 }, lpnColliders, lpHitbox, lpColliderType, lpCallback);
	lp.PushBack({ 529, 668, 83, 83 }, 10, { 33,5 }, lpnColliders, lpHitbox, lpColliderType, lpCallback);
	lp.PushBack({ 614, 671, 135, 80 }, 8, { 33,5 }, lpnColliders2, lpHitbox2, lpColliderType2, lpCallback2);
	lp.PushBack({ 529, 668, 83, 83 }, 10, { 33,5 }, lpnColliders, lpHitbox, lpColliderType, lpCallback);


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
	COLLIDER_TYPE mpColliderType2[mpnColliders2] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}};
	COLLIDER_TYPE mpColliderType3[mpnColliders3] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	COLLIDER_TYPE mpColliderType4[mpnColliders4] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2},{COLLIDER_PLAYER2_HIT} };
	COLLIDER_TYPE mpColliderType5[mpnColliders5] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* mpCallback[mpnColliders] = { {this}, {this}, {this} };
	Module* mpCallback2[mpnColliders2] = { {this}, {this}, {this} };
	Module* mpCallback3[mpnColliders3] = { {this}, {this}, {this}, {(Module*)App->ryu} };
	Module* mpCallback4[mpnColliders4] = { {this}, {this}, {this}, {(Module*)App->ryu} };
	Module* mpCallback5[mpnColliders5] = { {this}, {this}, {this} };


	mp.PushBack({ 751, 652, 74, 99 }, 3, { 33,5 }, mpnColliders, mpHitbox, mpColliderType, mpCallback);
	mp.PushBack({ 827, 674, 86, 77 }, 4, { 33,5 }, mpnColliders2, mpHitbox2, mpColliderType2, mpCallback2);
	mp.PushBack({ 0, 816, 150, 46 }, 4, { 33,5 }, mpnColliders3, mpHitbox3, mpColliderType3, mpCallback3);
	mp.PushBack({ 152, 816, 214, 46 }, 6, { 33,5 }, mpnColliders4, mpHitbox4, mpColliderType4, mpCallback4);
	mp.PushBack({ 0, 816, 150, 46 }, 4, { 33,5 }, mpnColliders5, mpHitbox5, mpColliderType5, mpCallback5);
	mp.PushBack({ 827, 674, 86, 77 }, 4, { 33,5 }, mpnColliders2, mpHitbox2, mpColliderType2, mpCallback2);
	mp.PushBack({ 751, 652, 74, 99 }, 1, { 33,5 }, mpnColliders, mpHitbox, mpColliderType, mpCallback);


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
	Module* hpCallback3[hpnColliders3] = { { this },{ this },{ (Module*)App->ryu }};
	Module* hpCallback4[hpnColliders4] = { { this },{ this },{ (Module*)App->ryu }};
	Module* hpCallback5[hpnColliders5] = { { this },{ this }, { this } };
	hp.PushBack({ 751, 652, 74, 99 }, 1, { 33,5 }, hpnColliders, hpHitbox, hpColliderType, hpCallback);
	hp.PushBack({ 827, 674, 86, 77 }, 4, { 33,5 }, hpnColliders2, hpHitbox2, hpColliderType2, hpCallback2);
	hp.PushBack({ 368, 816, 151, 46 }, 8, { 33,5 }, hpnColliders3, hpHitbox3, hpColliderType3, hpCallback3);
	hp.PushBack({ 521, 816, 247, 46 }, 10, { 33,5 }, hpnColliders4, hpHitbox4, hpColliderType4, hpCallback4);
	hp.PushBack({ 368, 816, 151, 46 }, 10, { 33,5 }, hpnColliders5, hpHitbox5, hpColliderType5, hpCallback5);
	hp.PushBack({ 827, 674, 86, 77 }, 4, { 33,5 }, hpnColliders2, hpHitbox2, hpColliderType2, hpCallback2);
	hp.PushBack({ 751, 652, 74, 99 }, 1, { 33,5 }, hpnColliders, hpHitbox, hpColliderType, hpCallback);

	// lk
	const int lknColliders = 3;
	const int lknColliders3 = 4;
	SDL_Rect lkHitbox[lknColliders] = { { -15, 79, 44, 16}, { -6, 37, 40, 47}, { -6, 0, 40, 37} };
	SDL_Rect lkHitbox3[lknColliders3] = { { 3, 84, 50, 27}, { -13, 57, 100, 27}, { -6, 0, 40, 57}, { -100, 77, 67, 26} };
	COLLIDER_TYPE lkColliderType[lknColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	COLLIDER_TYPE lkColliderType3[lknColliders3] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	Module* lkCallback[lknColliders] = { {this}, {this}, {this} };
	Module* lkCallback3[lknColliders3] = { {this}, {this}, {this}, {(Module*)App->ryu} };
	lk.PushBack({ 770, 770, 58, 92 }, 7, { 32,5 }, lknColliders, lkHitbox, lkColliderType, lkCallback);
	lk.PushBack({ 842, 753, 118, 109 }, 7, { 33,5 }, lknColliders, lkHitbox, lkColliderType, lkCallback);
	lk.PushBack({ 0, 896, 165, 109 }, 20, { 33,5 }, lknColliders3, lkHitbox3, lkColliderType3, lkCallback3);
	lk.PushBack({ 842, 753, 118, 109 }, 7, { 33,5 }, lknColliders, lkHitbox, lkColliderType, lkCallback);
	lk.PushBack({ 770, 770, 58, 92 }, 7, { 33,5 }, lknColliders, lkHitbox, lkColliderType, lkCallback);

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


	// Jumping neutral lp
	const int jlpnColliders = 3;
	const int jlpnColliders2 = 5;
	SDL_Rect jlpHitbox[jlpnColliders] = { { 0, 0, 0, 0}, { 0, 27, 48, 51}, { 0, 0, 48, 27} };
	SDL_Rect jlpHitbox2[jlpnColliders2] = { { -103, 21, 34, 26}, { -63, 41, 34, 26}, { 0, 27, 68, 51}, { 0, 0, 48, 27}, {-138, 0, 35, 30} };
	COLLIDER_TYPE jlpColliderType[jlpnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	COLLIDER_TYPE jlpColliderType2[jlpnColliders2] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	Module*jlpCallback[jlpnColliders] = { {this}, {this}, {this} };
	Module*jlpCallback2[jlpnColliders2] = { {this}, {this}, {this}, {this}, {(Module*)App->ryu} };

	jlp.PushBack({ 179, 398, 58, 88 }, 2, { 33,5 }, { jlpnColliders }, { jlpHitbox }, { jlpColliderType }, { jlpCallback });
	jlp.PushBack({ 239, 410, 111, 76 }, 100, { 33,5 }, { jlpnColliders }, { jlpHitbox }, { jlpColliderType }, { jlpCallback });
	jlp.PushBack({ 352, 401, 173, 85 }, 100, { 33,5 }, { jlpnColliders2 }, { jlpHitbox2 }, { jlpColliderType2 }, { jlpCallback2 });
	jlp.PushBack({ 239, 410, 111, 76 }, 100, { 33,5 }, { jlpnColliders }, { jlpHitbox }, { jlpColliderType }, { jlpCallback });
	jlp.PushBack({ 179, 398, 58, 88 }, 100, { 33,5 }, { jlpnColliders }, { jlpHitbox }, { jlpColliderType }, { jlpCallback });


	// Jumping neutral lk
	const int jlknColliders = 3;
	const int jlknColliders2 = 5;
	SDL_Rect jlkHitbox[jlknColliders] = { { 0, 0, 0, 0}, { -10, 27, 48, 51}, { -10, 0, 48, 27} };
	SDL_Rect jlkHitbox2[jlknColliders2] = { { -120, 0, 34, 26}, { -73, 17, 34, 26}, { -10, 27, 61, 51}, { -10, 0, 48, 27}, {-163, 0, 35, 24} };
	COLLIDER_TYPE jlkColliderType[jlknColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	COLLIDER_TYPE jlkColliderType2[jlknColliders2] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	Module*jlkCallback[jlknColliders] = { {this}, {this}, {this} };
	Module*jlkCallback2[jlknColliders2] = { {this}, {this}, {this}, {this}, {(Module*)App->ryu} };

	jlk.PushBack({ 527, 396, 76, 90 }, 2, { 33,5 }, { jlknColliders }, { jlkHitbox }, { jlkColliderType }, { jlkCallback });
	jlk.PushBack({ 605, 415, 133, 71 }, 100, { 33,5 }, { jlknColliders }, { jlkHitbox }, { jlkColliderType }, { jlkCallback });
	jlk.PushBack({ 740, 404, 202, 82 }, 100, { 33,5 }, { jlknColliders2 }, { jlkHitbox2 }, { jlkColliderType2 }, { jlkCallback2 });
	jlk.PushBack({ 605, 415, 133, 71 }, 100, { 33,5 }, { jlknColliders }, { jlkHitbox }, { jlkColliderType }, { jlkCallback });
	jlk.PushBack({ 527, 396, 76, 90 }, 100, { 33,5 }, { jlknColliders }, { jlkHitbox }, { jlkColliderType }, { jlkCallback });


	//Hadoken
	const int hdknColliders = 3;
	SDL_Rect hdkHitbox[hdknColliders] = { { -25, 79, 24, 16}, { -6, 37, 40, 47 }, { -6, 5, 40, 32 } };
	SDL_Rect hdkHitbox2[hdknColliders] = { { -62, 58, 24, 20}, { -43, 23, 40, 42}, { -10, 5, 78, 28} };
	COLLIDER_TYPE hdkColliderType[hdknColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* hdkCallback[hdknColliders] = { {this}, {this}, {this} };
	hdk.PushBack({ 462, 751, 74, 90 }, 5, { 29,5 }, hdknColliders, hdkHitbox, hdkColliderType, hdkCallback);		// Falta un trozo de animación, cuya durada depende de si es light, medium or hard
	hdk.PushBack({ 537, 757, 85, 84 }, 15, { 29,5 }, hdknColliders, hdkHitbox, hdkColliderType, hdkCallback);
	hdk.PushBack({ 623, 758, 90, 83 }, 3, { 29,5 }, hdknColliders, hdkHitbox, hdkColliderType, hdkCallback);
	hdk.PushBack({ 714, 764, 106, 77 }, 40, { 29,5 }, hdknColliders, hdkHitbox2, hdkColliderType, hdkCallback);


	// Standing reel
	const int streelnColliders = 3;

	SDL_Rect streelHitbox1[streelnColliders] = { { -11, 78, 24, 16}, { -6, 26, 40, 47}, { -6, 0, 40, 45} };
	COLLIDER_TYPE streelColliderType[streelnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* streelCallback[streelnColliders] = { {this}, {this}, {this} };

	streel.PushBack({ 70, 810, 72, 92 }, 10, { 30,5 }, streelnColliders, streelHitbox1, streelColliderType, streelCallback);
	streel.PushBack({ 144, 811, 69, 93 }, 10, { 31,5 }, streelnColliders, streelHitbox1, streelColliderType, streelCallback);
	streel.PushBack({ 215, 808, 72, 96 }, 10, { 47,5 }, streelnColliders, streelHitbox1, streelColliderType, streelCallback);
	streel.PushBack({ 289, 808, 86, 96 }, 10, { 63,5 }, streelnColliders, streelHitbox1, streelColliderType, streelCallback);


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


	//Crouching l punch
	const int clpnColliders = 3;
	const int clpnColliders2 = 4;
	SDL_Rect clpHitbox1[clpnColliders] = { { -32, 47, 26, 22}, { -6, 27, 46, 27}, { -6, 0, 46, 27} };
	SDL_Rect clpHitbox[clpnColliders] = { { -75, 52, 24, 16}, { -6, 0, 40, 53}, { -46, 0, 37, 53} };
	SDL_Rect clpHitbox2[clpnColliders2] = { { -103, 0, 65, 33}, { -6, 0, 40, 53}, { -46, 0, 57, 53}, {-163, 5, 35, 25} };
	COLLIDER_TYPE clpColliderType[clpnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	COLLIDER_TYPE clpColliderType2[clpnColliders2] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	Module*clpCallback[clpnColliders] = { {this}, {this}, {this} };
	Module*clpCallback2[clpnColliders2] = { {this}, {this}, {this}, {(Module*)App->ryu} };
	clp.PushBack({ 511, 170, 83, 75 }, 3, { 46,5 }, { clpnColliders }, { clpHitbox1 }, { clpColliderType }, { clpCallback });
	clp.PushBack({ 596, 193, 83, 52 }, 3, { 33,5 }, { clpnColliders }, { clpHitbox }, { clpColliderType }, { clpCallback });
	clp.PushBack({ 681, 211, 135, 34 }, 4, { 33,5 }, { clpnColliders }, { clpHitbox }, { clpColliderType }, { clpCallback });
	clp.PushBack({ 817, 211, 199, 34 }, 8, { 33,5 }, { clpnColliders2 }, { clpHitbox2 }, { clpColliderType2 }, { clpCallback2 });
	clp.PushBack({ 681, 211, 135, 34 }, 5, { 33,5 }, { clpnColliders }, { clpHitbox }, { clpColliderType }, { clpCallback });
	clp.PushBack({ 596, 193, 83, 52 }, 5, { 33,5 }, { clpnColliders }, { clpHitbox }, { clpColliderType }, { clpCallback });



	//Crouching l kik
	const int clknColliders = 3;
	const int clknColliders2 = 4;
	SDL_Rect clkHitbox[clknColliders] = { { -32, 47, 26, 22}, { -6, 27, 46, 27}, { -6, 0, 46, 27} };
	SDL_Rect clkHitbox2[clknColliders2] = { { -15, 30, 24, 16}, { -6, 0, 48, 43}, { -54, 0, 88, 43}, { -62, 5, 88, 17} };
	COLLIDER_TYPE clkColliderType[clknColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	COLLIDER_TYPE clkColliderType2[clknColliders2] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2_HIT} };
	Module*clkCallback[clknColliders] = { {this}, {this}, {this} };
	Module*clkCallback2[clknColliders2] = { {this}, {this}, {this}, {(Module*)App->ryu} };


	clk.PushBack({ 331, 181, 61, 66 }, 2, { 33,5 }, { clknColliders }, { clkHitbox }, { clkColliderType }, { clkCallback });
	clk.PushBack({ 190, 298, 65, 64 }, 2, { 33,5 }, { clknColliders }, { clkHitbox }, { clkColliderType }, { clkCallback });
	clk.PushBack({ 257, 318, 150, 43 }, 18, { 76,5 }, { clknColliders2 }, { clkHitbox2 }, { clkColliderType2 }, { clkCallback2 });
	clk.PushBack({ 190, 298, 65, 64 }, 5, { 33,5 }, { clknColliders }, { clkHitbox }, { clkColliderType }, { clkCallback });


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

	//Falling
	const int airreelnColliders = 3;
	SDL_Rect airreelHitbox1[airreelnColliders] = { { -11, 57, 24, 10}, { 0, 11, 73, 47}, { -31, -32, 40, 45} };
	SDL_Rect airreelHitbox2[airreelnColliders] = { { 0, 0, 0, 0}, { 0, 0, 0, 0}, { 0, -0, 105, 40} };
	SDL_Rect airreelHitbox3[airreelnColliders] = { { -12, 84, 24, 16}, { 0, 40, 55, 47}, { 0, 0, 40, 40} };
	COLLIDER_TYPE airreelColliderType[airreelnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* airreelCallback[airreelnColliders] = { {this}, {this}, {this} };

	airreel.PushBack({ 815, 883, 73, 65 }, 5, { 33,5 }, { airreelnColliders }, { airreelHitbox1 }, { airreelColliderType }, { airreelCallback });

	//Get up
	const int getupnColliders = 3;
	SDL_Rect getupHitbox1[getupnColliders] = { { 0, 0, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0} };
	COLLIDER_TYPE getupColliderType[getupnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* getupCallback[getupnColliders] = { {this}, {this}, {this} };

	getup.PushBack({ 311, 993, 127, 31 }, 5, { 33,5 }, { getupnColliders }, { getupHitbox1 }, { getupColliderType }, { getupCallback });
	getup.PushBack({ 440, 990, 92, 34 }, 10, { 33,5 }, { getupnColliders }, { getupHitbox1 }, { getupColliderType }, { getupCallback });
	getup.PushBack({ 534, 953, 56, 71 }, 10, { 33,5 }, { getupnColliders }, { getupHitbox1 }, { getupColliderType }, { getupCallback });
	getup.PushBack({ 633, 909, 46, 115 }, 10, { 33,5 }, { getupnColliders }, { getupHitbox1 }, { getupColliderType }, { getupCallback });
	getup.PushBack({ 681, 956, 79, 68 }, 10, { 33,5 }, { getupnColliders }, { getupHitbox1 }, { getupColliderType }, { getupCallback });


	//Ground
	ground.PushBack({ 310, 992, 129, 32 }, 20, { 33,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });

	inputs.Push(IN_END2);

	return ret;
}

bool ModuleDhalsim::CleanUp()
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

	App->textures->Unload(graphics);
	App->textures->Unload(graphics2);
	App->textures->Unload(graphics3);
	ClearColliders();
	idle = Animation();
	forward = Animation();
	backward = Animation();
	lp = lk = clp = clk = mp = hp = Animation();
	jlp = jlk = jflp = jflk = jblp = jblk = Animation();
	neutralJump = Animation();
	forwardJump = Animation();
	backwardJump = Animation();
	hdk = Animation();
	streel = Animation();
	stgreel = Animation();
	creel = Animation();
	airreel = Animation();
	getup = Animation();
	crouching = standing = crouch = Animation();
	win1 = win2 = Animation();
	ground = Animation();
	return true;
}

update_status ModuleDhalsim::PreUpdate() {
	ClearColliders();

	return UPDATE_CONTINUE;
}

// Update: draw background
update_status ModuleDhalsim::Update()
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
			if (App->frames - jump_timer > 27 && (App->frames - jump_timer <= D_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}
			if (App->frames - jump_timer < 28 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}
			break;

		case ST_JUMP_FORWARD2:
			current_animation = &forwardJump;
			if (App->frames - jump_timer > 27 && (App->frames - jump_timer <= D_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}
			if (App->frames - jump_timer < 28 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}

			if (position.x - 34 > -App->render->camera.x / SCREEN_SIZE)
				position.x -= 3;
			break;

		case ST_JUMP_BACKWARD2:
			current_animation = &backwardJump;
			if (App->frames - jump_timer > 27 && (App->frames - jump_timer <= D_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}
			if (App->frames - jump_timer < 28 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}

			if (position.x + 24 < -App->render->camera.x / SCREEN_SIZE + App->render->camera.w){
				position.x += 3;
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
			break;

		case M_PUNCH_CROUCH2:
			break;

		case F_PUNCH_CROUCH2:
			break;

		case L_PUNCH_STANDING2:
			texture = graphics3;
			current_animation = &lp;
			break;

		case M_PUNCH_STANDING2:
			texture = graphics3;
			current_animation = &mp;
			break;

		case F_PUNCH_STANDING2:
			texture = graphics3;
			current_animation = &hp;
			break;

		case L_PUNCH_NEUTRAL_JUMP2:
			current_animation = &jlp;
			if (App->frames - jump_timer > 27 && (App->frames - jump_timer <= D_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}
			if (App->frames - jump_timer < 28 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}
			break;

		case M_PUNCH_NEUTRAL_JUMP2:
			break;

		case F_PUNCH_NEUTRAL_JUMP2:
			break;

		case L_PUNCH_FORWARD_JUMP2:
			current_animation = &jlp;
			if (App->frames - jump_timer > 27 && (App->frames - jump_timer <= D_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}
			if (App->frames - jump_timer < 28 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}

			if (position.x - 34 > -App->render->camera.x / SCREEN_SIZE)
				position.x -= 3;
			break;

		case M_PUNCH_FORWARD_JUMP2:
			break;

		case F_PUNCH_FORWARD_JUMP2:
			break;

		case L_PUNCH_BACKWARD_JUMP2:

			current_animation = &jlp;

			if (App->frames - jump_timer > 27 && (App->frames - jump_timer <= D_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}
			if (App->frames - jump_timer < 28 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}

			if (position.x + 24 < -App->render->camera.x / SCREEN_SIZE + App->render->camera.w)
				position.x += 3;

			break;

		case M_PUNCH_BACKWARD_JUMP2:
			break;

		case F_PUNCH_BACKWARD_JUMP2:
			break;

		case L_KIK_CROUCH2:
			current_animation = &clk;
			break;

		case M_KIK_CROUCH2:
			break;

		case F_KIK_CROUCH2:
			break;

		case L_KIK_STANDING2:
			texture = graphics3;
			current_animation = &lk;
			break;

		case M_KIK_STANDING2:
			break;

		case F_KIK_STANDING2:
			break;

		case L_KIK_NEUTRAL_JUMP2:
			current_animation = &jlk;
			if (App->frames - jump_timer > 27 && (App->frames - jump_timer <= D_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}
			if (App->frames - jump_timer < 28 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}

			break;

		case M_KIK_NEUTRAL_JUMP2:
			break;

		case F_KIK_NEUTRAL_JUMP2:
			break;

		case L_KIK_FORWARD_JUMP2:
			current_animation = &jlk;
			if (App->frames - jump_timer > 27 && (App->frames - jump_timer <= D_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}
			if (App->frames - jump_timer < 28 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}

			if (position.x - 34 > -App->render->camera.x / SCREEN_SIZE)
				position.x -= 3;

			break;

		case M_KIK_FORWARD_JUMP2:
			break;

		case F_KIK_FORWARD_JUMP2:
			break;

		case L_KIK_BACKWARD_JUMP2:
			current_animation = &jlk;

			if (App->frames - jump_timer > 27 && (App->frames - jump_timer <= D_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}
			if (App->frames - jump_timer < 28 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}

			if (position.x + 24 < -App->render->camera.x / SCREEN_SIZE + App->render->camera.w)
				position.x += 3;

			break;

		case M_KIK_BACKWARD_JUMP2:
			break;

		case F_KIK_BACKWARD_JUMP2:
			break;

		case ST_DEFENDING:
			LOG("Defending");
			break;

		case ST_HEAD_REEL2:
			current_animation = &streel;
			break;

		case ST_GUT_REEL2:
			break;

		case ST_CROUCH_REEL2:
			current_animation = &creel;
			break;

		case ST_HADOKEN2:
			current_animation = &hdk;
			if (App->frames - hadoken_timer == 35)
			{
				App->particles->AddParticle(App->particles->hdk, flip, position.x + hdk_spawn, position.y - 70, 0, COLLIDER_PLAYER2_SHOT, hdk_voice, 200);
			}
			break;

		case ST_FALLING2:
			current_animation = &airreel;
			jumpHeight += speed + 3;
			if (jumpHeight == 0)
			{
				inputs.Push(IN_FALLING_FINISH2);
			}
			break;

		case ST_GETTING_UP2:
			current_animation = &getup;
			break;

		case LOOSE2:
			current_animation = &ground;
			break;

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
			}
			break;
		}
	}
	current_state = state;

	// Draw everything --------------------------------------	

	BlitCharacterAndAddColliders(current_animation, texture);

	return UPDATE_CONTINUE;
}

void ModuleDhalsim::ClearColliders() {
	for (int i = 0; i < MAX_COLLIDERS_PER_FRAME; i++)
	{
		if (colliders[i] != nullptr) {
			colliders[i]->to_delete = true;
			colliders[i] = nullptr;
		}
	}
}

bool ModuleDhalsim::IsntOnLeftLimit() {
	return position.x - 34 > -App->render->camera.x / SCREEN_SIZE;
}

bool ModuleDhalsim::IsntOnRightLimit() {
	return position.x + 34 < -App->render->camera.x / SCREEN_SIZE + App->render->camera.w;
}

void ModuleDhalsim::OnCollision(Collider* c1, Collider* c2) {
	if (invulnerabilityFrames < App->frames) {
		if (c1->type == COLLIDER_PLAYER2 && c2->type == COLLIDER_PLAYER_SHOT && (state != ST_JUMP_NEUTRAL2 && state != ST_JUMP_FORWARD2 && state != ST_JUMP_BACKWARD2 && state != L_PUNCH_NEUTRAL_JUMP2 && state != L_PUNCH_FORWARD_JUMP2 && state != L_PUNCH_BACKWARD_JUMP2 && state != L_KIK_NEUTRAL_JUMP2 && state != L_KIK_FORWARD_JUMP2 && state != L_KIK_BACKWARD_JUMP2))
		{
			life -= 12;
			invulnerabilityFrames = 25 + App->frames;
			App->audio->PlayChunk(hdk_hit);

			if (state == ST_CROUCHING2 || state == ST_CROUCH2 || state == ST_STANDING2 || state == L_PUNCH_CROUCH2 || state == L_KIK_CROUCH2)
				inputs.Push(IN_CROUCH_REEL2);

			else
				inputs.Push(IN_HEAD_REEL2);

		}

		if (c1->type == COLLIDER_PLAYER2 && c2->type == COLLIDER_PLAYER_HIT && (state != ST_JUMP_NEUTRAL2 && state != ST_JUMP_FORWARD2 && state != ST_JUMP_BACKWARD2 && state != L_PUNCH_NEUTRAL_JUMP2 && state != L_PUNCH_FORWARD_JUMP2 && state != L_PUNCH_BACKWARD_JUMP2 && state != L_KIK_NEUTRAL_JUMP2 && state != L_KIK_FORWARD_JUMP2 && state != L_KIK_BACKWARD_JUMP2))
		{
			life -= 7;
			invulnerabilityFrames = 25 + App->frames;

			if (App->ryu->state == L_KIK_STANDING || App->ryu->state == L_KIK_NEUTRAL_JUMP || App->ryu->state == L_KIK_FORWARD_JUMP || App->ryu->state == L_KIK_BACKWARD_JUMP)
				App->audio->PlayChunk(high_kick);
			else if (App->ryu->state == L_KIK_CROUCH)
				App->audio->PlayChunk(low_kick);
			else if (App->ryu->state == L_PUNCH_STANDING || App->ryu->state == L_PUNCH_NEUTRAL_JUMP || App->ryu->state == L_PUNCH_FORWARD_JUMP || App->ryu->state == L_PUNCH_BACKWARD_JUMP)
				App->audio->PlayChunk(high_fist);
			else if (App->ryu->state == L_PUNCH_CROUCH)
				App->audio->PlayChunk(low_fist);

			if (state == ST_CROUCHING2 || state == ST_CROUCH2 || state == ST_STANDING2 || state == L_PUNCH_CROUCH2 || state == L_KIK_CROUCH2)
				inputs.Push(IN_CROUCH_REEL2);

			else
				inputs.Push(IN_HEAD_REEL2);
		}

		if (c1->type == COLLIDER_PLAYER2 && c2->type == COLLIDER_PLAYER_SHOT && (state == ST_JUMP_NEUTRAL2 || state == ST_JUMP_FORWARD2 || state == ST_JUMP_BACKWARD2 || state == L_PUNCH_NEUTRAL_JUMP2 || state == L_PUNCH_FORWARD_JUMP2 || state == L_PUNCH_BACKWARD_JUMP2 || state == L_KIK_NEUTRAL_JUMP2 || state == L_KIK_FORWARD_JUMP2 || state == L_KIK_BACKWARD_JUMP2))
		{
			life -= 12;
			invulnerabilityFrames = 25 + App->frames;
			App->audio->PlayChunk(hdk_hit);
			inputs.Push(IN_FALLING2);
		}

		if (c1->type == COLLIDER_PLAYER2 && c2->type == COLLIDER_PLAYER_HIT && (state == ST_JUMP_NEUTRAL2 || state == ST_JUMP_FORWARD2 || state == ST_JUMP_BACKWARD2 || state == L_PUNCH_NEUTRAL_JUMP2 || state == L_PUNCH_FORWARD_JUMP2 || state == L_PUNCH_BACKWARD_JUMP2 || state == L_KIK_NEUTRAL_JUMP2 || state == L_KIK_FORWARD_JUMP2 || state == L_KIK_BACKWARD_JUMP2))
		{
			life -= 7;
			invulnerabilityFrames = 25 + App->frames;

			if (App->ryu->state == L_KIK_STANDING || App->ryu->state == L_KIK_NEUTRAL_JUMP || App->ryu->state == L_KIK_FORWARD_JUMP || App->ryu->state == L_KIK_BACKWARD_JUMP)
				App->audio->PlayChunk(high_kick);
			else if (App->ryu->state == L_KIK_CROUCH)
				App->audio->PlayChunk(low_kick);
			else if (App->ryu->state == L_PUNCH_STANDING || App->ryu->state == L_PUNCH_NEUTRAL_JUMP || App->ryu->state == L_PUNCH_FORWARD_JUMP || App->ryu->state == L_PUNCH_BACKWARD_JUMP)
				App->audio->PlayChunk(high_fist);
			else if (App->ryu->state == L_PUNCH_CROUCH)
				App->audio->PlayChunk(low_fist);

			inputs.Push(IN_FALLING2);
		}
	}
}

void ModuleDhalsim::BlitCharacterAndAddColliders(Animation* current_animation, SDL_Texture *texture) {
	Frame frame = current_animation->GetCurrentFrame();
	SDL_Rect r;
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

	if (flip)
		App->render->Blit(texture, position.x - (r.w - frame.pivotPosition.x), position.y - r.h + frame.pivotPosition.y + jumpHeight, &r, flip);
	else
		App->render->Blit(texture, position.x - frame.pivotPosition.x, position.y - r.h + frame.pivotPosition.y + jumpHeight, &r, flip);
}

bool ModuleDhalsim::external_input(p2Qeue<ryu_inputs2>& inputs)
{
	static bool left = false;
	static bool right = false;
	static bool down = false;
	static bool up = false;

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
			inputs.Push(IN_M_PUNCH2);
		}




		if ((App->input->keyboard[SDL_SCANCODE_KP_4] == KEY_STATE::KEY_DOWN) || (App->input->gameController1States[SDL_CONTROLLER_BUTTON_X] == KEY_DOWN))
		{
 			inputs.Push(IN_L_PUNCH2);
		}

		if ((App->input->keyboard[SDL_SCANCODE_KP_1] == KEY_STATE::KEY_DOWN) || (App->input->gameController1States[SDL_CONTROLLER_BUTTON_A] == KEY_DOWN))
		{
			inputs.Push(IN_L_KIK2);
		}

		if ((App->input->keyboard[SDL_SCANCODE_KP_5] == KEY_STATE::KEY_DOWN) || (App->input->gameController1States[SDL_CONTROLLER_BUTTON_Y] == KEY_DOWN))
		{
			inputs.Push(IN_M_PUNCH2);
		}

		if ((App->input->keyboard[SDL_SCANCODE_KP_2] == KEY_STATE::KEY_DOWN) || (App->input->gameController1States[SDL_CONTROLLER_BUTTON_B] == KEY_DOWN))
		{
			inputs.Push(IN_M_KIK2);
		}

		if ((App->input->keyboard[SDL_SCANCODE_KP_6] == KEY_STATE::KEY_DOWN) || (App->input->gameController1States[SDL_CONTROLLER_BUTTON_RIGHTSHOULDER] == KEY_DOWN))
		{
			inputs.Push(IN_F_PUNCH2);
		}

		if ((App->input->keyboard[SDL_SCANCODE_KP_3] == KEY_STATE::KEY_DOWN) || (App->input->gameController1AxisValues[SDL_CONTROLLER_AXIS_TRIGGERRIGHT] > JOYSTICK_DEAD_ZONE))
		{
			inputs.Push(IN_F_KIK2);
		}

		if (App->input->keyboard[SDL_SCANCODE_0] == KEY_STATE::KEY_DOWN)
		{
			if (state != ST_HADOKEN2)
				hdk.ResetAnimation();
			inputs.Push(IN_HADOKEN2);
		}

		if (/*App->input->keyboard[SDL_SCANCODE_Y] == KEY_STATE::KEY_DOWN || App->input->keyboard[SDL_SCANCODE_Y] == KEY_STATE::KEY_REPEAT*/App->input->gameController1AxisValues[SDL_CONTROLLER_AXIS_LEFTY] < -JOYSTICK_DEAD_ZONE)
		{
			if (state != ST_JUMP_NEUTRAL2 && state != ST_JUMP_FORWARD2 && state != ST_JUMP_BACKWARD2)
			{
				backwardJump.ResetAnimation();
				forwardJump.ResetAnimation();
				neutralJump.ResetAnimation();
			}
			up = true;
		}
		else
		{
			up = false;
		}

		if (/*App->input->keyboard[SDL_SCANCODE_H] == KEY_STATE::KEY_DOWN || App->input->keyboard[SDL_SCANCODE_H] == KEY_STATE::KEY_REPEAT*/ App->input->gameController1AxisValues[SDL_CONTROLLER_AXIS_LEFTY] > JOYSTICK_DEAD_ZONE)
		{
			down = true;
		}
		else {
			inputs.Push(IN_CROUCH_UP2);
			down = false;
		}

		if (/*App->input->keyboard[SDL_SCANCODE_J] == KEY_STATE::KEY_DOWN || App->input->keyboard[SDL_SCANCODE_J] == KEY_STATE::KEY_REPEAT*/ App->input->gameController1AxisValues[SDL_CONTROLLER_AXIS_LEFTX] > JOYSTICK_DEAD_ZONE)
		{

			left = true;
		}
		else
		{
			inputs.Push(IN_LEFT_UP2);
			left = false;
		}

		if (/*App->input->keyboard[SDL_SCANCODE_G] == KEY_STATE::KEY_DOWN || App->input->keyboard[SDL_SCANCODE_G] == KEY_STATE::KEY_REPEAT*/ App->input->gameController1AxisValues[SDL_CONTROLLER_AXIS_LEFTX] < -JOYSTICK_DEAD_ZONE)
		{
			right = true;
		}
		else
		{
			inputs.Push(IN_RIGHT_UP2);
			right = false;
		}

		if (left)
		{
			if (up)
				inputs.Push(IN_LEFT_AND_JUMP2);

			if (down)
				inputs.Push(IN_LEFT_AND_CROUCH2);

			else
				inputs.Push(IN_LEFT_DOWN2);
		}

		if (right)
		{
			if (up)
				inputs.Push(IN_RIGHT_AND_JUMP2);

			if (down)
				inputs.Push(IN_RIGHT_AND_CROUCH2);

			else
				inputs.Push(IN_RIGHT_DOWN2);
		}

		if (up && !right && !left)
		{
			inputs.Push(IN_JUMP2);
		}

		if (down && !right && !left)
		{
			inputs.Push(IN_CROUCH_DOWN2);
		}

		if (!down && !up && !right && !left)
		{
			inputs.Push(IN_IDLE2);
		}
		
	}
	else {
		left = false;
		right = false;
		down = false;
		up = false;
	}

	return true;
}

void ModuleDhalsim::internal_input(p2Qeue<ryu_inputs2>& inputs)
{
	if (jump_timer > 0)
	{
		if (App->frames - jump_timer > D_JUMP_TIME)
		{
			inputs.Push(IN_JUMP_FINISH2);
			jump_timer = 0;
		}
	}

	//light
	if (l_standing_punch_timer > 0)
	{
		if (App->frames - l_standing_punch_timer > D_L_STANDING_PUNCH_TIME)
		{
			inputs.Push(IN_PUNCH_FINISH2);
			l_standing_punch_timer = 0;
		}
	}

	if (l_crouching_punch_timer > 0)
	{
		if (App->frames - l_crouching_punch_timer > D_L_CROUCHING_PUNCH_TIME)
		{
			inputs.Push(IN_PUNCH_FINISH2);
			l_crouching_punch_timer = 0;
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
			l_standing_kik_timer = 0;
		}
	}

	if (l_crouching_kik_timer > 0)
	{
		if (App->frames - l_crouching_kik_timer > D_L_CROUCHING_KIK_TIME)
		{
			inputs.Push(IN_KIK_FINISH2);
			l_crouching_kik_timer = 0;
		}
	}

	if (l_d_jumping_kik_timer > 0)
	{
		if (App->frames - l_d_jumping_kik_timer > D_L_D_JUMPING_KIK_TIME)
		{
			inputs.Push(IN_KIK_FINISH2);
			l_d_jumping_kik_timer = 0;
		}
	}

	//medium
	if (m_standing_punch_timer > 0)
	{
		if (SDL_GetTicks() - m_standing_punch_timer > D_M_STANDING_PUNCH_TIME)
		{
			inputs.Push(IN_PUNCH_FINISH2);
			m_standing_punch_timer = 0;
		}
	}

	if (m_crouching_punch_timer > 0)
	{
		if (SDL_GetTicks() - m_crouching_punch_timer > D_M_CROUCHING_PUNCH_TIME)
		{
			inputs.Push(IN_PUNCH_FINISH2);
			m_crouching_punch_timer = 0;
		}
	}

	if (m_d_jumping_punch_timer > 0)
	{
		if (SDL_GetTicks() - m_d_jumping_punch_timer > D_M_D_JUMPING_PUNCH_TIME)
		{
			inputs.Push(IN_PUNCH_FINISH2);
			m_d_jumping_punch_timer = 0;
		}
	}

	if (m_standing_kik_timer > 0)
	{
		if (SDL_GetTicks() - m_standing_kik_timer > D_M_STANDING_KIK_TIME)
		{
			inputs.Push(IN_KIK_FINISH2);
			m_standing_kik_timer = 0;
		}
	}

	if (m_crouching_kik_timer > 0)
	{
		if (SDL_GetTicks() - m_crouching_kik_timer > D_M_CROUCHING_KIK_TIME)
		{
			inputs.Push(IN_KIK_FINISH2);
			m_crouching_kik_timer = 0;
		}
	}

	if (m_d_jumping_kik_timer > 0)
	{
		if (SDL_GetTicks() - m_d_jumping_kik_timer > D_M_D_JUMPING_KIK_TIME)
		{
			inputs.Push(IN_KIK_FINISH2);
			m_d_jumping_kik_timer = 0;
		}
	}

	//fierce
	if (f_standing_punch_timer > 0)
	{
		if (SDL_GetTicks() - f_standing_punch_timer > D_F_STANDING_PUNCH_TIME)
		{
			inputs.Push(IN_PUNCH_FINISH2);
			f_standing_punch_timer = 0;
		}
	}

	if (f_crouching_punch_timer > 0)
	{
		if (SDL_GetTicks() - f_crouching_punch_timer > D_F_CROUCHING_PUNCH_TIME)
		{
			inputs.Push(IN_PUNCH_FINISH2);
			f_crouching_punch_timer = 0;
		}
	}

	if (f_d_jumping_punch_timer > 0)
	{
		if (SDL_GetTicks() - f_d_jumping_punch_timer > D_F_D_JUMPING_PUNCH_TIME)
		{
			inputs.Push(IN_PUNCH_FINISH2);
			f_d_jumping_punch_timer = 0;
		}
	}

	if (f_standing_kik_timer > 0)
	{
		if (SDL_GetTicks() - f_standing_kik_timer > D_F_STANDING_KIK_TIME)
		{
			inputs.Push(IN_KIK_FINISH2);
			f_standing_kik_timer = 0;
		}
	}

	if (f_crouching_kik_timer > 0)
	{
		if (SDL_GetTicks() - f_crouching_kik_timer > D_F_CROUCHING_KIK_TIME)
		{
			inputs.Push(IN_KIK_FINISH2);
			f_crouching_kik_timer = 0;
		}
	}

	if (f_d_jumping_kik_timer > 0)
	{
		if (SDL_GetTicks() - f_d_jumping_kik_timer > D_F_D_JUMPING_KIK_TIME)
		{
			inputs.Push(IN_KIK_FINISH2);
			f_d_jumping_kik_timer = 0;
		}
	}


	if (hadoken_timer > 0)
	{
		if (App->frames - hadoken_timer > D_HADOKEN_TIME)
		{
			inputs.Push(IN_HADOKEN_FINISH2);
			hadoken_timer = 0;
		}
	}

	if (crouching_timer > 0)
	{
		if (App->frames - crouching_timer > D_CROUCHING_TIME)
		{
			inputs.Push(IN_CROUCHING_FINISH2);
			crouching_timer = 0;
		}
	}

	if (standing_timer > 0)
	{
		if (App->frames - standing_timer > D_STANDING_TIME)
		{
			inputs.Push(IN_STANDING_FINISH2);
			standing_timer = 0;
		}
	}

	if (head_reel_timer > 0)
	{
		if (App->frames - head_reel_timer > D_HEAD_REEL_TIME)
		{
			inputs.Push(IN_REEL_FINISH2);
			head_reel_timer = 0;
		}
	}

	if (gut_reel_timer > 0)
	{
		if (App->frames - gut_reel_timer > D_GUT_REEL_TIME)
		{
			inputs.Push(IN_REEL_FINISH2);
			gut_reel_timer = 0;
		}
	}

	if (crouch_reel_timer > 0)
	{
		if (App->frames - crouch_reel_timer > D_CROUCH_REEL_TIME)
		{
			inputs.Push(IN_REEL_FINISH2);
			crouch_reel_timer = 0;
		}
	}

	if (getting_up_timer > 0)
	{
		if (App->frames - getting_up_timer > D_GETTING_UP_TIME)
		{
			inputs.Push(IN_GETTING_UP_FINISH2);
			getting_up_timer = 0;
		}
	}

	if (defending_timer > 0)
	{
		if (SDL_GetTicks() - defending_timer > D_DEFENDING_TIME)
		{
			inputs.Push(IN_DEFENDING_FINISH2);
			defending_timer = 0;
		}
	}

}

ryu_states2 ModuleDhalsim::process_fsm(p2Qeue<ryu_inputs2>& inputs)
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

			case IN_L_PUNCH2: state = L_PUNCH_STANDING2; l_standing_punch_timer = App->frames;  break;
			case IN_L_KIK2: state = L_KIK_STANDING2; l_standing_kik_timer = App->frames;  break;

			case IN_M_PUNCH2: state = M_PUNCH_STANDING2; m_standing_punch_timer = SDL_GetTicks();  break;
			case IN_M_KIK2: state = M_KIK_STANDING2; m_standing_kik_timer = SDL_GetTicks();  break;

			case IN_F_PUNCH2: state = F_PUNCH_STANDING2; f_standing_punch_timer = SDL_GetTicks();  break;
			case IN_F_KIK2: state = F_KIK_STANDING2; f_standing_kik_timer = SDL_GetTicks();  break;

			case IN_HADOKEN2: state = ST_HADOKEN2; hadoken_timer = App->frames; break;
			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;

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
			case IN_JUMP2: state = ST_JUMP_FORWARD2; jump_timer = App->frames;  break;
			case IN_CROUCH_DOWN2: state = ST_CROUCH2; break;

			case IN_L_PUNCH2: state = L_PUNCH_STANDING2; l_standing_punch_timer = App->frames;  break;
			case IN_L_KIK2: state = L_KIK_STANDING2; l_standing_kik_timer = App->frames;  break;

			case IN_M_PUNCH2: state = M_PUNCH_STANDING2; m_standing_punch_timer = SDL_GetTicks();  break;
			case IN_M_KIK2: state = M_KIK_STANDING2; m_standing_kik_timer = SDL_GetTicks();  break;

			case IN_F_PUNCH2: state = F_PUNCH_STANDING2; f_standing_punch_timer = SDL_GetTicks();  break;
			case IN_F_KIK2: state = F_KIK_STANDING2; f_standing_kik_timer = SDL_GetTicks();  break;

			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;

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
			case IN_JUMP2: state = ST_JUMP_BACKWARD2; jump_timer = App->frames;  break;
			case IN_CROUCH_DOWN2: state = ST_CROUCH2; break;

			case IN_L_PUNCH2: state = L_PUNCH_STANDING2; l_standing_punch_timer = App->frames;  break;
			case IN_L_KIK2: state = L_KIK_STANDING2; l_standing_kik_timer = App->frames;  break;

			case IN_M_PUNCH2: state = M_PUNCH_STANDING2; m_standing_punch_timer = SDL_GetTicks();  break;
			case IN_M_KIK2: state = M_KIK_STANDING2; m_standing_kik_timer = SDL_GetTicks();  break;

			case IN_F_PUNCH2: state = F_PUNCH_STANDING2; f_standing_punch_timer = SDL_GetTicks();  break;
			case IN_F_KIK2: state = F_KIK_STANDING2; f_standing_kik_timer = SDL_GetTicks();  break;

			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;

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

			case IN_F_PUNCH2: state = F_PUNCH_NEUTRAL_JUMP2; break;
			case IN_F_KIK2: state = F_KIK_NEUTRAL_JUMP2; break;

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

			case IN_M_PUNCH2: state = M_PUNCH_FORWARD_JUMP2; m_d_jumping_punch_timer = SDL_GetTicks(); break;
			case IN_M_KIK2: state = M_KIK_FORWARD_JUMP2; m_d_jumping_kik_timer = SDL_GetTicks(); break;

			case IN_F_PUNCH2: state = F_PUNCH_FORWARD_JUMP2; f_d_jumping_punch_timer = SDL_GetTicks(); break;
			case IN_F_KIK2: state = F_KIK_FORWARD_JUMP2; f_d_jumping_kik_timer = SDL_GetTicks(); break;

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

			case IN_M_PUNCH2: state = M_PUNCH_BACKWARD_JUMP2; m_d_jumping_punch_timer = SDL_GetTicks(); break;
			case IN_M_KIK2: state = M_KIK_BACKWARD_JUMP2; m_d_jumping_kik_timer = SDL_GetTicks(); break;

			case IN_F_PUNCH2: state = F_PUNCH_BACKWARD_JUMP2; f_d_jumping_punch_timer = SDL_GetTicks(); break;
			case IN_F_KIK2: state = F_KIK_BACKWARD_JUMP2; f_d_jumping_kik_timer = SDL_GetTicks(); break;

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

		case ST_HADOKEN2:
		{
			switch (last_input)
			{
			case IN_HADOKEN_FINISH2: state = ST_IDLE2; break;
			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;
			case IN_VICTORY2: state = VICTORY2; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case L_PUNCH_FORWARD_JUMP2:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH2: state = ST_IDLE2; break;
			case IN_PUNCH_FINISH2: state = ST_JUMP_FORWARD2; break;
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
			case IN_KIK_FINISH2: state = ST_JUMP_FORWARD2; break;
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
			case IN_PUNCH_FINISH2: state = ST_JUMP_FORWARD2; break;
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
			case IN_KIK_FINISH2: state = ST_JUMP_FORWARD2; break;
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
			case IN_PUNCH_FINISH2: state = ST_JUMP_FORWARD2; break;
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
			case IN_KIK_FINISH2: state = ST_JUMP_FORWARD2; break;
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
			case IN_PUNCH_FINISH2: state = ST_JUMP_BACKWARD2; break;
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
			case IN_KIK_FINISH2: state = ST_JUMP_BACKWARD2; break;
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
			case IN_PUNCH_FINISH2: state = ST_JUMP_BACKWARD2; break;
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
			case IN_KIK_FINISH2: state = ST_JUMP_BACKWARD2; break;
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
			case IN_PUNCH_FINISH2: state = ST_JUMP_BACKWARD2; break;
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
			case IN_KIK_FINISH2: state = ST_JUMP_BACKWARD2; break;
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
			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = SDL_GetTicks(); break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = SDL_GetTicks(); break;
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
			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = SDL_GetTicks(); break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = SDL_GetTicks(); break;
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
			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = SDL_GetTicks(); break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = SDL_GetTicks(); break;
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
			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = SDL_GetTicks(); break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = SDL_GetTicks(); break;
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
			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; crouch_reel_timer = App->frames; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case ST_STANDING2:
		{
			switch (last_input)
			{
			case IN_STANDING_FINISH2: state = ST_IDLE2; break;
			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; crouch_reel_timer = App->frames; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case ST_CROUCH2:
		{
			switch (last_input)
			{
			case IN_CROUCH_UP2: state = ST_STANDING2; standing_timer = App->frames; break;

			case IN_L_PUNCH2: state = L_PUNCH_CROUCH2; l_crouching_punch_timer = App->frames; break;
			case IN_L_KIK2: state = L_KIK_CROUCH2; l_crouching_kik_timer = App->frames; break;

			case IN_M_PUNCH2: state = M_PUNCH_CROUCH2; m_crouching_punch_timer = SDL_GetTicks(); break;
			case IN_M_KIK2: state = M_KIK_CROUCH2; m_crouching_kik_timer = SDL_GetTicks(); break;

			case IN_F_PUNCH2: state = F_PUNCH_CROUCH2; f_crouching_punch_timer = SDL_GetTicks(); break;
			case IN_F_KIK2: state = F_KIK_CROUCH2; f_crouching_kik_timer = SDL_GetTicks(); break;

			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; crouch_reel_timer = App->frames; break;

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
			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; crouch_reel_timer = App->frames; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case L_KIK_CROUCH2:
		{
			switch (last_input)
			{
			case IN_KIK_FINISH2: state = ST_CROUCH2; break;
			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; crouch_reel_timer = App->frames; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case M_PUNCH_CROUCH2:
		{
			switch (last_input)
			{
			case IN_PUNCH_FINISH2: state = ST_CROUCH2; break;
			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; crouch_reel_timer = SDL_GetTicks(); break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case M_KIK_CROUCH2:
		{
			switch (last_input)
			{
			case IN_KIK_FINISH2: state = ST_CROUCH2; break;
			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; crouch_reel_timer = SDL_GetTicks(); break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case F_PUNCH_CROUCH2:
		{
			switch (last_input)
			{
			case IN_PUNCH_FINISH2: state = ST_CROUCH2; break;
			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; crouch_reel_timer = SDL_GetTicks(); break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case F_KIK_CROUCH2:
		{
			switch (last_input)
			{
			case IN_KIK_FINISH2: state = ST_CROUCH2; break;
			case IN_CROUCH_REEL2: state = ST_CROUCH_REEL2; crouch_reel_timer = SDL_GetTicks(); break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case ST_HEAD_REEL2:
		{
			switch (last_input)
			{
			case IN_REEL_FINISH2:state = ST_IDLE2; break;
			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case ST_GUT_REEL2:
		{
			switch (last_input)
			{
			case IN_REEL_FINISH2:state = ST_IDLE2; break;
			case IN_HEAD_REEL2: state = ST_HEAD_REEL2; head_reel_timer = App->frames; break;
			case IN_GUT_REEL2: state = ST_GUT_REEL2; gut_reel_timer = App->frames; break;
			case IN_LOOSE2: state = LOOSE2; break;
			}
		}
		break;

		case ST_CROUCH_REEL2:
		{
			switch (last_input)
			{
			case IN_REEL_FINISH2:state = ST_CROUCH2; break;
			case IN_HEAD_REEL2: state = ST_CROUCH_REEL2; crouch_reel_timer = App->frames; break;
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
			case IN_GETTING_UP_FINISH2:state = ST_IDLE2; break;
			}
		}
		break;

		case ST_DEFENDING2:
		{
			switch (last_input)
			{
			case IN_DEFENDING_FINISH2:state = ST_IDLE2; break;
			case IN_GUT_REEL:state = ST_GUT_REEL2; break;
			}
		}
		break;

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
