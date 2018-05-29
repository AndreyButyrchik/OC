#include <windows.h>
#include <tchar.h>
#include <stdio.h>

typedef struct _message
{
	int sender;
	int receiver;
	TCHAR text[50];
} MESSAGE;

int WriteToPipe(MESSAGE Message, HANDLE stdIn)
{
	DWORD dwWritten;
	BOOL bSuccess = FALSE;

	if (!WriteFile(stdIn, &Message, sizeof(Message), &dwWritten, NULL))
	{
		int error = GetLastError();
		_tprintf_s(TEXT("%d\n"), error);
		return -1;
	}

	return 0;
}