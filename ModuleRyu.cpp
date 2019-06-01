#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleRyu.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"
#include "ModuleDhalsim.h"
#include "p2Qeue.h"

#include "SDL\include\SDL.h"


ModuleRyu::ModuleRyu()
{
}

ModuleRyu::~ModuleRyu()
{}

// Load assets
bool ModuleRyu::Start()
{
	LOG("Loading ryu textures");
	bool ret = true;
	graphics = App->textures->Load("assets/images/sprites/characters/ryu1.png"); // arcade version
	graphics2 = App->textures->Load("assets/images/sprites/characters/ryu2-ken.png"); // arcade version

	hdk_voice = App->audio->LoadChunk("assets/sfx/voices/ryu_ken_hadouken.wav");
	hdk_hit = App->audio->LoadChunk("assets/sfx/effects/fist_intro.wav");
	low_kick = App->audio->LoadChunk("assets/sfx/effects/low_kick.wav");
	low_fist = App->audio->LoadChunk("assets/sfx/effects/low_fist.wav");
	high_fist = App->audio->LoadChunk("assets/sfx/effects/high_fist.wav");
	high_kick = App->audio->LoadChunk("assets/sfx/effects/high_kick.wav");

	position.x = 100;
	position.y = 215;

	life = 100;
	freeze = true;
	victoryExecuted = invulnerabilityFrames = puntuation = 0;

	Animation* current_animation;


	// idle animation (arcade sprite sheet)
	const int idlenColliders = 3;
	SDL_Rect idleHitbox[idlenColliders] = { { -25, 79, 24, 16}, { -6, 37, 40, 47}, { -6, 5, 40, 32} };
	COLLIDER_TYPE idleColliderType[idlenColliders] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER} };
	Module* idleCallback[idlenColliders] = { {this}, {this}, {this} };

	idle.PushBack({ 348, 3, 61, 95 }, 6, { 33,5 }, idlenColliders, idleHitbox, idleColliderType, idleCallback);
	idle.PushBack({ 408, 3, 60, 95 }, 6, { 33,5 }, idlenColliders, idleHitbox, idleColliderType, idleCallback);
	idle.PushBack({ 468, 3, 58, 95 }, 6, { 33,5 }, idlenColliders, idleHitbox, idleColliderType, idleCallback);
	idle.PushBack({ 526, 3, 55, 95 }, 6, { 33,5 }, idlenColliders, idleHitbox, idleColliderType, idleCallback);


	// walk forward animation (arcade sprite sheet)
	const int forwardnColliders = 3;
	SDL_Rect forwardHitbox[forwardnColliders] = { { -28, 79, 24, 16}, { -6, 37, 40, 47}, { -6, 5, 40, 32} };
	COLLIDER_TYPE forwardColliderType[forwardnColliders] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER} };
	Module* forwardCallback[forwardnColliders] = { {this}, {this}, {this} };

	forward.PushBack({ 1, 3, 53, 94 }, 6, { 35,5 }, forwardnColliders, forwardHitbox, forwardColliderType, forwardCallback);
	forward.PushBack({ 53, 3, 62, 95 }, 6, { 35,5 }, forwardnColliders, forwardHitbox, forwardColliderType, forwardCallback);
	forward.PushBack({ 115, 3, 63, 95 }, 6, { 35,5 }, forwardnColliders, forwardHitbox, forwardColliderType, forwardCallback);
	forward.PushBack({ 179, 3, 64, 94 }, 6, { 35,5 }, forwardnColliders, forwardHitbox, forwardColliderType, forwardCallback);
	forward.PushBack({ 243, 3, 54, 95 }, 6, { 35,5 }, forwardnColliders, forwardHitbox, forwardColliderType, forwardCallback);
	forward.PushBack({ 298, 3, 49, 94 }, 6, { 35,5 }, forwardnColliders, forwardHitbox, forwardColliderType, forwardCallback);


	// walk backward animation (arcade sprite sheet)
	const int backwardnColliders = 3;
	SDL_Rect backwardHitbox[backwardnColliders] = { { -18, 79, 24, 16}, { -6, 37, 40, 47}, { -6, 5, 40, 32} };
	COLLIDER_TYPE backwardColliderType[backwardnColliders] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER} };
	Module* backwardCallback[backwardnColliders] = { {this}, {this}, {this} };

	backward.PushBack({ 694, 3, 56, 93 }, 6, { 34,5 }, backwardnColliders, backwardHitbox, backwardColliderType, backwardCallback);
	backward.PushBack({ 752, 3, 60, 93 }, 6, { 34,5 }, backwardnColliders, backwardHitbox, backwardColliderType, backwardCallback);
	backward.PushBack({ 813, 3, 58, 93 }, 6, { 34,5 }, backwardnColliders, backwardHitbox, backwardColliderType, backwardCallback);
	backward.PushBack({ 872, 3, 56, 93 }, 6, { 34,5 }, backwardnColliders, backwardHitbox, backwardColliderType, backwardCallback);
	backward.PushBack({ 930, 3, 57, 93 }, 6, { 34,5 }, backwardnColliders, backwardHitbox, backwardColliderType, backwardCallback);
	backward.PushBack({ 0, 102, 57, 92 }, 6, { 34,5 }, backwardnColliders, backwardHitbox, backwardColliderType, backwardCallback);


	// lp

	const int lpnColliders = 3;
	const int lpnColliders2 = 4;
	SDL_Rect lpHitbox[lpnColliders] = { { -25, 79, 24, 16}, { -6, 37, 40, 47}, { -6, 5, 40, 32} };
	SDL_Rect lpHitbox2[lpnColliders2] = { { -25, 79, 24, 16}, { -6, 37, 40, 47}, { -6, 5, 40, 32}, { -43, 75, 50, 18} };
	COLLIDER_TYPE lpColliderType[lpnColliders] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER} };
	COLLIDER_TYPE lpColliderType2[lpnColliders2] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER_HIT} };
	Module* lpCallback[lpnColliders] = { {this}, {this}, {this} };
	Module* lpCallback2[lpnColliders2] = { {this}, {this}, {this}, {(Module*)App->dhalsim} };
	lp.PushBack({ 59, 101, 64, 95 }, 5, { 32,5 }, lpnColliders, lpHitbox, lpColliderType, lpCallback);
	lp.PushBack({ 124, 101, 92, 95 }, 10, { 32,5 }, lpnColliders2, lpHitbox2, lpColliderType2, lpCallback2);
	lp.PushBack({ 59, 101, 64, 95 }, 8, { 32,5 }, lpnColliders, lpHitbox, lpColliderType, lpCallback);
	lp.PushBack({ 409, 3, 58, 95 }, 2, { 33,5 }, lpnColliders, lpHitbox, lpColliderType, lpCallback);


	// lk
	const int lknColliders = 3;
	const int lknColliders2 = 3;
	const int lknColliders3 = 4;
	SDL_Rect lkHitbox[lknColliders] = { { -25, 79, 24, 16}, { -6, 37, 40, 47}, { -6, 5, 40, 32} };
	SDL_Rect lkHitbox2[lknColliders2] = { { -7, 75, 30, 20}, { 9, 36, 42, 52}, { 0, 22, 44, 50} };
	SDL_Rect lkHitbox3[lknColliders3] = { { -5, 75, 29, 16}, { -13, 37, 40, 47}, { -38, 0, 40, 42}, { -50, 67, 67, 26} };
	COLLIDER_TYPE lkColliderType[lknColliders] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER} };
	COLLIDER_TYPE lkColliderType3[lknColliders3] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER_HIT} };
	Module* lkCallback[lknColliders] = { {this}, {this}, {this} };
	Module* lkCallback3[lknColliders3] = { {this}, {this}, {this}, {(Module*)App->dhalsim} };
	lk.PushBack({ 1, 3, 53, 94 }, 7, { 29,5 }, lknColliders, lkHitbox, lkColliderType, lkCallback);
	lk.PushBack({ 400, 102, 63, 93 }, 7, { 46,5 }, lknColliders2, lkHitbox2, lkColliderType, lkCallback);
	lk.PushBack({ 467, 100, 115, 93 }, 20, { 68,5 }, lknColliders3, lkHitbox3, lkColliderType3, lkCallback3);
	lk.PushBack({ 400, 102, 63, 93 }, 10, { 46,5 }, lknColliders2, lkHitbox2, lkColliderType, lkCallback);
	lk.PushBack({ 1, 3, 53, 94 }, 2, { 29,5 }, lknColliders, lkHitbox, lkColliderType, lkCallback);

	// Neutral jump
	const int neutraljumpnColliders = 3;
	SDL_Rect neutraljumpHitbox[neutraljumpnColliders] = { { -25, 79, 24, 16}, { -6, 37, 40, 47}, { -6, 5, 40, 32} };
	COLLIDER_TYPE neutraljumpColliderType[neutraljumpnColliders] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER} };
	Module* neutraljumpCallback[neutraljumpnColliders] = { {this}, {this}, {this} };
	neutralJump.PushBack({ 195, 512, 55, 105 }, 4, { 29,5 }, neutraljumpnColliders, neutraljumpHitbox, neutraljumpColliderType, neutraljumpCallback);		// La velocidad es adecuada, pero las animaciones están mal / leen mal el tiempo
	neutralJump.PushBack({ 252, 528, 50, 89 }, 4, { 29,5 }, neutraljumpnColliders, neutraljumpHitbox, neutraljumpColliderType, neutraljumpCallback);
	neutralJump.PushBack({ 303, 540, 54, 77 }, 8, { 29,5 }, neutraljumpnColliders, neutraljumpHitbox, neutraljumpColliderType, neutraljumpCallback);
	neutralJump.PushBack({ 358, 547, 48, 70 }, 12, { 29,5 }, neutraljumpnColliders, neutraljumpHitbox, neutraljumpColliderType, neutraljumpCallback);
	neutralJump.PushBack({ 407, 528, 48, 89 }, 18, { 29,5 }, neutraljumpnColliders, neutraljumpHitbox, neutraljumpColliderType, neutraljumpCallback);
	neutralJump.PushBack({ 195, 512, 55, 105 }, 4, { 29,5 }, neutraljumpnColliders, neutraljumpHitbox, neutraljumpColliderType, neutraljumpCallback);


	// forward jump
	const int forwardjumpnColliders = 3;
	SDL_Rect forwardjumpHitbox[forwardjumpnColliders] = { { -25, 79, 24, 16}, { -6, 37, 40, 47}, { -6, 5, 40, 32} };
	COLLIDER_TYPE forwardjumpColliderType[forwardjumpnColliders] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER} };
	Module* forwardjumpCallback[forwardjumpnColliders] = { {this}, {this}, {this} };

	forwardJump.PushBack({ 641, 403, 55, 112 }, 8, { 29,5 }, forwardjumpnColliders, forwardjumpHitbox, forwardjumpColliderType, forwardjumpCallback);		// La velocidad es adecuada, pero las animaciones están mal / leen mal el tiempo
	forwardJump.PushBack({ 697, 410, 54, 102 }, 6, { 29,5 }, forwardjumpnColliders, forwardjumpHitbox, forwardjumpColliderType, forwardjumpCallback);
	forwardJump.PushBack({ 753, 435, 60, 77 }, 3, { 29,5 }, forwardjumpnColliders, forwardjumpHitbox, forwardjumpColliderType, forwardjumpCallback);
	forwardJump.PushBack({ 815, 469, 104, 43 }, 4, { 29,5 }, forwardjumpnColliders, forwardjumpHitbox, forwardjumpColliderType, forwardjumpCallback);
	forwardJump.PushBack({ 920, 430, 51, 82 }, 4, { 29,5 }, forwardjumpnColliders, forwardjumpHitbox, forwardjumpColliderType, forwardjumpCallback);
	forwardJump.PushBack({ 0, 573, 122, 43 }, 7, { 29,5 }, forwardjumpnColliders, forwardjumpHitbox, forwardjumpColliderType, forwardjumpCallback);
	forwardJump.PushBack({ 123, 530, 70, 86 }, 8, { 29,5 }, forwardjumpnColliders, forwardjumpHitbox, forwardjumpColliderType, forwardjumpCallback);
	forwardJump.PushBack({ 195, 512, 55, 105 }, 8, { 29,5 }, forwardjumpnColliders, forwardjumpHitbox, forwardjumpColliderType, forwardjumpCallback);


	// backward jump
	const int backwardjumpnColliders = 3;
	SDL_Rect backwardjumpHitbox[backwardjumpnColliders] = { { -25, 79, 24, 16}, { -6, 37, 40, 47}, { -6, 5, 40, 32} };
	COLLIDER_TYPE backwardjumpColliderType[backwardjumpnColliders] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER} };
	Module* backwardjumpCallback[backwardjumpnColliders] = { {this}, {this}, {this} };

	backwardJump.PushBack({ 641, 403, 55, 112 }, 8, { 29,5 }, backwardjumpnColliders, backwardjumpHitbox, backwardjumpColliderType, backwardjumpCallback);		// La velocidad es adecuada, pero las animaciones están mal / leen mal el tiempo
	backwardJump.PushBack({ 195, 512, 55, 105 }, 8, { 29,5 }, backwardjumpnColliders, backwardjumpHitbox, backwardjumpColliderType, backwardjumpCallback);
	backwardJump.PushBack({ 123, 530, 70, 86 }, 6, { 29,5 }, backwardjumpnColliders, backwardjumpHitbox, backwardjumpColliderType, backwardjumpCallback);
	backwardJump.PushBack({ 0, 573, 122, 43 }, 2, { 29,5 }, backwardjumpnColliders, backwardjumpHitbox, backwardjumpColliderType, backwardjumpCallback);
	backwardJump.PushBack({ 920, 430, 51, 82 }, 4, { 29,5 }, backwardjumpnColliders, backwardjumpHitbox, backwardjumpColliderType, backwardjumpCallback);
	backwardJump.PushBack({ 815, 469, 104, 43 }, 6, { 29,5 }, backwardjumpnColliders, backwardjumpHitbox, backwardjumpColliderType, backwardjumpCallback);
	backwardJump.PushBack({ 753, 435, 60, 77 }, 8, { 29,5 }, backwardjumpnColliders, backwardjumpHitbox, backwardjumpColliderType, backwardjumpCallback);
	backwardJump.PushBack({ 697, 410, 54, 102 }, 6, { 29,5 }, backwardjumpnColliders, backwardjumpHitbox, backwardjumpColliderType, backwardjumpCallback);

	// Jumping neutral lp
	const int jlpnColliders = 3;
	const int jlpnColliders2 = 4;
	SDL_Rect jlpHitbox[jlpnColliders] = { { -28, 54, 24, 16}, { 0, 27, 48, 30}, { 0, 0, 48, 27} };
	SDL_Rect jlpHitbox2[jlpnColliders2] = { { -28, 54, 24, 16}, { 0, 27, 48, 30}, { 0, 0, 48, 27}, {-48, 28, 35, 30} };
	COLLIDER_TYPE jlpColliderType[jlpnColliders] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER} };
	COLLIDER_TYPE jlpColliderType2[jlpnColliders2] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER_HIT} };
	Module*jlpCallback[jlpnColliders] = { {this}, {this}, {this} };
	Module*jlpCallback2[jlpnColliders2] = { {this}, {this}, {this}, {(Module*)App->dhalsim} };

	jlp.PushBack({ 455, 547, 52, 69 }, 2, { 29,5 }, { jlpnColliders }, { jlpHitbox }, { jlpColliderType }, { jlpCallback });
	jlp.PushBack({ 508, 545, 82, 72 }, 100, { 29,5 }, { jlpnColliders2 }, { jlpHitbox2 }, { jlpColliderType2 }, { jlpCallback2 });


	// Jumping backward lp
	const int jblpnColliders = 3;
	const int jblpnColliders2 = 4;
	SDL_Rect jblpHitbox[jblpnColliders] = { { -28, 54, 24, 16}, { 0, 27, 48, 30}, { 0, 0, 48, 27} };
	SDL_Rect jblpHitbox2[jblpnColliders2] = { { -28, 54, 24, 16}, { 0, 27, 48, 30}, { 0, 0, 48, 27}, {-48, 28, 35, 30} };
	COLLIDER_TYPE jblpColliderType[jblpnColliders] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER} };
	COLLIDER_TYPE jblpColliderType2[jblpnColliders2] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER_HIT} };
	Module*jblpCallback[jblpnColliders] = { {this}, {this}, {this} };
	Module*jblpCallback2[jblpnColliders2] = { {this}, {this}, {this}, {(Module*)App->dhalsim} };

	jblp.PushBack({ 455, 547, 52, 69 }, 2, { 29,5 }, { jblpnColliders }, { jblpHitbox }, { jblpColliderType }, { jblpCallback });
	jblp.PushBack({ 508, 545, 82, 72 }, 100, { 29,5 }, { jblpnColliders2 }, { jblpHitbox2 }, { jblpColliderType2 }, { jblpCallback2 });

	// Jumping forward lp
	const int jflpnColliders = 3;
	const int jflpnColliders2 = 4;
	SDL_Rect jflpHitbox[jflpnColliders] = { { -28, 54, 24, 16}, { 0, 27, 48, 30}, { 0, 0, 48, 27} };
	SDL_Rect jflpHitbox2[jflpnColliders2] = { { -28, 54, 24, 16}, { 0, 27, 48, 30}, { 0, 0, 48, 27}, {-48, 28, 35, 30} };
	COLLIDER_TYPE jflpColliderType[jflpnColliders] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER} };
	COLLIDER_TYPE jflpColliderType2[jflpnColliders2] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER_HIT} };
	Module*jflpCallback[jflpnColliders] = { {this}, {this}, {this} };
	Module*jflpCallback2[jflpnColliders2] = { {this}, {this}, {this}, {(Module*)App->dhalsim} };

	jflp.PushBack({ 455, 547, 52, 69 }, 2, { 29,5 }, { jflpnColliders }, { jflpHitbox }, { jflpColliderType }, { jflpCallback });
	jflp.PushBack({ 508, 545, 82, 72 }, 100, { 29,5 }, { jflpnColliders2 }, { jflpHitbox2 }, { jflpColliderType2 }, { jflpCallback2 });



	// Jumping neutral lk
	const int jlknColliders = 3;
	const int jlknColliders2 = 4;
	SDL_Rect jlkHitbox[jlknColliders] = { { -28, 80, 24, 16}, { 0, 50, 48, 30}, { 0, 25, 48, 27} };
	SDL_Rect jlkHitbox2[jlknColliders2] = { { -28, 80, 24, 16}, { 0, 50, 48, 30}, { 0, 25, 48, 27}, {-10, 50, 65, 40} };
	COLLIDER_TYPE jlkColliderType[jlknColliders] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER} };
	COLLIDER_TYPE jlkColliderType2[jlknColliders2] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER_HIT} };
	Module*jlkCallback[jlknColliders] = { {this}, {this}, {this} };
	Module*jlkCallback2[jlknColliders2] = { {this}, {this}, {this}, {(Module*)App->dhalsim} };

	jlk.PushBack({ 252, 528, 49, 88 }, 3, { 29,5 }, { jlknColliders }, { jlkHitbox }, { jlkColliderType }, { jlkCallback });
	jlk.PushBack({ 948, 525, 75, 90 }, 100, { 29,5 }, { jlknColliders2 }, { jlkHitbox2 }, { jlkColliderType2 }, { jlkCallback2 });


	// Jumping backward lk
	const int jblknColliders = 3;
	const int jblknColliders2 = 4;
	SDL_Rect jblkHitbox[jblknColliders] = { { -22, 55, 30, 16}, { 0, 25, 48, 30}, { 0, 0, 48, 27} };
	SDL_Rect jblkHitbox2[jblknColliders2] = { { -32, 50, 30, 16}, { -10, 20, 48, 30}, { -10, -5, 48, 27}, {-20, 2, 57, 30} };
	COLLIDER_TYPE jblkColliderType[jblknColliders] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER} };
	COLLIDER_TYPE jblkColliderType2[jblknColliders2] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER_HIT} };
	Module*jblkCallback[jblknColliders] = { {this}, {this}, {this} };
	Module*jblkCallback2[jblknColliders2] = { {this}, {this}, {this}, {(Module*)App->dhalsim} };

	jblk.PushBack({ 750, 540, 58, 77 }, 2, { 29,5 }, { jblknColliders }, { jblkHitbox }, { jblkColliderType }, { jblkCallback });
	jblk.PushBack({ 810, 543, 55, 73 }, 3, { 29,5 }, { jblknColliders }, { jblkHitbox }, { jblkColliderType }, { jblkCallback });
	jblk.PushBack({ 865, 550, 77, 66 }, 100, { 29,5 }, { jblknColliders2 }, { jblkHitbox2 }, { jblkColliderType2 }, { jblkCallback2 });


	// Jumping forward lk
	const int jflknColliders = 3;
	const int jflknColliders2 = 4;
	SDL_Rect jflkHitbox[jflknColliders] = { { -22, 55, 30, 16}, { 0, 25, 48, 30}, { 0, 0, 48, 27} };
	SDL_Rect jflkHitbox2[jflknColliders2] = { { -32, 50, 30, 16}, { -10, 20, 48, 30}, { -10, -5, 48, 27}, {-20, 2, 57, 30} };
	COLLIDER_TYPE jflkColliderType[jflknColliders] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER} };
	COLLIDER_TYPE jflkColliderType2[jflknColliders2] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER_HIT} };
	Module*jflkCallback[jflknColliders] = { {this}, {this}, {this} };
	Module*jflkCallback2[jflknColliders2] = { {this}, {this}, {this}, {(Module*)App->dhalsim} };

	jflk.PushBack({ 750, 540, 58, 77 }, 2, { 29,5 }, { jflknColliders }, { jflkHitbox }, { jflkColliderType }, { jflkCallback });
	jflk.PushBack({ 810, 543, 55, 73 }, 3, { 29,5 }, { jflknColliders }, { jflkHitbox }, { jflkColliderType }, { jflkCallback });
	jflk.PushBack({ 865, 550, 77, 66 }, 100, { 29,5 }, { jflknColliders2 }, { jflkHitbox2 }, { jflkColliderType2 }, { jflkCallback2 });


	//Hadoken
	const int hdknColliders = 3;
	SDL_Rect hdkHitbox[hdknColliders] = { { -25, 79, 24, 16}, { -6, 37, 40, 47 }, { -6, 5, 40, 32 } };
	SDL_Rect hdkHitbox2[hdknColliders] = { { -62, 58, 24, 20}, { -43, 23, 40, 42}, { -10, 5, 78, 28} };
	COLLIDER_TYPE hdkColliderType[hdknColliders] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER} };
	Module* hdkCallback[hdknColliders] = { {this}, {this}, {this} };
	hdk.PushBack({ 462, 751, 74, 90 }, 5, { 29,5 }, hdknColliders, hdkHitbox, hdkColliderType, hdkCallback);		// Falta un trozo de animación, cuya durada depende de si es light, medium or hard
	hdk.PushBack({ 537, 757, 85, 84 }, 15, { 29,5 }, hdknColliders, hdkHitbox, hdkColliderType, hdkCallback);
	hdk.PushBack({ 623, 758, 90, 83 }, 3, { 29,5 }, hdknColliders, hdkHitbox, hdkColliderType, hdkCallback);
	hdk.PushBack({ 714, 764, 106, 77 }, 40, { 29,5 }, hdknColliders, hdkHitbox2, hdkColliderType, hdkCallback);


	// Standing reel
	const int streelnColliders = 3;
	SDL_Rect streelHitbox1[streelnColliders] = { { -11, 68, 24, 16}, { -6, 26, 40, 47}, { -26, 0, 40, 45} };
	SDL_Rect streelHitbox2[streelnColliders] = { { -6, 68, 24, 16}, { -6, 26, 40, 47}, { -26, 0, 40, 45} };
	SDL_Rect streelHitbox3[streelnColliders] = { { 0, 68, 24, 16}, { -6, 26, 40, 47}, { -26, 0, 40, 45} };
	SDL_Rect streelHitbox4[streelnColliders] = { { -31, 68, 24, 16}, { -6, 26, 40, 47}, { -26, 0, 40, 45} };
	COLLIDER_TYPE streelColliderType[streelnColliders] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER} };
	Module* streelCallback[streelnColliders] = { {this}, {this}, {this} };

	streel.PushBack({ 143, 857, 67, 92 }, 6, { 48,5 }, streelnColliders, streelHitbox1, streelColliderType, streelCallback);
	streel.PushBack({ 213, 857, 69, 91 }, 6, { 53,5 }, streelnColliders, streelHitbox2, streelColliderType, streelCallback);
	streel.PushBack({ 285, 857, 80, 91 }, 6, { 57,5 }, streelnColliders, streelHitbox3, streelColliderType, streelCallback);
	streel.PushBack({ 367, 857, 66, 91 }, 6, { 36,5 }, streelnColliders, streelHitbox4, streelColliderType, streelCallback);


	// Standing gut reel
	const int stgreelnColliders = 3;

	SDL_Rect stgreelHitbox2[stgreelnColliders] = { { -25, 68, 24, 16}, { -3, 32, 40, 37}, { -3, 0, 45, 32} };
	SDL_Rect stgreelHitbox3[stgreelnColliders] = { { -25, 64, 24, 16}, { 0, 32, 40, 42}, { -3, 0, 45, 32} };
	SDL_Rect stgreelHitbox4[stgreelnColliders] = { { -28, 63, 26, 18}, { -3, 37, 40, 25}, { -8, 0, 45, 37} };
	SDL_Rect stgreelHitbox5[stgreelnColliders] = { { -22, 47, 24, 16}, { -3, 32, 40, 32}, { -6, 0, 45, 32} };
	COLLIDER_TYPE stgreelColliderType[stgreelnColliders] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER} };
	Module* stgreelCallback[stgreelnColliders] = { {this}, {this}, {this} };

	stgreel.PushBack({ 435, 865, 54, 84 }, 6, { 37,5 }, stgreelnColliders, stgreelHitbox2, stgreelColliderType, stgreelCallback);
	stgreel.PushBack({ 494, 868, 65, 81 }, 6, { 44,5 }, stgreelnColliders, stgreelHitbox3, stgreelColliderType, stgreelCallback);
	stgreel.PushBack({ 561, 866, 70, 83 }, 6, { 47,5 }, stgreelnColliders, stgreelHitbox4, stgreelColliderType, stgreelCallback);
	stgreel.PushBack({ 683, 878, 74, 71 }, 6, { 47,5 }, stgreelnColliders, stgreelHitbox5, stgreelColliderType, stgreelCallback);


	// Crouching reel
	const int creelnColliders = 3;
	SDL_Rect creelHitbox1[creelnColliders] = { { -27, 44, 24, 16}, { -6, 27, 53, 21}, { -6, 0, 53, 27} };
	SDL_Rect creelHitbox2[creelnColliders] = { { 0, 47, 24, 16}, { -4, 27, 36, 21}, { -16, 0, 48, 32} };
	SDL_Rect creelHitbox3[creelnColliders] = { { 0, 47, 24, 16}, { -4, 27, 36, 21}, { -16, 0, 48, 32} };
	COLLIDER_TYPE creelColliderType[creelnColliders] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER} };
	Module* creelCallback[creelnColliders] = { {this}, {this}, {this} };
	creel.PushBack({ 889, 884, 66, 66 }, 6, { 31,5 }, creelnColliders, { creelHitbox1 }, { creelColliderType }, { creelCallback });
	creel.PushBack({ 955, 884, 68, 66 }, 6, { 42,5 }, creelnColliders, { creelHitbox2 }, { creelColliderType }, { creelCallback });
	creel.PushBack({ 0, 953, 66, 71 }, 6, { 42,5 }, creelnColliders, { creelHitbox3 }, { creelColliderType }, { creelCallback });


	//Crouching
	const int crouchingnColliders = 3;
	SDL_Rect crouchingHitbox[crouchingnColliders] = { { -28, 44, 24, 16}, { -6, 27, 40, 21}, { -6, 0, 48, 27} };
	COLLIDER_TYPE crouchingColliderType[crouchingnColliders] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER} };
	Module* crouchingCallback[crouchingnColliders] = { {this}, {this}, {this} };

	crouching.PushBack({ 0, 317, 57, 70 }, 1, { 29,5 }, crouchingnColliders, crouchingHitbox, crouchingColliderType, crouchingCallback);


	//Standing
	const int standingnColliders = 3;
	SDL_Rect standingHitbox[standingnColliders] = { { -28, 44, 24, 16}, { -6, 27, 40, 21}, { -6, 0, 48, 27} };
	COLLIDER_TYPE standingColliderType[standingnColliders] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER} };
	Module* standingCallback[standingnColliders] = { {this}, {this}, {this} };

	standing.PushBack({ 0, 317, 57, 70 }, 1, { 29,5 }, standingnColliders, standingHitbox, standingColliderType, standingCallback);


	//Crouch
	const int crouchnColliders = 3;
	SDL_Rect crouchHitbox[crouchnColliders] = { { -28, 44, 24, 16}, { -6, 27, 40, 21}, { -6, 0, 48, 27} };
	COLLIDER_TYPE crouchColliderType[crouchnColliders] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER} };
	Module* crouchCallback[crouchnColliders] = { {this}, {this}, {this} };

	crouch.PushBack({ 57, 325, 62, 62 }, 1, { 29,5 }, crouchnColliders, crouchHitbox, crouchColliderType, crouchCallback);

	//Crouching l punch
	const int clpnColliders = 3;
	const int clpnColliders2 = 4;
	SDL_Rect clpHitbox[clpnColliders] = { { -28, 44, 24, 16}, { -6, 27, 40, 21}, { -6, 0, 48, 27} };
	SDL_Rect clpHitbox2[clpnColliders2] = { { -28, 44, 24, 16}, { -6, 27, 40, 21}, { -6, 0, 48, 27}, {-38, 33, 57, 20} };
	COLLIDER_TYPE clpColliderType[clpnColliders] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER} };
	COLLIDER_TYPE clpColliderType2[clpnColliders2] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER_HIT} };
	Module*clpCallback[clpnColliders] = { {this}, {this}, {this} };
	Module*clpCallback2[clpnColliders2] = { {this}, {this}, {this}, {(Module*)App->dhalsim} };

	clp.PushBack({ 227, 326, 69, 61 }, 5, { 29,5 }, { clpnColliders }, { clpHitbox }, { clpColliderType }, { clpCallback });
	clp.PushBack({ 296, 325, 96, 61 }, 10, { 29,5 }, { clpnColliders2 }, { clpHitbox2 }, { clpColliderType2 }, { clpCallback2 });
	clp.PushBack({ 227, 326, 69, 61 }, 10, { 29,5 }, { clpnColliders }, { clpHitbox }, { clpColliderType }, { clpCallback });
	clp.PushBack({ 227, 326, 69, 61 }, 2, { 29,5 }, { clpnColliders }, { clpHitbox }, { clpColliderType }, { clpCallback });


	//Crouching l kik
	const int clknColliders = 3;
	const int clknColliders2 = 4;
	SDL_Rect clkHitbox[clknColliders] = { { -28, 44, 24, 16}, { -6, 27, 40, 21}, { -6, 0, 48, 27} };
	SDL_Rect clkHitbox2[clknColliders2] = { { -28, 44, 24, 16}, { -6, 27, 40, 21}, { -6, 0, 48, 27}, {-49, 0, 65, 22} };
	COLLIDER_TYPE clkColliderType[clknColliders] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER} };
	COLLIDER_TYPE clkColliderType2[clknColliders2] = { {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER}, {COLLIDER_PLAYER_HIT} };
	Module*clkCallback[clknColliders] = { {this}, {this}, {this} };
	Module*clkCallback2[clknColliders2] = { {this}, {this}, {this}, {(Module*)App->dhalsim} };


	clk.PushBack({ 617, 322, 71, 65 }, 5, { 29,5 }, { clknColliders }, { clkHitbox }, { clkColliderType }, { clkCallback });
	clk.PushBack({ 688, 322, 113, 65 }, 10, { 29,5 }, { clknColliders2 }, { clkHitbox2 }, { clkColliderType2 }, { clkCallback2 });
	clk.PushBack({ 617, 322, 71, 65 }, 10, { 29,5 }, { clknColliders }, { clkHitbox }, { clkColliderType }, { clkCallback });
	clk.PushBack({ 617, 322, 71, 65 }, 2, { 29,5 }, { clknColliders }, { clkHitbox }, { clkColliderType }, { clkCallback });

	// Win1

	const int winnColliders = 3;
	SDL_Rect winHitbox1[winnColliders] = { { 0, 0, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0} };
	COLLIDER_TYPE winColliderType[winnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* winCallback[winnColliders] = { {this}, {this}, {this} };

	win1.PushBack({ 155, 110, 60, 113 }, 5, { 29,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });
	win1.PushBack({ 216, 110, 59, 113 }, 10, { 29,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });
	win1.PushBack({ 277, 110, 56, 113 }, 10, { 29,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });
	win1.loop = false;

	// Win2
	const int win2nColliders = 3;
	SDL_Rect win2Hitbox1[win2nColliders] = { { 0, 0, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0} };
	COLLIDER_TYPE win2ColliderType[win2nColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* win2Callback[win2nColliders] = { {this}, {this}, {this} };

	win2.PushBack({ 335, 127, 52, 96 }, 5, { 29,5 }, { win2nColliders }, { win2Hitbox1 }, { win2ColliderType }, { win2Callback });
	win2.PushBack({ 389, 127, 53, 96 }, 10, { 29,5 }, { win2nColliders }, { win2Hitbox1 }, { win2ColliderType }, { win2Callback });
	win2.PushBack({ 444, 127, 53, 96 }, 10, { 29,5 }, { win2nColliders }, { win2Hitbox1 }, { win2ColliderType }, { win2Callback });
	win2.PushBack({ 499, 127, 53, 96 }, 10, { 29,5 }, { win2nColliders }, { win2Hitbox1 }, { win2ColliderType }, { win2Callback });
	win2.loop = false;

	//Falling
	const int airreelnColliders = 3;
	SDL_Rect airreelHitbox1[airreelnColliders] = { { -11, 57, 24, 10}, { 0, 11, 73, 47}, { -31, -32, 40, 45} };
	SDL_Rect airreelHitbox2[airreelnColliders] = { { 0, 0, 0, 0}, { 0, 0, 0, 0}, { 0, -0, 105, 40} };
	SDL_Rect airreelHitbox3[airreelnColliders] = { { -12, 84, 24, 16}, { 0, 40, 55, 47}, { 0, 0, 40, 40} };
	COLLIDER_TYPE airreelColliderType[airreelnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* airreelCallback[airreelnColliders] = { {this}, {this}, {this} };

	airreel.PushBack({ 815, 883, 73, 65 }, 5, { 29,5 }, { airreelnColliders }, { airreelHitbox1 }, { airreelColliderType }, { airreelCallback });

	//getting up
	const int getupnColliders = 3;
	SDL_Rect getupHitbox1[getupnColliders] = { { 0, 0, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0} };
	COLLIDER_TYPE getupColliderType[getupnColliders] = { {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2}, {COLLIDER_PLAYER2} };
	Module* getupCallback[getupnColliders] = { {this}, {this}, {this} };

	getup.PushBack({ 311, 993, 127, 31 }, 5, { 29,5 }, { getupnColliders }, { getupHitbox1 }, { getupColliderType }, { getupCallback });
	getup.PushBack({ 440, 990, 92, 34 }, 10, { 29,5 }, { getupnColliders }, { getupHitbox1 }, { getupColliderType }, { getupCallback });
	getup.PushBack({ 534, 953, 56, 71 }, 10, { 29,5 }, { getupnColliders }, { getupHitbox1 }, { getupColliderType }, { getupCallback });
	getup.PushBack({ 633, 909, 46, 115 }, 10, { 29,5 }, { getupnColliders }, { getupHitbox1 }, { getupColliderType }, { getupCallback });
	getup.PushBack({ 681, 956, 79, 68 }, 10, { 29,5 }, { getupnColliders }, { getupHitbox1 }, { getupColliderType }, { getupCallback });

	//ground
	ground.PushBack({ 310, 992, 129, 32 }, 20, { 29,5 }, { winnColliders }, { winHitbox1 }, { winColliderType }, { winCallback });


	inputs.Push(IN_END);

	return ret;
}

// Unload assets
bool ModuleRyu::CleanUp()
{
	LOG("Unloading ryu 1");

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
	graphics = nullptr;
	App->textures->Unload(graphics2);
	ClearColliders();


	idle = Animation();
	forward = Animation();
	backward = Animation();
	lp = lk = clp = clk = Animation();
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

update_status ModuleRyu::PreUpdate() {
	ClearColliders();

	return UPDATE_CONTINUE;
}

// Update: draw background
update_status ModuleRyu::Update()
{
	ryu_states current_state = ST_UNKNOWN;
	Animation* current_animation = &idle;
	SDL_Texture* texture = graphics;

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
		case ST_IDLE:
			current_animation = &idle;
			typeofattack = 0;
			break;

		case ST_WALK_FORWARD:
			current_animation = (!flip ? &forward : &backward);
			if (IsntOnRightLimit())
				position.x++;
			break;

		case ST_WALK_BACKWARD:
			current_animation = (!flip ? &backward : &forward);
			if (IsntOnLeftLimit())
				position.x--;
			break;

		case ST_JUMP_NEUTRAL:
			current_animation = &neutralJump;
			if (App->frames - jump_timer > 27 && (App->frames - jump_timer <= R_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}
			if (App->frames - jump_timer < 28 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}

			break;

		case ST_JUMP_FORWARD:
			current_animation = &forwardJump;
			if (App->frames - jump_timer > 27 && (App->frames - jump_timer <= R_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}
			if (App->frames - jump_timer < 28 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}
			if (IsntOnRightLimit())
				position.x += 3;


			break;

		case ST_JUMP_BACKWARD:
			current_animation = &backwardJump;
			if (App->frames - jump_timer > 27 && (App->frames - jump_timer <= R_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}
			if (App->frames - jump_timer < 28 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}
			if (IsntOnLeftLimit())
				position.x -= 3;
			//LOG("JUMPING BACKWARD ^^<<\n");
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
			typeofattack = 1;
			break;

		case L_PUNCH_STANDING:
			current_animation = &lp;
			typeofattack = 1;
			dizzydamage = 5;
			break;

		case L_PUNCH_NEUTRAL_JUMP:
			current_animation = &jlp;
			if (App->frames - jump_timer > 27 && (App->frames - jump_timer <= R_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}
			if (App->frames - jump_timer < 28 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}
			typeofattack = 1;
			break;

		case L_PUNCH_FORWARD_JUMP:
			current_animation = &jflp;

			if (App->frames - jump_timer > 27 && (App->frames - jump_timer <= R_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}
			if (App->frames - jump_timer < 28 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}
			if (IsntOnRightLimit())
				position.x += 3;
			typeofattack = 1;
			break;

		case L_PUNCH_BACKWARD_JUMP:
			current_animation = &jblp;

			if (App->frames - jump_timer > 27 && (App->frames - jump_timer <= R_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}
			if (App->frames - jump_timer < 28 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}
			if (IsntOnLeftLimit())
				position.x -= 3;
			typeofattack = 1;
			break;

		case L_KIK_CROUCH:
			current_animation = &clk;
			typeofattack = 1;
			break;

		case L_KIK_STANDING:
			current_animation = &lk;
			typeofattack = 1;
			break;

		case L_KIK_NEUTRAL_JUMP:
			current_animation = &jlk;
			if (App->frames - jump_timer > 27 && (App->frames - jump_timer <= R_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}
			if (App->frames - jump_timer < 28 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}
			typeofattack = 1;
			break;


		case L_KIK_FORWARD_JUMP:
			current_animation = &jflk;
			if (App->frames - jump_timer > 27 && (App->frames - jump_timer <= R_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}
			if (App->frames - jump_timer < 28 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}
			if (IsntOnRightLimit())
				position.x += 3;
			typeofattack = 1;
			break;

		case L_KIK_BACKWARD_JUMP:
			current_animation = &jblk;
			if (App->frames - jump_timer > 27 && (App->frames - jump_timer <= R_JUMP_TIME))
			{
				jumpHeight += speed + 3;
			}
			if (App->frames - jump_timer < 28 && (App->frames - jump_timer >= 0))
			{
				jumpHeight -= speed + 3;
			}
			if (IsntOnLeftLimit())
				position.x -= 3;
			typeofattack = 1;
			break;

		case ST_HEAD_REEL:
			current_animation = &streel;
			break;

		case ST_GUT_REEL:
			break;

		case ST_CROUCH_REEL:
			current_animation = &creel;
			break;

		case ST_HADOKEN:
			current_animation = &hdk;
			if (App->frames - hadoken_timer == 35)
			{
				App->particles->AddParticle(App->particles->hdk, flip, position.x + hdk_spawn, position.y - 70, 0, COLLIDER_PLAYER_SHOT, hdk_voice, 200);
			}
			typeofattack = 1;
			break;

		case ST_FALLING:
			current_animation = &airreel;
			jumpHeight += speed + 3;
			if (jumpHeight == 0)
			{
				inputs.Push(IN_FALLING_FINISH);
			}
			break;

		case GRABBED:
			current_animation = &airreel;

			break;

		case ST_GETTING_UP:
			current_animation = &getup;
			break;

		case LOOSE:
			current_animation = &ground;
			break;

		case VICTORY:
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

	//GOD MODE
	if (App->input->keyboard[SDL_SCANCODE_F5] == KEY_STATE::KEY_DOWN) {
		if (godmode == false)
			godmode = true;
		else
			godmode = false;
	}

	BlitCharacterAndAddColliders(current_animation, texture);

	return UPDATE_CONTINUE;
}

void ModuleRyu::ClearColliders() {
	for (int i = 0; i < MAX_COLLIDERS_PER_FRAME; i++)
	{
		if (colliders[i] != nullptr) {
			colliders[i]->to_delete = true;
			colliders[i] = nullptr;
		}
	}
}

bool ModuleRyu::IsntOnRightLimit() {
	return position.x + 42 < -App->render->camera.x / SCREEN_SIZE + App->render->camera.w;
}

bool ModuleRyu::IsntOnLeftLimit() {
	return position.x - 42 > -App->render->camera.x / SCREEN_SIZE;
}

void ModuleRyu::OnCollision(Collider* c1, Collider* c2) {
	
	if (c1->type != COLLIDER_PLAYER2_SHOT && c2->type != COLLIDER_PLAYER2_SHOT && c1->type != COLLIDER_PLAYER_SHOT && c2->type != COLLIDER_PLAYER_SHOT) { App->dhalsim->colliding = true; }
	else { App->dhalsim->colliding = false; }
	
	//PUSHBACK CHECK
	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_PLAYER2_HIT) {
		if (App->dhalsim->typeofattack == 1) { pushbacktimerhit = 10; pushbackspeed = 2; }
		if (App->dhalsim->typeofattack == 2) { pushbacktimerhit = 15; pushbackspeed = 2; }
		if (App->dhalsim->typeofattack == 3) { pushbacktimerhit = 20; pushbackspeed = 2; }
		//		App->particles->AddParticle(App->particles->ground_dust, flip, position.x, position.y, 0, COLLIDER_WALL, 0, 0);
	}

	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_PLAYER2_SHOT) {
		if (App->dhalsim->typeofattack == 1) { pushbacktimerprojectile = 20; pushbackspeed = 2; }
		if (App->dhalsim->typeofattack == 2) { pushbacktimerprojectile = 15; pushbackspeed = 2; }
		if (App->dhalsim->typeofattack == 3) { pushbacktimerprojectile = 10; pushbackspeed = 2; }
	}

	//PUSHBACK CHECK END

	if (invulnerabilityFrames < App->frames) {
		if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_PLAYER2_SHOT && (state != ST_JUMP_NEUTRAL && state != ST_JUMP_FORWARD && state != ST_JUMP_BACKWARD && state != L_PUNCH_NEUTRAL_JUMP && state != L_PUNCH_FORWARD_JUMP && state != L_PUNCH_BACKWARD_JUMP && state != L_KIK_NEUTRAL_JUMP && state != L_KIK_FORWARD_JUMP && state != L_KIK_BACKWARD_JUMP))
		{
			life -= 12;
			App->audio->PlayChunk(hdk_hit);
			invulnerabilityFrames = 25 + App->frames;

			if (!flip)
				position.x--;

			if (state == ST_CROUCHING || state == ST_CROUCH || state == ST_STANDING || state == L_PUNCH_CROUCH || state == L_KIK_CROUCH)
				inputs.Push(IN_CROUCH_REEL);

			else
				inputs.Push(IN_HEAD_REEL);
		}

		if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_PLAYER2_HIT && (state != ST_JUMP_NEUTRAL && state != ST_JUMP_FORWARD && state != ST_JUMP_BACKWARD &&
			state != L_PUNCH_NEUTRAL_JUMP && state != L_PUNCH_FORWARD_JUMP && state != L_PUNCH_BACKWARD_JUMP && state != L_KIK_NEUTRAL_JUMP && state != L_KIK_FORWARD_JUMP && state != L_KIK_BACKWARD_JUMP))
		{
			life -= 7;
			invulnerabilityFrames = 25 + App->frames;

			if (App->dhalsim->state == L_KIK_STANDING2 || App->dhalsim->state == L_KIK_NEUTRAL_JUMP2 || App->dhalsim->state == L_KIK_FORWARD_JUMP2 || App->dhalsim->state == L_KIK_BACKWARD_JUMP2 || App->dhalsim->state == M_KIK_STANDING2 || App->dhalsim->state == M_KIK_NEUTRAL_JUMP2 || App->dhalsim->state == M_KIK_FORWARD_JUMP2 || App->dhalsim->state == M_KIK_BACKWARD_JUMP2
				|| App->dhalsim->state == F_KIK_STANDING2 || App->dhalsim->state == F_KIK_NEUTRAL_JUMP2 || App->dhalsim->state == F_KIK_FORWARD_JUMP2 || App->dhalsim->state == F_KIK_BACKWARD_JUMP2 || App->dhalsim->state == L_KIK_CLOSE2 || App->dhalsim->state == M_KIK_CLOSE2 || App->dhalsim->state == F_KIK_CLOSE2)
				App->audio->PlayChunk(high_kick);
			else if (App->dhalsim->state == L_KIK_CROUCH2 || App->dhalsim->state == M_KIK_CROUCH2 || App->dhalsim->state == F_KIK_CROUCH2 || App->dhalsim->state == L_KIK_CROUCHCLOSE2 || App->dhalsim->state == M_KIK_CROUCHCLOSE2)
				App->audio->PlayChunk(low_kick);
			else if (App->dhalsim->state == L_PUNCH_STANDING2 || App->dhalsim->state == L_PUNCH_NEUTRAL_JUMP2 || App->dhalsim->state == L_PUNCH_FORWARD_JUMP2 || App->dhalsim->state == L_PUNCH_BACKWARD_JUMP2 || App->dhalsim->state == M_PUNCH_STANDING2 || App->dhalsim->state == M_PUNCH_NEUTRAL_JUMP2 || App->dhalsim->state == M_PUNCH_FORWARD_JUMP2 || App->dhalsim->state == M_PUNCH_BACKWARD_JUMP2
				|| App->dhalsim->state == F_PUNCH_STANDING2 || App->dhalsim->state == F_PUNCH_NEUTRAL_JUMP2 || App->dhalsim->state == F_PUNCH_FORWARD_JUMP2 || App->dhalsim->state == F_PUNCH_BACKWARD_JUMP2 || App->dhalsim->state == L_PUNCH_CLOSE2 || App->dhalsim->state == M_PUNCH_CLOSE2 || App->dhalsim->state == F_PUNCH_CLOSE2)
				App->audio->PlayChunk(high_fist);
			else if (App->dhalsim->state == L_PUNCH_CROUCH2 || App->dhalsim->state == M_PUNCH_CROUCH2 || App->dhalsim->state == F_PUNCH_CROUCH2 || App->dhalsim->state == L_PUNCH_CROUCHCLOSE2 || App->dhalsim->state == M_PUNCH_CROUCHCLOSE2)
				App->audio->PlayChunk(low_fist);

			if (state == ST_CROUCHING || state == ST_CROUCH || state == ST_STANDING || state == L_PUNCH_CROUCH2 || state == L_KIK_CROUCH2)
				inputs.Push(IN_CROUCH_REEL);

			else
				inputs.Push(IN_HEAD_REEL);
		}

		if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_PLAYER2_SHOT && (state == ST_JUMP_NEUTRAL || state == ST_JUMP_FORWARD || state == ST_JUMP_BACKWARD || state == L_PUNCH_NEUTRAL_JUMP || state == L_PUNCH_FORWARD_JUMP || state == L_PUNCH_BACKWARD_JUMP || state == L_KIK_NEUTRAL_JUMP || state == L_KIK_FORWARD_JUMP || state == L_KIK_BACKWARD_JUMP))
		{
			life -= 12;
			App->audio->PlayChunk(hdk_hit);
			invulnerabilityFrames = 25 + App->frames;
			inputs.Push(IN_FALLING);
		}

		if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_PLAYER2_HIT && (state == ST_JUMP_NEUTRAL || state == ST_JUMP_FORWARD || state == ST_JUMP_BACKWARD || state == L_PUNCH_NEUTRAL_JUMP || state == L_PUNCH_FORWARD_JUMP || state == L_PUNCH_BACKWARD_JUMP || state == L_KIK_NEUTRAL_JUMP || state == L_KIK_FORWARD_JUMP || state == L_KIK_BACKWARD_JUMP))
		{
			life -= 7;
			invulnerabilityFrames = 25 + App->frames;

			if (App->dhalsim->state == L_KIK_STANDING2 || App->dhalsim->state == L_KIK_NEUTRAL_JUMP2 || App->dhalsim->state == L_KIK_FORWARD_JUMP2 || App->dhalsim->state == L_KIK_BACKWARD_JUMP2 || App->dhalsim->state == M_KIK_STANDING2 || App->dhalsim->state == M_KIK_NEUTRAL_JUMP2 || App->dhalsim->state == M_KIK_FORWARD_JUMP2 || App->dhalsim->state == M_KIK_BACKWARD_JUMP2
				|| App->dhalsim->state == F_KIK_STANDING2 || App->dhalsim->state == F_KIK_NEUTRAL_JUMP2 || App->dhalsim->state == F_KIK_FORWARD_JUMP2 || App->dhalsim->state == F_KIK_BACKWARD_JUMP2 || App->dhalsim->state == L_KIK_CLOSE2 || App->dhalsim->state == M_KIK_CLOSE2 || App->dhalsim->state == F_KIK_CLOSE2)
				App->audio->PlayChunk(high_kick);
			else if (App->dhalsim->state == L_KIK_CROUCH2 || App->dhalsim->state == M_KIK_CROUCH2 || App->dhalsim->state == F_KIK_CROUCH2 || App->dhalsim->state == L_KIK_CROUCHCLOSE2 || App->dhalsim->state == M_KIK_CROUCHCLOSE2)
				App->audio->PlayChunk(low_kick);
			else if (App->dhalsim->state == L_PUNCH_STANDING2 || App->dhalsim->state == L_PUNCH_NEUTRAL_JUMP2 || App->dhalsim->state == L_PUNCH_FORWARD_JUMP2 || App->dhalsim->state == L_PUNCH_BACKWARD_JUMP2 || App->dhalsim->state == M_PUNCH_STANDING2 || App->dhalsim->state == M_PUNCH_NEUTRAL_JUMP2 || App->dhalsim->state == M_PUNCH_FORWARD_JUMP2 || App->dhalsim->state == M_PUNCH_BACKWARD_JUMP2
				|| App->dhalsim->state == F_PUNCH_STANDING2 || App->dhalsim->state == F_PUNCH_NEUTRAL_JUMP2 || App->dhalsim->state == F_PUNCH_FORWARD_JUMP2 || App->dhalsim->state == F_PUNCH_BACKWARD_JUMP2 || App->dhalsim->state == L_PUNCH_CLOSE2 || App->dhalsim->state == M_PUNCH_CLOSE2 || App->dhalsim->state == F_PUNCH_CLOSE2)
				App->audio->PlayChunk(high_fist);
			else if (App->dhalsim->state == L_PUNCH_CROUCH2 || App->dhalsim->state == M_PUNCH_CROUCH2 || App->dhalsim->state == F_PUNCH_CROUCH2 || App->dhalsim->state == L_PUNCH_CROUCHCLOSE2 || App->dhalsim->state == M_PUNCH_CROUCHCLOSE2)
				App->audio->PlayChunk(low_fist);

			inputs.Push(IN_FALLING);
		}

		if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_PLAYER2)
		{
			

			if (state == ST_WALK_FORWARD && App->dhalsim->state == ST_WALK_FORWARD2) {
				position.x--;
				App->dhalsim->position.x++;
			}
			else if (state == ST_WALK_BACKWARD && App->dhalsim->state == ST_WALK_BACKWARD2) {
				position.x++;
				App->dhalsim->position.x--;
			}
			else {
				if (state != ST_IDLE && state != ST_JUMP_NEUTRAL && state != ST_JUMP_FORWARD && state != ST_JUMP_BACKWARD && state != L_PUNCH_NEUTRAL_JUMP && state != L_PUNCH_FORWARD_JUMP && state != L_PUNCH_BACKWARD_JUMP && state != L_KIK_NEUTRAL_JUMP && state != L_KIK_FORWARD_JUMP && state != L_KIK_BACKWARD_JUMP) {
					if (App->dhalsim->IsntOnLeftLimit() && position.x > App->dhalsim->position.x)
						App->dhalsim->position.x--;
					else if (!App->dhalsim->IsntOnLeftLimit() && position.x > App->dhalsim->position.x)
						position.x++;
					else if (App->dhalsim->IsntOnRightLimit() && position.x < App->dhalsim->position.x)
						App->dhalsim->position.x++;
					else if (!App->dhalsim->IsntOnRightLimit() && position.x < App->dhalsim->position.x)
						position.x--;
				}
				else if (App->dhalsim->state != ST_IDLE2 && App->dhalsim->state != ST_JUMP_NEUTRAL2 && App->dhalsim->state != ST_JUMP_FORWARD2 && App->dhalsim->state != ST_JUMP_BACKWARD2 && App->dhalsim->state != L_PUNCH_NEUTRAL_JUMP2 && App->dhalsim->state != L_PUNCH_FORWARD_JUMP2 && App->dhalsim->state != L_PUNCH_BACKWARD_JUMP2 && App->dhalsim->state != L_KIK_NEUTRAL_JUMP2 && App->dhalsim->state != L_KIK_FORWARD_JUMP2 && App->dhalsim->state != L_KIK_BACKWARD_JUMP2) {
					if (IsntOnLeftLimit() && position.x < App->dhalsim->position.x)
						position.x--;
					else if (App->dhalsim->IsntOnLeftLimit() && position.x < App->dhalsim->position.x)
						App->dhalsim->position.x++;
					else if (IsntOnRightLimit() && position.x > App->dhalsim->position.x)
						position.x++;
					else if (App->dhalsim->IsntOnRightLimit() && position.x > App->dhalsim->position.x)
						App->dhalsim->position.x--;
				}
				else if (state == ST_IDLE && App->dhalsim->state == ST_IDLE2) {
					if (IsntOnLeftLimit() && !flip)
						position.x--;
					else if (App->dhalsim->IsntOnLeftLimit() && !flip)
						App->dhalsim->position.x++;
					else if (IsntOnRightLimit() && flip)
						position.x++;
					else if (IsntOnRightLimit() && !flip)
						App->dhalsim->position.x++;
					else if (App->dhalsim->IsntOnRightLimit() && flip)
						App->dhalsim->position.x--;
				}
			}
		} 
	}
}

void ModuleRyu::BlitCharacterAndAddColliders(Animation* current_animation, SDL_Texture* texture) {
	Frame frame = current_animation->GetCurrentFrame();
	SDL_Rect r;
	int hitboxesQnt = frame.GetColliderQnt();

	for (int i = 0; i < hitboxesQnt; i++)
	{
		r = frame.hitBoxeRects[i];
		if (!godmode || frame.types[i] != COLLIDER_PLAYER)
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

bool ModuleRyu::external_input(p2Qeue<ryu_inputs>& inputs)
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
			inputs.Push(IN_F_KIK);
		}
		if (App->input->keyboard[SDL_SCANCODE_K] == KEY_STATE::KEY_DOWN) //trying pushback

		{
			inputs.Push(IN_L_KIK);
		}
		if ((App->input->keyboard[SDL_SCANCODE_3] == KEY_STATE::KEY_DOWN) || (App->input->gameController2States[SDL_CONTROLLER_BUTTON_X] == KEY_DOWN))
		{
			inputs.Push(IN_L_PUNCH);
		}

		if ((App->input->keyboard[SDL_SCANCODE_4] == KEY_STATE::KEY_DOWN) || (App->input->gameController2States[SDL_CONTROLLER_BUTTON_A] == KEY_DOWN))
		{
			inputs.Push(IN_L_KIK);
		}

		if ((App->input->keyboard[SDL_SCANCODE_KP_5] == KEY_STATE::KEY_DOWN) || (App->input->gameController2States[SDL_CONTROLLER_BUTTON_Y] == KEY_DOWN))
		{
			inputs.Push(IN_M_PUNCH);
		}

		if ((App->input->keyboard[SDL_SCANCODE_KP_2] == KEY_STATE::KEY_DOWN) || (App->input->gameController2States[SDL_CONTROLLER_BUTTON_B] == KEY_DOWN))
		{
			inputs.Push(IN_M_KIK);
		}

		if ((App->input->keyboard[SDL_SCANCODE_KP_6] == KEY_STATE::KEY_DOWN) || (App->input->gameController2States[SDL_CONTROLLER_BUTTON_RIGHTSHOULDER] == KEY_DOWN))
		{
			inputs.Push(IN_F_PUNCH);
		}

		if ((App->input->keyboard[SDL_SCANCODE_KP_3] == KEY_STATE::KEY_DOWN) || (App->input->gameController2AxisValues[SDL_CONTROLLER_AXIS_TRIGGERRIGHT] > JOYSTICK_DEAD_ZONE))
		{
			inputs.Push(IN_F_KIK);
		}

		if (App->input->keyboard[SDL_SCANCODE_5] == KEY_STATE::KEY_DOWN)
		{
			inputs.Push(IN_HADOKEN);
		}



		if (App->input->pads[1].left)
		{
			if (App->input->pads[1].up)
				inputs.Push(IN_LEFT_AND_JUMP);

			if (App->input->pads[1].down) {
				inputs.Push(IN_LEFT_AND_CROUCH);
				crouch = true;
			}

			else
				inputs.Push(IN_LEFT_DOWN);
		}
		else
			inputs.Push(IN_LEFT_UP);


		if (App->input->pads[1].right)
		{
			if (App->input->pads[1].up)
				inputs.Push(IN_RIGHT_AND_JUMP);

			if (App->input->pads[1].down) {
				inputs.Push(IN_RIGHT_AND_CROUCH);
				crouch = true;
			}

			else
				inputs.Push(IN_RIGHT_DOWN);
		}
		else
			inputs.Push(IN_RIGHT_UP);


		if (App->input->pads[1].up && !App->input->pads[1].right && !App->input->pads[1].left)
		{
			inputs.Push(IN_JUMP);
		}

		if (App->input->pads[1].down && !App->input->pads[1].right && !App->input->pads[1].left)
		{
			inputs.Push(IN_CROUCH_DOWN);
			crouch = true;
		}

		if (crouch == false)
		{
			inputs.Push(IN_CROUCH_UP);
		}

		if (!App->input->pads[1].down && !App->input->pads[1].up && !App->input->pads[1].right && !App->input->pads[1].left)
		{
			inputs.Push(IN_IDLE);
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

void ModuleRyu::internal_input(p2Qeue<ryu_inputs>& inputs)
{
	if (jump_timer > 0)
	{
		if (App->frames - jump_timer > R_JUMP_TIME)
		{
			inputs.Push(IN_JUMP_FINISH);
			jump_timer = 0;
		}
	}

	if (l_standing_punch_timer > 0)
	{
		if (App->frames - l_standing_punch_timer > R_L_STANDING_PUNCH_TIME)
		{
			inputs.Push(IN_PUNCH_FINISH);
			l_standing_punch_timer = 0;
		}
	}

	if (l_crouching_punch_timer > 0)
	{
		if (App->frames - l_crouching_punch_timer > R_L_CROUCHING_PUNCH_TIME)
		{
			inputs.Push(IN_PUNCH_FINISH);
			l_crouching_punch_timer = 0;
		}
	}

	if (l_d_jumping_punch_timer > 0)
	{
		if (App->frames - l_d_jumping_punch_timer > R_L_D_JUMPING_PUNCH_TIME)
		{
			inputs.Push(IN_PUNCH_FINISH);
			l_d_jumping_punch_timer = 0;
		}
	}

	if (l_standing_kik_timer > 0)
	{
		if (App->frames - l_standing_kik_timer > R_L_STANDING_KIK_TIME)
		{
			inputs.Push(IN_KIK_FINISH);
			l_standing_kik_timer = 0;
		}
	}

	if (l_crouching_kik_timer > 0)
	{
		if (App->frames - l_crouching_kik_timer > R_L_CROUCHING_KIK_TIME)
		{
			inputs.Push(IN_KIK_FINISH);
			l_crouching_kik_timer = 0;
		}
	}

	if (l_d_jumping_kik_timer > 0)
	{
		if (App->frames - l_d_jumping_kik_timer > R_L_D_JUMPING_KIK_TIME)
		{
			inputs.Push(IN_KIK_FINISH);
			l_d_jumping_kik_timer = 0;
		}
	}

	if (hadoken_timer > 0)
	{
		if (App->frames - hadoken_timer > R_HADOKEN_TIME)
		{
			inputs.Push(IN_HADOKEN_FINISH);
			hadoken_timer = 0;
		}
	}

	if (crouching_timer > 0)
	{
		if (App->frames - crouching_timer > R_CROUCHING_TIME)
		{
			inputs.Push(IN_CROUCHING_FINISH);
			crouching_timer = 0;
		}
	}

	if (standing_timer > 0)
	{
		if (App->frames - standing_timer > R_STANDING_TIME)
		{
			inputs.Push(IN_STANDING_FINISH);
			standing_timer = 0;
		}
	}

	if (head_reel_timer > 0)
	{
		if (App->frames - head_reel_timer > R_HEAD_REEL_TIME)
		{
			inputs.Push(IN_REEL_FINISH);
			head_reel_timer = 0;
		}
	}

	if (gut_reel_timer > 0)
	{
		if (App->frames - gut_reel_timer > R_GUT_REEL_TIME)
		{
			inputs.Push(IN_REEL_FINISH);
			gut_reel_timer = 0;
		}
	}

	if (crouch_reel_timer > 0)
	{
		if (App->frames - crouch_reel_timer > R_CROUCH_REEL_TIME)
		{
			inputs.Push(IN_REEL_FINISH);
			crouch_reel_timer = 0;
		}
	}

	if (getting_up_timer > 0)
	{
		if (App->frames - getting_up_timer > R_GETTING_UP_TIME)
		{
			inputs.Push(IN_GETTING_UP_FINISH);
			getting_up_timer = 0;
		}
	}

	if (grabbed_timer > 0)
	{
		if (App->frames - grabbed_timer > R_GRABBED_TIME)
		{
			inputs.Push(IN_GRABBED_FINISH);
			grabbed_timer = 0;
		}
	}

}

ryu_states ModuleRyu::process_fsm(p2Qeue<ryu_inputs>& inputs)
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
			case IN_JUMP: state = ST_JUMP_NEUTRAL; jump_timer = App->frames;  break;
			case IN_CROUCH_DOWN: state = ST_CROUCHING; crouching_timer = App->frames; break;
			case IN_L_PUNCH: state = L_PUNCH_STANDING; l_standing_punch_timer = App->frames;  break;
			case IN_L_KIK: state = L_KIK_STANDING; l_standing_kik_timer = App->frames;  break;
			case IN_HADOKEN: state = ST_HADOKEN; hadoken_timer = App->frames; break;
			case IN_HEAD_REEL: state = ST_HEAD_REEL; head_reel_timer = App->frames; break;
			case IN_GUT_REEL: state = ST_GUT_REEL; gut_reel_timer = App->frames; break;
			case IN_GRABBED: state = GRABBED; grabbed_timer = App->frames; break;

			case IN_VICTORY: state = VICTORY; break;
			case IN_LOOSE: state = LOOSE; break;
			}
		}
		break;

		case ST_WALK_FORWARD:
		{
			switch (last_input)
			{
			case IN_RIGHT_UP: state = ST_IDLE; break;
			case IN_LEFT_AND_RIGHT: state = ST_IDLE; break;
			case IN_JUMP: state = ST_JUMP_FORWARD; jump_timer = App->frames;  break;
			case IN_CROUCH_DOWN: state = ST_CROUCH; break;
			case IN_L_PUNCH: state = L_PUNCH_STANDING; l_standing_punch_timer = App->frames;  break;
			case IN_L_KIK: state = L_KIK_STANDING; l_standing_kik_timer = App->frames;  break;
			case IN_HEAD_REEL: state = ST_HEAD_REEL; head_reel_timer = App->frames; break;
			case IN_GUT_REEL: state = ST_GUT_REEL; gut_reel_timer = App->frames; break;

			case IN_VICTORY: state = VICTORY; break;
			case IN_LOOSE: state = LOOSE; break;
			}
		}
		break;

		case ST_WALK_BACKWARD:
		{
			switch (last_input)
			{
			case IN_LEFT_UP: state = ST_IDLE; break;
			case IN_LEFT_AND_RIGHT: state = ST_IDLE; break;
			case IN_JUMP: state = ST_JUMP_BACKWARD; jump_timer = App->frames;  break;
			case IN_CROUCH_DOWN: state = ST_CROUCH; break;
			case IN_L_PUNCH: state = L_PUNCH_STANDING; l_standing_punch_timer = App->frames;  break;
			case IN_L_KIK: state = L_KIK_STANDING; l_standing_kik_timer = App->frames;  break;
			case IN_HEAD_REEL: state = ST_HEAD_REEL; head_reel_timer = App->frames; break;
			case IN_GUT_REEL: state = ST_GUT_REEL; gut_reel_timer = App->frames; break;

			case IN_VICTORY: state = VICTORY; break;
			case IN_LOOSE: state = LOOSE; break;
			}
		}
		break;

		case ST_JUMP_NEUTRAL:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH: state = ST_IDLE; break;
			case IN_L_PUNCH: state = L_PUNCH_NEUTRAL_JUMP; break;
			case IN_L_KIK: state = L_KIK_NEUTRAL_JUMP; break;
			case IN_FALLING: state = ST_FALLING; break;

			case IN_LOOSE: state = LOOSE; break;
			}
		}
		break;

		case ST_JUMP_FORWARD:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH: state = ST_IDLE; break;
			case IN_L_PUNCH: state = L_PUNCH_FORWARD_JUMP; l_d_jumping_punch_timer = App->frames; break;
			case IN_L_KIK: state = L_KIK_FORWARD_JUMP; l_d_jumping_kik_timer = App->frames; break;
			case IN_FALLING: state = ST_FALLING; break;

			case IN_LOOSE: state = LOOSE; break;
			}
		}
		break;

		case ST_JUMP_BACKWARD:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH: state = ST_IDLE; break;
			case IN_L_PUNCH: state = L_PUNCH_BACKWARD_JUMP; l_d_jumping_punch_timer = App->frames; break;
			case IN_L_KIK: state = L_KIK_BACKWARD_JUMP; l_d_jumping_kik_timer = App->frames; break;
			case IN_FALLING: state = ST_FALLING; break;

			case IN_LOOSE: state = LOOSE; break;
			}
		}
		break;

		case L_PUNCH_NEUTRAL_JUMP:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH: state = ST_IDLE; break;
			case IN_FALLING: state = ST_FALLING; break;

			case IN_LOOSE: state = LOOSE; break;
			}
		}
		break;

		case L_KIK_NEUTRAL_JUMP:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH: state = ST_IDLE; break;
			case IN_FALLING: state = ST_FALLING; break;

			case IN_LOOSE: state = LOOSE; break;
			}
		}
		break;

		case ST_HADOKEN:
		{
			switch (last_input)
			{
			case IN_HADOKEN_FINISH: state = ST_IDLE; break;
			case IN_HEAD_REEL: state = ST_HEAD_REEL; head_reel_timer = App->frames; break;
			case IN_GUT_REEL: state = ST_GUT_REEL; gut_reel_timer = App->frames; break;
			case IN_VICTORY: state = VICTORY; break;
			case IN_LOOSE: state = LOOSE; break;
			}
		}
		break;

		case L_PUNCH_FORWARD_JUMP:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH: state = ST_IDLE; break;
			case IN_PUNCH_FINISH: state = ST_JUMP_FORWARD; break;
			case IN_FALLING: state = ST_FALLING; break;

			case IN_LOOSE: state = LOOSE; break;
			}
		}
		break;

		case L_KIK_FORWARD_JUMP:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH: state = ST_IDLE; break;
			case IN_KIK_FINISH: state = ST_JUMP_FORWARD; break;
			case IN_FALLING: state = ST_FALLING; break;

			case IN_LOOSE: state = LOOSE; break;
			}
		}
		break;

		case L_PUNCH_BACKWARD_JUMP:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH: state = ST_IDLE; break;
			case IN_PUNCH_FINISH: state = ST_JUMP_BACKWARD; break;
			case IN_FALLING: state = ST_FALLING; break;

			case IN_LOOSE: state = LOOSE; break;
			}
		}
		break;

		case L_KIK_BACKWARD_JUMP:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH: state = ST_IDLE; break;
			case IN_KIK_FINISH: state = ST_JUMP_BACKWARD; break;
			case IN_FALLING: state = ST_FALLING; break;

			case IN_LOOSE: state = LOOSE; break;
			}
		}
		break;

		case L_PUNCH_STANDING:
		{
			switch (last_input)
			{
			case IN_PUNCH_FINISH: state = ST_IDLE; break;
			case IN_HEAD_REEL: state = ST_HEAD_REEL; head_reel_timer = App->frames; break;
			case IN_GUT_REEL: state = ST_GUT_REEL; gut_reel_timer = App->frames; break;
			case IN_VICTORY: state = VICTORY; break;
			case IN_LOOSE: state = LOOSE; break;
			}
		}
		break;

		case L_KIK_STANDING:
		{
			switch (last_input)
			{
			case IN_KIK_FINISH: state = ST_IDLE; break;
			case IN_HEAD_REEL: state = ST_HEAD_REEL; head_reel_timer = App->frames; break;
			case IN_GUT_REEL: state = ST_GUT_REEL; gut_reel_timer = App->frames; break;
			case IN_VICTORY: state = VICTORY; break;
			case IN_LOOSE: state = LOOSE; break;
			}
		}
		break;

		case ST_CROUCHING:
		{
			switch (last_input)
			{
			case IN_CROUCHING_FINISH: state = ST_CROUCH; break;
			case IN_CROUCH_REEL: state = ST_CROUCH_REEL; crouch_reel_timer = App->frames; break;
			case IN_LOOSE: state = LOOSE; break;
			}
		}
		break;

		case ST_STANDING:
		{
			switch (last_input)
			{
			case IN_STANDING_FINISH: state = ST_IDLE; break;
			case IN_CROUCH_REEL: state = ST_CROUCH_REEL; crouch_reel_timer = App->frames; break;
			case IN_LOOSE: state = LOOSE; break;
			}
		}
		break;

		case ST_CROUCH:
		{
			switch (last_input)
			{
			case IN_CROUCH_UP: state = ST_STANDING; standing_timer = App->frames; break;
			case IN_L_PUNCH: state = L_PUNCH_CROUCH; l_crouching_punch_timer = App->frames; break;
			case IN_L_KIK: state = L_KIK_CROUCH; l_crouching_kik_timer = App->frames; break;
			case IN_CROUCH_REEL: state = ST_CROUCH_REEL; crouch_reel_timer = App->frames; break;
			case IN_VICTORY: state = VICTORY; break;
			case IN_LOOSE: state = LOOSE; break;
			}
		}
		break;

		case L_PUNCH_CROUCH:
		{
			switch (last_input)
			{
			case IN_PUNCH_FINISH: state = ST_CROUCH; break;
			case IN_CROUCH_UP && IN_PUNCH_FINISH: state = ST_STANDING;  standing_timer = App->frames; break;
			case IN_CROUCH_REEL: state = ST_CROUCH_REEL; crouch_reel_timer = App->frames; break;
			case IN_LOOSE: state = LOOSE; break;
			}
		}
		break;

		case L_KIK_CROUCH:
		{
			switch (last_input)
			{
			case IN_KIK_FINISH: state = ST_CROUCH; break;
			case IN_CROUCH_UP && IN_KIK_FINISH: state = ST_STANDING; standing_timer = App->frames; break;
			case IN_CROUCH_REEL: state = ST_CROUCH_REEL; crouch_reel_timer = App->frames; break;
			case IN_LOOSE: state = LOOSE; break;
			}
		}
		break;

		case ST_HEAD_REEL:
		{
			switch (last_input)
			{
			case IN_REEL_FINISH:state = ST_IDLE; break;
			case IN_HEAD_REEL: state = ST_HEAD_REEL; head_reel_timer = App->frames; break;
			case IN_GUT_REEL: state = ST_GUT_REEL; gut_reel_timer = App->frames; break;
			case IN_LOOSE: state = LOOSE; break;
			}
		}
		break;

		case ST_GUT_REEL:
		{
			switch (last_input)
			{
			case IN_REEL_FINISH:state = ST_IDLE; break;
			case IN_HEAD_REEL: state = ST_HEAD_REEL; head_reel_timer = App->frames; break;
			case IN_GUT_REEL: state = ST_GUT_REEL; gut_reel_timer = App->frames; break;
			case IN_LOOSE: state = LOOSE; break;
			}
		}
		break;

		case ST_CROUCH_REEL:
		{
			switch (last_input)
			{
			case IN_REEL_FINISH:state = ST_CROUCH; break;
			case IN_CROUCH_UP && IN_KIK_FINISH: state = ST_STANDING; standing_timer = App->frames; break;
			case IN_HEAD_REEL: state = ST_CROUCH_REEL; crouch_reel_timer = App->frames; break;
			case IN_LOOSE: state = LOOSE; break;
			}
		}
		break;

		case ST_FALLING:
		{
			switch (last_input)
			{
			case IN_FALLING_FINISH:state = ST_GETTING_UP; getting_up_timer = App->frames;  break;
			case IN_LOOSE: state = LOOSE; break;
			}
		}
		break;

		case ST_GETTING_UP:
		{
			switch (last_input)
			{
			case IN_GETTING_UP_FINISH:state = ST_IDLE; break;
			}
		}
		break;

		case GRABBED:
		{
			switch (last_input)
			{
			case IN_GRABBED_FINISH:state = ST_IDLE; break;
			}
		}
		break;

		case LOOSE:
		{
			switch (last_input)
			{
			case IN_END:state = ST_IDLE; break;
			}
		}
		break;

		case VICTORY:
		{
			switch (last_input)
			{
			case IN_END:state = ST_IDLE; break;
			}
		}
		break;
		}
	}

	return state;
}