#ifndef __MODULECHSELECTION_H__
#define __MODULECHSELECTION_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "SDL_Mixer/include/SDL_mixer.h"
#include <string>

struct Mix_Chunk;
struct SDL_Texture;

class ModuleChSelection : public Module
{
public:
	ModuleChSelection();
	~ModuleChSelection();

	bool Start();
	update_status Update();
	bool CleanUp();

private:
	void ReadPlayer1Inputs();
	void ReadPlayer2Inputs();
	void PrintMap();
	void PrintPanel();
	void PrintPortraits();
	void PrintPlayerCursors();

public:
	SDL_Rect screen;
	SDL_Texture* graphics = nullptr;
	SDL_Texture* portraits = nullptr;
	int typography;
	int typographyBig;
	bool isFading;
	SDL_Rect map;
	SDL_Rect thailandFlag;

	Mix_Music *music = nullptr;
	Mix_Chunk* move = nullptr;
	Mix_Chunk* invalidSelect = nullptr;
	Mix_Chunk* select = nullptr;
	Uint32 timeSelector;
	Uint32 canSelectTimer;
	bool lettersOn;
	
	Uint32 player1MoveTimer;
	SDL_Rect player1Pointer;
	SDL_Rect player1Cursor;
	SDL_Rect player1CursorLocked;
	iPoint player1Position;
	SDL_Rect player1Portrait;
	char* player1Name;
	bool player1Lock;

	Uint32 player2MoveTimer;
	SDL_Rect player2Pointer;
	SDL_Rect player2Cursor;
	SDL_Rect player2CursorLocked;
	iPoint player2Position;
	SDL_Rect player2Portrait;
	char* player2Name;
	bool player2Lock;

	SDL_Rect airplane;

	SDL_Rect ryuFace;
	SDL_Rect ryuPortrait;
	std::string ryuName = "RYU";
	SDL_Rect hondaFace;
	SDL_Rect hondaPortrait;
	std::string hondaName = "E. HONDA";
	SDL_Rect blankaFace;
	SDL_Rect blankaPortrait;
	std::string blankaName = "BLANKA";
	SDL_Rect guileFace;
	SDL_Rect guilePortrait;
	std::string guileName = "GUILE";
	SDL_Rect kenFace;
	SDL_Rect kenPortrait;
	std::string kenName = "KEN";
	SDL_Rect chunLiFace;
	SDL_Rect chunLiPortrait;
	std::string chunLiName = "CHUN LI";
	SDL_Rect zangiefFace;
	SDL_Rect zangiefPortrait;
	std::string zangiefName = "ZANGIEF";
	SDL_Rect dhalsimFace;
	SDL_Rect dhalsimPortrait;
	std::string dhalsimName = "DHALSIM";
};

#endif // __MODULEENDBATTLE_H__