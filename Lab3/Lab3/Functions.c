#include <tchar.h>
#include <windows.h>
#include <stdio.h>

extern struct _massData
{
	int* mass;
	int dim;
} massData;

extern int *terminateThreads;

extern HANDLE *hDeadlockEvent;

int cntMarkValues(int mark)
{
	int count = 0;
	for (int i = 0; i < massData.dim; i++)
	{
		if (massData.mass[i] == mark)
			count++;
	}
	return count;
};

void nullMarkValues(int mark)
{
	for (int i = 0; i < massData.dim; i++)
	{
		if (massData.mass[i] == mark)
			massData.mass[i] = 0;
	}
};

void showMass()
{
	_tprintf_s(TEXT("Source array: "));
	for (int i = 0; i < massData.dim; i++)
		_tprintf_s(TEXT("%d "), massData.mass[i]);
	_tprintf_s(TEXT("\n"));
}

void markTerminateThreads(int cntThreads)
{
	for (int i = 0; i < cntThreads; i++)
		if (terminateThreads[i] == 1)
			SetEvent(hDeadlockEvent[i]);
};