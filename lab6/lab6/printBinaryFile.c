#include <windows.h>
#include <tchar.h>
#include <stdio.h>

typedef struct _student
{
	int num;
	TCHAR name[10];
	double grade;
} student;

int printBinFile(TCHAR* fBinName)
{
	student* records = (student*)malloc(sizeof(student) * 10);
	FILE* fStudBin;
	_tfopen_s(&fStudBin, fBinName, TEXT("rb"));
	int i = 0;
	while (fread(&records[i], sizeof(student), 1, fStudBin))
		_tprintf_s(TEXT("Record %d:\n\tName: %s\n\tGrade: %.1lF\n\tNumber student's record book: %d\n"), i + 1, records[i].name, records[i].grade, records[i].num);
	fclose(fStudBin);
	free(records);
	return 0;
}