#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleFadeToBlack.h"
#include "ModuleFonts.h"
#include<string.h>

ModuleUI::ModuleUI()
{
	lifeBarP1.x = 0;
	lifeBarP1.y = 2;
	lifeBarP1.w = 148;
	lifeBarP1.h = 14;

	lifeBarP2.x = 0;
	lifeBarP2.y = 2;
	lifeBarP2.w = 148;
	lifeBarP2.h = 14;

	KO.x = 337;
	KO.y = 0;
	KO.w = 27;
	KO.h = 23;
}

ModuleUI::~ModuleUI()
{}

// Load assets
bool ModuleUI::Start()
{
	LOG("Loading UI assets");
	bool ret = true;

	numbers = App->fonts->Load("assets/images/ui/Letters.png", "abcdefghijklmnopqrstuvwxyz.;:1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ,_|@#~€¬/()='", 1);
	lifeBars = App->textures->Load("assets/images/ui/Life_bar.png");

	return ret;
}

// Load assets
bool ModuleUI::CleanUp()
{

	App->textures->Unload(lifeBars);
	App->fonts->UnLoad(numbers);

	lifeBars = nullptr;

	return true;
}

// Update: draw background
update_status ModuleUI::Update()
{
	App->render->Blit(lifeBars, -App->render->camera.x/SCREEN_SIZE+24, 20, &lifeBarP1, false);

	App->render->Blit(lifeBars, -App->render->camera.x /SCREEN_SIZE + lifeBarP1.w +51, 20, &lifeBarP2, false);

	App->render->Blit(lifeBars, -App->render->camera.x / SCREEN_SIZE + lifeBarP1.w +23 , 15, &KO, false);
	return UPDATE_CONTINUE;
}

// Render text using a bitmap font
void ModuleUI::BlitText(int x, int y, int font_id, const char* text) const
{
	if (text == nullptr || font_id < 0 || font_id >= MAX_FONTS || App->fonts->fonts[font_id].graphic == nullptr)
	{
		LOG("Unable to render text with bmp font id %d", font_id);
		return;
	}

	const Font* font = &App->fonts->fonts[font_id];
	SDL_Rect rect;
	uint len = strlen(text);

	rect.w = font->char_w;
	rect.h = font->char_h;

	for (int i = 0; i < len; i++)
	{
		bool foundCharacter = false;
		int j = 0;
		do
		{
			// TODO 2: Find the character in the table and its position in the texture, then Blit
			if (font->table[j] == text[i]) {
				rect.x = j * font->char_w;
				rect.y = 0;
				App->render->Blit(font->graphic, x, y, &rect, 0, false);
				x += font->char_w;
				foundCharacter = true;
			}
			else
				j++;

		} while (!foundCharacter && j < font->len);
	}
}