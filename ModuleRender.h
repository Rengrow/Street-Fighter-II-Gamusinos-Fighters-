#ifndef __ModuleRenderer_H__
#define __ModuleRenderer_H__

#include "Module.h"
#include "SDL\include\SDL_rect.h"

struct SDL_Renderer;
struct SDL_Texture;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PostUpdate();
	update_status Update();
	update_status PreUpdate();
	bool CleanUp();

	
	bool Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section, bool flip, float speed = 1.0f);
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera = true);

	void StartCameraShake(int duration, float magnitude);
	void UpdateCameraShake();
	
public:
	SDL_Renderer* renderer = nullptr;
	SDL_Rect camera;
	SDL_Rect limit1Box;
	SDL_Rect limit2Box;
	int cameraSpeed = 1;  //this used to be 2, but caused players to vibrate
	int scenelimit;

private:
	bool shaking = false;

	int shake_duration = 0;
	int shake_timer = 0;
	int shake_magnitude = 0;

	SDL_Point camera_offset;
	
};

#endif //__ModuleRenderer_H__