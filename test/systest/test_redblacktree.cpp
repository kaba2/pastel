// Description: Testing for RedBlackTree
// DocumentationOf: redblacktree.h

#include "pastelsystest.h"

#include "pastel/sys/redblacktree_tools.h"
#include "pastel/sys/rbtpolicy_all.h"
#include "pastel/sys/random_uniform.h"

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
			: TestSuite(&sysTestReport())
		{
		}

		virtual void run()
		{
			testRandom();
			testSimple();
		}

		class Counting_RbtPolicy
		{
		public:
			typedef integer ValueType;

			void swap(Counting_RbtPolicy& that)
			{
			}

			template <typename Iterator>
			void updateInsertDown(const Iterator& iter)
			{
			}

			template <typename Iterator>
			void updateHierarchical(const Iterator& iter)
			{
				iter->value() = 
					iter.left()->value() + 
					iter.right()->value() + 
					(iter.red() ? 1 : 0);
			}
		};

		//typedef RedBlackTree<integer, std::less<integer>, Map_RbtPolicy<integer> > Tree;
		typedef RedBlackTree<integer, std::less<integer>, Counting_RbtPolicy> Tree;
		//typedef RedBlackTree<integer> Tree;
		typedef Tree::Iterator Iterator;
		typedef Tree::ConstIterator ConstIterator;

		void testRandom()
		{
			Tree tree(Counting_RbtPolicy(), 0, 0);

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
			Tree tree(Counting_RbtPolicy(), 0, 0);

			tree.insert(1, 1);
			tree.insert(5, 1);
			tree.insert(3, 1);
			tree.insert(4, 1);
			tree.insert(8, 1);
			tree.insert(7, 1);
			tree.insert(6, 1);
			tree.insert(9, 1);
			tree.insert(2, 1);

			std::cout << "Minimum " << tree.begin()->key() << std::endl;
			std::cout << "Maximum " << tree.last()->key() << std::endl;
			
			ConstIterator iter = tree.begin();
			const ConstIterator iterEnd = tree.end();
			while(iter != iterEnd)
			{
				std::cout << "(" << iter->key() << " : " 
					<< iter->value() << "), ";
				++iter;
			}

			std::cout << "end." << std::endl;

			{
				// The iterator should stay on end().
				ConstIterator copyIter = iter;
				++copyIter;
				TEST_ENSURE(copyIter == iter);
			}

			do
			{
				--iter;			
				std::cout << "(" << iter->key() << " : " 
					<< iter->value() << "), ";
			}
			while(iter != tree.begin());

			std::cout << "end." << std::endl;

			{
				// The iterator should stay on begin().
				ConstIterator copyIter = iter;
				--copyIter;
				TEST_ENSURE(copyIter == iter);
			}

			std::cout << "Root " 
				<< tree.root()->key() << " : " 
				<< tree.root()->value() << "." << std::endl;

			find(tree, 3);
			find(tree, -1);
			find(tree, 10);
			find(tree, 9);
			find(tree, 1);
		}

		void find(const Tree& tree, integer that)
		{
			ConstIterator iter = tree.find(that);
			if (iter != tree.end())
			{
				std::cout << "Found " << iter->key() << " : " << iter->value() << "." << std::endl;
			}
			else
			{
				std::cout << "Not found " << that << std::endl;
			}
		}
	};

	void testRedBlackTree()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		sysTestList().add("RedBlackTree", testRedBlackTree);
	}

	CallFunction run(addTest);

}
