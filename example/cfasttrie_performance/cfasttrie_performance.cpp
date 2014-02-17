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

	double time = Chrono::duration_cast<Chrono::milliseconds>(toc - tic).count();
	std::cout << time << "ms ";
	return time;
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
		a.insert(randomInteger());
		//a.insert(i);
		/*
		if (!checkInvariants(a))
		{
			std::cout << "Error: " << i << std::endl;
			print(a);
			return;
		}
		*/
	}

	std::cout << (real)a.finds / n << " average queries per element." << std::endl;
}

template <typename Set>
void g(const Set& a, integer n)
{
	for (integer i = 0; i < n; ++i)
	{
		a.lower_bound(randomInteger());
	}
}

enum{ Bits = 64 };

template <typename Set>
void test()
{
	for (integer i = 1; i <= (1 << 18); i *= 2)
	{
		std::cout << i << " : ";

		Set a;
		measureTime([&]()
		{
			f(a, i);
		});

		measureTime([&]()
		{
			g(a, i);
		});

		std::cout << std::endl;

	}
}

int main()
{
	std::cout << "CFastTrie_Set" << std::endl;
	test<CFastTrie_Set<Bits>>();

	/*
	std::cout << "std::set" << std::endl;
	test<std::set<Unsigned_Integer<Bits>>>();

	std::cout << "std::set2" << std::endl;
	test<std::set<integer>>();

	std::cout << "SkipList_Set" << std::endl;
	test<SkipList_Set<Unsigned_Integer<Bits>>>();

	std::cout << "RedBlack_Set" << std::endl;
	test<RedBlack_Set<Unsigned_Integer<Bits>>>();
	*/

	return 0;
}