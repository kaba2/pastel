// Description: Testing for Tree
// DocumentationOf: tree.h

#include "pastelsystest.h"

#include "pastel/sys/tree.h"

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
		typedef Tree_Iterator<integer> Iterator;
		typedef Tree_ConstIterator<integer> ConstIterator;

		void test()
		{
			Tree tree;
			TEST_ENSURE(tree.empty());
			TEST_ENSURE_OP(tree.size(), ==, 0);

			Iterator aIter = tree.insert(tree.cend(), Tree::Left, 0);
			TEST_ENSURE_OP(tree.size(), ==, 1);
			TEST_ENSURE(!tree.empty());

			Iterator bIter = tree.insert(aIter, Tree::Left, 1);
			TEST_ENSURE_OP(tree.size(), ==, 2);

			Iterator cIter = tree.insert(bIter, Tree::Right, 2);
			TEST_ENSURE_OP(tree.size(), ==, 3);

			print(tree);
			printReverse(tree);

			tree.rotate(aIter, Tree::Right);
			print(tree);

			Tree copyTree(tree);
			TEST_ENSURE(!copyTree.empty());
			TEST_ENSURE_OP(copyTree.size(), ==, 3);

			print(copyTree);
			printReverse(copyTree);

			tree.clear();
			TEST_ENSURE(tree.empty());
			TEST_ENSURE_OP(tree.size(), ==, 0);

			tree = copyTree;
			TEST_ENSURE(!tree.empty());
			TEST_ENSURE_OP(tree.size(), ==, 3);

			print(tree);
			printReverse(tree);
		}

		void testInsert()
		{
			Tree tree;
			Iterator aIter = tree.insert(tree.cend(), Tree::Left, 0);
			Iterator bIter = tree.insert(aIter, Tree::Left, 1);
			Iterator cIter = tree.insert(bIter, Tree::Right, 2);
			
			tree.insert(bIter, Tree::Left, tree);

			print(tree);
		}

		void print(const Tree& tree)
		{
			ConstIterator iter = tree.cbegin();
			ConstIterator iterEnd = tree.cend();
			while(iter != iterEnd)
			{
				std::cout << *iter << ", ";
				++iter;
			}

			std::cout << "end." << std::endl;
		}

		void printReverse(const Tree& tree)
		{
			if (!tree.empty())
			{
				ConstIterator iter = tree.cend();
				ConstIterator iterBegin = tree.cbegin();
				do
				{
					--iter;
					std::cout << *iter << ", ";
				}
				while(iter != iterBegin);
			}

			std::cout << "end." << std::endl;
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
