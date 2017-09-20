# Welcome to the eight puzzle solver. 

# Files 

eight_puzzle.py contains the Puzzle class and all required methods. 
play_puzzle.py contains the command
line parser and file reader for interacting with the puzzle. 

When the puzzle is initialized, it will be set to the goal state.
Several methods exist for interacting with the puzzle from the command line 
or a text file. 

Command line arguments

-setState state_string		Sets the current state of the puzzle. The format 
for the state_string is "b12 345 678" where each triple is a row of the puzzle and the b is the blank.

-randomizeState num_moves		Randomizes the state of the puzzle taking an 
integer number of random moves backwards from the goal state. This ensures 
the resulting puzzle state will have a solution. (Only half of all random
starting states are otherwise solvable)

-printState (no arguments) This will display the current state of the puzzle
in the format "b12 345 678" where each triple is a row and the b is the blank 
tile. 

-move action 		Executes one specified action where the action is 
one of [up, down, left, right] 

-solveAStar	heuristic		Solves the puzzle using astar search and
the specified heuristic. The heuristic is one of [h1 h2]. h1 counts the number 
of misplaced tiles. h2 is the sum of the Manhattan distances of all the tiles
from the goal position.

-solveBeam k		Solves the puzzle using local beam search with k successor
nodes considered at each iteration. A wider beam takes up more memory, but is
more likely to find a solution

-maxNodes max_nodes			Set a maximum number of nodes to be considered for
the searches. A node is considered when it is generated. If no number is 
specified, the default maximum is 10000 nodes. The search will return 
unsuccessfully if it does not find a solution within the max number of nodes.

-prettyPrint (no arguments) 		If specified after the puzzle has been
solved, will display the solution path in an easy to understand format.

-readCommands f.txt 		Read a series of the above commands from a text 
file and execute them in order. Commands are in the same format as from the 
command line except for no dashes. 

EXAMPLES 

COMMAND LINE OPERATION

Navigate to the folder with the code. Start up a command shell in the folder. 
To do this quickly, shift + right click in the folder and hit open command 
prompt (or open powershell window) here. 

1. Randomize the state of the puzzle with 100 random moves from the goal,
print the state, and try to solve using astar with the h1 heuristic and 2000 max nodes. 

python play_puzzle.py -randomizeState 100 -printState -solveAStar h1 -maxNodes 2000

2. Set the state of the puzzle to "312 645 b78"
and solve using astar with the h2 heuristic and 1000 max nodes. Pretty
print the resulting solution. 

python play_puzzle.py -setState "312 645 b78" -solveAStar h2 -maxNodes 1000 -prettyPrint

3. Randomize the state of the puzzle with 10 random moves from the goal and 
solve using local beam with k = 50 and 5000 max nodes. 

python play_puzzle.py -randomizeState 10 -solveBeam 50 -maxNodes 5000

4. Set the state of the puzzle to "3b5 421 678", and try to
solve using local beam search with k = 100 and 5000 max nodes. Pretty print
the solution. 

python play_puzzle.py -setState "3b5 421 678" -solveBeam 100 -maxNodes 5000 -prettyPrint

FILE OPERATION

1. Randomize the state of the puzzle using 100 random moves from the goal 
and try to solve using astar search with the default number of max nodes. 

Command line input

python play_puzzle.py -readCommands tests\randomize_solve_astar.txt

randomize_solve_astar.txt:

randomizeState 100 solveAStar h2 

2. Set state to "3b2 615 748", print the state, and try to solve using 
local beam search with k = 50 and 2000 max nodes. 

Command line input

python -play_puzzle.py -readCommands tests\set_state_solve_beam.txt

set_state_solve_beam.txt:

setState "3b2 615 748" printState solveBeam 50 maxNodes 2000
