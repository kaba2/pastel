// Description: Testing for HashedTree
// DocumentationOf: hashed_tree.h

#include "test_pastelsys.h"

#include "pastel/sys/hashed_tree.h"
#include "pastel/sys/random/random_uniform.h"

#include <iostream>
#include <list>

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

		virtual void run()
		{
			test();
		}

		using Tree = Hashed_Map<integer, integer>;
		using Iterator = Tree::Iterator;
		using ConstIterator = Tree::ConstIterator;

		void test()
		{
			integer keySet[] = {1, 5, 3, 2, 7};

			Tree tree;
			TEST_ENSURE_OP(tree.hash(), ==, 0);

			std::vector<hash_integer> hashSet;
			for (integer key : keySet)
			{
				tree.insert(key, 0);
				hashSet.push_back(tree.hash());
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
		testRunner().add("HashedTree", test);
	}

	CallFunction run(addTest);

}
