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
	int typography;
	Uint32 timeLetters;
	bool quoteOption;
	std::string quote1 = "i will meditate and then destroy you!!";
	std::string quote2 = "now you've realized the inner mysteries of yoga!";
	std::string quoteTemp;
	char *cstr;
	int quoteLetters = 0;
};

#endif // __MODULEENDBATTLE_H__