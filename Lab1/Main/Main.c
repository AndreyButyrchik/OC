#include <windows.h>
#include <tchar.h>
#include <stdio.h>

typedef struct _student
{
	TCHAR name[10];
	int grade;
	double num;
}student;

int printBinFile(TCHAR* fBinName)
{
	student* nodes = (student*)malloc(sizeof(student) * 10);
	FILE* fStudBin;
	_tfopen_s(&fStudBin, fBinName, TEXT("rb"));
	int i = 0;
	while (fread(&nodes[i], sizeof(student), 1, fStudBin))
	{ 
		_tprintf_s(TEXT("Node %d:\n\tName: %s\n\tGrade: %d\n\tAverage mark: %.1lF\n"), (i++), nodes[i].name, nodes[i].grade, nodes[i].num);
	}
	fclose(fStudBin);
	free(nodes);
	return 0;
}

int printRepFile(TCHAR* fRepName)
{
	FILE* fReport;
	TCHAR buff[500];
	_tfopen_s(&fReport, fRepName, TEXT("r"));
	while (_fgetts(buff, 2, fReport))
	{
		_tprintf_s(TEXT("%s"), buff);
	}
	fclose(fReport);
	return 0;
}

int _tmain(int argc, TCHAR* argv[])
{
	TCHAR fBinName[10];
	TCHAR cNodes[3];
	_tprintf_s(TEXT("Please, enter name of binary file.\n"));
	_tscanf_s(TEXT("%s"), fBinName, (unsigned)_countof(fBinName));
	_tprintf_s(TEXT("Please, enter count of nodes.\n"));
	_tscanf_s(TEXT("%s"), cNodes, (unsigned)_countof(cNodes));

	TCHAR conArgCreator[30];
	_tcscpy_s(conArgCreator, _countof(conArgCreator), TEXT("Creator "));
	_tcscat_s(conArgCreator, _countof(conArgCreator), fBinName); // "filename"+.txt
	_tcscat_s(conArgCreator, _countof(conArgCreator), TEXT(" "));
	_tcscat_s(conArgCreator, _countof(conArgCreator), cNodes);

	STARTUPINFO siBin;
	PROCESS_INFORMATION piBin;
	ZeroMemory(&siBin, sizeof(STARTUPINFO));
	siBin.cb = sizeof(STARTUPINFO);
	CreateProcessW(NULL, conArgCreator, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &siBin, &piBin);
	WaitForSingleObject(piBin.hProcess, INFINITE);
	CloseHandle(piBin.hProcess);
	CloseHandle(piBin.hThread);

	_tprintf_s(TEXT("\tBinary file:\n"));
	printBinFile(fBinName);


	TCHAR fRepName[20];
	TCHAR _fRepName[20];
	TCHAR averMark[5];
	_tprintf_s(TEXT("Please, enter name of report file.\n"));
	_tscanf_s(TEXT("%s"), fRepName, (unsigned)_countof(fRepName));
	_tcscpy_s(_fRepName, _countof(_fRepName),fRepName);
	_tprintf_s(TEXT("Please, enter average mark.\n"));
	_tscanf_s(TEXT("%s"), averMark, (unsigned)_countof(averMark));
	TCHAR conArgReport[50];
	_tcscpy_s(conArgReport, _countof(conArgReport), TEXT("Reporter "));
	_tcscat_s(conArgReport, _countof(conArgReport), fBinName);
	_tcscat_s(conArgReport, _countof(conArgReport), TEXT(" "));
	_tcscat_s(conArgReport, _countof(conArgReport), fRepName);
	_tcscat_s(conArgReport, _countof(conArgReport), TEXT(" "));
	_tcscat_s(conArgReport, _countof(conArgReport), averMark);

	STARTUPINFO siRep;
	PROCESS_INFORMATION piRep;
	ZeroMemory(&siRep, sizeof(STARTUPINFO));
	siRep.cb = sizeof(STARTUPINFO);
	CreateProcessW(NULL, conArgReport, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &siRep, &piRep);
	WaitForSingleObject(piRep.hProcess, INFINITE);
	CloseHandle(piRep.hProcess);
	CloseHandle(piRep.hThread);

	_tcscat_s(fRepName, _countof(fRepName), TEXT(".txt"));
	printRepFile(fRepName);

	system("pause");
	return 0;
}