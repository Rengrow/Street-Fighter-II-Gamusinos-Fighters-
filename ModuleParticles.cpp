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

		if (p->Update() == false)
		{
			delete p;
			active[i] = nullptr;
		}

		else if (SDL_GetTicks() >= p->born)
		{
				App->render->Blit(graphics, p->position.x, p->position.y, &(p->anim.GetCurrentFrameBox()), p->flip);
		}
		p->collider->SetPos(p->position);
	}

	return UPDATE_CONTINUE;
}

void ModuleParticles::AddParticle(const Particle& particle, bool flip, int x, int y, int ryu, COLLIDER_TYPE collider_type, Mix_Chunk* sfx, Uint32 delay)
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
				p->speed = { 3, 0 };
				p->flip = false;
			}

			if (flip == true){
				p->speed = { -3, 0 };
				p->flip = true;
			}

			if (collider_type != COLLIDER_NONE)
				p->collider = App->collisions->AddCollider(p->anim.GetCurrentFrameBox(), collider_type, this);

			active[i] = p;

			App->audio->PlayChunk(p->sfx);

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

	if (life > 0)
	{
		if ((SDL_GetTicks() - born) > life)
			ret = false;
	}
	else
		if (anim.Finished())
			ret = false;

	position.x += speed.x;
	position.y += speed.y;

	return ret;
}