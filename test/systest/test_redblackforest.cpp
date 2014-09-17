// Description: Testing for red-black forest
// DocumentationOf: redblackforest.h

#include "test_pastelsys.h"

#include <pastel/sys/redblackforest.h>

using namespace Pastel;

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

		using Tree = RedBlackTree_Set<integer>;
		using Forest = RedBlackForest_Set<Tree>;
		using Iterator = Forest::Iterator;
		using ConstIterator = Forest::ConstIterator;
		using Element_Iterator = Forest::Element_Iterator;
		using Element_ConstIterator = Forest::Element_ConstIterator;
		using Tree_Iterator = Forest::Tree_Iterator;
		using Tree_ConstIterator = Forest::Tree_ConstIterator;

		virtual void run()
		{
			test();
			testIterators();
		}

		void testIterators()
		{
			Forest forest;
			{
				Tree_ConstIterator a = Tree_Iterator();
			}
			{
				ConstIterator a = Iterator();
			}
			{
				Element_Iterator a = Iterator();
				Iterator b = Element_Iterator();
			}
			{
				Element_ConstIterator a = ConstIterator();
				ConstIterator b = Element_ConstIterator();
			}
			{
				Element_ConstIterator a = Iterator();
				Element_ConstIterator b = Element_Iterator();
				ConstIterator c = Element_Iterator();
			}
			{
				Iterator a = forest.cast(ConstIterator());
			}
			{
				Element_Iterator a = forest.cast(Element_ConstIterator());
			}

			// The following should not compile.
			/*
			{
				Iterator a = ConstIterator();
			}
			{
				Element_Iterator a = Element_ConstIterator();
			}
			{
				Element_Iterator a = ConstIterator();
			}
			{
				Iterator a = Element_ConstIterator();
			}
			*/
		}

		void test()
		{
		
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
			}

			forest.erase(cTree);
			TEST_ENSURE(testInvariants(forest));
			{
				integer correctTree[] = { 0, 1, 2, 3, 4, 5, 6 };
				TEST_ENSURE(boost::equal(forest.range(), correctTree));
				TEST_ENSURE_OP(forest.trees(), == , 2);
			}

			{
				// TODO: Make it well-defined what will happen in
				// this case; we want closed traversal.
				/*
				ConstIterator a = forest.cbegin();
				--a;
				*/
			}
			{
				ConstIterator a = forest.cend();
				++a;
				TEST_ENSURE(a == forest.cend());
			}
			{
				ConstIterator a = forest.cend();
				--a;
				TEST_ENSURE(a != forest.cend());
				TEST_ENSURE(a == forest.clast());
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
