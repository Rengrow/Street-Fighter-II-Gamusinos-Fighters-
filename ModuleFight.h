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
	void StartNewRound();
	void EndFullFight();
	int GetTimer();
	int round;

public:
	bool roundStarted;
	bool endFightStarted;
	bool stopedFight;
	int player1RoundWinned;
	int player2RoundWinned;
	Uint32 timeOutTimer;
	Uint32 countdownStartFight;
	Uint32 endFightTimer;
};

#endif // __ModuleFight_H__