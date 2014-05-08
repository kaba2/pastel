// Description: Testing for red-black forest
// DocumentationOf: redblackforest.h

#include "test_pastelsys.h"

#include <pastel/sys/redblackforest.h>

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

		void test()
		{
			using Tree = RedBlack_Set<integer>;
			using Forest = RedBlackForest_Set<Tree>;
			using Iterator = Forest::Iterator;
			using ConstIterator = Forest::ConstIterator;
			using Tree_Iterator = Forest::Tree_Iterator;
			using Tree_ConstIterator = Forest::Tree_ConstIterator;
			
			Forest forest;
			TEST_ENSURE(testInvariants(forest));

			Tree_Iterator aTree = forest.insert();
			aTree->insert(0);
			aTree->insert(1);
			aTree->insert(2);
			{
				integer correctTree[] = { 0, 1, 2 };
				TEST_ENSURE(boost::equal(forest.range(), correctTree));
				TEST_ENSURE_OP(forest.trees(), == , 1);
			}

			Tree_Iterator bTree = forest.insert();
			TEST_ENSURE(testInvariants(forest));

			bTree->insert(3);
			TEST_ENSURE(testInvariants(forest));

			bTree->insert(4);
			TEST_ENSURE(testInvariants(forest));

			bTree->insert(5);
			TEST_ENSURE(testInvariants(forest));

			bTree->insert(6);
			TEST_ENSURE(testInvariants(forest));
			{
				integer correctTree[] = { 0, 1, 2, 3, 4, 5, 6 };
				TEST_ENSURE(boost::equal(forest.range(), correctTree));
				TEST_ENSURE_OP(forest.trees(), == , 2);
			}
			
			Tree_Iterator cTree = forest.insert();
			TEST_ENSURE(testInvariants(forest));

			*cTree = bTree->split(5);
			TEST_ENSURE(testInvariants(forest));
			{
				integer correctTree[] = { 0, 1, 2, 3, 4, 5, 6 };
				TEST_ENSURE(boost::equal(forest.range(), correctTree));
				TEST_ENSURE_OP(forest.trees(), == , 3);
			}

			bTree->join(*cTree);
			TEST_ENSURE(testInvariants(forest));
			{
				integer correctTree[] = { 0, 1, 2, 3, 4, 5, 6 };
				TEST_ENSURE(boost::equal(forest.range(), correctTree));

				for (integer i : forest)
				{
					std::cout << i << std::endl;
				}
			}

			forest.erase(cTree);
			TEST_ENSURE(testInvariants(forest));
			{
				integer correctTree[] = { 0, 1, 2, 3, 4, 5, 6 };
				TEST_ENSURE(boost::equal(forest.range(), correctTree));
				TEST_ENSURE_OP(forest.trees(), == , 2);
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
		testRunner().add("RedBlackForest", test);
	}

	CallFunction run(addTest);

}
