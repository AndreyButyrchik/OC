#include <windows.h>
#include <tchar.h>
#include <stdio.h>

typedef struct _student
{
	int num;
	TCHAR name[10];
	double grade;
} student;

BOOL setRecord(TCHAR* fBinName, student record, int num)
{
	student* records = (student*)malloc(sizeof(student) * 10);
	FILE* fStudBin;
	_tfopen_s(&fStudBin, fBinName, TEXT("rb"));
	int i = 0;
	while (fread(&records[i], sizeof(student), 1, fStudBin))
	{
		if (records[i].num == num)
			records[i] = record;
		i++;
	}
	fclose(fStudBin);

	_tfopen_s(&fStudBin, fBinName, TEXT("wb"));
	for (int j = 0; j < i; j++)
		fwrite(&records[j], sizeof(student), 1, fStudBin);
	fclose(fStudBin);
	free(records);
	return TRUE;
}