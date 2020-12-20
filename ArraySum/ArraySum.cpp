// ArraySum.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <Windows.h>
#include <omp.h>

using namespace std;

const int LENGTH = 100000;

void init_array(int arrayOfNumerics[])
{
	srand(GetTickCount64());
	for (int i = 0; i < LENGTH; i++)
		arrayOfNumerics[i] = rand() % 100;
}

void nonParallelCount(int arrayOfNumerics[])
{
	auto start = omp_get_wtime();
	int counter = 0;
	for (int i = 0; i < LENGTH; i++)
		if (arrayOfNumerics[i] % 3 == 0)
			counter += 1;

	auto end = omp_get_wtime();
	auto duration = end - start;
	printf("TIME: %.8f sec\n", duration);

	cout << "COUNT NONPARALLEL: " << counter << endl;
}


void parallelCount(int arrayOfNumerics[])
{
	unsigned int const block_size = 1000;
	unsigned int const need_threads = (LENGTH + block_size - 1) / block_size;

	unsigned int const hardware_threads = omp_get_max_threads();
	unsigned int const count_threads = min(hardware_threads != 0 ? hardware_threads : 2, need_threads);

	auto start = omp_get_wtime();
	int i, j, counter = 0;
	double duration = 0;
	omp_set_num_threads(count_threads);
#pragma omp parallel shared(arrayOfNumerics) private(i,j, counter) 
	{
#pragma omp for
		for (i = 0; i < 1; i++)
		{
			counter = 0;
			for (j = 0; j < LENGTH; j++)
			{
				if (arrayOfNumerics[j] % 3 == 0)
					counter++;
			}
			auto end = omp_get_wtime();
			duration = end - start;

			printf("TIME: %.8f sec\n", duration);

			cout << "COUNT DYNAMIC PARALLEL: " << counter << endl;
		}
	}
}


int main()
{
	int arrayOfNumerics[LENGTH];
	init_array(arrayOfNumerics);
	nonParallelCount(arrayOfNumerics);
	cout << "----------------------------------------------------";
	cout << endl;
	parallelCount(arrayOfNumerics);

	cin.ignore();
	return 0;
}