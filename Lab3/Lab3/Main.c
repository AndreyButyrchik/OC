#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <time.h>

struct _massData
{
	int* mass;
	int dim;
} massData;

HANDLE hBeginEvent, *hTerminateEvent, *hContinueEvent, *hDeadlockEvent;
CRITICAL_SECTION csMassData;

DWORD WINAPI markerWork(LPVOID lpParam);

int *terminateThreads;

void showMass();

void markTerminateThreads(cntThreads);

int _tmain()
{
	int cntThreads, numbTerminateThread;
	_tprintf_s(TEXT("Please, enter matrix dimension.\n"));
	_tscanf_s(TEXT("%d"), &massData.dim);

	massData.mass = (int*)malloc(massData.dim * sizeof(int));
	for (int i = 0; i < massData.dim; i++)
		massData.mass[i] = 0;

	_tprintf_s(TEXT("Please, enter count of threads.\n"));
	_tscanf_s(TEXT("%d"), &cntThreads);

	DWORD* massThreadId = (DWORD*)malloc(massData.dim * sizeof(DWORD));
	HANDLE* massHThread = (HANDLE*)malloc(massData.dim * sizeof(HANDLE));
	hTerminateEvent = (HANDLE*)malloc(massData.dim * sizeof(HANDLE));
	hContinueEvent = (HANDLE*)malloc(massData.dim * sizeof(HANDLE));
	hDeadlockEvent = (HANDLE*)malloc(massData.dim * sizeof(HANDLE));
	terminateThreads = (int*)malloc(massData.dim * sizeof(int));
	InitializeCriticalSection(&csMassData);
	for (int i = 0; i < cntThreads; i++)
	{
		massHThread[i] = CreateThread(NULL, 0, markerWork, (LPVOID)(i + 1), 0, &massThreadId[i]);
		hDeadlockEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
		hTerminateEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
		hContinueEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
		terminateThreads[i] = 0;
	}
	hBeginEvent = CreateEvent(NULL, TRUE, TRUE, NULL);


	while (TRUE)
	{
		WaitForMultipleObjects(cntThreads, hDeadlockEvent, TRUE, INFINITE);
		showMass();
		_tprintf_s(TEXT("Please, enter number of thread you want to terminate.\n"));
		_tscanf_s(TEXT("%d"), &numbTerminateThread);
		while (numbTerminateThread < 0 || numbTerminateThread > massData.dim)
		{
			_tprintf_s(TEXT("You enter invalid number, please try again.\n"));
			_tscanf_s(TEXT("%d"), &numbTerminateThread);
		}
		SetEvent(hTerminateEvent[numbTerminateThread - 1]);
		terminateThreads[numbTerminateThread - 1] = 1;
		WaitForSingleObject(massHThread[numbTerminateThread - 1], INFINITE);
		markTerminateThreads(cntThreads);
		showMass();
		for (int i = 0; i < cntThreads; i++)
		{
			if (i != (numbTerminateThread - 1))
			{
				SetEvent(hContinueEvent[i]);
			}
		}
	}
	DeleteCriticalSection(&csMassData);

	system("pause");
	return 0;
}