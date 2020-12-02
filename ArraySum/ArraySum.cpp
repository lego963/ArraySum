// ArraySum.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>
#include <mutex>
#include <Windows.h>

using namespace std;

const int LENGTH = 10000;
const int MAX_NTHREAD = 4;

mutex Mutex;

void init_array(int arr[])
{
	srand(GetTickCount64());
	for (int i = 0; i < LENGTH; i++)
		arr[i] = rand() % 100;
}

int count_nonparallel(int arr[])
{
	auto start = chrono::high_resolution_clock::now();
	int s = 0;
	for (int i = 0; i < LENGTH; i++)
		if (i % 3 == 0)
			s += 1;

	auto end = chrono::high_resolution_clock::now();
	chrono::duration<float> duration = end - start;
	printf("Time: %.7f sec\n", duration.count());
	return s;
}

void count_elems(int arr[], unsigned int start, unsigned int finish, int& gloabal_counter)
{
	int local_couner = 0;
	for (int i = start; i < finish; i++)
		if (i % 3 == 0)
			local_couner += 1;

	lock_guard<std::mutex> lock(Mutex);
	gloabal_counter += local_couner;
	cout << this_thread::get_id() << ": " << gloabal_counter << endl;
}

int count_parallel(int arr[])
{
	unsigned int const min_block_size = 25;
	unsigned int const max_threads = (LENGTH + min_block_size - 1) / min_block_size;

	unsigned int const hardware_threads = thread::hardware_concurrency();
	unsigned int const count_threads = min(hardware_threads != 0 ? hardware_threads : 2, max_threads);

	unsigned int block_size = LENGTH / count_threads; // вспомогательная переменная для определения границ потоков
	auto start = chrono::high_resolution_clock::now();
	int global_sum = 0;
	vector<thread> threads(count_threads - 1);

	// создание потоков, согласно границам
	for (int i = 0; i < count_threads - 1; i++)
	{
		//конструкторы потоков
		threads[i] = thread(count_elems, arr, block_size * i, block_size * (i + 1), ref(global_sum));
	}

	// обработка последнего участка
	count_elems(arr, block_size * (count_threads - 1), LENGTH, global_sum);

	//ожидание завершения работы потоков
	for (int i = 0; i < count_threads - 1; i++)
		threads[i].join();

	auto end = chrono::high_resolution_clock::now();
	chrono::duration<float> duration = end - start;
	cout << "Time: " << duration.count() << " sec\n";

	return global_sum; //возвращаем результат
}

//double countTwoThreads(int arr[])
//{
//	unsigned int blockSize = LENGTH / 2;
//
//	cout << endl << "Two Threads" << endl;
//
//	int global_sum = 0;
//
//	auto start = chrono::high_resolution_clock::now();
//
//	thread addThread = thread(count_elems, arr, 0, blockSize, ref(global_sum));
//
//	count_elems(arr, blockSize, LENGTH, global_sum);
//
//	addThread.join();
//
//	auto end = chrono::high_resolution_clock::now();
//	chrono::duration<float> duration = end - start;
//	cout << "Time: " << duration.count() << " sec" << endl;
//
//	return global_sum;
//}

int main()
{
	int a[LENGTH];
	init_array(a);
	cout << "Count NON PP: " << count_nonparallel(a) << endl;
	cout << endl;
	cout << "Count DYN PP: " << count_parallel(a) << endl;

	cin.ignore();
	return 0;
}