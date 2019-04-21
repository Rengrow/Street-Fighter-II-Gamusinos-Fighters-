#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleSceneKen.h"
#include "ModuleSceneSagat.h"
#include "ModulePlayer.h"
#include "ModuleFadeToBlack.h"
#include "ModuleWelcomePage.h"
#include "ModuleEndBattle.h"
#include "ModuleAudio.h"
#include "ModuleParticles.h"
#include "ModuleSecondPlayer.h"
#include "ModuleCollision.h"
#include "ModuleFonts.h"
#include "ModuleUI.h"

Application::Application()
{
	modules[0] = window = new ModuleWindow();
	modules[1] = render = new ModuleRender();
	modules[2] = input = new ModuleInput();
	modules[3] = textures = new ModuleTextures();
	modules[4] = fonts = new ModuleFonts();
	modules[5] = audio = new ModuleAudio();
	modules[6] = scene_Sagat = new ModuleSceneSagat();
	modules[7] = scene_ken = new ModuleSceneKen();
	modules[8] = player = new ModulePlayer();	
	modules[9] = player2 = new ModuleSecondPlayer();
	modules[10] = particles = new ModuleParticles();
	modules[11] = welcomePage = new ModuleWelcomePage();
	modules[12] = endBattle = new ModuleEndBattle();
	modules[13] = collisions = new ModuleCollision();
	modules[14] = UI = new ModuleUI();
	modules[15] = fade = new ModuleFadeToBlack();

}

Application::~Application()
{
	for (int i = NUM_MODULES - 1; i >= 0; --i)
		delete modules[i];
}

bool Application::Init()
{
	bool ret = true;

	player->Disable();
	player2->Disable();
	particles->Disable();
	collisions->Disable();
	scene_Sagat->Disable();
	scene_ken->Disable();
	endBattle->Disable();
	UI->Disable();
	
	for (int i = 0; i < NUM_MODULES && ret == true; ++i)
		ret = modules[i]->Init();

	for (int i = 0; i < NUM_MODULES && ret == true; ++i)
		ret = modules[i]->IsEnabled() ? modules[i]->Start() : true;

	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	for (int i = 0; i < NUM_MODULES && ret == UPDATE_CONTINUE; ++i)
		ret = modules[i]->IsEnabled() ? modules[i]->PreUpdate() : UPDATE_CONTINUE;

	for (int i = 0; i < NUM_MODULES && ret == UPDATE_CONTINUE; ++i)
		ret = modules[i]->IsEnabled() ? modules[i]->Update() : UPDATE_CONTINUE;

	for (int i = 0; i < NUM_MODULES && ret == UPDATE_CONTINUE; ++i)
		ret = modules[i]->IsEnabled() ? modules[i]->PostUpdate() : UPDATE_CONTINUE;

	App->frames++;

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (int i = NUM_MODULES - 1; i >= 0 && ret == true; --i)
		ret = modules[i]->CleanUp();

	return ret;
}


// Just making a comment to test Github