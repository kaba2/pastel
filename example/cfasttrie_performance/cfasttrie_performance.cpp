#include <iostream>
#include <chrono>
#include <set>
#include <unordered_set>

#include <pastel/sys/cfasttrie.h>
#include <pastel/sys/redblacktree.h>
#include <pastel/sys/skiplist.h>
#include <pastel/sys/random.h>
#include <pastel/sys/logging.h>

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
	for (auto&& element : a.ckeyRange())
	{
		std::cout << element.word(0) << std::endl;
	}
}

#define SKIP_ONLY

template <typename Set>
void f(Set& a, integer n)
{
#ifdef SKIP_ONLY
	a.finds = 0;
#endif
	for (integer i = 0; i < n; ++i)
	{
		a.insert(randomUinteger());
		//a.insert(n - i);
		if (!a.testInvariants())
		{
			std::cout << "Error: " << i << std::endl;
			//print(a);
			return;
		}
	}

#ifdef SKIP_ONLY
	std::cout << (real)a.finds / n << " ";
#endif
}

template <typename Set>
void g(const Set& a, integer n)
{
#ifdef SKIP_ONLY
	a.finds = 0;
	integer maxFinds = 0;
#endif
	for (integer i = 0; i < n; ++i)
	{
#ifdef SKIP_ONLY
		integer prevFinds = a.finds;
#endif
		//a.find(randomUinteger());
		a.lower_bound(randomUinteger());
		//a.rightGapBound(randomUinteger());
#ifdef SKIP_ONLY
		integer delta = a.finds - prevFinds;
		if (delta > maxFinds)
		{
			maxFinds = delta;
		}
#endif
	}

#ifdef SKIP_ONLY
	std::cout << (real)a.finds / n << " "
		<< ", max " << maxFinds << " ";
#endif
}

enum{ Bits = 64 };

template <typename Set>
void test()
{
	for (integer i = (1 << 14); i <= (1 << 14); i *= 2)
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
	Stream_Logger streamLogger(&std::cout);
	log().addLogger(&streamLogger);

	std::cout << "CFastTrie_Set" << std::endl;
	test<CFastTrie_Set<Bits>>();

#ifndef SKIP_ONLY
	std::cout << "SkipList_Set" << std::endl;
	test<SkipList_Set<Unsigned_Integer<Bits>>>();

	std::cout << "std::set" << std::endl;
	test<std::set<Unsigned_Integer<Bits>>>();

	std::cout << "std::unordered_set" << std::endl;
	test<std::unordered_set<Unsigned_Integer<Bits>>>();

	std::cout << "RedBlack_Set" << std::endl;
	test<RedBlack_Set<Unsigned_Integer<Bits>>>();
#endif

	return 0;
}