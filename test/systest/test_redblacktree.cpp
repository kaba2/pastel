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

		typedef Map<integer, integer> Tree;
		typedef Tree::Iterator Iterator;
		typedef Tree::ConstIterator ConstIterator;

		using Tree_Settings = Map_Settings<integer, integer>;
		class Counting_Customization
			: public Empty_RedBlackTree_Customization<Tree_Settings>
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

		typedef RedBlackTree<Tree_Settings, Counting_Customization> Counting_Tree;
		typedef Counting_Tree::Iterator Counting_Iterator;
		typedef Counting_Tree::ConstIterator Counting_ConstIterator;

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
			TEST_ENSURE(check(tree));

			tree.insert(1, 1);
			TEST_ENSURE(check(tree));
			tree.insert(5, 2);
			TEST_ENSURE(check(tree));
			tree.insert(3, 3);
			TEST_ENSURE(check(tree));
			tree.insert(4, 4);
			TEST_ENSURE(check(tree));
			tree.insert(8, 5);
			TEST_ENSURE(check(tree));
			tree.insert(7, 6);
			TEST_ENSURE(check(tree));
			tree.insert(6, 7);
			TEST_ENSURE(check(tree));
			tree.insert(9, 8);
			TEST_ENSURE(check(tree));
			tree.insert(2, 9);
			TEST_ENSURE(check(tree));

			integer correctDataSet[] = 
			{
				1, 9, 3, 4, 2, 7, 6, 5, 8
			};

			integer correctKeySet[] = 
			{
				1, 2, 3, 4, 5, 6, 7, 8, 9
			};

			{
				TEST_ENSURE_OP(boost::distance(tree), ==, 9)
				TEST_ENSURE_OP(boost::distance(tree.cdataRange()), ==, 9)
				TEST_ENSURE_OP(boost::distance(tree.ckeyRange()), ==, 9)

				TEST_ENSURE(boost::equal(tree, correctDataSet));
				TEST_ENSURE(boost::equal(tree.cdataRange(), correctDataSet));
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctKeySet));
			}

			{
				// The iterator should stay on end().
				ConstIterator copyIter = tree.cend();
				++copyIter;
				TEST_ENSURE(copyIter == tree.cend());
			}

			{
				TEST_ENSURE(boost::equal(
					tree | boost::adaptors::reversed, 
					correctDataSet | boost::adaptors::reversed));

				TEST_ENSURE(boost::equal(
					tree.cdataRange() | boost::adaptors::reversed, 
					correctDataSet | boost::adaptors::reversed));

				TEST_ENSURE(boost::equal(
					tree.ckeyRange() | boost::adaptors::reversed, 
					correctKeySet | boost::adaptors::reversed));
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
