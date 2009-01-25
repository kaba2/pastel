#include "pastelsystest.h"
#include "pastel/sys/arenaallocator.h"

#include <vector>

using namespace std;
using namespace Pastel;

namespace
{

	void testAllocate()
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

	void testDeAllocate()
	{
		ArenaAllocator allocator;
		std::vector<void*> memoryList;

		void* memory = allocator.allocate(1);
		memoryList.push_back(memory);

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

	void testLarge()
	{
		std::vector<int*> memoryList;
		ArenaAllocator allocator;
		const int Units = 100000;

		for (int i = 0;i < Units;++i)
		{
			memoryList.push_back((int*)allocator.allocate(sizeof(int)));
		}

		allocator.clear();
	}

	void testBegin()
	{
		testAllocate();
		testDeAllocate();
		testLarge();
	}

	void addTest()
	{
		sysTestList().add(
			"ArenaAllocator", testBegin);
	}

	CallFunction run(addTest);

}
