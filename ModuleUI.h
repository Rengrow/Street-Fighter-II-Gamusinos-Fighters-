#ifndef __MODULEUI_H__
#define __MODULEUI_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"

#define LIFE_BAR_LENGHT 150

struct SDL_Texture;

class ModuleUI : public Module
{
public:
	ModuleUI();
	~ModuleUI();

	bool Start();
	update_status PostUpdate();
	bool CleanUp();
	void StartTimer();
	void StartFight();
	void StartEndFight(int player);
	int GetTimer();

private:
	void TimerBlit(int font_id);
	void LifeBarsBlit();
	void KoBlit();
	void StartFightBlit(int font_id);
	void EndFight();

public:
	int numbers;
	int typography1;
	Uint32 timeOutTimer;
	Uint32 countdownStartFight;
	Uint32 endFightTimer;
	int stopedTimer;
	SDL_Texture* lifeBars = nullptr;
	SDL_Rect lifeBarP1;
	SDL_Rect lifeBarP2;
	SDL_Rect KO; 
	SDL_Rect redKO;
	int timerx = 170;
	char tiempo[2];
	bool redKoEnabled;
	bool endFightStarted;
	bool stopedFight;

private:
	bool timerStarted;
	bool starFight;	
	int winnerPlayer = 0;
};

#endif // __MODULEUI_H__
