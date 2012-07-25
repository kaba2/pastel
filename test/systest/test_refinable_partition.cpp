// Description: Testing for refinable partition
// DocumentationOf: refinable_partition.h

#include "pastelsystest.h"

#include <pastel/sys/refinable_partition.h>

#include <algorithm>

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
			testRemove();
		}

		typedef RefinablePartition<integer, integer>
			Partition;
		typedef Partition::Set_ConstIterator 
			Set_ConstIterator;
		typedef Partition::Element_ConstIterator 
			Element_ConstIterator;

		template <int N, typename Type>
		bool same(Type (&data)[N], 
			Partition::Set_ConstIterator set) const
		{
			if (N != set->elements())
			{
				return false;
			}

			std::vector<Type> aSet(
				std::begin(data), std::end(data));
			std::sort(aSet.begin(), aSet.end());

			auto iter = set->begin();			
			auto end = set->end();

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
				std::begin(data), std::end(data), 0);
			{
				TEST_ENSURE_OP(partition.splits(), ==, 0);
				TEST_ENSURE_OP(partition.sets(), ==, 1);
				TEST_ENSURE_OP(partition.elements(), ==, 5);
				TEST_ENSURE(same(data, partition.setBegin()));
				TEST_ENSURE_OP(partition.setBegin()->data(), ==, 0);
			}

			partition.mark(
				partition.elementBegin());
			{
				TEST_ENSURE_OP(partition.splits(), ==, 1);
			}

			partition.mark(
				std::next(partition.elementBegin(), 2));
			{
				TEST_ENSURE_OP(partition.splits(), ==, 1);
			}

			partition.mark(
				std::next(partition.elementBegin(), 4));
			{
				TEST_ENSURE_OP(
					partition.setBegin()->marked(), ==, 3);
				TEST_ENSURE_OP(
					partition.setBegin()->unmarked(), ==, 2);
			}

			partition.split();
			{
				TEST_ENSURE_OP(
					partition.splits(), ==, 0);
				TEST_ENSURE_OP(
					partition.sets(), ==, 2);
				TEST_ENSURE_OP(
					partition.elements(), ==, 5);

				// In the splitting, the smaller part
				// should get the new set.
				int data[] = {0, 2, 4};
				TEST_ENSURE(
					same(data, partition.setBegin()));

				auto set = partition.setBegin();
				++set;

				int data2[] = {1, 3};
				TEST_ENSURE(same(data2, set));
				TEST_ENSURE_OP(set->marked(), ==, 0);
				TEST_ENSURE_OP(set->unmarked(), ==, 2);
				TEST_ENSURE_OP(set->elements(), ==, 2);

				TEST_ENSURE(
					std::next(partition.elementBegin())->set() == set);
				TEST_ENSURE(
					std::next(partition.elementBegin(), 3)->set() == set);
			}

			partition.mark(
				std::next(partition.elementBegin(), 1));
			partition.mark(
				std::next(partition.elementBegin(), 3));
			{
				Partition::Set_ConstIterator set = 
					partition.setBegin();
				++set;

				TEST_ENSURE_OP(
					set->elements(), ==, 2);
				TEST_ENSURE_OP(
					set->marked(), ==, 2);
				TEST_ENSURE_OP(
					set->unmarked(), ==, 0);
				TEST_ENSURE_OP(
					partition.splits(), ==, 1);
			}

			partition.split();
			{
				// Nothing should happen when all
				// the elements in a set are marked.

				TEST_ENSURE_OP(
					partition.sets(), ==, 2);

				TEST_ENSURE_OP(
					partition.splits(), ==, 0);
			}

			partition.clear();
			TEST_ENSURE_OP(partition.splits(), ==, 0);
			TEST_ENSURE_OP(partition.sets(), ==, 0);
			TEST_ENSURE_OP(partition.elements(), ==, 0);
		}

		void print(const Partition& partition)
		{
			std::for_each(
				partition.cSetBegin()->begin(),
				partition.cSetBegin()->end(),
				[&](const Element_ConstIterator& element)
			{
				std::cout << element->data() << " ";
			});
			std::cout << std::endl;
		}

		void testRemove()
		{
			integer data[] = {0, 1, 2, 3, 4};

			Partition partition(
				std::begin(data), std::end(data));

			partition.erase(
				std::next(partition.elementBegin()));
			{
				Set_ConstIterator set =
					partition.setBegin();

				TEST_ENSURE_OP(set->elements(), ==, 4);
				TEST_ENSURE_OP(set->marked(), ==, 0);
				TEST_ENSURE_OP(set->unmarked(), ==, 4);
				TEST_ENSURE_OP(partition.elements(), ==, 4);

				integer data[] = {0, 2, 3, 4};
				TEST_ENSURE(same(data, set));
			}

			partition.mark(
				std::next(partition.elementBegin()));
			{
				Set_ConstIterator set =
					partition.setBegin();

				TEST_ENSURE_OP(set->elements(), ==, 4);
				TEST_ENSURE_OP(set->marked(), ==, 1);
				TEST_ENSURE_OP(set->unmarked(), ==, 3);
				TEST_ENSURE_OP(partition.elements(), ==, 4);
				TEST_ENSURE_OP(partition.splits(), ==, 1);
			}

			partition.erase(
				std::next(partition.elementBegin()));
			{
				Set_ConstIterator set =
					partition.setBegin();

				TEST_ENSURE_OP(set->elements(), ==, 3);
				TEST_ENSURE_OP(set->marked(), ==, 0);
				TEST_ENSURE_OP(set->unmarked(), ==, 3);
				TEST_ENSURE_OP(partition.elements(), ==, 3);
				TEST_ENSURE_OP(partition.splits(), ==, 0);

				integer data[] = {0, 3, 4};
				TEST_ENSURE(same(data, set));
			}
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
