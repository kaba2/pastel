// Description: Testing for ArenaAllocator
// DocumentationOf: arena_allocator.h

#include "test/test_init.h"
#include "pastel/sys/allocator/arena_allocator.h"

#include <vector>

TEST_CASE("Allocate (ArenaAllocator)")
{
	ArenaAllocator allocator;

	void* memory = allocator.allocate(1);
	unused(memory);

	allocator.clear();

	for (int i = 0;i < 5;++i)
	{
		allocator.allocate(1);
	}

	allocator.clear();

	for (int i = 0;i < 231;++i)
	{
		allocator.allocate(1);
	}

	allocator.clear();

	for (int i = 0;i < 2054;++i)
	{
		allocator.allocate(1);
	}

	allocator.clear();
}

TEST_CASE("Deallocate (ArenaAllocator)")
{
	ArenaAllocator allocator;
	std::vector<void*> memoryList;

	{
		void* memory = allocator.allocate(1);
		memoryList.push_back(memory);
	}

	allocator.clear();
	memoryList.clear();

	for (int i = 0;i < 5;++i)
	{
		void* memory = allocator.allocate(1);
		memoryList.push_back(memory);
	}

	allocator.clear();
	memoryList.clear();

	for (int i = 0;i < 231;++i)
	{
		void* memory = allocator.allocate(1);
		memoryList.push_back(memory);
	}

	allocator.clear();
	memoryList.clear();

	for (int i = 0;i < 2054;++i)
	{
		void* memory = allocator.allocate(1);
		memoryList.push_back(memory);
	}

	allocator.clear();
	memoryList.clear();
}

TEST_CASE("Large (ArenaAllocator)")
{
	std::vector<int*> memoryList;
	ArenaAllocator allocator;
	int Units = 100000;

	for (int i = 0;i < Units;++i)
	{

		memoryList.push_back((int*)allocator.allocate(sizeof(int)));
	}

	allocator.clear();
}
