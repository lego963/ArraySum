// ArraySum.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>
#include <mutex>
#include <Windows.h>

using namespace std;

const int LENGTH = 100000;
const int MAX_NTHREAD = 5;

mutex Mutex;

void init_array(int arr[])
{
	srand(GetTickCount64());
	for (int i = 0; i < LENGTH; i++)
		arr[i] = rand() % 100;
}

int count_nonparallel(int arr[])
{
	int s = 0;
	for (int i = 0; i < LENGTH; i++)
		if (i % 3 == 0)
			s += 1;
	return s;
}

void count_elems(int arr[], unsigned int start, unsigned int finish, int& gloabal_counter)
{
	int local_couner = 0;
	for (int i = start; i < finish; i++)
		if (i % 3 == 0)
			local_couner += 1; // результат передаётся по адресу
	gloabal_counter += local_couner;

	lock_guard<std::mutex> lock(Mutex);
	cout << this_thread::get_id() << ": " << gloabal_counter << endl;
}

int count_parallel(int arr[])
{
	thread t[MAX_NTHREAD]; // создание объектов потоков
	unsigned int n = LENGTH / MAX_NTHREAD; // вспомогательная переменная для определения границ потоков

	int global_sum = 0;

	// создание потоков, согласно границам
	for (int i = 0; i < MAX_NTHREAD; i++)
	{
		if (i == MAX_NTHREAD - 1)
			//конструкторы потоков
			t[i] = thread(count_elems, arr, n * i, LENGTH, ref(global_sum));
		else
			t[i] = thread(count_elems, arr, n * i, n * (i + 1), ref(global_sum));
	}
	//ожидание завершения работы потоков
	for (int i = 0; i < MAX_NTHREAD; i++)
		t[i].join();

	return global_sum; //возвращаем результат
}

int main()
{
	int a[LENGTH];
	init_array(a);
	cout << "count nonparallel = " << count_nonparallel(a) << endl;
	cout << "count parallel    = " << count_parallel(a) << endl;

	cin.ignore();
	return 0;
}