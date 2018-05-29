#include <windows.h>
#include <tchar.h>
#include <stdio.h>

typedef struct _student
{
	int num;
	TCHAR name[10];
	double grade;
} student;

typedef struct _command {
	int num;
	int type;
} COMMAND;

int _tmain(int argc, TCHAR* argv[])
{
	int a, b, c, d, id;
	TCHAR pipeRead[50];
	TCHAR pipeWrite[50];

	_tprintf_s(TEXT("Enter server IP-address and client-ID (10.160.1.193-0) :\n"));
	_tscanf_s(TEXT("%d.%d.%d.%d-%d"), &a, &b, &c, &d, &id);
	wsprintf(pipeWrite, TEXT("\\\\%d.%d.%d.%d\\pipe\\Pipe_%d_%d"), a, b, c, d, 1, id);
	if (!WaitNamedPipe(pipeWrite, NMPWAIT_USE_DEFAULT_WAIT))
	{
		_tprintf_s(TEXT("Connection failed.\n"));
		return 1;
	}

	wsprintf(pipeRead, TEXT("\\\\%d.%d.%d.%d\\pipe\\Pipe_%d_%d"), a, b, c, d, 2, id);
	if (!WaitNamedPipe(pipeRead, NMPWAIT_USE_DEFAULT_WAIT))
	{
		_tprintf_s(TEXT("Connection failed.\n"));
		return 1;
	}
	_tprintf_s(TEXT("Connection success!\n"));

	HANDLE hWriteHandle = CreateFile(pipeWrite,	GENERIC_WRITE, FILE_SHARE_READ,	NULL, OPEN_EXISTING, 0, NULL);
	HANDLE hReadHandle = CreateFile(pipeRead, GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	int operation = 0;
	COMMAND command;
	student record;
	DWORD bytes;
	BOOL exit = FALSE;
	BOOL block;
	_tprintf_s(TEXT("Operations:\n1 - modify\n2 - read\n3 - exit\n"));
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
				record.num = 0;
				command.type = 1;
				_tprintf_s(TEXT("Please, enter number of student's record book\n"));
				_tscanf_s(TEXT("%d"), &command.num);
				getchar();

				WriteFile(hWriteHandle, &command, sizeof(command), &bytes, 0);
				ReadFile(hReadHandle, &record, sizeof(record), &bytes, 0);
				if (record.num) 
				{
					_tprintf_s(TEXT("Record:\n\tName: %s\n\tGrade: %.1lF\n\tNumber student's record book: %d\n"), record.name, record.grade, record.num);
					_tprintf_s(TEXT("Plese, enter new record:\n"));
					_tscanf_s(TEXT("%s%lF%d"), record.name, (unsigned)_countof(record.name), &record.grade, &record.num);
					system("pause");
					WriteFile(hWriteHandle, &record, sizeof(record), &bytes, 0);
					ReadFile(hReadHandle, &block, sizeof(record), &bytes, 0);	// ответит когда выполнит
					system("pause");
					command.type = 4;
					WriteFile(hWriteHandle, &command, sizeof(command), &bytes, 0);
				}
				else 
					_tprintf_s(TEXT("Error\n"));
				break;
			case 2:
				record.num = 0;
				command.type = 2;
				_tprintf_s(TEXT("Please, enter number of student's record book\n"));
				_tscanf_s(TEXT("%d"), &command.num);
				getchar();
				WriteFile(hWriteHandle, &command, sizeof(command), &bytes, 0);
				ReadFile(hReadHandle, &record, sizeof(record), &bytes, 0);
				if (record.num)
					_tprintf_s(TEXT("Record:\n\tName: %s\n\tGrade: %.1lF\n\tNumber student's record book: %d\n"), record.name, record.grade, record.num);
				else
					_tprintf_s(TEXT("Error\n"));
				system("pause");
				command.type = 5;
				WriteFile(hWriteHandle, &command, sizeof(command), &bytes, 0);
				break;
			case 3:
				command.type = 3;
				WriteFile(hWriteHandle, &command, sizeof(command), &bytes, 0);
				exit = TRUE;
				break;
			default:
				_tprintf_s(TEXT("Please, enter valid number of operation\n"));
				break;
		}
	}

	return 0;
}