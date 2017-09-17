#ifndef GUI_H
#define GUI_H

#include<fstream>
#include"include/GL/glui.h"
namespace GUI{
	void myGlutReshape(int,int);//根據視窗大小設定可見範圍 
	void myGlutDisplay();//繪圖 
	void control_cb(int);//圖形介面所有按鈕控制 
	int startDraw(int,char *[]);
	void showScore();
	
	void doOneStep();
	void doOneRun();
	void doAllRun();
	void nextRun();
	void newGame();
};
#endif
