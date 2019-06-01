#ifndef __MODULEUI_H__
#define __MODULEUI_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"

#define LIFE_BAR_LENGHT 136

struct SDL_Texture;
struct Mix_Chunk;

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
	void StartEndFight(int ryu);

private:
	void TimerBlit(int font_id);
	void LifeBarsBlit();
	void StartFightBlit();
	void RoundsWinnedBlit();
	void EndFight();
	void BlitGamePadDebug();

public:
	int numbers;
	int typography1;
	int typographyDebug;
	int stopedTimer;

	SDL_Texture* graphics = nullptr;
	SDL_Rect lifeBars;
	SDL_Rect yelllowBar1;
	SDL_Rect yelllowBar2;
	SDL_Rect iconRoundWinned;
	SDL_Rect roundRect;
	SDL_Rect round1Rect;
	SDL_Rect round2Rect;
	SDL_Rect round3Rect;
	SDL_Rect roundFRect;
	SDL_Rect fightRect;

	Mix_Chunk* round_snd = nullptr;
	Mix_Chunk* one_snd = nullptr;
	Mix_Chunk* two_snd = nullptr;
	Mix_Chunk* three_snd = nullptr;
	Mix_Chunk* final_snd = nullptr;
	Mix_Chunk* fight_snd = nullptr;
	Mix_Chunk* you_snd = nullptr;
	Mix_Chunk* win_snd = nullptr;
	Mix_Chunk* lose_snd = nullptr;

	int timerx = 170;
	char tiempo[2];
	bool redKoEnabled;
	bool debugGamepads = false;
	int winnerPlayer = 0;

private:
	bool timerStarted;
	bool starFight;
	bool roundSoundPlayed;
	bool numberRoundSoundPlayed;
	bool fightSoundPlayed;

	bool youFinalSound;
	bool winLoseFinalSound;
};

#endif // __MODULEUI_H__
