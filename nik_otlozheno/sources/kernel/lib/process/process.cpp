#include "process.h"

Process::Process(){
	nextTaskIndex = 0;
	tasks = new ITask*[TASKS_COUNT];
}

void Process::addTask(ITask *task){
	if(nextTaskIndex < TASKS_COUNT){
		tasks[nextTaskIndex++] = task;
	}
}

void Process::step(){
	for(unsigned int i = 0; i < nextTaskIndex; ++i){
		tasks[i]->step();
	}
}