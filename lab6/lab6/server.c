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

int printBinFile(TCHAR* fBinName);

int writeBin(TCHAR* fileName, student* records, int cntRecords);

DWORD WINAPI clientProcess(LPVOID lpPipe);

int _tmain(int argc, TCHAR* argv[])
{
	TCHAR fBinName[10];
	TCHAR cntRecordsChar[3];
	_tprintf_s(TEXT("Please, enter name of binary file.\n"));
	_tscanf_s(TEXT("%s"), fBinName, (unsigned)_countof(fBinName));
	_tprintf_s(TEXT("Please, enter count of records.\n"));
	_tscanf_s(TEXT("%s"), cntRecordsChar, (unsigned)_countof(cntRecordsChar));
	int cntRecords = _ttoi(cntRecordsChar);

	student *records = (student*)malloc(sizeof(student) * cntRecords);
	_tprintf_s(TEXT("Please, enter %d records of students.\nExample: Name (Andrey) Average mark (7.5) Number student's record book (1623353)\n"), cntRecords);
	for (int i = 0; i < cntRecords; i++)
	{
		_tprintf_s(TEXT("Plese, enter %d record:\n"), i + 1);
		_tscanf_s(TEXT("%s%lF%d"), records[i].name, (unsigned)_countof(records[i].name), &records[i].grade, &records[i].num);
	}

	writeBin(fBinName, records, cntRecords);
	_tprintf_s(TEXT("\nBinary file:\n"));
	printBinFile(fBinName);

	TCHAR cntClientsChar[3];
	_tprintf_s(TEXT("Please, enter count of clients.\n"));
	_tscanf_s(TEXT("%s"), cntClientsChar, (unsigned)_countof(cntClientsChar));
	int cntClients = _ttoi(cntClientsChar);

	HANDLE hEventWrite = CreateEvent(NULL, TRUE, TRUE, NULL);
	HANDLE hEventRead = CreateEvent(NULL, TRUE, TRUE, NULL);
	HANDLE *arrayHThread = (HANDLE*)malloc(sizeof(HANDLE) * cntClients);
	DWORD *arrayHThreadId = (DWORD*)malloc(sizeof(DWORD) * cntClients);
	THREADINFO *pipeArray = (THREADINFO*)malloc(sizeof(THREADINFO) * cntClients);


	TCHAR pipeName[50];
	for (int i = 0; i < cntClients; i++)
	{
		wsprintf(pipeName, TEXT("\\\\.\\pipe\\Pipe_%d_%d"), 1, i);
		_tprintf(TEXT("\nPipe Server: Main thread awaiting client connection on %s\n"), pipeName);
		pipeArray[i].hRead = CreateNamedPipe(pipeName, PIPE_ACCESS_INBOUND, PIPE_TYPE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 0, 0, 0, NULL);

		if (pipeArray[i].hRead == INVALID_HANDLE_VALUE)
		{
			_tprintf(TEXT("CreateNamedPipe failed, GLE=%d.\n"), GetLastError());
			return -1;
		}
		wsprintf(pipeName, TEXT("\\\\.\\pipe\\Pipe_%d_%d"), 2, i);
		pipeArray[i].hWrite = CreateNamedPipe(pipeName, PIPE_ACCESS_OUTBOUND, PIPE_TYPE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 0, 0, 0, NULL);

		if (pipeArray[i].hWrite == INVALID_HANDLE_VALUE)
		{
			_tprintf(TEXT("CreateNamedPipe failed, GLE=%d.\n"), GetLastError());
			return -1;
		}
		pipeArray[i].fBinName = fBinName;
		pipeArray[i].hEventRead = hEventRead;
		pipeArray[i].hEventWrite = hEventWrite;
		arrayHThread[i] = CreateThread(NULL, 0, clientProcess, &pipeArray[i], 0, &arrayHThreadId[i]);
	}
	WaitForMultipleObjects(cntClients, arrayHThread, TRUE, INFINITE);


	for (int i = 0; i < cntClients; i++)
		CloseHandle(arrayHThread[i]);
	return 0;
}