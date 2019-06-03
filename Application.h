#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Globals.h"

#define NUM_MODULES 20

class ModuleWindow;
class ModuleInput;
class ModuleTextures;
class ModuleParticles;
class ModuleRender;
class ModuleSceneKen;
class ModuleSceneSagat;
class ModulePlayer1;
class ModuleFadeToBlack;
class ModuleWelcomePage;
class ModuleEndBattle;
class ModuleAudio;
class ModulePlayer2;
class ModuleCollision;
class ModuleFonts;
class ModuleUI;
class ModuleFight;
class ModuleSlowdown;
class ModuleVsScreen;
class ModuleChSelection;
class Module;

class Application
{
public:
	Uint32 frames = 0;
	Module* modules[NUM_MODULES];
	ModuleWindow* window;
	ModuleRender* render;
	ModuleInput* input;
	ModuleTextures* textures;
	ModuleParticles* particles;
	ModuleAudio* audio;
	ModuleSceneKen* scene_ken;
	ModuleSceneSagat* scene_Sagat;
	ModulePlayer1* player1;
	ModuleFadeToBlack* fade;
	ModuleWelcomePage* welcomePage;
	ModuleEndBattle* endBattle;
	ModulePlayer2* player2;
	ModuleCollision* collisions;
	ModuleFonts* fonts;
	ModuleUI* UI;
	ModuleFight* fight;
	ModuleVsScreen* vsScreen;
	ModuleChSelection* chSelectionScreen;
	ModuleSlowdown* slowdown;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	Uint32 GetFrame();

};

// Global var made extern for Application ---
extern Application* App;

#endif // __APPLICATION_H__