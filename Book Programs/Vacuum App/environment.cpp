
#include <iostream>
using std::cout;
using std::endl;
#include <iomanip>
using std::setw;
#include <string>
using std::string;
#include "stdlib.h"

#include "environment.h"
#include "random_num_gen.h"
#include"include/GL/glui.h"

Environment::Environment(ifstream &infile)
{
	string comment;
	getline(infile, comment);
	infile >> agentPosX_ >> agentPosY_ >> dirtyProb_ >> randomSeed_;;
	
	for (int row=0; row<MAZE_SIZE; row+=1)
	{
		for (int col=0; col<MAZE_SIZE; col+=1)
		{
			char c;
			infile >> c;
			if (c == MAP_ROAD) {
   				maze_[row][col] = 0;
			}
			else if (c == MAP_OBSTACLE) {
   				maze_[row][col] = OBSTACLE;
			}
			else {
				cout << c << " is an unknown symbol in agent.map!"; exit(1);
			}
		}// for - col
	}// for - row
	
	bump_ = false;
	preAction_=Agent::actIDLE;
}
// -----------------------------------------------------------
int Environment::DirtAmount(int x, int y) const
{
	if (maze_[x][y] == OBSTACLE) return 0;
	else return maze_[x][y];
}
// -----------------------------------------------------------	
void Environment::Show(int w,int h) const
{
	const static float dirty_level=50.0;
	float length=(float)(w>h?h:w)/(float)(MAZE_SIZE+4);
	for (int row=0; row<MAZE_SIZE; row+=1)
	{
		for (int col=0; col<MAZE_SIZE; col+=1)
		{
			float x=(col-MAZE_SIZE/2)*length*2+length,y=(MAZE_SIZE/2-row)*length*2-length;
			if (maze_[row][col] == OBSTACLE){
				//cout << "XXXXXX";
				
				glBegin(GL_POLYGON);
					glColor3f(0.5,0.25,0.0);
					glVertex2f(x-length,y-length);	glVertex2f(x+length,y-length);
					glVertex2f(x+length,y+length);	glVertex2f(x-length,y+length);
				glEnd();
			}
			else 
			{
				char symbol = ' ';
   				if (row == agentPosX_ && col == agentPosY_){
					float size=0.8*length;
					symbol = '=';
					switch(preAction_){
						case Agent::actIDLE:
							glBegin(GL_POLYGON);
								glColor3f(0.0,1.0,0.0);
								glVertex2f(x-size,y-size);	glVertex2f(x+size,y-size);
								glVertex2f(x+size,y+size);	glVertex2f(x-size,y+size);
							glEnd();
							break;
						case Agent::actSUCK:
							glBegin(GL_POLYGON);
								glColor3f(1.0,0.8,0.3);
								glVertex2f(x-size,y-size);	glVertex2f(x+size,y-size);
								glVertex2f(x+size,y+size);	glVertex2f(x-size,y+size);
							glEnd();
							break;
						case Agent::actUP:
							if(bump_){
								glColor3f(1.0,0.0,0.0);
							}
							else{
								glColor3f(0.0,1.0,0.0);
							}
							glBegin(GL_POLYGON);
								glVertex2f(x,y+size);
								glVertex2f(x+size,y-size);	glVertex2f(x-size,y-size);
							glEnd();
							break;
						case Agent::actDOWN:
							if(bump_){
								glColor3f(1.0,0.0,0.0);
							}
							else{
								glColor3f(0.0,1.0,0.0);
							}
							glBegin(GL_POLYGON);
								glVertex2f(x,y-size);
								glVertex2f(x+size,y+size);	glVertex2f(x-size,y+size);
							glEnd();
							break;
						case Agent::actLEFT:
							if(bump_){
								glColor3f(1.0,0.0,0.0);
							}
							else{
								glColor3f(0.0,1.0,0.0);
							}
							glBegin(GL_POLYGON);
								glVertex2f(x-size,y);
								glVertex2f(x+size,y-size);	glVertex2f(x+size,y+size);
							glEnd();
							break;
						case Agent::actRIGHT:
							if(bump_){
								glColor3f(1.0,0.0,0.0);
							}
							else{
								glColor3f(0.0,1.0,0.0);
							}
							glBegin(GL_POLYGON);
								glVertex2f(x+size,y);
								glVertex2f(x-size,y-size);	glVertex2f(x-size,y+size);
							glEnd();
							break;    	
							
					}
				}
				glBegin(GL_POLYGON);
					glColor3f(((float)(dirty_level-maze_[row][col]))/dirty_level,((float)(dirty_level-maze_[row][col]))/dirty_level,((float)(dirty_level-maze_[row][col]))/dirty_level);
					glVertex2f(x-length,y-length);	glVertex2f(x+length,y-length);
					glVertex2f(x+length,y+length);	glVertex2f(x-length,y+length);
				glEnd();
				//cout << symbol;
				//cout << setw(4) << maze_[row][col];
				//cout << symbol;
				
			}
		}// for - col
		//cout << endl;
	}// for - row
}
// -----------------------------------------------------------
void Environment::Change(const RandomNumberGenerator &rng)
{
	for (int row=0; row<MAZE_SIZE; row+=1)
	{
		for (int col=0; col<MAZE_SIZE; col+=1)
		{
			if (maze_[row][col] != OBSTACLE && 
			    static_cast<double>(rng.RandomValue())/RAND_MAX < dirtyProb_) // probability: 0.01
			{
				maze_[row][col] += 1;
			}
		}// for - col
	}// for - row
}
// -----------------------------------------------------------
void Environment::AcceptAction(Agent::ActionType action)
{
	bump_ = false;
	switch (action)
	{
		case Agent::actSUCK:
			if(maze_[agentPosX_][agentPosY_] > 0){
				maze_[agentPosX_][agentPosY_] -= 1;
			}
			break;
		case Agent::actUP:
			if (maze_[agentPosX_-1][agentPosY_] != OBSTACLE) agentPosX_-=1;
			else bump_ = true;
			break;
		case Agent::actDOWN:
			if (maze_[agentPosX_+1][agentPosY_] != OBSTACLE) agentPosX_+=1;
			else bump_ = true;			
			break;
		case Agent::actLEFT:
			if (maze_[agentPosX_][agentPosY_-1] != OBSTACLE) agentPosY_-=1;
			else bump_ = true;			
			break;
		case Agent::actRIGHT:
			if (maze_[agentPosX_][agentPosY_+1] != OBSTACLE) agentPosY_+=1;
			else bump_ = true;			
			break;    		
	}
	preAction_=action;
}
// -----------------------------------------------------------
