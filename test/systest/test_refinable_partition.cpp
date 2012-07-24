// Description: Testing for refinable partition
// DocumentationOf: refinable_partition.h

#include "pastelsystest.h"

#include <pastel/sys/refinable_partition.h>

using namespace Pastel;
using namespace std;

namespace
{

	class Test
		: public TestSuite
	{
	public:
		Test()
			: TestSuite(&testReport())
		{
		}

		virtual void run()
		{
			test();
		}

		typedef RefinablePartition<integer>
			Partition;

		template <int N, typename Type>
		bool same(Type (&data)[N], 
			Partition::Block_ConstIterator block) const
		{
			if (N != block->elements())
			{
				return false;
			}

			std::vector<Type> aSet(
				std::begin(data), std::end(data));
			std::sort(aSet.begin(), aSet.end());

			auto iter = block->begin();			
			auto end = block->end();

			std::vector<Type> bSet;
			while(iter != end)
			{
				bSet.push_back((*iter)->data());
				++iter;
			}
			std::sort(bSet.begin(), bSet.end());

			return std::equal(aSet.begin(),	aSet.end(),
				bSet.begin());
		}

		void test()
		{
			integer data[] = {0, 1, 2, 3, 4};

			Partition partition(
				std::begin(data), std::end(data));
			{
				TEST_ENSURE_OP(partition.splits(), ==, 0);
				TEST_ENSURE_OP(partition.blocks(), ==, 1);
				TEST_ENSURE_OP(partition.elements(), ==, 5);
				TEST_ENSURE(same(data, partition.blockBegin()));
			}

			partition.mark(
				partition.elementBegin() + 0);
			{
				TEST_ENSURE_OP(partition.splits(), ==, 1);
			}

			partition.mark(
				partition.elementBegin() + 2);
			{
				TEST_ENSURE_OP(partition.splits(), ==, 1);
			}

			partition.mark(
				partition.elementBegin() + 4);
			{
				TEST_ENSURE_OP(
					partition.blockBegin()->marked(), ==, 3);
				TEST_ENSURE_OP(
					partition.blockBegin()->unmarked(), ==, 2);
			}

			partition.split();
			{
				TEST_ENSURE_OP(
					partition.splits(), ==, 0);
				TEST_ENSURE_OP(
					partition.blocks(), ==, 2);
				TEST_ENSURE_OP(
					partition.elements(), ==, 5);

				// In the splitting, the smaller part
				// should get the new block.
				int data[] = {0, 2, 4};
				TEST_ENSURE(
					same(data, partition.blockBegin()));
			}

			partition.swap(partition);

			partition.clear();
			TEST_ENSURE_OP(partition.splits(), ==, 0);
			TEST_ENSURE_OP(partition.blocks(), ==, 0);
			TEST_ENSURE_OP(partition.elements(), ==, 0);
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("RefinablePartition", test);
	}

	CallFunction run(addTest);

}
