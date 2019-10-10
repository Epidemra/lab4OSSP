#include "stdafx.h"
#include "TasksQueue.h"

#define SPIN_COUNT 4000

TasksQueue::TasksQueue()
{
	InitializeCriticalSectionAndSpinCount(&lock, SPIN_COUNT);
	this->count = 0;
	this->head = 0;
}

VOID TasksQueue::push(TASK *task)
{
	EnterCriticalSection(&lock);
	InterlockedIncrement(&count);
	if (tasks == NULL)
	{
		tasks = (TASK *)malloc(sizeof(TASK) * count);
	}
	else
	{
		tasks = (TASK *)realloc(tasks, sizeof(TASK) * count);
	}
	tasks[count - 1] = *task;
	LeaveCriticalSection(&lock);
}

TASK* TasksQueue::pop()
{
	EnterCriticalSection(&lock);
	if (count == 0)
	{
		return NULL;
	}
	TASK *task = &tasks[head];
	InterlockedIncrement(&head);
	LeaveCriticalSection(&lock);
	//reallloc
	return task;
}

TASK* TasksQueue::front()
{
	return &tasks[head];
}

long TasksQueue::size()
{
	return count - head;
}

TasksQueue::~TasksQueue()
{
	DeleteCriticalSection(&lock);
}
