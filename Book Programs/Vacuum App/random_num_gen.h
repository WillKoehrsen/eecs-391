
#ifndef RANDOMNUMGENERATOR__
#define RANDOMNUMGENERATOR__

#include <vector>
using std::vector;

// -----------------------------------------------------------
//				class RandomNumberGenerator
// -----------------------------------------------------------
class RandomNumberGenerator
{
public:
	RandomNumberGenerator(int seed);
	int RandomValue() const { 
 		if (index_ >= rand_num_.size())
 		{
 			cout << "Insufficient random values!"; exit(1);
 		}
   		return rand_num_[index_++]; 
   }
private:	
	vector<int> rand_num_;
	mutable int	index_;
};
// -----------------------------------------------------------

#endif
