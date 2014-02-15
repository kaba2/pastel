#include <iostream>
#include <chrono>
#include <set>
#include <unordered_set>

#include <pastel/sys/cfasttrie.h>
#include <pastel/sys/redblacktree.h>
#include <pastel/sys/skiplist.h>
#include <pastel/sys/random.h>

using namespace Pastel;

template <typename Type>
double measureTime(const Type& f)
{
	namespace Chrono = std::chrono;
	using Clock = Chrono::high_resolution_clock;
	using Time = Chrono::time_point<Clock>;
	
	Time tic = Clock::now();
	f();
	Time toc = Clock::now();

	return Chrono::duration_cast<Chrono::milliseconds>(toc - tic).count();
}

template <int N>
void print(const CFastTrie_Set<N>& a)
{
	for (auto&& element : a)
	{
		std::cout << element.key().word(0) << " : "
			<< element.chain()->first.word(0)
			<< std::endl;
	}
}

template <typename Set>
void f(Set& a, integer n)
{
	for (integer i = 0; i < n; ++i)
	{
		//a.insert(randomInteger());
		a.insert(i);
		if (!checkInvariants(a))
		{
			std::cout << "Error: " << i << std::endl;
			print(a);
			return;
		}
	}
}

template <typename Set>
void g(const Set& a, integer n)
{
	for (integer i = 0; i < n; ++i)
	{
		a.lowerBound(randomInteger());
	}
}

int main()
{
	using Set = CFastTrie_Set<32>;
	//using Set = std::set<Unsigned_Integer<32>>;
	for (integer i = 1; i <= (1 << 18); i *= 2)
	{
		std::cout << i << " : ";

		Set a;
		f(a, i);
				
		double time = measureTime([&]()
		{
			g(a, i);
		});
		
		std::cout << time << "ms" << std::endl;
	}

	return 0;
}