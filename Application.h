#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Globals.h"

#define NUM_MODULES 13

class ModuleWindow;
class ModuleInput;
class ModuleTextures;
class ModuleParticles;
class ModuleRender;
class ModuleSceneKen;
class ModuleSceneHonda;
class ModulePlayer;
class ModuleFadeToBlack;
class ModuleWelcomePage;
class ModuleEndBattle;
class ModuleAudio;
class ModuleSecondPlayer;
class Module;

class Application
{
public:

	Module* modules[NUM_MODULES];
	ModuleWindow* window;
	ModuleRender* render;
	ModuleInput* input;
	ModuleTextures* textures;
	ModuleParticles* particles;
	ModuleAudio* audio;
	ModuleSceneKen* scene_ken;
	ModuleSceneHonda* scene_honda;
	ModulePlayer* player;
	ModuleFadeToBlack* fade;
	ModuleWelcomePage* welcomePage;
	ModuleEndBattle* endBattle;
	ModuleSecondPlayer* player2;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

};

// Global var made extern for Application ---
extern Application* App;

#endif // __APPLICATION_H__