// Description: Testing for RedBlackTree
// DocumentationOf: redblacktree.h

#include "test_pastelsys.h"

#include "pastel/sys/redblacktree_tools.h"
#include "pastel/sys/random_uniform.h"

#include <boost/range/adaptor/reversed.hpp> 

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
			testRandom();
			testSimple();
		}

		class Counting_Customization
			: public RedBlackTree_Concepts::Customization<
			integer, LessThan, integer>
		{
		public:
			template <typename Iterator>
			void updateHierarchical(const Iterator& iter)
			{
				iter.data() = 
					iter.left().data() + 
					iter.right().data() + 
					(iter.red() ? 1 : 0);
			}
		};

		typedef RedBlackTree<integer, std::less<integer>, integer, Counting_Customization> Tree;
		typedef Tree::Iterator Iterator;
		typedef Tree::ConstIterator ConstIterator;

		void testRandom()
		{
			Tree tree(0, 0);

			std::list<integer> dataSet;

			const integer treeSizeSet[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 100, 1000};
			const integer treeSizes = sizeof(treeSizeSet) / sizeof(integer);

			for (integer k = 0;k < treeSizes;++k)
			{
				const integer treeSize = treeSizeSet[k];
				tree.clear();
				dataSet.clear();
				for (integer i = 0;i < treeSize;++i)
				{
					const integer n = randomInteger();
					dataSet.push_back(n);

					tree.insert(n, 1);
					TEST_ENSURE(check(tree));

					if (tree.size() > treeSize)
					{
						tree.erase(dataSet.front());
						TEST_ENSURE(check(tree));
						dataSet.pop_front();
					}
				}
			}

			{
				Tree copyTree(tree);
				copyTree.swap(tree);
				TEST_ENSURE(check(tree));
			}
		}

		void testSimple()
		{
			Tree tree(0, 0);

			tree.insert(1, 1);
			tree.insert(5, 1);
			tree.insert(3, 1);
			tree.insert(4, 1);
			tree.insert(8, 1);
			tree.insert(7, 1);
			tree.insert(6, 1);
			tree.insert(9, 1);
			tree.insert(2, 1);
			{
				integer correctSet[] = 
				{
					1, 2, 3, 4, 5, 6, 7, 8, 9
				};
				TEST_ENSURE(boost::equal(
					range(tree.cbegin(), tree.cend()), 
					range(correctSet)));
			}

			{
				// The iterator should stay on end().
				ConstIterator copyIter = tree.cend();
				++copyIter;
				TEST_ENSURE(copyIter == tree.cend());
			}

			{
				integer correctSet[] = 
				{
					9, 8, 7, 6, 5, 4, 3, 2, 1
				};

				TEST_ENSURE(boost::equal(
					range(tree.cbegin(), tree.cend()) | boost::adaptors::reversed, 
					range(correctSet)));
			}

			{
				// The iterator should stay on begin().
				ConstIterator copyIter = tree.cbegin();
				--copyIter;
				TEST_ENSURE(copyIter == tree.cbegin());
			}

			TEST_ENSURE(tree.find(3) != tree.end());
			TEST_ENSURE(tree.find(-1) == tree.end());
			TEST_ENSURE(tree.find(10) == tree.end());
			TEST_ENSURE(tree.find(9) != tree.end());
			TEST_ENSURE(tree.find(1) != tree.end());
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("RedBlackTree", test);
	}

	CallFunction run(addTest);

}
