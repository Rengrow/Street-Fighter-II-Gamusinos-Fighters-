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
	void TimerBlit(int font_id);

public:
	int numbers;
	SDL_Texture* lifeBars = nullptr;
	SDL_Rect lifeBarP1;
	SDL_Rect lifeBarP2;
	SDL_Rect KO; 
	SDL_Rect redKO;

	bool end;
	Uint32 timer = 0;

	int timerx = 170;
	char tiempo[2] = { '9', '9' };
};

#endif // __MODULEUI_H__
