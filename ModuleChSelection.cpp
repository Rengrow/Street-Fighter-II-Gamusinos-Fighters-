#include "Globals.h"
#include "Application.h"
#include "ModuleChSelection.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
#include "ModuleFonts.h"

#include "SDL\include\SDL.h"


ModuleChSelection::ModuleChSelection()
{
#pragma region Read Sprites

	map.x = 0;
	map.y = 0;
	map.w = 192;
	map.h = 112;

	thailandFlag.x = 204;
	thailandFlag.y = 225;
	thailandFlag.w = 22;
	thailandFlag.h = 27;

	player1Pointer.x = 78;
	player1Pointer.y = 124;
	player1Pointer.w = 20;
	player1Pointer.h = 11;

	player1Cursor.x = 0;
	player1Cursor.y = 115;
	player1Cursor.w = 28;
	player1Cursor.h = 42;

	player2Pointer.x = 78;
	player2Pointer.y = 137;
	player2Pointer.w = 22;
	player2Pointer.h = 11;

	player2Cursor.x = 31;
	player2Cursor.y = 115;
	player2Cursor.w = 28;
	player2Cursor.h = 42;

	airplane.x = 60;
	airplane.y = 129;
	airplane.w = 15;
	airplane.h = 15;

	ryuFace.x = 0;
	ryuFace.y = 253;
	ryuFace.w = 28;
	ryuFace.h = 37;

	ryuPortrait.x = 0;
	ryuPortrait.y = 0;
	ryuPortrait.w = 105;
	ryuPortrait.h = 140;

	hondaFace.x = 38;
	hondaFace.y = 253;
	hondaFace.w = 28;
	hondaFace.h = 37;

	hondaPortrait.x = 125;
	hondaPortrait.y = 0;
	hondaPortrait.w = 105;
	hondaPortrait.h = 140;

	blankaFace.x = 78;
	blankaFace.y = 253;
	blankaFace.w = 28;
	blankaFace.h = 37;

	blankaPortrait.x = 247;
	blankaPortrait.y = 0;
	blankaPortrait.w = 105;
	blankaPortrait.h = 140;

	guileFace.x = 116;
	guileFace.y = 253;
	guileFace.w = 28;
	guileFace.h = 37;

	guilePortrait.x = 0;
	guilePortrait.y = 173;
	guilePortrait.w = 105;
	guilePortrait.h = 140;

	kenFace.x = 0;
	kenFace.y = 300;
	kenFace.w = 28;
	kenFace.h = 37;

	kenPortrait.x = 125;
	kenPortrait.y = 173;
	kenPortrait.w = 105;
	kenPortrait.h = 140;

	chunLiFace.x = 38;
	chunLiFace.y = 300;
	chunLiFace.w = 28;
	chunLiFace.h = 37;

	chunLiPortrait.x = 247;
	chunLiPortrait.y = 173;
	chunLiPortrait.w = 105;
	chunLiPortrait.h = 140;

	zangiefFace.x = 78;
	zangiefFace.y = 300;
	zangiefFace.w = 28;
	zangiefFace.h = 37;

	zangiefPortrait.x = 373;
	zangiefPortrait.y = 0;
	zangiefPortrait.w = 105;
	zangiefPortrait.h = 140;

	dhalsimFace.x = 116;
	dhalsimFace.y = 208;
	dhalsimFace.w = 28;
	dhalsimFace.h = 37;

	dhalsimPortrait.x = 497;
	dhalsimPortrait.y = 0;
	dhalsimPortrait.w = 105;
	dhalsimPortrait.h = 140;

#pragma endregion
}

ModuleChSelection::~ModuleChSelection()
{}

// Load assets
bool ModuleChSelection::Start()
{
	LOG("Loading background assets");
	bool ret = true;
	graphics = App->textures->Load("assets/images/ui/character_selection2.png");
	portraits = App->textures->Load("assets/images/ui/portraits.png");
	music = App->audio->LoadSong("assets/music/player_select.ogg");
	move = App->audio->LoadChunk("assets/sfx/effects/score_up.wav");
	select = App->audio->LoadChunk("assets/sfx/effects/select_menu_option.wav");
	App->audio->PlaySongDelay(music, -1, 10000);
	App->render->camera.x = App->render->camera.y = 0;

	player1Position.x = SCREEN_WIDTH / 2 + 24;
	player1Position.y = SCREEN_HEIGHT / 2 + 62;

	player2Position.x = SCREEN_WIDTH / 2 - 60;
	player2Position.y = SCREEN_HEIGHT / 2 + 62;

	player1Portrait = dhalsimPortrait;
	player2Portrait = kenPortrait;

	return ret;
}

// Load assets
bool ModuleChSelection::CleanUp()
{
	LOG("Unloading Vs Screen");

	App->audio->UnloadChunk(move);
	move = nullptr;

	App->audio->UnloadChunk(select);
	select = nullptr;

	App->audio->UnloadSong(music);
	music = nullptr;

	App->textures->Unload(graphics);

	return true;
}

// Update: draw background
update_status ModuleChSelection::Update()
{
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 121, 255);

	if (timeSelector < App->frames) {
		lettersOn = !lettersOn;
		timeSelector = App->frames + 5;
	}

	//Player 1
	if (App->input->keyboard[SDL_SCANCODE_A] == 1) {
		App->audio->PlayChunk(move);
		if (player1Position.x != SCREEN_WIDTH / 2 - 60) {
			player1Position.x -= 28;

			if (SDL_RectEquals(&player1Portrait, &dhalsimPortrait))
				player1Portrait = zangiefPortrait;
			else if (SDL_RectEquals(&player1Portrait, &zangiefPortrait))
				player1Portrait = chunLiPortrait;
			else if (SDL_RectEquals(&player1Portrait, &chunLiPortrait))
				player1Portrait = kenPortrait;
			else if (SDL_RectEquals(&player1Portrait, &guilePortrait))
				player1Portrait = blankaPortrait;
			else if (SDL_RectEquals(&player1Portrait, &blankaPortrait))
				player1Portrait = hondaPortrait;
			else if (SDL_RectEquals(&player1Portrait, &hondaPortrait))
				player1Portrait = ryuPortrait;
		}
	}

	if (App->input->keyboard[SDL_SCANCODE_D] == 1) {
		App->audio->PlayChunk(move);
		if (player1Position.x != SCREEN_WIDTH / 2 + 24) {
			player1Position.x += 28;

			if (SDL_RectEquals(&player1Portrait, &kenPortrait))
				player1Portrait = chunLiPortrait;
			else if (SDL_RectEquals(&player1Portrait, &chunLiPortrait))
				player1Portrait = zangiefPortrait;
			else if (SDL_RectEquals(&player1Portrait, &zangiefPortrait))
				player1Portrait = dhalsimPortrait;
			else if (SDL_RectEquals(&player1Portrait, &ryuPortrait))
				player1Portrait = hondaPortrait;
			else if (SDL_RectEquals(&player1Portrait, &hondaPortrait))
				player1Portrait = blankaPortrait;
			else if (SDL_RectEquals(&player1Portrait, &blankaPortrait))
				player1Portrait = guilePortrait;
		}
	}

	if (App->input->keyboard[SDL_SCANCODE_W] == 1) {
		App->audio->PlayChunk(move);
		if (player1Position.y != SCREEN_HEIGHT / 2 + 25) {
			player1Position.y -= 37;


			if (SDL_RectEquals(&player1Portrait, &kenPortrait))
				player1Portrait = ryuPortrait;
			else if (SDL_RectEquals(&player1Portrait, &chunLiPortrait))
				player1Portrait = hondaPortrait;
			else if (SDL_RectEquals(&player1Portrait, &zangiefPortrait))
				player1Portrait = blankaPortrait;
			else if (SDL_RectEquals(&player1Portrait, &dhalsimPortrait))
				player1Portrait = guilePortrait;
		}
	}

	if (App->input->keyboard[SDL_SCANCODE_S] == 1) {
		App->audio->PlayChunk(move);
		if (player1Position.y != SCREEN_HEIGHT / 2 + 62) {
			player1Position.y += 37;

			if (SDL_RectEquals(&player1Portrait, &ryuPortrait))
				player1Portrait = kenPortrait;
			else if (SDL_RectEquals(&player1Portrait, &hondaPortrait))
				player1Portrait = chunLiPortrait;
			else if (SDL_RectEquals(&player1Portrait, &blankaPortrait))
				player1Portrait = zangiefPortrait;
			else if (SDL_RectEquals(&player1Portrait, &guilePortrait))
				player1Portrait = dhalsimPortrait;

		}
	}
	//


	//Player 2
	if (App->input->keyboard[SDL_SCANCODE_LEFT] == 1) {
		App->audio->PlayChunk(move);
		if (player2Position.x != SCREEN_WIDTH / 2 - 60) {
			player2Position.x -= 28;

			if (SDL_RectEquals(&player2Portrait, &dhalsimPortrait))
				player2Portrait = zangiefPortrait;
			else if (SDL_RectEquals(&player2Portrait, &zangiefPortrait))
				player2Portrait = chunLiPortrait;
			else if (SDL_RectEquals(&player2Portrait, &chunLiPortrait))
				player2Portrait = kenPortrait;
			else if (SDL_RectEquals(&player2Portrait, &guilePortrait))
				player2Portrait = blankaPortrait;
			else if (SDL_RectEquals(&player2Portrait, &blankaPortrait))
				player2Portrait = hondaPortrait;
			else if (SDL_RectEquals(&player2Portrait, &hondaPortrait))
				player2Portrait = ryuPortrait;
		}
	}

	if (App->input->keyboard[SDL_SCANCODE_RIGHT] == 1) {
		App->audio->PlayChunk(move);
		if (player2Position.x != SCREEN_WIDTH / 2 + 24) {
			player2Position.x += 28;

			if (SDL_RectEquals(&player2Portrait, &kenPortrait))
				player2Portrait = chunLiPortrait;
			else if (SDL_RectEquals(&player2Portrait, &chunLiPortrait))
				player2Portrait = zangiefPortrait;
			else if (SDL_RectEquals(&player2Portrait, &zangiefPortrait))
				player2Portrait = dhalsimPortrait;
			else if (SDL_RectEquals(&player2Portrait, &ryuPortrait))
				player2Portrait = hondaPortrait;
			else if (SDL_RectEquals(&player2Portrait, &hondaPortrait))
				player2Portrait = blankaPortrait;
			else if (SDL_RectEquals(&player2Portrait, &blankaPortrait))
				player2Portrait = guilePortrait;
		}
	}

	if (App->input->keyboard[SDL_SCANCODE_UP] == 1) {
		App->audio->PlayChunk(move);
		if (player2Position.y != SCREEN_HEIGHT / 2 + 25) {
			player2Position.y -= 37;


			if (SDL_RectEquals(&player2Portrait, &kenPortrait))
				player2Portrait = ryuPortrait;
			else if (SDL_RectEquals(&player2Portrait, &chunLiPortrait))
				player2Portrait = hondaPortrait;
			else if (SDL_RectEquals(&player2Portrait, &zangiefPortrait))
				player2Portrait = blankaPortrait;
			else if (SDL_RectEquals(&player2Portrait, &dhalsimPortrait))
				player2Portrait = guilePortrait;
		}
	}

	if (App->input->keyboard[SDL_SCANCODE_DOWN] == 1) {
		App->audio->PlayChunk(move);
		if (player2Position.y != SCREEN_HEIGHT / 2 + 62) {
			player2Position.y += 37;

			if (SDL_RectEquals(&player2Portrait, &ryuPortrait))
				player2Portrait = kenPortrait;
			else if (SDL_RectEquals(&player2Portrait, &hondaPortrait))
				player2Portrait = chunLiPortrait;
			else if (SDL_RectEquals(&player2Portrait, &blankaPortrait))
				player2Portrait = zangiefPortrait;
			else if (SDL_RectEquals(&player2Portrait, &guilePortrait))
				player2Portrait = dhalsimPortrait;

		}
	}
	//


	App->render->Blit(graphics, SCREEN_WIDTH / 2 - map.w / 2, 18, &map, false);
	App->render->Blit(graphics, 165, 80, &thailandFlag, false);

	//Panel
	App->render->Blit(graphics, SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2 + 30, &ryuFace, false);
	App->render->Blit(graphics, SCREEN_WIDTH / 2 - 32, SCREEN_HEIGHT / 2 + 30, &hondaFace, false);
	App->render->Blit(graphics, SCREEN_WIDTH / 2 - 4, SCREEN_HEIGHT / 2 + 30, &blankaFace, false);
	App->render->Blit(graphics, SCREEN_WIDTH / 2 + 24, SCREEN_HEIGHT / 2 + 30, &guileFace, false);
	App->render->Blit(graphics, SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2 + 67, &kenFace, false);
	App->render->Blit(graphics, SCREEN_WIDTH / 2 - 32, SCREEN_HEIGHT / 2 + 67, &chunLiFace, false);
	App->render->Blit(graphics, SCREEN_WIDTH / 2 - 4, SCREEN_HEIGHT / 2 + 67, &zangiefFace, false);
	App->render->Blit(graphics, SCREEN_WIDTH / 2 + 24, SCREEN_HEIGHT / 2 + 67, &dhalsimFace, false);
	//

	if (lettersOn)
		App->render->Blit(graphics, player1Position.x, player1Position.y, &player1Cursor, false);
	else
		App->render->Blit(graphics, player2Position.x, player2Position.y, &player2Cursor, false);


	App->render->Blit(portraits, 0, SCREEN_HEIGHT / 2 - 20, &player1Portrait, false);
	App->render->Blit(portraits, SCREEN_WIDTH - player2Portrait.w, SCREEN_HEIGHT / 2 - 20, &player2Portrait, true);

	App->render->Blit(graphics, SCREEN_WIDTH / 2 - 140, SCREEN_HEIGHT / 2 - 20, &player1Pointer, false);
	App->render->Blit(graphics, SCREEN_WIDTH / 2 + 140, SCREEN_HEIGHT / 2 - 20, &player2Pointer, false);


	if (App->input->keyboard[SDL_SCANCODE_SPACE] == 1) {
		Mix_FadeOutMusic(2000);
		App->fade->FadeToBlack(this, (Module*)App->vsScreen, 2);
	}


	return UPDATE_CONTINUE;
}