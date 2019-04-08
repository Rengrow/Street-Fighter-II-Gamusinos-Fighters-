#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"

struct SDL_Texture;

class ModulePlayer : public Module
{
public:
	ModulePlayer();
	~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	Collider* collider = nullptr;
	Collider* punch_collider = nullptr;
	SDL_Texture* graphics = nullptr;
	Animation idle;
	Animation forward;
	Animation backward;
	Animation lp, lk;
	Animation neutralJump;
	Animation hdk;
	Animation streel;
	iPoint position;
	void OnCollision(Collider* c1, Collider* c2);
	bool atacar = false;
	bool jump = false;
	bool godmode = false;
	bool avanzar = true;
	bool retroceder = true;
	int framesAtaque = 0;
	int framesJump = 0;
	int mov; //lp, mp, hp, lk, mk, hk
};

#endif