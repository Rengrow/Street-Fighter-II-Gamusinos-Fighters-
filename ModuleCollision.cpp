#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModulePlayer1.h"
#include "ModulePlayer2.h"
#include "ModuleSceneKen.h"
#include "ModuleSceneSagat.h"
#include "ModuleFadeToBlack.h"

ModuleCollision::ModuleCollision()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER2] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER_SHOT] = true;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER2_SHOT] = true;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER_HIT] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER2_HIT] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER2_GRAB] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER2_GRABBOX] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER_GRAB] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER_GRABBOX] = false;

	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER2] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER_SHOT] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER2_SHOT] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER_HIT] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER2_HIT] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER_GRAB] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER_GRABBOX] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER2_GRAB] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER2_GRABBOX] = false;

	matrix[COLLIDER_PLAYER2][COLLIDER_WALL] = false;
	matrix[COLLIDER_PLAYER2][COLLIDER_PLAYER2] = false;
	matrix[COLLIDER_PLAYER2][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER2][COLLIDER_PLAYER2_SHOT] = false;
	matrix[COLLIDER_PLAYER2][COLLIDER_PLAYER_SHOT] = true;
	matrix[COLLIDER_PLAYER2][COLLIDER_PLAYER2_HIT] = false;
	matrix[COLLIDER_PLAYER2][COLLIDER_PLAYER_HIT] = true;
	matrix[COLLIDER_PLAYER2][COLLIDER_PLAYER_GRAB] = true;
	matrix[COLLIDER_PLAYER2][COLLIDER_PLAYER_GRABBOX] = false;
	matrix[COLLIDER_PLAYER2][COLLIDER_PLAYER2_GRAB] = false;
	matrix[COLLIDER_PLAYER2][COLLIDER_PLAYER2_GRABBOX] = false;

	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_PLAYER2] = true;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_PLAYER_SHOT] = false;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_PLAYER2_SHOT] = true;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_PLAYER_HIT] = false;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_PLAYER2_HIT] = false;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_PLAYER_GRAB] = false;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_PLAYER_GRABBOX] = false;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_PLAYER2_GRAB] = false;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_PLAYER2_GRABBOX] = false;

	matrix[COLLIDER_PLAYER2_SHOT][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER2_SHOT][COLLIDER_PLAYER2] = false;
	matrix[COLLIDER_PLAYER2_SHOT][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_PLAYER2_SHOT][COLLIDER_PLAYER2_SHOT] = false;
	matrix[COLLIDER_PLAYER2_SHOT][COLLIDER_PLAYER_SHOT] = true;
	matrix[COLLIDER_PLAYER2_SHOT][COLLIDER_PLAYER2_HIT] = false;
	matrix[COLLIDER_PLAYER2_SHOT][COLLIDER_PLAYER_HIT] = false;
	matrix[COLLIDER_PLAYER2_SHOT][COLLIDER_PLAYER_GRAB] = false;
	matrix[COLLIDER_PLAYER2_SHOT][COLLIDER_PLAYER_GRABBOX] = false;
	matrix[COLLIDER_PLAYER2_SHOT][COLLIDER_PLAYER2_GRAB] = false;
	matrix[COLLIDER_PLAYER2_SHOT][COLLIDER_PLAYER2_GRABBOX] = false;

	matrix[COLLIDER_PLAYER_HIT][COLLIDER_WALL] = false;
	matrix[COLLIDER_PLAYER_HIT][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER_HIT][COLLIDER_PLAYER2] = true;
	matrix[COLLIDER_PLAYER_HIT][COLLIDER_PLAYER_SHOT] = false;
	matrix[COLLIDER_PLAYER_HIT][COLLIDER_PLAYER2_SHOT] = false;
	matrix[COLLIDER_PLAYER_HIT][COLLIDER_PLAYER_HIT] = false;
	matrix[COLLIDER_PLAYER_HIT][COLLIDER_PLAYER2_HIT] = false;
	matrix[COLLIDER_PLAYER_HIT][COLLIDER_PLAYER_GRAB] = false;
	matrix[COLLIDER_PLAYER_HIT][COLLIDER_PLAYER_GRABBOX] = false;
	matrix[COLLIDER_PLAYER_HIT][COLLIDER_PLAYER2_GRAB] = false;
	matrix[COLLIDER_PLAYER_HIT][COLLIDER_PLAYER2_GRABBOX] = false;

	matrix[COLLIDER_PLAYER2_HIT][COLLIDER_WALL] = false;
	matrix[COLLIDER_PLAYER2_HIT][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_PLAYER2_HIT][COLLIDER_PLAYER2] = false;
	matrix[COLLIDER_PLAYER2_HIT][COLLIDER_PLAYER_SHOT] = false;
	matrix[COLLIDER_PLAYER2_HIT][COLLIDER_PLAYER2_SHOT] = false;
	matrix[COLLIDER_PLAYER2_HIT][COLLIDER_PLAYER_HIT] = false;
	matrix[COLLIDER_PLAYER2_HIT][COLLIDER_PLAYER2_HIT] = false;
	matrix[COLLIDER_PLAYER2_HIT][COLLIDER_PLAYER_GRAB] = false;
	matrix[COLLIDER_PLAYER2_HIT][COLLIDER_PLAYER_GRABBOX] = false;
	matrix[COLLIDER_PLAYER2_HIT][COLLIDER_PLAYER2_GRAB] = false;
	matrix[COLLIDER_PLAYER2_HIT][COLLIDER_PLAYER2_GRABBOX] = false;

	matrix[COLLIDER_PLAYER_GRAB][COLLIDER_WALL] = false;
	matrix[COLLIDER_PLAYER_GRAB][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER_GRAB][COLLIDER_PLAYER2] = true;
	matrix[COLLIDER_PLAYER_GRAB][COLLIDER_PLAYER_SHOT] = false;
	matrix[COLLIDER_PLAYER_GRAB][COLLIDER_PLAYER2_SHOT] = false;
	matrix[COLLIDER_PLAYER_GRAB][COLLIDER_PLAYER_HIT] = false;
	matrix[COLLIDER_PLAYER_GRAB][COLLIDER_PLAYER2_HIT] = false;
	matrix[COLLIDER_PLAYER_GRAB][COLLIDER_PLAYER_GRAB] = false;
	matrix[COLLIDER_PLAYER_GRAB][COLLIDER_PLAYER2_GRAB] = false;
	matrix[COLLIDER_PLAYER_GRAB][COLLIDER_PLAYER_GRABBOX] = false;
	matrix[COLLIDER_PLAYER_GRAB][COLLIDER_PLAYER2_GRABBOX] = true;

	matrix[COLLIDER_PLAYER2_GRAB][COLLIDER_WALL] = false;
	matrix[COLLIDER_PLAYER2_GRAB][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_PLAYER2_GRAB][COLLIDER_PLAYER2] = false;
	matrix[COLLIDER_PLAYER2_GRAB][COLLIDER_PLAYER_SHOT] = false;
	matrix[COLLIDER_PLAYER2_GRAB][COLLIDER_PLAYER2_SHOT] = false;
	matrix[COLLIDER_PLAYER2_GRAB][COLLIDER_PLAYER_HIT] = false;
	matrix[COLLIDER_PLAYER2_GRAB][COLLIDER_PLAYER2_HIT] = false;
	matrix[COLLIDER_PLAYER2_GRAB][COLLIDER_PLAYER_GRAB] = false;
	matrix[COLLIDER_PLAYER2_GRAB][COLLIDER_PLAYER2_GRAB] = false;
	matrix[COLLIDER_PLAYER2_GRAB][COLLIDER_PLAYER_GRABBOX] = true;
	matrix[COLLIDER_PLAYER2_GRAB][COLLIDER_PLAYER2_GRABBOX] = false;

	matrix[COLLIDER_PLAYER_GRABBOX][COLLIDER_WALL] = false;
	matrix[COLLIDER_PLAYER_GRABBOX][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER_GRABBOX][COLLIDER_PLAYER2] = false;
	matrix[COLLIDER_PLAYER_GRABBOX][COLLIDER_PLAYER_SHOT] = false;
	matrix[COLLIDER_PLAYER_GRABBOX][COLLIDER_PLAYER_SHOT] = false;
	matrix[COLLIDER_PLAYER_GRABBOX][COLLIDER_PLAYER_HIT] = false;
	matrix[COLLIDER_PLAYER_GRABBOX][COLLIDER_PLAYER2_HIT] = false;
	matrix[COLLIDER_PLAYER_GRABBOX][COLLIDER_PLAYER_GRAB] = false;
	matrix[COLLIDER_PLAYER_GRABBOX][COLLIDER_PLAYER2_GRAB] = true;
	matrix[COLLIDER_PLAYER_GRABBOX][COLLIDER_PLAYER_GRABBOX] = false;
	matrix[COLLIDER_PLAYER_GRABBOX][COLLIDER_PLAYER2_GRABBOX] = false;

	matrix[COLLIDER_PLAYER2_GRABBOX][COLLIDER_WALL] = false;
	matrix[COLLIDER_PLAYER2_GRABBOX][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER2_GRABBOX][COLLIDER_PLAYER2] = false;
	matrix[COLLIDER_PLAYER2_GRABBOX][COLLIDER_PLAYER_SHOT] = false;
	matrix[COLLIDER_PLAYER2_GRABBOX][COLLIDER_PLAYER_SHOT] = false;
	matrix[COLLIDER_PLAYER2_GRABBOX][COLLIDER_PLAYER_HIT] = false;
	matrix[COLLIDER_PLAYER2_GRABBOX][COLLIDER_PLAYER2_HIT] = false;
	matrix[COLLIDER_PLAYER2_GRABBOX][COLLIDER_PLAYER_GRAB] = false;
	matrix[COLLIDER_PLAYER2_GRABBOX][COLLIDER_PLAYER2_GRAB] = false;
	matrix[COLLIDER_PLAYER2_GRABBOX][COLLIDER_PLAYER_GRABBOX] = false;
	matrix[COLLIDER_PLAYER2_GRABBOX][COLLIDER_PLAYER2_GRABBOX] = false;
}

// Destructor
ModuleCollision::~ModuleCollision()
{}

update_status ModuleCollision::PreUpdate()
{
	RemoveDeletedColliders();

	return UPDATE_CONTINUE;
}

// Called before render is available
update_status ModuleCollision::Update()
{
	DebugDraw();

	return UPDATE_CONTINUE;
}

update_status ModuleCollision::PostUpdate()
{
	CalculateCollisions();

	return UPDATE_CONTINUE;
}

void ModuleCollision::RemoveDeletedColliders() {

	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
}

void ModuleCollision::CalculateCollisions() {
	// Calculate collisions
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c1->CheckCollision(c2->rect) == true)
			{
				if (matrix[c1->type][c2->type] && c1->callback)
					c1->callback->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->callback)
					c2->callback->OnCollision(c2, c1);
			}
		}
	}
}

void ModuleCollision::DebugDraw()
{
	if (App->input->keyboard[SDL_SCANCODE_F1] == KEY_DOWN)
		debug = !debug;

	if (debug == false)
		return;

	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case COLLIDER_NONE: // white
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
			break;

		case COLLIDER_WALL: // blue
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
			break;

		case COLLIDER_PLAYER: // green
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;

		case COLLIDER_PLAYER2: // red
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
			break;

		case COLLIDER_PLAYER_SHOT: // yellow
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 0, alpha);
			break;

		case COLLIDER_PLAYER2_SHOT: // cyan
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 255, alpha);
			break;

		case COLLIDER_PLAYER_HIT: // magenta
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 255, alpha);
			break;

		case COLLIDER_PLAYER2_HIT: // black
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 0, alpha);
			break;

		case COLLIDER_PLAYER_GRAB: //dark yellowish
			App->render->DrawQuad(colliders[i]->rect, 204, 204, 0, alpha);
			break;

		case COLLIDER_PLAYER2_GRAB: //faded yellow
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 102, alpha);
			break;

		case COLLIDER_PLAYER_GRABBOX: //dark greenish
			App->render->DrawQuad(colliders[i]->rect, 0, 153, 0, alpha);
			break;

		case COLLIDER_PLAYER2_GRABBOX: //faded green
			App->render->DrawQuad(colliders[i]->rect, 153, 255, 153, alpha);
			break;
		}
	}
	App->render->DrawQuad({ App->player1->position.x, App->player1->position.y, 2, 2 }, 0, 0, 0, 100);
	App->render->DrawQuad({ App->player2->position.x, App->player2->position.y, 2, 2 }, 0, 0, 0, 100);
}

// Called before quitting
bool ModuleCollision::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* ModuleCollision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}

	return ret;
}

Collider* ModuleCollision::AddCollider(Collider collider)
{
	Collider* ret = new Collider(collider.rect, collider.type, collider.callback);

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			colliders[i] = ret;
			break;
		}
	}

	return ret;
}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return !(rect.x + rect.w < r.x ||
		rect.y + rect.h < r.y ||
		r.x + r.w < rect.x ||
		r.y + r.h < rect.y);
}