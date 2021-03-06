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
	void Win(int ryu);
	void StartNewRound();
	void EndFullFight();
	int GetTimer();
	void CheckFlipPlayers();
	int round;

public:
	Module * stage;
	bool roundStarted;
	bool endFightStarted;
	bool stopedFight;
	bool left = false;
	bool right = true;
	int player1RoundWinned;
	int player2RoundWinned;
	Uint32 timeOutTimer;
	Uint32 countdownStartFight;
	Uint32 endFightTimer;
	Uint32 autoWinLoseTimer;
};

#endif // __ModuleFight_H__