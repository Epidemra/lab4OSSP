#include "stdafx.h"
#include "ThreadPool.h"

#define SPIN_COUNT 4000

ThreadPool::ThreadPool(INT threadCount)
{
	InitializeCriticalSectionAndSpinCount(&lock, SPIN_COUNT);
	InitializeConditionVariable(&conditionVariable);
	isClosed = false;
	this->tasks = new TasksQueue();
	this->threadCount = threadCount;
	this->threads = new HANDLE[threadCount];
	for (int i = 0; i < threadCount; i++)
	{
		threads[i] = CreateThread(NULL, 0, ThreadFunc, this, 0, NULL);
	}
}

void ThreadPool::enqueueTask(TASK* task)
{
	EnterCriticalSection(&lock);
	tasks->push(task);
	WakeConditionVariable(&conditionVariable);
	LeaveCriticalSection(&lock);
}

void ThreadPool::Close()
{
	if (!isClosed)
	{
		EnterCriticalSection(&lock);
		isClosed = true;
		WakeAllConditionVariable(&conditionVariable);
		LeaveCriticalSection(&lock);
	}
	for (int i = 0; i < threadCount; i++)
	{
		WaitForSingleObject(threads[i], INFINITE);
		CloseHandle(threads[i]);
	}
	delete[] threads;
	delete tasks;
}

ThreadPool::~ThreadPool()
{
	if (!isClosed)
	{
		Close();
	}
}

TASK* ThreadPool::dequeueTask()
{
	EnterCriticalSection(&lock);
	while (tasks->size() == 0 && !isClosed)
	{
		// Queue is empty => sleep while anybody added task
		SleepConditionVariableCS(&conditionVariable, &lock, INFINITE);
	}
	TASK* task = NULL;
	if (tasks->size() != 0)
	{
		//task = tasks->front();
		task = tasks->pop();
	}
	LeaveCriticalSection(&lock);
	return task;
}

DWORD WINAPI ThreadPool::ThreadFunc(LPVOID param)
{
	ThreadPool *threadPool = (ThreadPool*)param;

	while (!threadPool->isClosed)
	{
		TASK *task = threadPool->dequeueTask();
		if (!task)
		{
			break;
		}
		else
		{
			task->func(task->param);
		}
	}

	return 0;
}

