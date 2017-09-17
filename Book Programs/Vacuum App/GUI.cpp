#include<fstream>
#include<sstream>
#include<iomanip>
#include<iostream>
#include<windows.h>
using namespace std;
#include"GUI.h"
using namespace GUI;
#include<stdlib.h>
#include"random_num_gen.h"
#include"environment.h"
#include"agent.h"
#include"evaluator.h"

const int NEWGAME_BTN_ID=102,SELECTMAP_BTN_ID=103,DISPLAY_BTN_ID=104,NEWMAP_BTN_ID=105;
const int NEWMAP_OK_BTN_ID=202,NEWMAP_CANCEL_BTN_ID=203;
const int SELECTMAP_SELECT_ID=302,SELECTMAP_CANCEL_BTN_ID=303;
const int DO_ONE_STEP_BTN_ID=402,DO_ONE_RUN_BTN_ID=403,NEXT_RUN_BTN_ID=404,DO_ALL_RUN_BTN_ID=405;
const int ERROR_OK_BTN_ID=502;

char life_time_input[1000]="2000",total_runs_input[1000]="10",file_name_input[1000]="agent.map";
char display_step[20]="100",display_time[20]="10";
char life_time_temp[1000],total_runs_temp[1000],file_name_temp[1000];
int tx,ty,tw,th;
float magnification_x,magnification_y;
long long life_time=2000,current_time,total_runs=10,current_run,dirty_degree,consumed_energy;
long long total_dirty_degree,total_consumed_energy;
char time_step_msg[100],action_msg[100],dirty_degree_msg[100],consumed_energy_msg[100];

Environment *env=NULL;
Agent *agent=NULL;
RandomNumberGenerator *rng=NULL;
Evaluator *evaluator=NULL;
Agent::ActionType action=Agent::actIDLE;
ifstream fin;

GLUI *main_glui,*score_glui,*new_map_glui,*select_map_glui,*error_glui;
GLUI_Button *new_map_btn,*select_map_btn,*new_map_ok_btn,*new_map_cancel_btn;
GLUI_Button *do_one_step_btn,*display_btn,*do_one_run_btn,*next_run_btn,*do_all_run_btn;
GLUI_StaticText *time_step_text,*action_text,*dirty_degree_text,*consumed_energy_text;
GLUI_StaticText *complete_runs_text,*total_score_text,*average_score_text;
GLUI_FileBrowser *map_list;

void GUI::myGlutReshape(int w,int h){
	GLUI_Master.get_viewport_area(&tx,&ty,&tw,&th);
	glViewport(tx,ty,tw,th);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(tw<=th){
		magnification_x=1;
		magnification_y=(GLfloat)th/(GLfloat)tw;
	}
	else{
		magnification_x=(GLfloat)tw/(GLfloat)th;
		magnification_y=1;
	}
	glOrtho(-10.0*magnification_x,10.0*magnification_x,-10.0*magnification_y,10.0*magnification_y,-1.0,1.0);
    glMatrixMode(GL_MODELVIEW);
}
void GUI::myGlutDisplay(){
	glClearColor(200.0/255,200.0/255,200.0/255,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluOrtho2D(-50.0,50.0,-50.0,50.0);
	if(env!=NULL){
		env->Show(tw,th);
		showScore();
	}
	glFlush();
	glutSwapBuffers();
}
void GUI::control_cb(int id){
	switch(id){
		case NEWMAP_BTN_ID:
			new_map_btn->disable();
			select_map_btn->disable();
			strcpy(life_time_temp,life_time_input);
			strcpy(total_runs_temp,total_runs_input);
			strcpy(file_name_temp,file_name_input);
			new_map_glui=GLUI_Master.create_glui("New Game Setting",0,100,100);
				new GLUI_EditText(new_map_glui,"Life Time:",life_time_temp);
				new GLUI_EditText(new_map_glui,"Test Case:",total_runs_temp);
				new GLUI_EditText(new_map_glui,"File Name:",file_name_temp);
				new_map_ok_btn=new GLUI_Button(new_map_glui,"OK",NEWMAP_OK_BTN_ID,control_cb);
				new_map_cancel_btn=new GLUI_Button(new_map_glui,"Cancel",NEWMAP_CANCEL_BTN_ID,control_cb);
			break;
		case SELECTMAP_BTN_ID:
			new_map_btn->disable();
			select_map_btn->disable();
			strcpy(life_time_temp,life_time_input);
			strcpy(total_runs_temp,total_runs_input);
			strcpy(file_name_temp,file_name_input);
			select_map_glui=GLUI_Master.create_glui("New Game Setting",0,100,100);
				new GLUI_EditText(select_map_glui,"Life Time:",life_time_temp);
				new GLUI_EditText(select_map_glui,"Test Case:",total_runs_temp);
				map_list=new GLUI_FileBrowser(select_map_glui,"",false,SELECTMAP_SELECT_ID,control_cb);
				new_map_cancel_btn=new GLUI_Button(select_map_glui,"Cancel",NEWMAP_CANCEL_BTN_ID,control_cb);
			break;
		case DISPLAY_BTN_ID:
			if(true){
				long long sleep = atol(display_time);
				for(long long t=atol(display_step);t>0 && current_time<life_time;--t){
					doOneStep();
					myGlutDisplay();
					Sleep(sleep);
				}
			}
			break;
		case NEWMAP_OK_BTN_ID:
			strcpy(life_time_input,life_time_temp);
			strcpy(total_runs_input,total_runs_temp);
			strcpy(file_name_input,file_name_temp);
			life_time=atol(life_time_input);
			total_runs=atol(total_runs_input);
			current_run=1;
			total_dirty_degree=0;
			total_consumed_energy=0;
			newGame();
			new_map_btn->enable();
			select_map_btn->enable();
			new_map_glui->close();
			break;
		case NEWMAP_CANCEL_BTN_ID:
			new_map_btn->enable();
			select_map_btn->enable();
			new_map_glui->close();
			break;
		case SELECTMAP_SELECT_ID:
			strcpy(life_time_input,life_time_temp);
			strcpy(total_runs_input,total_runs_temp);
			strcpy(file_name_input,map_list->get_file());
			life_time=atol(life_time_input);
			total_runs=atol(total_runs_input);
			current_run=1;
			total_dirty_degree=0;
			total_consumed_energy=0;
			newGame();
			new_map_btn->enable();
			select_map_btn->enable();
			select_map_glui->close();
			break;
		case SELECTMAP_CANCEL_BTN_ID:
			new_map_btn->enable();
			select_map_btn->enable();
			select_map_glui->close();
			break;
		case DO_ONE_STEP_BTN_ID:
			doOneStep();
			break;
		case DO_ONE_RUN_BTN_ID:
			doOneRun();
			break;
		case NEXT_RUN_BTN_ID:
			nextRun();
			break;
		case DO_ALL_RUN_BTN_ID:
			doAllRun();
			break;
		case ERROR_OK_BTN_ID:
			error_glui->close();
			break;
		default:
			break;
	}
	glutPostRedisplay();
}
int GUI::startDraw(int argc,char **argv){
	glutInit(&argc,argv);
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowPosition(50,50);
	glutInitWindowSize(668,693);
	
	int main_window=glutCreateWindow("Agent");
	GLUI_Master.set_glutReshapeFunc(myGlutReshape);
	glutDisplayFunc(myGlutDisplay);
		glEnable(GL_DEPTH_TEST);
			
	score_glui=GLUI_Master.create_glui_subwindow(main_window,GLUI_SUBWINDOW_BOTTOM);
		new GLUI_StaticText(score_glui,"");//以空白行做分隔 
		time_step_text = new GLUI_StaticText(score_glui,"");
		new GLUI_StaticText(score_glui,"");//以空白行做分隔 
		action_text = new GLUI_StaticText(score_glui,"");
		new GLUI_StaticText(score_glui,"");//以空白行做分隔 
		dirty_degree_text = new GLUI_StaticText(score_glui,"");
		new GLUI_StaticText(score_glui,"");//以空白行做分隔 
		consumed_energy_text = new GLUI_StaticText(score_glui,"");
		new GLUI_StaticText(score_glui,"");//以空白行做分隔 
		new GLUI_Column(score_glui,false);
		new GLUI_Column(score_glui,false);
		new GLUI_Column(score_glui,false);
		new GLUI_Column(score_glui,false);
		new GLUI_Column(score_glui,false);
		new GLUI_Column(score_glui,false);
		new GLUI_Column(score_glui,false);
		new GLUI_Column(score_glui,false);
		new GLUI_Column(score_glui,false);
		new GLUI_Column(score_glui,false);
		new GLUI_StaticText(score_glui,"");//以空白行做分隔 
		complete_runs_text = new GLUI_StaticText(score_glui,"");
		new GLUI_StaticText(score_glui,"");//以空白行做分隔 
		total_score_text = new GLUI_StaticText(score_glui,"");
		new GLUI_StaticText(score_glui,"");//以空白行做分隔 
		average_score_text = new GLUI_StaticText(score_glui,"");
		new GLUI_StaticText(score_glui,"");//以空白行做分隔 
	score_glui->set_main_gfx_window( main_window );
	main_glui=GLUI_Master.create_glui_subwindow(main_window,GLUI_SUBWINDOW_RIGHT);
		new GLUI_StaticText(main_glui,"");//以空白行做分隔 
		new_map_btn=new GLUI_Button(main_glui,"NewMap",NEWMAP_BTN_ID,control_cb);
		select_map_btn=new GLUI_Button(main_glui,"SelectMap",SELECTMAP_BTN_ID,control_cb);
		new GLUI_StaticText(main_glui,"");//以空白行做分隔 
		do_one_step_btn=new GLUI_Button(main_glui,"DoOneStep",DO_ONE_STEP_BTN_ID,control_cb);
		do_one_step_btn->disable();
		do_one_run_btn=new GLUI_Button(main_glui,"DoOneRun",DO_ONE_RUN_BTN_ID,control_cb);
		do_one_run_btn->disable();
		next_run_btn=new GLUI_Button(main_glui,"NextRun",NEXT_RUN_BTN_ID,control_cb);
		next_run_btn->disable();
		do_all_run_btn=new GLUI_Button(main_glui,"DoAllRun",DO_ALL_RUN_BTN_ID,control_cb);
		do_all_run_btn->disable();
		new GLUI_StaticText(main_glui,"");//以空白行做分隔 
		new GLUI_StaticText(main_glui,"");//以空白行做分隔 
		new GLUI_StaticText(main_glui,"");//以空白行做分隔 
		new GLUI_StaticText(main_glui,"DisplayOption");//以空白行做分隔 
		new GLUI_EditText(main_glui,"Steps:",display_step);
		new GLUI_EditText(main_glui,"Time:",display_time);
		new GLUI_StaticText(main_glui,"");//以空白行做分隔 
		display_btn=new GLUI_Button(main_glui,"Display",DISPLAY_BTN_ID,control_cb);
		display_btn->disable();
		new GLUI_StaticText(main_glui,"");//以空白行做分隔 
		new GLUI_StaticText(main_glui,"");//以空白行做分隔
		new GLUI_StaticText(main_glui,"");//以空白行做分隔 
		new GLUI_StaticText(main_glui,"");//以空白行做分隔 
		new GLUI_StaticText(main_glui,"");//以空白行做分隔 
		new GLUI_Button(main_glui,"Quit",0,(GLUI_Update_CB)exit );//QUIT按鈕結束程式 
	main_glui->set_main_gfx_window( main_window );
	glutMainLoop();
	return 0;
}

void GUI::showScore(){
	ostringstream sout;
	sout.str("");
	sout<<"Run "<<current_run<<"  ,Time step "<<current_time;
	time_step_text->set_text(sout.str().c_str());
	
	sout.str("");
	sout<<"Action "<<ActionStr(action)<<(env->isJustBump()?" Bump!":"");
	action_text->set_text(sout.str().c_str());
	
	sout.str("");
	sout<<"DirtyDegree = "<<dirty_degree;
	dirty_degree_text->set_text(sout.str().c_str());
	
	
	sout.str("");
	sout<<"ConsumedEnergy =  "<<consumed_energy;
	consumed_energy_text->set_text(sout.str().c_str());
	
	/*展示總分*/
	long long complete_runs=current_run-(current_time!=life_time);
	//The Round has been completed 
	sout.str("");
	sout<<"Completed Runs "<<complete_runs;
	complete_runs_text->set_text(sout.str().c_str());
	
	sout.str("");
	sout<<"Total dirty degree =  "<<total_dirty_degree<<"   ,toal consumed energy = "<<total_consumed_energy;
	total_score_text->set_text(sout.str().c_str());
	
	sout.str("");
	if(complete_runs>0){
		sout<<setprecision(3)<<fixed<<"Average dirty degree = "<<(double)total_dirty_degree/(double)complete_runs<<"  ,Average consumed energy = "<<(double)total_consumed_energy/(double)complete_runs;
	}
	else{
		sout<<"Average dirty degree = 0  ,Average consumed energy = 0";
	}
	average_score_text->set_text(sout.str().c_str());
	
}

void GUI::doOneStep(){
	if(current_time<life_time){
		env->Change(*rng);
		
		agent->Perceive(*env);
		
		action = agent->Think();
		env->AcceptAction(action);
		evaluator->Eval(action,*env);
		
		dirty_degree=evaluator->DirtyDegree();
		consumed_energy=evaluator->ConsumedEnergy();
		
		++current_time;
		if(current_time==life_time){//一輪結束 
			do_one_step_btn->disable();
			display_btn->disable();
			do_one_run_btn->disable();
			
			total_dirty_degree+=dirty_degree;
			total_consumed_energy+=consumed_energy;
			
			if(current_run<total_runs){//是否還有下一輪 
				next_run_btn->enable();
			}
			else{
				do_all_run_btn->disable();
			}
		}
	}
}
void GUI::doOneRun(){
	do_one_run_btn->disable();
	
	while(current_time<life_time){
		doOneStep();
	}
	
	if(current_run==total_runs){
		do_all_run_btn->disable();
	}
}
void GUI::doAllRun(){
	doOneRun();
	while(current_run<total_runs){
		nextRun();
		doOneRun();
	}
	do_all_run_btn->disable();
}
void GUI::nextRun(){
	if(current_run<total_runs){
		++current_run;
		do_one_step_btn->enable();
		display_btn->enable();
		do_one_run_btn->enable();
		next_run_btn->disable();
		newGame();
	}
}

void GUI::newGame(){
	fin.close();
	fin.clear();
	delete env;
	delete agent;
	delete rng;
	delete evaluator;

	fin.open(file_name_input);
	if(fin){
		current_time=0;
		dirty_degree=0;
		consumed_energy=0;
		env=new Environment(fin);
		rng=new RandomNumberGenerator(env->RandomSeed()+current_run);
		evaluator=new Evaluator();
		agent=new Agent();
	
		
		do_one_step_btn->enable();
		display_btn->enable();
		do_one_run_btn->enable();
		do_all_run_btn->enable();
	}
	else{
		fin.clear();
		char temp[1000]={"map/"};
		strcat(temp,file_name_input);
		fin.open(temp);
		if(fin){
			current_time=0;
			dirty_degree=0;
			consumed_energy=0;
			env=new Environment(fin);
			rng=new RandomNumberGenerator(env->RandomSeed()+current_run);
			evaluator=new Evaluator();
			agent=new Agent();
		
			
			do_one_step_btn->enable();
			display_btn->enable();
			do_one_run_btn->enable();
			do_all_run_btn->enable();
		}
		else{
			do_one_step_btn->disable();
			display_btn->disable();
			do_one_run_btn->disable();
			do_all_run_btn->disable();
			error_glui=GLUI_Master.create_glui("Error",0,100,100);
				char error_message[1000]="File ";
				strcat(error_message,file_name_input);
				strcat(error_message," can't be open");
				new GLUI_StaticText(error_glui,error_message);
				new_map_ok_btn=new GLUI_Button(error_glui,"OK",ERROR_OK_BTN_ID,control_cb);
		}
	}
}
