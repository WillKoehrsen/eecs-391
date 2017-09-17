#include "evaluator.h"
#include "environment.h"

void Evaluator::Eval(Agent::ActionType action, const Environment &env)
{
	if (action == Agent::actSUCK)
		consumedEnergy_ += 2;
	else if (action != Agent::actIDLE)
		consumedEnergy_ += 1;
		

	for (int row=0; row<Environment::MAZE_SIZE; row+=1)
	{
		for (int col=0; col<Environment::MAZE_SIZE; col+=1)
		{
			long long da = env.DirtAmount(row, col);
			dirtyDegree_ += (da*da);
		}
	}
}
