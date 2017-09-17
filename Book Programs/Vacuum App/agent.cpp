#include "agent.h"
#include "environment.h"


// -----------------------------------------------------------
Agent::ActionType Agent::Think()
{
//    return static_cast<ActionType> (rand()%4);
	
	if (dirty_) return actSUCK;
	else return static_cast<ActionType> (rand()%4);
}
// -----------------------------------------------------------







// -----------------------------------------------------------
// -----------------------------------------------------------
// -----------------------------------------------------------

















// -----------------------------------------------------------
void Agent::Perceive(const Environment &env)
{
	bump_ = env.isJustBump();
	dirty_ = env.isCurrentPosDirty();
}
// -----------------------------------------------------------
string ActionStr(Agent::ActionType action)
{
	switch (action)
	{
	case Agent::actUP: return "UP";
	case Agent::actDOWN: return "DOWN";
	case Agent::actLEFT: return "LEFT";
	case Agent::actRIGHT: return "RIGHT";
	case Agent::actSUCK: return "SUCK";
	case Agent::actIDLE: return "IDLE";
	default: return "???";
	}
}
