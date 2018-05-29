#include <windows.h>
#include <tchar.h>
#include <stdio.h>

typedef struct _message
{
	int sender;
	int receiver;
	TCHAR text[50];
} MESSAGE;

typedef struct _clientPipes
{
	HANDLE hPipe_Rd;
	HANDLE *hPipes_Wr;
} CLIENTPIPES;

int WriteToPipe(MESSAGE Message, HANDLE stdIn);

MESSAGE ReadFromPipe(HANDLE stdOut);

DWORD WINAPI subscribeOnClient(CLIENTPIPES clientPipe);


int _tmain(int argc, TCHAR* argv[])
{
	int cntClients = 0;
	_tprintf_s(TEXT("Please, enter count of clients.\n"));
	_tscanf_s(TEXT("%d"), &cntClients);
	getchar();												// get symbol of newline after scanf

	STARTUPINFO *si = (STARTUPINFO*)malloc(sizeof(STARTUPINFO) * cntClients);
	PROCESS_INFORMATION *pi = (PROCESS_INFORMATION*)malloc(sizeof(PROCESS_INFORMATION) * cntClients);

	DWORD *arrayHThreadId = (DWORD*)malloc(sizeof(DWORD) * cntClients);
	HANDLE *arrayHThread = (HANDLE*)malloc(sizeof(HANDLE) * cntClients);

	CLIENTPIPES *hClientPipes = (CLIENTPIPES*)malloc(sizeof(CLIENTPIPES) * cntClients);

	HANDLE *hOutPipe_Rd = (HANDLE*)malloc(sizeof(HANDLE) * cntClients);
	HANDLE *hOutPipe_Wr = (HANDLE*)malloc(sizeof(HANDLE) * cntClients);
	HANDLE *hInPipe_Rd = (HANDLE*)malloc(sizeof(HANDLE) * cntClients);
	HANDLE *hInPipe_Wr = (HANDLE*)malloc(sizeof(HANDLE) * cntClients);
	
	SECURITY_ATTRIBUTES saPipe;
	saPipe.nLength = sizeof(SECURITY_ATTRIBUTES);
	saPipe.bInheritHandle = TRUE;
	saPipe.lpSecurityDescriptor = NULL;

	for (int i = 0; i < cntClients; i++)
	{
		if (!CreatePipe(&hOutPipe_Rd[i], &hOutPipe_Wr[i], &saPipe, 0))
			_tprintf_s(TEXT("StdIn CreatePipe error\n"));

		if (!SetHandleInformation(hOutPipe_Wr[i], HANDLE_FLAG_INHERIT, 0))
			_tprintf_s(TEXT("StdIn SetHandleInformation error\n"));

		if (!CreatePipe(&hInPipe_Rd[i], &hInPipe_Wr[i], &saPipe, 0))
			_tprintf_s(TEXT("StdIn CreatePipe error\n"));

		if (!SetHandleInformation(hInPipe_Rd[i], HANDLE_FLAG_INHERIT, 0))
			_tprintf_s(TEXT("StdOut SetHandleInformation error\n"));


		TCHAR comandLine[100];
		wsprintf(comandLine, TEXT("Client.exe %d %d %d\0"), i + 1, hInPipe_Wr[i], hOutPipe_Rd[i]);

		ZeroMemory(&si[i], sizeof(STARTUPINFO));
		ZeroMemory(&pi[i], sizeof(PROCESS_INFORMATION));
		si[i].cb = sizeof(STARTUPINFO);

		if (!CreateProcessW(NULL, comandLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si[i], &pi[i]))
		{
			int error = GetLastError();
			_tprintf_s(TEXT("%d\n"), error);
		}
	}

	for (int i = 0; i < cntClients; i++)
	{
		hClientPipes[i].hPipe_Rd = hInPipe_Rd[i];
		hClientPipes[i].hPipes_Wr = hOutPipe_Wr;
		arrayHThread[i] = CreateThread(NULL, 0, subscribeOnClient, (LPVOID)&hClientPipes[i], 0, &arrayHThreadId[i]);
	}
	
	WaitForMultipleObjects(cntClients, arrayHThread, TRUE, INFINITE);
	for (int i = 0; i < cntClients; i++)
	{
		CloseHandle(pi[i].hThread);
		CloseHandle(pi[i].hProcess);
		CloseHandle(hInPipe_Rd[i]);
		CloseHandle(hInPipe_Wr[i]);
		CloseHandle(hOutPipe_Rd[i]);
		CloseHandle(hOutPipe_Wr[i]);
	}

	return 0;
}
