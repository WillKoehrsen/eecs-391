
#ifndef AGENT__
#define AGENT__

#include <string>
using std::string;

// -----------------------------------------------------------
//				class Agent
// -----------------------------------------------------------
class Environment;
class Agent
{
public:
	Agent():bump_(false), dirty_(false) {}
	
	enum ActionType { actUP, actDOWN, actLEFT, actRIGHT, actSUCK, actIDLE };

	void Perceive(const Environment &env);
	ActionType Think();
private:
	bool bump_,
		 dirty_;
};

string ActionStr(Agent::ActionType);

#endif
