#include "Globals.h"
#include "Application.h"
#include "ModuleWelcomePage.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
#include "ModuleFonts.h"

#include "SDL\include\SDL.h"

ModuleWelcomePage::ModuleWelcomePage()
{
	screen = { 0, 0, SCREEN_WIDTH * SCREEN_SIZE, SCREEN_HEIGHT * SCREEN_SIZE };
	logo = { 0,0,384,224 };
	building = { 0,0,320,480 };
	sky1 = { 320,0,79,297 };
	sky2 = { 560,0,79,297 };
	p11 = { 320,298,131,83 };
	p21 = { 452,298,117,93 };
	pHit = { 569,298,281,128 };
	p12 = { 850, 251, 93, 144 };
	p22 = { 320,425,245,102 };
	p23 = { 569,426,222,96 };
}

ModuleWelcomePage::~ModuleWelcomePage()
{}

// Load assets
bool ModuleWelcomePage::Start()
{
	LOG("Loading background assets");
	bool ret = true;
	intro = App->textures->Load("assets/images/ui/intro.png");
	graphics1 = App->textures->Load("assets/images/ui/logo1.png");
	graphics2 = App->textures->Load("assets/images/ui/logo2.png");
	typography = App->fonts->Load("assets/images/ui/FontMain.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789~'!@#$%^&*()-_+=[]{}| :;¨º<>,./?", 1);
	music = App->audio->LoadSong("assets/music/title.ogg");
	start = App->audio->LoadChunk("assets/sfx/effects/coin.wav");
	fist = App->audio->LoadChunk("assets/sfx/effects/fist_intro.wav");

	people.PushBack({ 0, 480, 128, 79 }, 20, { 0,0 }, 0, {}, {}, {});
	people.PushBack({ 131, 480, 128, 79 }, 20, { 0,0 }, 0, {}, {}, {});

	p1Face.PushBack({ 640, 0, 71, 54 }, 10, { 0,0 }, 0, {}, {}, {});
	/*p1Face.PushBack({ 711, 0, 71, 54 }, 10, { 0,0 }, 0, {}, {}, {});
	p1Face.PushBack({ 782, 0, 71, 54 }, 10, { 0,0 }, 0, {}, {}, {});
	p1Face.PushBack({ 853, 0, 71, 54 }, 10, { 0,0 }, 0, {}, {}, {});
	p1Face.PushBack({ 924, 0, 71, 54 }, 10, { 0,0 }, 0, {}, {}, {});
	p1Face.PushBack({ 640, 54, 71, 54 }, 10, { 0,0 }, 0, {}, {}, {});
	p1Face.PushBack({ 711, 54, 71, 54 }, 10, { 0,0 }, 0, {}, {}, {});
	p1Face.PushBack({ 782, 54, 71, 54 }, 10, { 0,0 }, 0, {}, {}, {});
	p1Face.PushBack({ 854, 54, 73, 53 }, 10, { 2,0 }, 0, {}, {}, {});
	p1Face.PushBack({ 926, 54, 74, 54 }, 10, { 3,0 }, 0, {}, {}, {});
	p1Face.PushBack({ 640, 109, 75, 56 }, 10, { 4,0 }, 0, {}, {}, {});
	p1Face.PushBack({ 715, 109, 76, 60 }, 10, { 5,0 }, 0, {}, {}, {});
	p1Face.PushBack({ 791, 109, 78, 62 }, 10, { 7,0 }, 0, {}, {}, {});
	p1Face.PushBack({ 869, 109, 79, 54 }, 10, { 8,0 }, 0, {}, {}, {});
	p1Face.PushBack({ 640, 169, 76, 67 }, 10, { 5,0 }, 0, {}, {}, {});
	p1Face.PushBack({ 716, 171, 75, 69 }, 10, { 5,0 }, 0, {}, {}, {});
	p1Face.PushBack({ 793, 171, 72, 72 }, 10, { 1,0 }, 0, {}, {}, {});
	p1Face.PushBack({ 865, 173, 71, 76 }, 10, { 0,0 }, 0, {}, {}, {});
	p1Face.PushBack({ 936, 169, 70, 79 }, 10, { 0,0 }, 0, {}, {}, {});*/
	p1Face.loop = false;

	App->audio->PlaySongDelay(music, -1, 10000);
	App->render->camera.x = App->render->camera.y = fila = columna = timeLetters = pageTimer = p1PostionUp = p2PostionUp = alpha = 0;
	segundaSheet = lettersOn = endedIntro = false;
	cameraPosition = p1Position = p2Position = { 0,0 };

	return ret;
}

// Load assets
bool ModuleWelcomePage::CleanUp()
{
	LOG("Unloading End battle screen");

	App->textures->Unload(graphics2);
	App->textures->Unload(graphics1);
	App->textures->Unload(intro);

	App->audio->UnloadChunk(fist);
	fist = nullptr;

	App->audio->UnloadChunk(start);
	start = nullptr;

	App->audio->UnloadSong(music);
	music = nullptr;

	people = p1Face = Animation();

	return true;
}

// Update: draw background
update_status ModuleWelcomePage::Update()
{
	// Draw everything --------------------------------------

	if (!endedIntro) {
		SDL_SetRenderDrawColor(App->render->renderer, 103, 203, 237, 255);
		SDL_RenderFillRect(App->render->renderer, &screen);

		App->render->Blit(intro, cameraPosition.x, cameraPosition.y - 182, &sky1, false);
		App->render->Blit(intro, cameraPosition.x + 310, cameraPosition.y - 182, &sky2, false);

		App->render->Blit(intro, cameraPosition.x + 32, cameraPosition.y - 256, &building, false);
		App->render->Blit(intro, cameraPosition.x - 287, cameraPosition.y - 256, &building, false);
		App->render->Blit(intro, cameraPosition.x + 350, cameraPosition.y - 256, &building, false);

		App->render->Blit(intro, cameraPosition.x, cameraPosition.y + 145, &people.GetCurrentFrame().frame, false);
		App->render->Blit(intro, cameraPosition.x + 128, cameraPosition.y + 145, &people.GetCurrentFrame().frame, false);
		App->render->Blit(intro, cameraPosition.x + 256, cameraPosition.y + 145, &people.GetCurrentFrame().frame, false);

		if (pageTimer < 240) {
			if (pageTimer % 30 == 0) {
				if (p1PostionUp) {
					p1Position.y++;
					p1PostionUp = !p1PostionUp;
				}
				else {
					p1Position.y--;
					p1PostionUp = !p1PostionUp;
				}
			}
			if (pageTimer % 20 == 0) {
				if (p2PostionUp) {
					p2Position.y++;
					p2PostionUp = !p2PostionUp;
				}
				else {
					p2Position.y--;
					p2PostionUp = !p2PostionUp;
				}
			}
			App->render->Blit(intro, cameraPosition.x + p1Position.x + 50, cameraPosition.y + p1Position.y + 145, &p11, false);
			App->render->Blit(intro, cameraPosition.x + p2Position.x + 210, cameraPosition.y + p2Position.y + 135, &p21, false);
		}

		if (pageTimer == 240) {
			App->audio->PlayChunk(fist);
			p1Position = p2Position = { 0,0 };
		}

		if (pageTimer > 240 && pageTimer < 245)
			App->render->Blit(intro, cameraPosition.x + p1Position.x + 55, cameraPosition.y + p1Position.y + 95, &pHit, false);

		if (pageTimer > 245 && pageTimer < 540) {
			App->render->Blit(intro, cameraPosition.x + p1Position.x + 60, cameraPosition.y + p1Position.y + 85, &p12, false);
			App->render->Blit(intro, cameraPosition.x + p1Position.x + 40 - p1Face.GetCurrentFrame().pivotPosition.x, cameraPosition.y + p1Position.y + 129, &p1Face.GetCurrentFrame().frame, false);
			if (pageTimer < 270)
				App->render->Blit(intro, cameraPosition.x + p2Position.x + 90, cameraPosition.y + p2Position.y + 125, &p22, false);
			else
				App->render->Blit(intro, cameraPosition.x + p2Position.x + 120, cameraPosition.y + p2Position.y + 130, &p23, false);
			if (pageTimer % 10 == 0) {
				p1Position.x -= 5;
				p1Position.y += 5;
			}
		}

		SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, alpha);
		SDL_RenderFillRect(App->render->renderer, &screen);

		App->render->Blit(graphics1, 0, cameraPosition.y - 249, &logo, false);

		if (pageTimer > 270 && pageTimer < 520 && pageTimer % 3 == 0) {
			cameraPosition.y += 3;
		}

		if (pageTimer > 520 && pageTimer < 600) {
			
			if (alpha + 4 > 250)
				alpha = 250;
			else
				alpha += 4;
		}

		if (pageTimer == 600) {
			endedIntro = true;
			alpha = 250;
			pageTimer = 0;
			cameraPosition = { 0,0 };
		}
	}
	else {
		SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, alpha);
		SDL_RenderFillRect(App->render->renderer, &screen);

		SDL_Texture* graphics;
		if (!segundaSheet)
			graphics = graphics1;
		else
			graphics = graphics2;

		App->render->Blit(graphics, 0, 0, &logo, false);

		if (pageTimer > 90) {
			if (!segundaSheet || fila != 4 || columna != 8) {
				if (fila != 4)
					fila++;
				else if (columna != 8) {
					fila = 0;
					columna++;
				}
				else {
					fila = 0;
					columna = 0;
					segundaSheet = true;
				}
				logo.x = 384 * fila;
				logo.y = 224 * columna;
			}
			App->fonts->BlitText(0, 0, typography, "Fanmade copy by: Gamusinos Fighters");
			App->fonts->BlitText(SCREEN_WIDTH / 2 - 95, SCREEN_HEIGHT / 2 + 90, typography, "@CAPCOM CO.. LTD.");
		}
	}

	PrintInsertCoin();
	pageTimer++;

	if (App->input->pads[0].start) {
		if (endedIntro) {
			App->audio->PlayChunk(start);
			Mix_FadeOutMusic(2000);
			App->fade->FadeToBlack(this, (Module*)App->chSelectionScreen, 1);
		}
		else {
			endedIntro = true;
			pageTimer = 0;
			alpha = 250;
			cameraPosition = { 0,0 };
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleWelcomePage::PrintInsertCoin() {
	if (timeLetters < App->frames) {
		lettersOn = !lettersOn;
		timeLetters = App->frames + 30;
	}

	if (lettersOn)
		App->fonts->BlitText(SCREEN_WIDTH / 2 - 65, SCREEN_HEIGHT / 2 + 50, typography, "INSERT COIN.");
}