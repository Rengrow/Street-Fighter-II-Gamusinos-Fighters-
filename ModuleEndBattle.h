#ifndef __MODULEENDBATTLE_H__
#define __MODULEENDBATTLE_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include <string>
#include "SDL_Mixer/include/SDL_mixer.h" //Arreglar

struct SDL_Texture;

class ModuleEndBattle : public Module
{
public:
	ModuleEndBattle();
	~ModuleEndBattle();

	bool Start();
	update_status Update();
	bool CleanUp();
	void UpdateQuote();

public:
	SDL_Texture* portraits = nullptr;
	Mix_Music *music = nullptr;
	SDL_Rect portrait1;
	SDL_Rect portrait2;
	SDL_Rect portraitP1;
	SDL_Rect portraitP2;
	int typography;
	Uint32 timeLetters;
	bool quoteOption;

	std::string quote1 = "I will meditate and then"; //awesome line jump implemented here
	std::string quote1sub = "destroy you!!";
	std::string quote2 = "Now you've realized the ";
	std::string quote2sub = "inner mysteries of yoga!";

	std::string quoteTemp;
	char *cstr;
	char *cstr2;
	int quoteLetters = 0;
};

#endif // __MODULEENDBATTLE_H__