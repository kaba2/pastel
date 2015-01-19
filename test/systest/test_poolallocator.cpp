// Description: Testing for PoolAllocator
// DocumentationOf: pool_allocator.h

#include "test_pastelsys.h"
#include "pastel/sys/allocator/pool_allocator.h"

#include "pastel/sys/random.h"

#include <vector>

using namespace Pastel;

namespace
{

	void testAllocate()
	{
		using MemoryList = std::vector<void*>;
		MemoryList memoryList;

		PoolAllocator allocator(15);

		void* memory = allocator.allocate();
		memoryList.push_back(memory);
		unused(memory);

		for (int i = 0;i < 2054;++i)
		{
			memory = allocator.allocate();
			memoryList.push_back(memory);
		}

		for (int i = 0;i < memoryList.size();++i)
		{
			allocator.deallocate(memoryList[i]);
		}
	}

	void testDeallocate()
	{
		using MemoryList = std::vector<void*>;
		MemoryList memoryList;

		PoolAllocator allocator(15);

		void* memory = allocator.allocate();
		memoryList.push_back(memory);

		for (integer i = 0;i < memoryList.size();++i)
		{
			allocator.deallocate(memoryList[i]);
		}
		memoryList.clear();

		for (int i = 0;i < 5;++i)
		{
			void* memory = allocator.allocate();
			memoryList.push_back(memory);
		}

		for (integer i = 0;i < memoryList.size();++i)
		{
			allocator.deallocate(memoryList[i]);
		}
		memoryList.clear();

		for (int i = 0;i < 231;++i)
		{
			void* memory = allocator.allocate();
			memoryList.push_back(memory);
		}

		for (integer i = 0;i < memoryList.size();++i)
		{
			allocator.deallocate(memoryList[i]);
		}
		memoryList.clear();

		for (int i = 0;i < 2054;++i)
		{
			void* memory = allocator.allocate();
			memoryList.push_back(memory);
		}

		for (integer i = 0;i < memoryList.size();++i)
		{
			allocator.deallocate(memoryList[i]);
		}
		memoryList.clear();
	}

	void testRandomDeallocate()
	{
		using MemoryList = std::vector<int*>;
		MemoryList memoryList;

		PoolAllocator allocator(sizeof(int));
		int Units = 100000;

		for (int i = 0;i < Units;++i)
		{

			memoryList.push_back((int*)allocator.allocate());
		}

		for (int i = 0;i < Units;++i)
		{
			int index = randomInteger(Units);
			if (memoryList[index] != 0)
			{
				allocator.deallocate(memoryList[index]);
				memoryList[index] = 0;
			}
		}

		for (int i = 0;i < Units;++i)
		{
			int index = i;
			if (memoryList[index] != 0)
			{
				allocator.deallocate(memoryList[index]);
				memoryList[index] = 0;
			}
		}

	}

	void test()
	{
		testAllocate();
		testDeallocate();
		testRandomDeallocate();
	}

	void addTest()
	{
		testRunner().add("PoolAllocator", test);
	}

	CallFunction run(addTest);

}
