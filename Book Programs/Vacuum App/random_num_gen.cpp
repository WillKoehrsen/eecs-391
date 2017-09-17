
#include <iostream>
using std::cout;

#include <stdlib.h>

#include "random_num_gen.h"
#include "evaluator.h"
#include "environment.h"

RandomNumberGenerator::RandomNumberGenerator(int seed)
{
	srand(seed);

	rand_num_.resize(Environment::MAZE_SIZE*Environment::MAZE_SIZE
					 *Evaluator::LIFE_TIME);
	for (int i=0; i<rand_num_.size(); i+=1)
	{
		rand_num_[i] = rand();
	}
	index_ = 0;
}
