// Description: Testing for IndexAllocator
// DocumentationOf: indexallocator.h

#include "test/test_init.h"

#include "pastel/sys/indexallocator.h"

TEST_CASE("Reserve (IndexAllocator)")
{
	IndexAllocator a;
	a.reserve(100);
}

TEST_CASE("Single (IndexAllocator)")
{
	IndexAllocator a;

	a.reserve(100);
	a.allocate();
}

TEST_CASE("Many (IndexAllocator)")
{
	IndexAllocator a;

	a.reserve(100);
	for (integer i = 0;i < 90;++i)
	{
		a.allocate();
	}
}

TEST_CASE("All (IndexAllocator)")
{
	IndexAllocator a;

	a.reserve(100);
	for (integer i = 0;i < 100;++i)
	{
		a.allocate();
	}
}

TEST_CASE("TooMany (IndexAllocator)")
{
	IndexAllocator a;

	a.reserve(100);
	for (integer i = 0;i < 101;++i)
	{
		a.allocate();
	}
}

TEST_CASE("Deallocate (IndexAllocator)")
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

