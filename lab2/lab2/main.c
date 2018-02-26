#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <time.h>

struct _mulData
{
	int** matrix;
	int* vector;
	int* resultVector;
	int dim;
} mulData;

DWORD WINAPI mulMatrixVector(LPVOID lpParam)
{
	int tmp = 0;
	for (int i = 0; i < mulData.dim; i++)
		tmp += mulData.matrix[(int)lpParam][i] * mulData.vector[i];
	Sleep(7);
	mulData.resultVector[(int)lpParam] = tmp;
	return 0;
}

int _tmain()
{
	srand((unsigned int)time(NULL));

	int dim;
	_tprintf_s(TEXT("Please, enter matrix dimension.\n"));
	_tscanf_s(TEXT("%d"), &dim);

	int** matrix = (int**)malloc(dim * sizeof(int*));
	for(int i = 0; i < dim; i++)
		matrix[i] = (int*)malloc(dim * sizeof(int));
	_tprintf_s(TEXT("Please, enter matrix.\n"));
	for (int i = 0; i < dim; i++)
		for(int j = 0; j < dim; j++)
			_tscanf_s(TEXT("%d"), &matrix[i][j]);

	int* vector = (int*)malloc(dim * (sizeof(int)));
	_tprintf_s(TEXT("Please, enter vector.\n"));
	for (int i = 0; i < dim; i++)
		_tscanf_s(TEXT("%d"), &vector[i]);

	_tprintf_s(TEXT("Matrix:\n"));
	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < dim; j++)
			_tprintf_s(TEXT("%d\t"), matrix[i][j]);
		_tprintf_s(TEXT("\n"));
	}

	_tprintf_s(TEXT("Vector:\n"));
	for(int i = 0; i < dim; i++)
		_tprintf_s(TEXT("%d\t\n"), vector[i]);

	int* resultVector = (int*)malloc(dim * (sizeof(int)));
	mulData.dim = dim;
	mulData.matrix = matrix;
	mulData.vector = vector;
	mulData.resultVector = resultVector;
	DWORD* massThreadId = (DWORD*)malloc(dim * sizeof(DWORD));
	HANDLE* massHThread = (HANDLE*)malloc(dim * sizeof(HANDLE));
	for (int i = 0; i < dim; i++)
		massHThread[i] = CreateThread(NULL, 0, mulMatrixVector, (LPVOID)i, 0, &massThreadId[i]);
	WaitForMultipleObjects(dim, massHThread, TRUE, INFINITE);
	for (int i = 0; i < dim; i++)
		CloseHandle(massHThread[i]);

	_tprintf_s(TEXT("Result vector:\n"));
	for (int i = 0; i < dim; i++)
		_tprintf_s(TEXT("%d\n"), mulData.resultVector[i]);

	system("pause");
	return 0;
}