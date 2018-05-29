#include <windows.h>
#include <tchar.h>
#include <stdio.h>

typedef struct _student
{
	int num;
	TCHAR name[10];
	double grade;
} student;

typedef struct _threadinfo {
	HANDLE hRead;
	HANDLE hWrite;
	TCHAR *fBinName;
	HANDLE hEventWrite;
	HANDLE hEventRead;
} THREADINFO;

typedef struct _command {
	int num;
	int type;
} COMMAND;

student getRecord(TCHAR* fBinName, int num);

BOOL setRecord(TCHAR* fBinName, student record, int num);
int cntReaders = 0;

DWORD WINAPI clientProcess(LPVOID thinf)
{
	THREADINFO threadInfo = *(THREADINFO*)thinf;
	if (!ConnectNamedPipe(threadInfo.hRead, NULL))
	{
		_tprintf_s(TEXT("Client don't connected\n"));
		CloseHandle(threadInfo.hRead);
	}

	if (!ConnectNamedPipe(threadInfo.hWrite, NULL))
	{
		_tprintf_s(TEXT("Client don't connected\n"));
		CloseHandle(threadInfo.hWrite);
	}
	_tprintf_s(TEXT("Client connected\n"));

	COMMAND command;
	student record;
	BOOL exit = FALSE;
	DWORD bytes;
	BOOL isFinish = TRUE;
	int idxNum = 0;
	do
	{
		ReadFile(threadInfo.hRead, &command, sizeof(command), &bytes, 0);	
		switch (command.type)
		{
			case 1:
				WaitForSingleObject(threadInfo.hEventWrite, INFINITE);
				record = getRecord(threadInfo.fBinName, command.num);
				if (record.num)
				{
					WriteFile(threadInfo.hWrite, &record, sizeof(record), &bytes, 0);
					idxNum = record.num;
					ReadFile(threadInfo.hRead, &record, sizeof(record), &bytes, 0);
					WaitForSingleObject(threadInfo.hEventRead, INFINITE);
					ResetEvent(threadInfo.hEventWrite);
					setRecord(threadInfo.fBinName, record, idxNum);
					WriteFile(threadInfo.hWrite, &isFinish, sizeof(isFinish), &bytes, 0);
				}
				break;
			case 2:
				WaitForSingleObject(threadInfo.hEventWrite, INFINITE);
				cntReaders++;
				ResetEvent(threadInfo.hEventRead);
				record = getRecord(threadInfo.fBinName, command.num);
				if (record.num)
					WriteFile(threadInfo.hWrite, &record, sizeof(record), &bytes, 0);
				break;
			case 3:
				exit = TRUE;
				break;
			case 4:
				SetEvent(threadInfo.hEventWrite);
				break;
			case 5:
				cntReaders--;
				if (cntReaders == 0)
					SetEvent(threadInfo.hEventRead);
				break;
			default:
				_tprintf_s(TEXT("Invalid number of operation\n"));
				break;
		}
	} while (command.type != 3);

	DisconnectNamedPipe(threadInfo.hRead);
	DisconnectNamedPipe(threadInfo.hWrite);
	return 0;
}