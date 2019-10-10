#pragma once
#include "Task.h"
#include "TasksQueue.h"

using namespace std;

class ThreadPool
{
public:
	ThreadPool(INT threadCount);
	void enqueueTask(TASK* taskStruct);
	void Close();
	~ThreadPool();
private:
	TasksQueue *tasks;
	HANDLE* threads;
	CRITICAL_SECTION lock;
	CONDITION_VARIABLE conditionVariable;
	volatile BOOL isClosed;
	INT threadCount;
	TASK* dequeueTask();
	static DWORD WINAPI ThreadFunc(LPVOID param);
};


