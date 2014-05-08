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
			using Set_Iterator = Forest::Set_Iterator;
			using Set_ConstIterator = Forest::Set_ConstIterator;
			
			Forest forest;

			Set_Iterator aSet = forest.insert();
			aSet->insert(0);
			aSet->insert(1);
			aSet->insert(2);
			{
				integer correctSet[] = { 0, 1, 2 };
				TEST_ENSURE(boost::equal(forest.range(), correctSet));
			}

			Set_Iterator bSet = forest.insert();
			bSet->insert(3);
			bSet->insert(4);
			bSet->insert(5);
			bSet->insert(6);
			{
				integer correctSet[] = { 0, 1, 2, 3, 4, 5, 6 };
				TEST_ENSURE(boost::equal(forest.range(), correctSet));
			}
			
			Set_Iterator cSet = forest.insert();
			*cSet = bSet->split(5);
			{
				integer correctSet[] = { 0, 1, 2, 3, 4, 5, 6 };
				TEST_ENSURE(boost::equal(forest.range(), correctSet));
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
