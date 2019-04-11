#ifndef __ModuleSecondPlayer_H__
#define __ModuleSecondPlayer_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"

struct SDL_Texture;

class ModuleSecondPlayer : public Module
{
public:
	ModuleSecondPlayer();
	~ModuleSecondPlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

public:

	Collider* collider = nullptr;
	SDL_Texture* graphics2 = nullptr;
	Animation idle2;
	Animation forward2;
	Animation backward2;
	Animation lp2, lk2;
	Animation neutralJump2;
	iPoint position;
	Animation hdk;
	Animation streel;
	p2Point <int> pivot2 = { 250, 220 };		// The pivot is found in the furthest toe from the enemy. The 250 equals to the x = 0 that the character has respect his spritesheet
	void OnCollision(Collider* c1, Collider* c2);
	bool atacar = false;
	bool jump = false;
	bool flip = true;
	bool avanzar = true;
	bool retroceder = true;
	int framesAtaque = 0;
	int framesJump = 0;
	int mov; //lp, mp, hp, lk, mk, hk
};

#endif