#include "Globals.h"
#include "Application.h"
#include "ModuleVsScreen.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
#include "ModuleFonts.h"

#include "ModuleRyu.h"
#include "ModuleDhalsim.h"


ModuleVsScreen::ModuleVsScreen()
{
	portrait1.x = 497;
	portrait1.y = 2;
	portrait1.w = 99;
	portrait1.h = 141;

	portrait2.x = 498;
	portrait2.y = 317;
	portrait2.w = 98;
	portrait2.h = 140;

	vs.x = 0;
	vs.y = 681;
	vs.w = 168;
	vs.h = 63;
}

ModuleVsScreen::~ModuleVsScreen()
{}

// Load assets
bool ModuleVsScreen::Start()
{
	LOG("Loading background assets");
	bool ret = true;
	portraits = App->textures->Load("assets/images/ui/portraits.png");

	App->ryu->position.x = 100;
	App->ryu->position.y = 215;
	App->dhalsim->position.x = 250;
	App->dhalsim->position.y = 215;
	App->render->camera.x = App->render->camera.y = 0;
	isFading = false;
	timeScreen = App->frames + 90;

	return ret;
}

// Load assets
bool ModuleVsScreen::CleanUp()
{
	LOG("Unloading Vs Screen");

	App->textures->Unload(portraits);

	return true;
}

// Update: draw background
update_status ModuleVsScreen::Update()
{
	// Draw everything --------------------------------------	
	App->render->Blit(portraits, 35, 10, &portrait1, false);
	App->render->Blit(portraits, SCREEN_WIDTH - portrait1.w - 35, 10, &portrait1, true);
	App->render->Blit(portraits, SCREEN_WIDTH / 2 - 80, 150, &vs, false);

	if (timeScreen < App->frames && !isFading) {
		App->fade->FadeToBlack(this, (Module*)App->fight, 2);
		isFading = true;
	}


	return UPDATE_CONTINUE;
}