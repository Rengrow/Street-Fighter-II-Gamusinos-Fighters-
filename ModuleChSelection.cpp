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

	screen = { 0, 0, SCREEN_WIDTH * SCREEN_SIZE, SCREEN_HEIGHT * SCREEN_SIZE };

	map = { 0,0,192,112 };
	thailandFlag = { 204,225,22,27 };
	player1Pointer = { 78,124,20,11 };
	player1Cursor = { 0,115,28,42 };
	player1CursorLocked = { 102,115,28,42 };
	player2Pointer = { 78,137,22,11 };
	player2Cursor = { 31,115,28,42 };
	player2CursorLocked = { 133,115,28,42 };
	airplane = { 60,129,15,15 };
	ryuFace = { 0,253,28,37 };
	ryuPortrait = { 0,0,105,140 };
	hondaFace = { 38,253,28,37 };
	hondaPortrait = { 125,0,105,140 };
	blankaFace = { 78,253,28,37 };
	blankaPortrait = { 247,0,105,140 };
	guileFace = { 116,253,28,37 };
	guilePortrait = { 0,173,105,140 };
	kenFace = { 0,300,28,37 };
	kenPortrait = { 125,173,105,140 };
	chunLiFace = { 38,300,28,37 };
	chunLiPortrait = { 247,173,105,140 };
	zangiefFace = { 78,300,28,37 };
	zangiefPortrait = { 373,0,105,140 };
	dhalsimFace = { 116,208,28,37 };
	dhalsimPortrait = { 497,0,105,140 };

#pragma endregion
}

ModuleChSelection::~ModuleChSelection()
{}

// Load assets
bool ModuleChSelection::Start()
{
	LOG("Loading background assets");
	bool ret = true;
	graphics = App->textures->Load("assets/images/ui/character_selection.png");
	portraits = App->textures->Load("assets/images/ui/portraits.png");
	typography = App->fonts->Load("assets/images/ui/FontMain.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789~'!@#$%^&*()-_+=[]{}| :;¨º<>,./?", 1);
	typographyBig = App->fonts->Load("assets/images/ui/Font_Big.png", "ABCDEFGHIJKLMNOPQRSTUVW$XYZ0123456789.", 1);
	music = App->audio->LoadSong("assets/music/player_select.ogg");
	move = App->audio->LoadChunk("assets/sfx/effects/score_up.wav");
	invalidSelect = App->audio->LoadChunk("assets/sfx/effects/86H.wav");
	select = App->audio->LoadChunk("assets/sfx/effects/select_menu_option.wav");
	App->audio->PlaySongDelay(music, -1, 10000);
	App->render->camera.x = App->render->camera.y = 0;

	isFading = false;

	player1Position.x = SCREEN_WIDTH / 2 + 24;
	player1Position.y = SCREEN_HEIGHT / 2 + 62;

	player2Position.x = SCREEN_WIDTH / 2 - 60;
	player2Position.y = SCREEN_HEIGHT / 2 + 62;

	player1Portrait = dhalsimPortrait;
	player1Name = new char[dhalsimName.length() + 1];
	strcpy_s(player1Name, dhalsimName.length() + 1, dhalsimName.c_str());
	player1Lock = false;

	player2Portrait = kenPortrait;
	player2Name = new char[kenName.length() + 1];
	strcpy_s(player2Name, kenName.length() + 1, kenName.c_str());
	player2Lock = false;

	canSelectTimer = App->frames + 60;

	return ret;
}

// Load assets
bool ModuleChSelection::CleanUp()
{
	LOG("Unloading Vs Screen");

	App->fonts->UnLoad(typography);
	App->fonts->UnLoad(typographyBig);

	App->audio->UnloadChunk(move);
	move = nullptr;

	App->audio->UnloadChunk(invalidSelect);
	invalidSelect = nullptr;

	App->audio->UnloadChunk(select);
	select = nullptr;

	App->audio->UnloadSong(music);
	music = nullptr;

	App->textures->Unload(portraits);
	App->textures->Unload(graphics);

	return true;
}

// Update: draw background
update_status ModuleChSelection::Update()
{
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 121, 255);
	SDL_RenderFillRect(App->render->renderer, &screen);

	if (timeSelector < App->frames) {
		lettersOn = !lettersOn;
		timeSelector = App->frames + 5;
	}

	ReadPlayer1Inputs();

	ReadPlayer2Inputs();

	PrintMap();

	PrintPanel();

	PrintPlayerCursors();

	PrintPortraits();

	if (player1Lock && player2Lock && !isFading) {
		Mix_FadeOutMusic(2000);
		App->fade->FadeToBlack(this, (Module*)App->vsScreen, 2);
		isFading = true;
	}

	return UPDATE_CONTINUE;
}

void ModuleChSelection::ReadPlayer1Inputs() {
	if (!player1Lock) {
		if (App->input->keyboard[SDL_SCANCODE_A] == 1) {
			App->audio->PlayChunk(move);
			if (player1Position.x != SCREEN_WIDTH / 2 - 60) {
				player1Position.x -= 28;

				if (SDL_RectEquals(&player1Portrait, &dhalsimPortrait)) {
					player1Portrait = zangiefPortrait;
					player1Name = new char[zangiefName.length() + 1];
					strcpy_s(player1Name, zangiefName.length() + 1, zangiefName.c_str());
				}
				else if (SDL_RectEquals(&player1Portrait, &zangiefPortrait)) {
					player1Portrait = chunLiPortrait;
					player1Name = new char[chunLiName.length() + 1];
					strcpy_s(player1Name, chunLiName.length() + 1, chunLiName.c_str());
				}
				else if (SDL_RectEquals(&player1Portrait, &chunLiPortrait)) {
					player1Portrait = kenPortrait;
					player1Name = new char[kenName.length() + 1];
					strcpy_s(player1Name, kenName.length() + 1, kenName.c_str());
				}
				else if (SDL_RectEquals(&player1Portrait, &guilePortrait)) {
					player1Portrait = blankaPortrait;
					player1Name = new char[blankaName.length() + 1];
					strcpy_s(player1Name, blankaName.length() + 1, blankaName.c_str());
				}
				else if (SDL_RectEquals(&player1Portrait, &blankaPortrait)) {
					player1Portrait = hondaPortrait;
					player1Name = new char[hondaName.length() + 1];
					strcpy_s(player1Name, hondaName.length() + 1, hondaName.c_str());
				}
				else if (SDL_RectEquals(&player1Portrait, &hondaPortrait)) {
					player1Portrait = ryuPortrait;
					player1Name = new char[ryuName.length() + 1];
					strcpy_s(player1Name, ryuName.length() + 1, ryuName.c_str());
				}
			}
		}

		if (App->input->keyboard[SDL_SCANCODE_D] == 1) {
			App->audio->PlayChunk(move);
			if (player1Position.x != SCREEN_WIDTH / 2 + 24) {
				player1Position.x += 28;

				if (SDL_RectEquals(&player1Portrait, &kenPortrait)) {
					player1Portrait = chunLiPortrait;
					player1Name = new char[chunLiName.length() + 1];
					strcpy_s(player1Name, chunLiName.length() + 1, chunLiName.c_str());
				}
				else if (SDL_RectEquals(&player1Portrait, &chunLiPortrait)) {
					player1Portrait = zangiefPortrait;
					player1Name = new char[zangiefName.length() + 1];
					strcpy_s(player1Name, zangiefName.length() + 1, zangiefName.c_str());
				}
				else if (SDL_RectEquals(&player1Portrait, &zangiefPortrait)) {
					player1Portrait = dhalsimPortrait;
					player1Name = new char[dhalsimName.length() + 1];
					strcpy_s(player1Name, dhalsimName.length() + 1, dhalsimName.c_str());
				}
				else if (SDL_RectEquals(&player1Portrait, &ryuPortrait)) {
					player1Portrait = hondaPortrait;
					player1Name = new char[hondaName.length() + 1];
					strcpy_s(player1Name, hondaName.length() + 1, hondaName.c_str());
				}
				else if (SDL_RectEquals(&player1Portrait, &hondaPortrait)) {
					player1Portrait = blankaPortrait;
					player1Name = new char[blankaName.length() + 1];
					strcpy_s(player1Name, blankaName.length() + 1, blankaName.c_str());
				}
				else if (SDL_RectEquals(&player1Portrait, &blankaPortrait)) {
					player1Portrait = guilePortrait;
					player1Name = new char[guileName.length() + 1];
					strcpy_s(player1Name, guileName.length() + 1, guileName.c_str());
				}
			}
		}

		if (App->input->keyboard[SDL_SCANCODE_W] == 1) {
			App->audio->PlayChunk(move);
			if (player1Position.y != SCREEN_HEIGHT / 2 + 25) {
				player1Position.y -= 37;

				if (SDL_RectEquals(&player1Portrait, &kenPortrait)) {
					player1Portrait = ryuPortrait;
					player1Name = new char[ryuName.length() + 1];
					strcpy_s(player1Name, ryuName.length() + 1, ryuName.c_str());
				}
				else if (SDL_RectEquals(&player1Portrait, &chunLiPortrait)) {
					player1Portrait = hondaPortrait;
					player1Name = new char[hondaName.length() + 1];
					strcpy_s(player1Name, hondaName.length() + 1, hondaName.c_str());
				}
				else if (SDL_RectEquals(&player1Portrait, &zangiefPortrait)) {
					player1Portrait = blankaPortrait;
					player1Name = new char[blankaName.length() + 1];
					strcpy_s(player1Name, blankaName.length() + 1, blankaName.c_str());
				}
				else if (SDL_RectEquals(&player1Portrait, &dhalsimPortrait)) {
					player1Portrait = guilePortrait;
					player1Name = new char[guileName.length() + 1];
					strcpy_s(player1Name, guileName.length() + 1, guileName.c_str());
				}
			}
		}

		if (App->input->keyboard[SDL_SCANCODE_S] == 1) {
			App->audio->PlayChunk(move);
			if (player1Position.y != SCREEN_HEIGHT / 2 + 62) {
				player1Position.y += 37;

				if (SDL_RectEquals(&player1Portrait, &ryuPortrait)) {
					player1Portrait = kenPortrait;
					player1Name = new char[kenName.length() + 1];
					strcpy_s(player1Name, kenName.length() + 1, kenName.c_str());
				}
				else if (SDL_RectEquals(&player1Portrait, &hondaPortrait)) {
					player1Portrait = chunLiPortrait;
					player1Name = new char[chunLiName.length() + 1];
					strcpy_s(player1Name, chunLiName.length() + 1, chunLiName.c_str());
				}
				else if (SDL_RectEquals(&player1Portrait, &blankaPortrait)) {
					player1Portrait = zangiefPortrait;
					player1Name = new char[zangiefName.length() + 1];
					strcpy_s(player1Name, zangiefName.length() + 1, zangiefName.c_str());
				}
				else if (SDL_RectEquals(&player1Portrait, &guilePortrait)) {
					player1Portrait = dhalsimPortrait;
					player1Name = new char[dhalsimName.length() + 1];
					strcpy_s(player1Name, dhalsimName.length() + 1, dhalsimName.c_str());
				}

			}
		}

		if (App->input->keyboard[SDL_SCANCODE_U] == 1) {
			if (canSelectTimer < App->frames)
				if (SDL_RectEquals(&player1Portrait, &dhalsimPortrait)) {
					App->audio->PlayChunk(select);
					player1Lock = true;
				}
				else
					App->audio->PlayChunk(invalidSelect);
		}
	}
}

void ModuleChSelection::ReadPlayer2Inputs() {
	if (!player2Lock) {
		if (App->input->keyboard[SDL_SCANCODE_LEFT] == 1) {
			App->audio->PlayChunk(move);
			if (player2Position.x != SCREEN_WIDTH / 2 - 60) {
				player2Position.x -= 28;

				if (SDL_RectEquals(&player2Portrait, &dhalsimPortrait)) {
					player2Portrait = zangiefPortrait;
					player2Name = new char[zangiefName.length() + 1];
					strcpy_s(player2Name, zangiefName.length() + 1, zangiefName.c_str());
				}
				else if (SDL_RectEquals(&player2Portrait, &zangiefPortrait)) {
					player2Portrait = chunLiPortrait;
					player2Name = new char[chunLiName.length() + 1];
					strcpy_s(player2Name, chunLiName.length() + 1, chunLiName.c_str());
				}
				else if (SDL_RectEquals(&player2Portrait, &chunLiPortrait)) {
					player2Portrait = kenPortrait;
					player2Name = new char[kenName.length() + 1];
					strcpy_s(player2Name, kenName.length() + 1, kenName.c_str());
				}
				else if (SDL_RectEquals(&player2Portrait, &guilePortrait)) {
					player2Portrait = blankaPortrait;
					player2Name = new char[blankaName.length() + 1];
					strcpy_s(player2Name, blankaName.length() + 1, blankaName.c_str());
				}
				else if (SDL_RectEquals(&player2Portrait, &blankaPortrait)) {
					player2Portrait = hondaPortrait;
					player2Name = new char[hondaName.length() + 1];
					strcpy_s(player2Name, hondaName.length() + 1, hondaName.c_str());
				}
				else if (SDL_RectEquals(&player2Portrait, &hondaPortrait)) {
					player2Portrait = ryuPortrait;
					player2Name = new char[ryuName.length() + 1];
					strcpy_s(player2Name, ryuName.length() + 1, ryuName.c_str());
				}
			}
		}

		if (App->input->keyboard[SDL_SCANCODE_RIGHT] == 1) {
			App->audio->PlayChunk(move);
			if (player2Position.x != SCREEN_WIDTH / 2 + 24) {
				player2Position.x += 28;

				if (SDL_RectEquals(&player2Portrait, &kenPortrait)) {
					player2Portrait = chunLiPortrait;
					player2Name = new char[chunLiName.length() + 1];
					strcpy_s(player2Name, chunLiName.length() + 1, chunLiName.c_str());
				}
				else if (SDL_RectEquals(&player2Portrait, &chunLiPortrait)) {
					player2Portrait = zangiefPortrait;
					player2Name = new char[zangiefName.length() + 1];
					strcpy_s(player2Name, zangiefName.length() + 1, zangiefName.c_str());
				}
				else if (SDL_RectEquals(&player2Portrait, &zangiefPortrait)) {
					player2Portrait = dhalsimPortrait;
					player2Name = new char[dhalsimName.length() + 1];
					strcpy_s(player2Name, dhalsimName.length() + 1, dhalsimName.c_str());
				}
				else if (SDL_RectEquals(&player2Portrait, &ryuPortrait)) {
					player2Portrait = hondaPortrait;
					player2Name = new char[hondaName.length() + 1];
					strcpy_s(player2Name, hondaName.length() + 1, hondaName.c_str());
				}
				else if (SDL_RectEquals(&player2Portrait, &hondaPortrait)) {
					player2Portrait = blankaPortrait;
					player2Name = new char[blankaName.length() + 1];
					strcpy_s(player2Name, blankaName.length() + 1, blankaName.c_str());
				}
				else if (SDL_RectEquals(&player2Portrait, &blankaPortrait)) {
					player2Portrait = guilePortrait;
					player2Name = new char[guileName.length() + 1];
					strcpy_s(player2Name, guileName.length() + 1, guileName.c_str());
				}
			}
		}

		if (App->input->keyboard[SDL_SCANCODE_UP] == 1) {
			App->audio->PlayChunk(move);
			if (player2Position.y != SCREEN_HEIGHT / 2 + 25) {
				player2Position.y -= 37;

				if (SDL_RectEquals(&player2Portrait, &kenPortrait)) {
					player2Portrait = ryuPortrait;
					player2Name = new char[ryuName.length() + 1];
					strcpy_s(player2Name, ryuName.length() + 1, ryuName.c_str());
				}
				else if (SDL_RectEquals(&player2Portrait, &chunLiPortrait)) {
					player2Portrait = hondaPortrait;
					player2Name = new char[hondaName.length() + 1];
					strcpy_s(player2Name, hondaName.length() + 1, hondaName.c_str());
				}
				else if (SDL_RectEquals(&player2Portrait, &zangiefPortrait)) {
					player2Portrait = blankaPortrait;
					player2Name = new char[blankaName.length() + 1];
					strcpy_s(player2Name, blankaName.length() + 1, blankaName.c_str());
				}
				else if (SDL_RectEquals(&player2Portrait, &dhalsimPortrait)) {
					player2Portrait = guilePortrait;
					player2Name = new char[guileName.length() + 1];
					strcpy_s(player2Name, guileName.length() + 1, guileName.c_str());
				}
			}
		}

		if (App->input->keyboard[SDL_SCANCODE_DOWN] == 1) {
			App->audio->PlayChunk(move);
			if (player2Position.y != SCREEN_HEIGHT / 2 + 62) {
				player2Position.y += 37;

				if (SDL_RectEquals(&player2Portrait, &ryuPortrait)) {
					player2Portrait = kenPortrait;
					player2Name = new char[kenName.length() + 1];
					strcpy_s(player2Name, kenName.length() + 1, kenName.c_str());
				}
				else if (SDL_RectEquals(&player2Portrait, &hondaPortrait)) {
					player2Portrait = chunLiPortrait;
					player2Name = new char[chunLiName.length() + 1];
					strcpy_s(player2Name, chunLiName.length() + 1, chunLiName.c_str());
				}
				else if (SDL_RectEquals(&player2Portrait, &blankaPortrait)) {
					player2Portrait = zangiefPortrait;
					player2Name = new char[zangiefName.length() + 1];
					strcpy_s(player2Name, zangiefName.length() + 1, zangiefName.c_str());
				}
				else if (SDL_RectEquals(&player2Portrait, &guilePortrait)) {
					player2Portrait = dhalsimPortrait;
					player2Name = new char[dhalsimName.length() + 1];
					strcpy_s(player2Name, dhalsimName.length() + 1, dhalsimName.c_str());
				}

			}
		}

		if ((App->input->keyboard[SDL_SCANCODE_KP_4] == 1) || App->input->keyboard[SDL_SCANCODE_0] == 1) {
			if (canSelectTimer < App->frames)
				if (SDL_RectEquals(&player2Portrait, &dhalsimPortrait)) {
					App->audio->PlayChunk(select);
					player2Lock = true;
				}
				else
					App->audio->PlayChunk(invalidSelect);
		}

	}
}

void ModuleChSelection::PrintMap() {
	App->fonts->BlitText(SCREEN_WIDTH / 4 + 20, 5, typography, "PLAYER SELECT");
	App->render->Blit(graphics, SCREEN_WIDTH / 2 - map.w / 2, 25, &map, false);
	App->render->Blit(graphics, 165, 80, &thailandFlag, false);
}

void ModuleChSelection::PrintPanel() {
	App->render->Blit(graphics, SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2 + 30, &ryuFace, false);
	App->render->Blit(graphics, SCREEN_WIDTH / 2 - 32, SCREEN_HEIGHT / 2 + 30, &hondaFace, false);
	App->render->Blit(graphics, SCREEN_WIDTH / 2 - 4, SCREEN_HEIGHT / 2 + 30, &blankaFace, false);
	App->render->Blit(graphics, SCREEN_WIDTH / 2 + 24, SCREEN_HEIGHT / 2 + 30, &guileFace, false);
	App->render->Blit(graphics, SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2 + 67, &kenFace, false);
	App->render->Blit(graphics, SCREEN_WIDTH / 2 - 32, SCREEN_HEIGHT / 2 + 67, &chunLiFace, false);
	App->render->Blit(graphics, SCREEN_WIDTH / 2 - 4, SCREEN_HEIGHT / 2 + 67, &zangiefFace, false);
	App->render->Blit(graphics, SCREEN_WIDTH / 2 + 24, SCREEN_HEIGHT / 2 + 67, &dhalsimFace, false);
}

void ModuleChSelection::PrintPlayerCursors() {
	if (player1Lock)
		App->render->Blit(graphics, player1Position.x, player1Position.y, &player1CursorLocked, false);
	if (player2Lock)
		App->render->Blit(graphics, player2Position.x, player2Position.y, &player2CursorLocked, false);
	if (!player1Lock&& lettersOn)
		App->render->Blit(graphics, player1Position.x, player1Position.y, &player1Cursor, false);
	if (!player2Lock && !lettersOn)
		App->render->Blit(graphics, player2Position.x, player2Position.y, &player2Cursor, false);
}

void ModuleChSelection::PrintPortraits() {
	App->render->Blit(portraits, 0, SCREEN_HEIGHT - player1Portrait.h + 15, &player1Portrait, false);
	App->render->Blit(portraits, SCREEN_WIDTH - player2Portrait.w, SCREEN_HEIGHT - player2Portrait.h + 15, &player2Portrait, true);

	App->render->Blit(graphics, player1Portrait.w / 2, SCREEN_HEIGHT / 2 - 20, &player1Pointer, false);
	App->render->Blit(graphics, SCREEN_WIDTH - player2Portrait.w / 2 - player2Pointer.w / 2, SCREEN_HEIGHT / 2 - 20, &player2Pointer, false);

	App->fonts->BlitText(17 + player1Portrait.w / 2 - ((strlen(player1Name) * 17) / 2), SCREEN_HEIGHT / 2 - 5, typographyBig, player1Name);
	App->fonts->BlitText(45 + SCREEN_WIDTH - player2Portrait.w - ((strlen(player2Name) * 17) / 2), SCREEN_HEIGHT / 2 - 5, typographyBig, player2Name);
}