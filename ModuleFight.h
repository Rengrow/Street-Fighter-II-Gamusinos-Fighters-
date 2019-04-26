#ifndef __ModuleFight_H__
#define __ModuleFight_H__

#include "Module.h"

class ModuleFight : public Module
{
public:

	ModuleFight();

	// Destructor
	virtual ~ModuleFight();


	bool Init();
	bool CleanUp();
	//update_status Update();

public:
	
};

#endif // __ModuleFight_H__