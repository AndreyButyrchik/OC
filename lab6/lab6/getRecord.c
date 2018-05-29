#include <windows.h>
#include <tchar.h>
#include <stdio.h>

typedef struct _student
{
	int num;
	TCHAR name[10];
	double grade;
} student;

student getRecord(TCHAR* fBinName, int num)
{
	CRITICAL_SECTION csBinaryFile;
	InitializeCriticalSection(&csBinaryFile);
	EnterCriticalSection(&csBinaryFile);
	student* records = (student*)malloc(sizeof(student) * 10);
	FILE* fStudBin;
	_tfopen_s(&fStudBin, fBinName, TEXT("rb"));
	int i = 0;
	while (fread(&records[i], sizeof(student), 1, fStudBin))
	{
		if (records[i].num == num)
		{
			fclose(fStudBin);
			return records[i];
		}
	}

	fclose(fStudBin);
	free(records);
	LeaveCriticalSection(&csBinaryFile);
	return;
}