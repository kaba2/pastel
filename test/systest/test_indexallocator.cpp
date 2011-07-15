// Description: Testing for IndexAllocator
// DocumentationOf: indexallocator.h

#include "pastelsystest.h"

#include "pastel/sys/indexallocator.h"

using namespace Pastel;

namespace
{

	void testReserve()
	{
		IndexAllocator a;
		a.reserve(100);
	}

	void testSingle()
	{
		IndexAllocator a;

		a.reserve(100);
		a.allocate();
	}

	void testMany()
	{
		IndexAllocator a;

		a.reserve(100);
		for (integer i = 0;i < 90;++i)
		{
			a.allocate();
		}
	}

	void testAll()
	{
		IndexAllocator a;

		a.reserve(100);
		for (integer i = 0;i < 100;++i)
		{
			a.allocate();
		}
	}

	void testTooMany()
	{
		IndexAllocator a;

		a.reserve(100);
		for (integer i = 0;i < 101;++i)
		{
			a.allocate();
		}
	}

	void testDeallocate()
	{
		IndexAllocator a;

		a.reserve(100);
		for (integer i = 0;i < 100;++i)
		{
			a.allocate();
		}
		a.deallocate(10);
		a.deallocate(20);
		a.deallocate(0);
		a.deallocate(99);
		a.deallocate(11);
		a.deallocate(9);
		a.deallocate(13);
		a.deallocate(12);
		a.reserve(200);
	}

	void test()
	{
		testReserve();
		testSingle();
		testMany();
		testAll();
		testTooMany();
		testDeallocate();
	}

	void addTest()
	{
		testRunner().add("IndexAllocator", test);
	}

	CallFunction run(addTest);

}
