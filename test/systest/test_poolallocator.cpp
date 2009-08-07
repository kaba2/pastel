#include "pastelsystest.h"
#include "pastel/sys/poolallocator.h"

#include "pastel/sys/random.h"

#include <vector>

using namespace std;
using namespace Pastel;

namespace
{

	void testAllocate()
	{
		typedef std::vector<void*> MemoryList;
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
		typedef std::vector<void*> MemoryList;
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
		typedef std::vector<int*> MemoryList;
		MemoryList memoryList;

		PoolAllocator allocator(sizeof(int));
		const int Units = 100000;

		for (int i = 0;i < Units;++i)
		{
			memoryList.push_back((int*)allocator.allocate());
		}

		for (int i = 0;i < Units;++i)
		{
			int index = randomInteger() % Units;
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

	void testBegin()
	{
		testAllocate();
		testDeallocate();
		testRandomDeallocate();
	}

	void testAdd()
	{
		sysTestList().add("PoolAllocator", testBegin);
	}

	CallFunction run(testAdd);

}
