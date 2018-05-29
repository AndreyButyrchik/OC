#include <windows.h>
#include <tchar.h>

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

MESSAGE ReadFromPipe(HANDLE stdOut);

int WriteToPipe(MESSAGE Message, HANDLE stdIn);

DWORD WINAPI subscribeOnClient(CLIENTPIPES clientPipe)
{
	MESSAGE message;
	while (TRUE)
	{
		message = ReadFromPipe((HANDLE)clientPipe.hPipe_Rd);
		_tprintf_s(TEXT("Message: %s\nSender: %d\nReceiver: %d\n"), message.text, message.sender, message.receiver);
		if (message.receiver == -1)
			break;
		if (message.receiver != message.sender)
			WriteToPipe(message, (HANDLE)clientPipe.hPipes_Wr[message.receiver - 1]);
	}
	return 0;
}
