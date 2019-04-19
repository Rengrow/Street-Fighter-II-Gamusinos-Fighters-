#ifndef __ModuleFonts_H__
#define __ModuleFonts_H__

#include "Module.h"
#include "SDL\include\SDL_pixels.h"

#define MAX_FONTS 10
#define MAX_FONT_CHARS 256

struct SDL_Texture;

struct Font
{
	char table[MAX_FONT_CHARS];
	SDL_Texture* graphic = nullptr;
	uint rows, len, char_w, char_h, row_chars;
};

class ModuleFonts : public Module
{
public:

	ModuleFonts();
	~ModuleFonts();

	// Load Font
	int Load(const char* texture_path, const char* characters, uint rows = 1);
	void UnLoad(int font_id);

public:
	uint width;
	uint height;
	Uint32 timer_timer = 0;
	char tiempo[2] = {'9', '9'};
	bool end = false;
	int timerbuffx = 170;
	int life;
	int life2;

public:
	// Create a surface from text
	void BlitText(int x, int y, int bmp_font_id, const char* text) const;
	void TimerBlit(int font_id, Module *module_call);
	void LifeBlit(int module_call, SDL_Texture* texture, int x, int y, SDL_Rect* section, bool flip, float speed);

private:

	Font fonts[MAX_FONTS];
};


#endif // __ModuleFonts_H__