#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneKen.h"
#include "ModuleCollision.h"
#include "ModuleRyu.h"
#include "ModuleDhalsim.h"
#include "SDL/include/SDL.h"
#include <cstdlib>
#include <time.h>

ModuleRender::ModuleRender() : Module()
{
	camera.x = camera.y = camera_offset.x = camera_offset.y = 0;
	camera.w = SCREEN_WIDTH;
	camera.h = SCREEN_HEIGHT;

	srand(time(NULL));

}

// Destructor
ModuleRender::~ModuleRender()
{}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");
	bool ret = true;
	Uint32 flags = 0;

	if (REN_VSYNC == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
	}

	renderer = SDL_CreateRenderer(App->window->window, -1, flags);

	if (renderer == NULL)
	{
		LOG("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleRender::PreUpdate()
{
	SDL_RenderClear(renderer);

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleRender::Update()
{
	
	// TO DO: this exact coding causes the following problem: camera wont move right if opposite ryu is at left corner (the same way around)

	if (App->ryu->position.x > -camera.x / SCREEN_SIZE && App->ryu->position.x < -camera.x / SCREEN_SIZE + camera.w / 4 && camera.x != 0) {
		camera.x += cameraSpeed * SCREEN_SIZE;
		limit1Box.x -= cameraSpeed;
		limit2Box.x -= cameraSpeed;
	}


	if (App->dhalsim->position.x > -camera.x / SCREEN_SIZE && App->dhalsim->position.x < -camera.x / SCREEN_SIZE + camera.w / 4 && camera.x != 0) {
		camera.x += cameraSpeed * SCREEN_SIZE;
		limit1Box.x -= cameraSpeed;
		limit2Box.x -= cameraSpeed;
	}


	if ((App->ryu->position.x < -camera.x / SCREEN_SIZE + camera.w) && (App->ryu->position.x > -camera.x / SCREEN_SIZE + (camera.w * 3 / 4)) && (-camera.x / SCREEN_SIZE + camera.w != scenelimit) && (-camera.x / SCREEN_SIZE + camera.w < scenelimit)) {
		camera.x -= cameraSpeed * SCREEN_SIZE;
		limit1Box.x += cameraSpeed;
		limit2Box.x += cameraSpeed;
	}

	if (App->dhalsim->position.x < -camera.x / SCREEN_SIZE + camera.w && App->dhalsim->position.x > -camera.x / SCREEN_SIZE + (camera.w * 3 / 4) && (-camera.x / SCREEN_SIZE + camera.w != scenelimit) && (-camera.x / SCREEN_SIZE + camera.w < scenelimit)) {
		camera.x -= cameraSpeed * SCREEN_SIZE;
		limit1Box.x += cameraSpeed;
		limit2Box.x += cameraSpeed;
	}
	/*
	//CAMERA DEBUG
	if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_STATE::KEY_REPEAT) {
		if (camera.x != 0) {
			camera.x += cameraSpeed * SCREEN_SIZE;
			limit1Box.x -= cameraSpeed;
			limit2Box.x -= cameraSpeed;
		}
	}

	if (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_STATE::KEY_REPEAT) {
		if ((-camera.x + camera.w != scenelimit) && (-camera.x + camera.w < scenelimit)) {
			camera.x -= cameraSpeed * SCREEN_SIZE;
			limit1Box.x += cameraSpeed;
			limit2Box.x += cameraSpeed;
		}
	}*/
	
	return update_status::UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	SDL_RenderPresent(renderer);

	if (SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT) == 0)
	{
		return update_status::UPDATE_CONTINUE;
	}

	return update_status::UPDATE_ERROR;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy window
	if (renderer != NULL)
	{
		SDL_DestroyRenderer(renderer);
	}

	return true;
}

// Blit to screen
bool ModuleRender::Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section, bool flip, float speed)
{
	bool ret = true;
	SDL_Rect rect;
	rect.x = (int)((camera.x + camera_offset.x) * speed) + x * SCREEN_SIZE;
	rect.y = (int)((camera.y + camera_offset.y) * speed) + y * SCREEN_SIZE;

	if (section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= SCREEN_SIZE;
	rect.h *= SCREEN_SIZE;

	if (SDL_RenderCopyEx(renderer, texture, section, &rect, NULL, NULL, (flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE)) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera)
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if (use_camera)
	{
		rec.x = (int)(camera.x + rect.x * SCREEN_SIZE);
		rec.y = (int)(camera.y + rect.y * SCREEN_SIZE);
		rec.w *= SCREEN_SIZE;
		rec.h *= SCREEN_SIZE;
	}

	if (SDL_RenderFillRect(renderer, &rec) != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

void ModuleRender::StartCameraShake(int duration, float magnitude)
{
	shake_duration = duration;
	shake_magnitude = magnitude;
	shake_timer = 0;
	shaking = true;
}

void ModuleRender::UpdateCameraShake()
{
	if ((shake_duration - shake_timer) > 0) {
		camera_offset = { rand() % (int)shake_magnitude, rand() % (int)shake_magnitude };
		shake_timer++;
	}
	else
		shaking = false;
}