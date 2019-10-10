#pragma once

#include "Task.h"

class TasksQueue
{
public:
	TasksQueue();
	VOID push(TASK *task);
	TASK* pop();
	TASK* front();
	long size();
	~TasksQueue();
private:
	TASK *tasks = NULL;
	volatile long count;
	volatile long head;
	CRITICAL_SECTION lock;
};

