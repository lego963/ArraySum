// ArraySum.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>
#include <mutex>
#include <Windows.h>

using namespace std;

const int COUNT = 1000;
const int MAX_NTHREAD = 4;

mutex Mutex;

void init_array(int arr[])
{
	srand(GetTickCount64());
	for (int i = 0; i < COUNT; i++)
		arr[i] = rand() % 100;
}

int sum_nonparallel(int arr[])
{
	int s = 0;
	for (int i = 0; i < COUNT; i++)
		if (i % 3 == 0)
			s += arr[i];
	return s;
}

void sum(int arr[], int start, int finish, int& global_sum)
{
	for (int i = start; i < finish; i++)
		if (i % 3 == 0)
			global_sum += arr[i]; // результат передаётся по адресу
}

int sum_parallel(int arr[])
{
	thread t[MAX_NTHREAD]; // создание объектов потоков
	int n = COUNT / MAX_NTHREAD; // вспомогательная переменная для определения границ потоков

	int global_sum = 0;

	// создание потоков, согласно границам
	for (int i = 0; i < MAX_NTHREAD; i++)
	{
		if (i == MAX_NTHREAD - 1)
			//конструкторы потоков
			t[i] = thread(sum, arr, n * i, COUNT, ref(global_sum));
		else
			t[i] = thread(sum, arr, n * i, n * (i + 1), ref(global_sum));
	}
	//ожидание завершения работы потоков
	for (int i = 0; i < MAX_NTHREAD; i++)
		t[i].join();

	return global_sum; //возвращаем результат
}

int main()
{
	int a[COUNT];
	init_array(a);
	cout << "sum nonparallel = " << sum_nonparallel(a) << endl;
	cout << "sum parallel = " << sum_parallel(a) << endl;

	cin.ignore();
	return 0;
}