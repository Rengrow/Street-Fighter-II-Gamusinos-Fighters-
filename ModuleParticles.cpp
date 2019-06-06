#include <math.h>
#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"

#include "SDL/include/SDL_timer.h"

ModuleParticles::ModuleParticles()
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		active[i] = nullptr;
}

ModuleParticles::~ModuleParticles()
{}

// Load assets
bool ModuleParticles::Start()
{
	LOG("Loading particles");
	graphics = App->textures->Load("assets/images/sprites/sfx/sfx.png");

	//Hadoken  ATTENTION: THIS FIRES A YOGA FIRE (DHALSIM)
	hdk.anim.PushBack({ 51, 314, 44, 24 }, 2, { 0,0 }, 0, {}, {}, {});
	hdk.anim.PushBack({ 107, 315, 40, 23 }, 2, { 0,0 }, 0, {}, {}, {});
	hdk.anim.PushBack({ 153, 315, 43, 23 }, 2, { 0,0 }, 0, {}, {}, {});
	hdk.anim.loop = true;
	
	hdk.life = -1;

	ground_dust.anim.PushBack({ 295, 98, 21, 17 }, 2, { 0,0 }, 0, {}, {}, {});
	ground_dust.anim.PushBack({ 321, 96, 23, 19 }, 2, { 0,0 }, 0, {}, {}, {});
	ground_dust.anim.PushBack({ 347, 94, 25, 21 }, 2, { 0,0 }, 0, {}, {}, {});
	ground_dust.anim.PushBack({ 375, 92, 27, 23 }, 2, { 0,0 }, 0, {}, {}, {});
	ground_dust.anim.PushBack({ 405, 90, 30, 25 }, 2, { 0,0 }, 0, {}, {}, {});
	ground_dust.anim.loop = true;

	ground_dust.life = 10;

	lhead.anim.PushBack({ 242, 67, 8, 10 }, 2, { 0,0 }, 0, {}, {}, {});
	lhead.anim.PushBack({ 252, 67, 12, 10 }, 2, { 0,0 }, 0, {}, {}, {});
	lhead.anim.PushBack({ 281, 83, 12, 10 }, 2, { 0,0 }, 0, {}, {}, {});
	lhead.anim.PushBack({ 323, 67, 19, 15 }, 2, { 0,0 }, 0, {}, {}, {});
	lhead.anim.PushBack({ 295, 67, 26, 24 }, 2, { 0,0 }, 0, {}, {}, {});
	lhead.anim.loop = true;

	lhead.life = 10;

	return true;
}

// Unload assets
bool ModuleParticles::CleanUp()
{
	LOG("Unloading particles");
	App->textures->Unload(graphics);

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] != nullptr)
		{
			delete active[i];
			active[i] = nullptr;
		}
	}

	return true;
}

// Update: draw background
update_status ModuleParticles::Update()
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* p = active[i];

		if (p == nullptr)
			continue;
		if (p->Update() == false) {}
		p->collider->SetPos(p->position);
		if (SDL_GetTicks() >= p->born)
		{
			App->render->Blit(graphics, p->position.x, p->position.y, &(p->anim.GetCurrentFrameBox()), p->flip);
		}
		if (active[i] != nullptr && p->life == 0)
		{
			active[i]->collider->to_delete = true;

			delete active[i];
			active[i] = nullptr;
		}
		if(p->life > 0){
			p->life--;
		}
	}
	return UPDATE_CONTINUE;
}

void ModuleParticles::AddParticle(const Particle& particle, bool flip, int x, int y, int vx, int vy, int ryu, COLLIDER_TYPE collider_type, Mix_Chunk* sfx, Uint32 delay)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] == nullptr)
		{
			Particle* p = new Particle(particle);
			p->born = SDL_GetTicks() + delay;
			p->position.x = x;
			p->position.y = y;
			p->sfx = sfx;

			if (flip == false) {
				p->speed = { vx, vy };
				p->flip = false;
			}

			if (flip == true){
				p->speed = { -vx, vy };
				p->flip = true;
			}

			if (collider_type != COLLIDER_NONE)
				p->collider = App->collisions->AddCollider(p->anim.GetCurrentFrameBox(), collider_type, this);

			active[i] = p;

			if (p->sfx != 0) {
				App->audio->PlayChunk(p->sfx);
			}

			break;
		}
	}
}

void ModuleParticles::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		// Always destroy particles that collide
		if (active[i] != nullptr && active[i]->collider == c1)
		{
			active[i]->collider->to_delete = true;

			delete active[i];
			active[i] = nullptr; 
			break;
		}
	}
}

// -------------------------------------------------------------
// -------------------------------------------------------------

Particle::Particle()
{
	position.SetToZero();
	speed.SetToZero();
}

Particle::Particle(const Particle& p) :
	anim(p.anim), position(p.position), speed(p.speed),
	fx(p.fx), born(p.born), life(p.life)
{}

bool Particle::Update()
{
	bool ret = true;
	
	position.x += speed.x;
	position.y += speed.y;

	return ret;
}