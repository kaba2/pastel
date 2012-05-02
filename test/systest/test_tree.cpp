// Description: Testing for Tree
// DocumentationOf: tree.h

#include "pastelsystest.h"

#include "pastel/sys/tree.h"
#include "pastel/sys/range_for_all.h"
#include "pastel/sys/ranges.h"
#include "pastel/sys/predicates.h"

#include <iostream>

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
			testInsert();
		}

		typedef Tree<integer> Tree;
		typedef Tree::Iterator Iterator;
		typedef Tree::ConstIterator ConstIterator;
		typedef Tree::Range Range;
		typedef Tree::ConstRange ConstRange;

		template <int N>
		bool same(const Tree& tree, int (&correctSet)[N]) const
		{
			return rangeForAll(arrayRange(correctSet), tree.crange(), EqualTo());
		}

		bool same(const Tree& tree, const Tree& that)
		{
			return rangeForAll(tree.crange(), that.crange(), EqualTo());
		}

		void test()
		{
			Tree tree;
			{
				TEST_ENSURE(tree.empty());
				TEST_ENSURE_OP(tree.size(), ==, 0);
			}

			Iterator aIter = tree.insertRoot(0);
			{
				TEST_ENSURE_OP(tree.size(), ==, 1);
				TEST_ENSURE(!tree.empty());

				integer correctSet[] = {0};
				TEST_ENSURE(same(tree, correctSet));
			}

			Iterator bIter = tree.insert(aIter, Tree::Left, 1);
			{
				TEST_ENSURE_OP(tree.size(), ==, 2);

				integer correctSet[] = {1, 0};
				TEST_ENSURE(same(tree, correctSet));
			}

			Iterator cIter = tree.insert(bIter, Tree::Right, 2);
			{
				TEST_ENSURE_OP(tree.size(), ==, 3);

				integer correctSet[] = {1, 2, 0};
				TEST_ENSURE(same(tree, correctSet));
			}

			tree.rotate(aIter, Tree::Right);
			{
				integer correctSet[] = {1, 2, 0};
				TEST_ENSURE(same(tree, correctSet));
			}

			Tree copyTree(tree);
			{
				TEST_ENSURE(!copyTree.empty());
				TEST_ENSURE_OP(copyTree.size(), ==, 3);
				TEST_ENSURE(same(tree, copyTree));
			}

			tree.clear();
			{
				TEST_ENSURE(tree.empty());
				TEST_ENSURE_OP(tree.size(), ==, 0);
			}

			tree = copyTree;
			{
				TEST_ENSURE(!tree.empty());
				TEST_ENSURE_OP(tree.size(), ==, 3);
				TEST_ENSURE(same(tree, copyTree));
			}
		}

		void testInsert()
		{
			Tree tree;
			Iterator aIter = tree.insertRoot(0);
			Iterator bIter = tree.insert(aIter, Tree::Left, 1);
			Iterator cIter = tree.insert(bIter, Tree::Right, 2);
			{
				integer correctSet[] = {1, 2, 0};
				TEST_ENSURE(same(tree, correctSet));
			}
			
			tree.insert(bIter, Tree::Left, tree);
			{
				integer correctSet[] = {1, 2, 0, 1, 2, 0};
				TEST_ENSURE(same(tree, correctSet));
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
		testRunner().add("Tree", test);
	}

	CallFunction run(addTest);

}
