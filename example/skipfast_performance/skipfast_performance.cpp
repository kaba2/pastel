#include <iostream>
#include <chrono>
#include <set>
#include <unordered_set>

#include <boost/unordered_set.hpp>

#include <pastel/sys/skipfast.h>
#include <pastel/sys/redblacktree.h>
#include <pastel/sys/skiplist.h>
#include <pastel/sys/random.h>
#include <pastel/sys/logging.h>

#include <Windows.h>

using namespace Pastel;

struct HighResClock
{
	typedef long long                               rep;
	typedef std::nano                               period;
	typedef std::chrono::duration<rep, period>      duration;
	typedef std::chrono::time_point<HighResClock>   time_point;
	static PASTEL_CONSTEXPR bool is_steady = true;

	static time_point now();
};

namespace
{
	const long long g_Frequency = []() -> long long
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		return frequency.QuadPart;
	}();
}

HighResClock::time_point HighResClock::now()
{
	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);
	return time_point(duration(count.QuadPart * static_cast<rep>(period::den) / g_Frequency));
}

static PASTEL_CONSTEXPR int Bits = 6;

template <typename Type>
double measureTime(const Type& f)
{
	namespace Chrono = std::chrono;
	using Clock = HighResClock;
	using Time = Chrono::time_point<Clock>;
	
	Time tic = Clock::now();
	f();
	Time toc = Clock::now();

	double time = Chrono::duration_cast<Chrono::microseconds>(toc - tic).count();
	std::cout << time << "us ";
	return time;
}

template <int N>
void print(const SkipFast_Set<N>& a)
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
		uinteger key = randomUintegerBits(Bits);
		//std::cout << key << " ";
		auto result = a.insert(key);
		/*
		if (result.second)
		{
			std::cout << key << " ";
		}
		*/
		//a.insert(n - i);
		if (!testInvariants(a))
		{
			testInvariants(a);
			std::cout << "Error: " << i << std::endl;
			std::cout << "Key: " << key << std::endl;
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
	integer j = 0;
	for (integer i = 0; i < (1 << 12); ++i)
	{
#ifdef SKIP_ONLY
		integer prevFinds = a.finds;
#endif
		uinteger key = randomUinteger();
		if (a.find(key) != a.end())
		{
			++j;
		}
		//a.lower_bound(key);
		//a.lowestAncestor(key);
#ifdef SKIP_ONLY
		integer delta = a.finds - prevFinds;
		if (delta > maxFinds)
		{
			maxFinds = delta;
		}
#endif
	}

	//std::cout << "j = " << j << std::endl;

#ifdef SKIP_ONLY
	std::cout << (real)a.finds / n << " "
		<< ", max " << maxFinds << " ";
#endif
}

template <typename Set>
void test()
{
	for (integer i = (1 << 1); i <= (1 << 16); i *= 2)
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

class Identity_Hasher
{
public:
	template <typename Type>
	hash_integer operator()(const Type& that) const
	{
		return that;
	}
};

int main()
{
	Stream_Logger streamLogger(&std::cout);
	log().addLogger(&streamLogger);

	std::cout << "SkipFast_Set" << std::endl;
	test<SkipFast_Set<Bits>>();

#ifndef SKIP_ONLY
	std::cout << "SkipList_Set" << std::endl;
	test<SkipList_Set<Unsigned_Integer<Bits>>>();

	std::cout << "RedBlackTree_Set" << std::endl;
	test<RedBlackTree_Set<Unsigned_Integer<Bits>>>();

	std::cout << "std::set" << std::endl;
	test<std::set<Unsigned_Integer<Bits>>>();

	std::cout << "std::unordered_set" << std::endl;
	test<std::unordered_set<uinteger>>();

#endif

	return 0;
}