#include "Globals.h"
#include "Application.h"
#include "ModuleFight.h"
#include "SDL/include/SDL.h"

ModuleFight::ModuleFight() : Module()
{
}

// Destructor
ModuleFight::~ModuleFight()
{
}

// Called before render is available
bool ModuleFight::Init()
{
	return true;
}

// Called before quitting
bool ModuleFight::CleanUp()
{
	return true;
}

