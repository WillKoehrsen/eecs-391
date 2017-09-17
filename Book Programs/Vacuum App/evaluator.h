
#ifndef EVALUATOR__
#define EVALUATOR__

#include "agent.h"

class Environment;

class Evaluator
{
public:
	Evaluator():dirtyDegree_(0), consumedEnergy_(0) {};
	
	long long DirtyDegree() const { return dirtyDegree_; }
	long long ConsumedEnergy() const { return consumedEnergy_; }
	
	void Eval(Agent::ActionType action, const Environment &env);
	
	static const int LIFE_TIME = 2000;
	
private:
	long long dirtyDegree_, consumedEnergy_;
};

#endif
