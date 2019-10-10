#pragma once
#include <Windows.h>

struct TASK
{
	PTHREAD_START_ROUTINE func;
	LPVOID param;
};