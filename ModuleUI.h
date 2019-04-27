#ifndef __MODULEUI_H__
#define __MODULEUI_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"

#define LIFE_BAR_LENGHT 150

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
	void StartEndFight(int player);

private:
	void TimerBlit(int font_id);
	void LifeBarsBlit();
	void KoBlit();
	void StartFightBlit();
	void RoundsWinnedBlit();
	void EndFight();

public:
	int numbers;
	int typography1;
	int stopedTimer;

	SDL_Texture* lifeBars = nullptr;
	SDL_Rect lifeBarP1;
	SDL_Rect lifeBarP2;
	SDL_Rect KO;
	SDL_Rect redKO;
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
