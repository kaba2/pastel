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
			using Forest = RedBlackForest_Set<integer>;
			using Iterator = Forest::Iterator;
			using ConstIterator = Forest::ConstIterator;
			using Set_Iterator = Forest::Set_Iterator;
			using Set_ConstIterator = Forest::Set_ConstIterator;
			
			Forest forest;
			TEST_ENSURE_OP(forest.size(), == , 0);
			TEST_ENSURE(forest.empty());

			Set_Iterator aSet = forest.insertSet();

			Iterator a = forest.insert(aSet, 4).first;
			forest.insert(aSet, 5);

			TEST_ENSURE_OP(forest.size(), == , 1);
			TEST_ENSURE(!forest.empty());

			forest.find(aSet, 4);
			forest.lowerBound(aSet, 4);
			forest.upperBound(aSet, 4);
			Set_Iterator bSet = forest.split(aSet, 5);
			TEST_ENSURE(forest.exists(bSet, 5));
			forest.join(aSet, bSet);
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
