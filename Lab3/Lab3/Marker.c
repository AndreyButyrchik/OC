#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <time.h>

extern HANDLE hBeginEvent, *hTerminateEvent, *hContinueEvent, *hDeadlockEvent;
extern CRITICAL_SECTION csMassData;
extern struct _massData
{
	int* mass;
	int dim;
} massData;

int cntMarkValues(int mark);

void nullMarkValues(int mark);

DWORD WINAPI markerWork(LPVOID lpParam)
{
	WaitForSingleObject(hBeginEvent, INFINITE);
	HANDLE hThreadEvent[2];
	hThreadEvent[0] = hTerminateEvent[(unsigned int)lpParam - 1];
	hThreadEvent[1] = hContinueEvent[(unsigned int)lpParam - 1];
	srand((unsigned int)lpParam);
	int randValue;
	while (TRUE)
	{
		EnterCriticalSection(&csMassData);
		randValue = rand() % massData.dim;
		if (massData.mass[randValue] == 0)
		{
			Sleep(5);
			massData.mass[randValue] = (unsigned int)lpParam;
			Sleep(5);
			LeaveCriticalSection(&csMassData);
		}
		else
		{
			_tprintf_s(TEXT("Thread: %d\n"), (unsigned int)lpParam);
			_tprintf_s(TEXT("\tCount of mark values: %d\n"), cntMarkValues((unsigned int)lpParam));
			_tprintf_s(TEXT("\tIndex: %d\n"), randValue);
			LeaveCriticalSection(&csMassData);
			SetEvent(hDeadlockEvent[(unsigned int)lpParam - 1]);
			DWORD dwThread = WaitForMultipleObjects(2, hThreadEvent, FALSE, INFINITE);
			if (dwThread == WAIT_OBJECT_0)
			{
				nullMarkValues((unsigned int)lpParam);
				return TRUE;
			}
		}
	}
	return 0;
}