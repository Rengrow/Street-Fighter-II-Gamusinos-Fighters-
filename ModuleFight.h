#ifndef __ModuleFight_H__
#define __ModuleFight_H__

#include "Module.h"

class ModuleFight : public Module
{
public:

	ModuleFight();

	// Destructor
	virtual ~ModuleFight();


	bool Start();
	bool CleanUp();
	update_status Update();
	void Win(int player);

public:
	bool roundStarted;
};

#endif // __ModuleFight_H__