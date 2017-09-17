
#ifndef ENVIRONMENT__
#define ENVIRONMENT__

#include <fstream>
using std::ifstream;

#include "agent.h"
// -----------------------------------------------------------
//				class Environment
// -----------------------------------------------------------

class RandomNumberGenerator;

class Environment
{
public:
	static const int MAZE_SIZE = 10;

	Environment(ifstream &infile);
	void Show(int,int) const;
	void Change(const RandomNumberGenerator &rng);
	void AcceptAction(Agent::ActionType);
	
	bool isCurrentPosDirty() const { return maze_[agentPosX_][agentPosY_] > 0; }
	bool isJustBump() const { return bump_; }
	int  DirtAmount(int x, int y) const;
	int  RandomSeed() const { return randomSeed_; }
private:
	static const int OBSTACLE = -1;	
	static const char MAP_ROAD = '-', MAP_OBSTACLE = 'O';

	bool bump_;
	int agentPosX_, agentPosY_;
	int maze_[MAZE_SIZE][MAZE_SIZE]; // -1: Obstacle, >=0: amount of dirt
	int randomSeed_;
	double dirtyProb_;
	
	/**/
	Agent::ActionType preAction_;
};
// -----------------------------------------------------------

#endif
