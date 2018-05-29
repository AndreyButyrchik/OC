#include <windows.h>
#include <tchar.h>

typedef struct _message
{
	int sender;
	int receiver;
	TCHAR text[50];
} MESSAGE;

MESSAGE ReadFromPipe(HANDLE stdOut)
{
	DWORD dwRead;
	MESSAGE Message;
	BOOL bSuccess = FALSE;

	bSuccess = ReadFile(stdOut, &Message, sizeof(Message), &dwRead, NULL);
	if (!bSuccess || dwRead == 0)
	{
		int error = GetLastError();
		_tprintf_s(TEXT("%d\n"), error);
	}

	return Message;
}