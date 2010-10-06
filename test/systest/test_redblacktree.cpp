#include "pastelsystest.h"

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/rbtpolicy_all.h"

#include <iostream>

using namespace Pastel;

namespace
{

	class RedBlackTree_Test
		: public TestSuite
	{
	public:
		RedBlackTree_Test()
			: TestSuite(&sysTestReport())
		{
		}

		virtual void run()
		{
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
			void updateHierarchicalData(const Iterator& iter)
			{
				//iter->value() = 
				//	iter.left()->value() + iter.right()->value() + 1;
				iter->value() = 
					std::max(std::max(iter.left()->value(), iter.right()->value()),
					iter->key());
			}
		};

		//typedef RedBlackTree<integer, std::less<integer>, Map_RbtPolicy<integer> > Tree;
		typedef RedBlackTree<integer, std::less<integer>, Counting_RbtPolicy> Tree;
		//typedef RedBlackTree<integer> Tree;
		typedef Tree::Iterator Iterator;
		typedef Tree::ConstIterator ConstIterator;

		void testSimple()
		{
			Tree tree(Counting_RbtPolicy(), 0, 0);
			tree.insert(1, 1);
			tree.insert(8, 1);
			tree.insert(3, 1);
			tree.insert(5, 1);
			tree.insert(7, 1);
			tree.insert(2, 1);
			tree.insert(9, 1);
			// Nothing should happen.
			tree.insert(1, 1);

			Tree copyTree(tree);
			copyTree.swap(tree);

			std::cout << "Minimum " << tree.begin()->key() << std::endl;
			std::cout << "Maximum " << tree.last()->key() << std::endl;
			
			ConstIterator iter = tree.begin();
			const ConstIterator iterEnd = tree.end();
			while(iter != iterEnd)
			{
				std::cout << iter->key() << ", ";
				++iter;
			}

			// The iterator should stay on end().
			++iter;

			do
			{
				--iter;			
				std::cout << iter->key() << ", ";
			}
			while(iter != tree.begin());

			// The iterator should stay on begin().
			--iter;
			std::cout << iter->key() << ", ";

			std::cout << "end." << std::endl;
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
		RedBlackTree_Test test;
		test.run();
	}

	void addTest()
	{
		sysTestList().add("RedBlackTree", testRedBlackTree);
	}

	CallFunction run(addTest);

}
