#if(0)
#include<iostream>
#include "rectangular_lsap.h"
#include<random>
#include<chrono>
int main()
{
	size_t cases_size[]{ 10000,10000,10000,10000 };

	unsigned seed = 123;
	std::mt19937 generator(seed);

	std::uniform_int_distribution<int> distribution(0, 20);

	for (auto size : cases_size)
	{
		const size_t size2 = size * size;
		constexpr size_t repeat = 10;
		auto costs = std::make_unique<int[]>(size2 * repeat);
		auto col4row = std::make_unique<int64_t[]>(size);
		for (size_t i = 0; i < size2 * repeat; ++i) 
		{
			costs[i] = distribution(generator);
		}

		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
		{
			for(int i = 0; i < repeat; ++i)
				solve_rectangular_linear_sum_assignment(size, size, costs.get() + size2 * i, col4row.get());
		}
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> time_span = duration_cast<std::chrono::duration<double>>(t2 - t1);
		std::cout << "size " << size << " runs by " << repeat << " times: " << time_span.count() << "s" << std::endl;
	}
}
#endif