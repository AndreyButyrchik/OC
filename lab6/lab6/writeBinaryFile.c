#include <windows.h>
#include <tchar.h>
#include <stdio.h>

typedef struct _student
{
	int num;
	TCHAR name[10];
	double grade;
} student;

int writeBin(TCHAR* fileName, student* records, int cntRecords)
{
	FILE* fStudBin;
	_tfopen_s(&fStudBin, fileName, TEXT("wb"));
	for (int i = 0; i < cntRecords; i++)
		fwrite(&records[i], sizeof(student), 1, fStudBin);
	fclose(fStudBin);
	return 0;
}