#include <windows.h>
#include <tchar.h>
#include <stdio.h>

typedef struct _student
{
	TCHAR name[10];
	int grade;
	double num;
}student;

int getBinNodes(TCHAR* fileName, student* nodes)
{
	FILE* fStudBin;
	_tfopen_s(&fStudBin, fileName, TEXT("rb"));
	int i = 0;
	while(fread(&nodes[i], sizeof(student), 1, fStudBin)) { i++; }
	fclose(fStudBin);
	return i; // count of nodes
}

int writeNodes(student* nodes, TCHAR* fileName, TCHAR* fBinName, double num, int cNodes)
{
	FILE* fReport;
	_tfopen_s(&fReport, fileName, TEXT("w,ccs=UNICODE"));//  open file with UNICODE
	_ftprintf_s(fReport, TEXT("\tReport on the file \"%s\"\n\tList of students with an average mark higher \"%.1lf\"\n"), fBinName, num);
	for (int i = 0; i < cNodes; i++)
	{
		if (nodes[i].num > num)
		{
			_ftprintf_s(fReport, TEXT("Node ¹%d:\n\tName: %s\n\tGrade: %d\n\tAverage mark: %.1lF\n"), (i + 1), nodes[i].name, nodes[i].grade, nodes[i].num);
		}
	}
	fclose(fReport);
	return 0;
}

int _tmain(int argc, TCHAR* argv[])
{
	student* nodes = (student*)malloc(sizeof(student)*10);// struct array !FIX
	TCHAR* fileName = (TCHAR*)malloc(sizeof(TCHAR)*(_tcslen(argv[2]) + 5));
	_tcscpy_s(fileName, sizeof(TCHAR)*(_tcslen(argv[2])), argv[2]); // "filename"+.txt
	_tcscat_s(fileName, sizeof(TCHAR)*(_tcslen(argv[2])), TEXT(".txt"));
	int cNodes = getBinNodes(argv[1], nodes);
	writeNodes(nodes, fileName, argv[1], _ttof(argv[3]), cNodes);
	free(nodes);
	return 0;
}