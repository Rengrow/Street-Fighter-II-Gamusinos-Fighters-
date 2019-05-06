#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleSceneKen.h"
#include "ModuleSceneSagat.h"
#include "ModuleRyu.h"
#include "ModuleFadeToBlack.h"
#include "ModuleWelcomePage.h"
#include "ModuleEndBattle.h"
#include "ModuleAudio.h"
#include "ModuleParticles.h"
#include "ModuleDhalsim.h"
#include "ModuleCollision.h"
#include "ModuleFonts.h"
#include "ModuleUI.h"
#include "ModuleFight.h"

Application::Application()
{
	modules[0] = window = new ModuleWindow();
	modules[1] = input = new ModuleInput();
	modules[2] = textures = new ModuleTextures();
	modules[3] = fonts = new ModuleFonts();
	modules[4] = audio = new ModuleAudio();
	modules[5] = fight = new ModuleFight();
	modules[6] = scene_Sagat = new ModuleSceneSagat();
	modules[7] = scene_ken = new ModuleSceneKen();
	modules[8] = ryu = new ModuleRyu();	
	modules[9] = dhalsim = new ModuleDhalsim();
	modules[10] = particles = new ModuleParticles();
	modules[11] = welcomePage = new ModuleWelcomePage();
	modules[12] = endBattle = new ModuleEndBattle();
	modules[13] = collisions = new ModuleCollision();
	modules[14] = UI = new ModuleUI();
	modules[15] = fade = new ModuleFadeToBlack();
	modules[16] = render = new ModuleRender();

}

Application::~Application()
{
	for (int i = NUM_MODULES - 1; i >= 0; --i)
		delete modules[i];
}

bool Application::Init()
{
	bool ret = true;

	ryu->Disable();
	dhalsim->Disable();
	particles->Disable();
	collisions->Disable();
	fight->Disable();
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