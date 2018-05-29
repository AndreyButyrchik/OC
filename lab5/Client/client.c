#include <windows.h>
#include <tchar.h>
#include <stdio.h>

typedef struct _message
{
	int sender;		// номер процесса отправителя
	int receiver;	// номер процесса получателя
	TCHAR text[50]; // текст сообщения
} MESSAGE;

int WriteToPipe(MESSAGE Message, HANDLE stdIn);

MESSAGE ReadFromPipe(HANDLE stdOut);

int _tmain(int argc, TCHAR* argv[])
{
	HANDLE hPipe_Wr = (HANDLE)_ttoll(argv[2]);
	HANDLE hPipe_Rd = (HANDLE)_ttoll(argv[3]);

	_tprintf_s(TEXT("Number of client: %s\n"), argv[1]);

	_tprintf_s(TEXT("Operations:\n1 - send message\n2 - read message\n3 - exit\n"));
	int operation = 0;
	BOOL exit = FALSE;
	MESSAGE message;
	while (TRUE)
	{
		if (exit)
			break;
		_tprintf_s(TEXT("Please, choose operation\n"));
		_tscanf_s(TEXT("%d"), &operation);
		getchar();
		switch (operation)
		{
			case 1:
				_tprintf_s(TEXT("Please, enter your message\n"));
				_fgetts(message.text, 50 * sizeof(TCHAR), stdin);
				message.sender = _ttoi(argv[1]);
				_tprintf_s(TEXT("Please, enter number of receiver\n"));
				_tscanf_s(TEXT("%d"), &message.receiver);
				getchar();
				WriteToPipe(message, hPipe_Wr);
				break;
			case 2:
				message = ReadFromPipe(hPipe_Rd);
				_tprintf_s(TEXT("Message: %s\nSender: %d\nReceiver: %d\n"), message.text, message.sender, message.receiver);
				break;
			case 3:
				message.receiver = -1;
				WriteToPipe(message, hPipe_Wr);
				exit = TRUE;
				break;
			default:
				_tprintf_s(TEXT("Please, enter valid number of operation\n"));
				break;
		}
	}

	return 0;
}
