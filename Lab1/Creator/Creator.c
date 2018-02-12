#include <windows.h>
#include <tchar.h>
#include <stdio.h>

typedef struct _student
{
	TCHAR name[10];
	int grade;
	double num;
} student;

int loadNodes(int cNodes, student* nodes)
{
	_tprintf_s(TEXT("Please, enter %d notes of students. Example:\nName (Andrey)\nGroup (13)\nAverage mark (7.5)\n"), cNodes);
	for (int i = 0; i < cNodes; i++)
	{
		_tprintf_s(TEXT("Plese, enter %d node:\n"), i + 1);
		_tscanf_s(TEXT("%s"), nodes[i].name, (unsigned)_countof(nodes[i].name));
		_tscanf_s(TEXT("%d"), &nodes[i].grade);
		_tscanf_s(TEXT("%lf"), &nodes[i].num);
	}
	return 0;
}

int writeBin(TCHAR* fileName, student* nodes, int cNodes)
{
	FILE* fStudBin;
	_tfopen_s(&fStudBin, fileName, TEXT("wb"));// create binary file to write
	for (int i = 0; i < cNodes; i++)
		fwrite(&nodes[i], sizeof(student), 1, fStudBin);
	fclose(fStudBin);
	return 0;
}

int _tmain(int argc, TCHAR* argv[])
{
	if (argc != 3)
	{
		_tprintf_s(TEXT("Invalid parameters.\n"));
		return 0;
	}
	int cNodes = _ttoi(argv[2]); // atoi for UNICODE
	student* nodes = (student*)malloc(sizeof(student) * cNodes);// struct array
	loadNodes(cNodes, nodes);
	writeBin(argv[1], nodes, cNodes);
	free(nodes);
	return 0;
}