#ifndef __MODULEUI_H__
#define __MODULEUI_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"


struct SDL_Texture;

class ModuleUI : public Module
{
public:
	ModuleUI();
	~ModuleUI();

	bool Start();
	update_status Update();
	bool CleanUp();
	void BlitText(int x, int y, int bmp_font_id, const char* text) const;

public:
	int numbers;
	SDL_Texture* lifeBars = nullptr;
	SDL_Rect lifeBarP1;
	SDL_Rect lifeBarP2;
	SDL_Rect KO; 
};

#endif // __MODULEUI_H__
