// Description: Testing for RedBlackTree
// DocumentationOf: redblacktree.h

#include "test_pastelsys.h"

#include "pastel/sys/redblacktree.h"
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
			testSet();
			testMultiSet();
			testInsert();
			testRandom();
		}

		void testSet()
		{
			using Tree = RedBlack_Set<integer>;
			using Iterator = Tree::Iterator;
			using ConstIterator = Tree::ConstIterator;

			Tree tree;

			{
				auto test = [&](integer that)
				{
					auto iterAndNew = tree.insert(that);
					TEST_ENSURE(testInvariants(tree));
					TEST_ENSURE(iterAndNew.second);
					TEST_ENSURE_OP(*iterAndNew.first, == , that);
					TEST_ENSURE(tree.find(that) == iterAndNew.first);
					TEST_ENSURE(tree.lowerBound(that) == iterAndNew.first);
					TEST_ENSURE(tree.exists(that));
				};

				test(1);
				TEST_ENSURE_OP(tree.size(), == , 1);

				test(5);
				TEST_ENSURE_OP(tree.size(), == , 2);

				test(3);
				TEST_ENSURE_OP(tree.size(), == , 3);
			}
			{
				auto iterAndNew = tree.insert(1);
				TEST_ENSURE(!iterAndNew.second);
				TEST_ENSURE_OP(*iterAndNew.first, == , 1);
				TEST_ENSURE_OP(tree.size(), == , 3);
			}
			{
				Tree tree({ 4, 2, 1, 1, 1, 3 });
				integer correctSet[] = { 1, 2, 3, 4 };
				TEST_ENSURE_OP(tree.size(), == , 4);
				TEST_ENSURE(boost::equal(tree, correctSet));
			}
			{
				Tree tree;
				tree = { 4, 2, 1, 1, 1, 3 };
				
				integer correctSet[] = { 1, 2, 3, 4 };
				TEST_ENSURE_OP(tree.size(), == , 4);
				TEST_ENSURE(boost::equal(tree, correctSet));
			}
		}

		void testMultiSet()
		{
			using Tree = RedBlack_MultiSet<integer>;
			using Iterator = Tree::Iterator;
			using ConstIterator = Tree::ConstIterator;

			{
				Tree tree;

				auto test = [&](integer that)
				{
					Iterator iter = tree.insert(that);
					TEST_ENSURE(testInvariants(tree));
					TEST_ENSURE_OP(*iter, == , that);
					TEST_ENSURE(tree.find(that) == iter);
					TEST_ENSURE(tree.lowerBound(that) == iter);
					TEST_ENSURE(tree.exists(that));
				};

				test(1);
				TEST_ENSURE_OP(tree.size(), == , 1);

				test(5);
				TEST_ENSURE_OP(tree.size(), == , 2);

				test(3);
				TEST_ENSURE_OP(tree.size(), == , 3);
			}
			{
				Tree tree;

				auto test = [&](integer that)
				{
					Iterator iter = tree.insert(that);
					TEST_ENSURE(testInvariants(tree));
					TEST_ENSURE_OP(*iter, == , that);
					TEST_ENSURE(iter == std::prev(tree.cend()));
				};

				test(1);
				TEST_ENSURE_OP(tree.size(), ==, 1);

				test(1);
				TEST_ENSURE_OP(tree.size(), ==, 2);

				test(1);
				TEST_ENSURE_OP(tree.size(), ==, 3);

				test(5);
				TEST_ENSURE_OP(tree.size(), == , 4);

				test(5);
				TEST_ENSURE_OP(tree.size(), == , 5);

				test(5);
				TEST_ENSURE_OP(tree.size(), == , 6);
			}
		}

		typedef RedBlack_Map<integer, integer> Tree;
		typedef Tree::Iterator Iterator;
		typedef Tree::ConstIterator ConstIterator;

		using Tree_Settings = RedBlack_Settings<integer, integer>;
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
			Tree tree;

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
					TEST_ENSURE(testInvariants(tree));

					if (tree.size() > treeSize)
					{
						tree.erase(dataSet.front());
						TEST_ENSURE(testInvariants(tree));
						dataSet.pop_front();
					}
				}

				{
					Tree copyTree(tree);
					TEST_ENSURE(testInvariants(copyTree));
					TEST_ENSURE_OP(tree.size(), ==, copyTree.size());
					TEST_ENSURE(boost::equal(tree, copyTree));
					TEST_ENSURE(boost::equal(tree.ckeyRange(), copyTree.ckeyRange()));
					TEST_ENSURE(boost::equal(tree.cdataRange(), copyTree.cdataRange()));
				}
			}
		}

		void testInsert()
		{
			Tree tree;
			{
				TEST_ENSURE(tree.empty());
				TEST_ENSURE_OP(tree.size(), ==, 0);
				TEST_ENSURE(testInvariants(tree));
			}

			tree.insert(1, 1);
			{
				TEST_ENSURE(!tree.empty());
				TEST_ENSURE_OP(tree.size(), ==, 1);
				TEST_ENSURE(testInvariants(tree));

				integer correctKeySet[] = { 1 };
				TEST_ENSURE(boost::equal(tree, correctKeySet));
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctKeySet));

				integer correctDataSet[] = { 1 };
				TEST_ENSURE(boost::equal(tree.cdataRange(), correctDataSet));
			}
			
			tree.insert(5, 2);
			{
				TEST_ENSURE(!tree.empty());
				TEST_ENSURE_OP(tree.size(), ==, 2);
				TEST_ENSURE(testInvariants(tree));

				integer correctKeySet[] = { 1, 5 };
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctKeySet));

				integer correctDataSet[] = { 1, 2 };
				TEST_ENSURE(boost::equal(tree, correctDataSet));
				TEST_ENSURE(boost::equal(tree.cdataRange(), correctDataSet));
			}

			tree.insert(3, 3);
			{
				TEST_ENSURE(!tree.empty());
				TEST_ENSURE_OP(tree.size(), ==, 3);
				TEST_ENSURE(testInvariants(tree));

				integer correctKeySet[] = { 1, 3, 5 };
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctKeySet));

				integer correctDataSet[] = { 1, 3, 2 };
				TEST_ENSURE(boost::equal(tree, correctDataSet));
				TEST_ENSURE(boost::equal(tree.cdataRange(), correctDataSet));
			}

			tree.insert(4, 4);
			{
				TEST_ENSURE(!tree.empty());
				TEST_ENSURE_OP(tree.size(), ==, 4);
				TEST_ENSURE(testInvariants(tree));

				integer correctKeySet[] = { 1, 3, 4, 5 };
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctKeySet));

				integer correctDataSet[] = { 1, 3, 4, 2 };
				TEST_ENSURE(boost::equal(tree, correctDataSet));
				TEST_ENSURE(boost::equal(tree.cdataRange(), correctDataSet));
			}

			tree.insert(8, 5);
			{
				TEST_ENSURE(!tree.empty());
				TEST_ENSURE_OP(tree.size(), ==, 5);
				TEST_ENSURE(testInvariants(tree));

				integer correctKeySet[] = { 1, 3, 4, 5, 8 };
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctKeySet));

				integer correctDataSet[] = { 1, 3, 4, 2, 5 };
				TEST_ENSURE(boost::equal(tree, correctDataSet));
				TEST_ENSURE(boost::equal(tree.cdataRange(), correctDataSet));
			}

			tree.insert(7, 6);
			{
				TEST_ENSURE(!tree.empty());
				TEST_ENSURE_OP(tree.size(), ==, 6);
				TEST_ENSURE(testInvariants(tree));

				integer correctKeySet[] = { 1, 3, 4, 5, 7, 8 };
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctKeySet));

				integer correctDataSet[] = { 1, 3, 4, 2, 6, 5 };
				TEST_ENSURE(boost::equal(tree, correctDataSet));
				TEST_ENSURE(boost::equal(tree.cdataRange(), correctDataSet));
			}

			tree.insert(6, 7);
			{
				TEST_ENSURE(!tree.empty());
				TEST_ENSURE_OP(tree.size(), ==, 7);
				TEST_ENSURE(testInvariants(tree));

				integer correctKeySet[] = { 1, 3, 4, 5, 6, 7, 8 };
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctKeySet));

				integer correctDataSet[] = { 1, 3, 4, 2, 7, 6, 5 };
				TEST_ENSURE(boost::equal(tree, correctDataSet));
				TEST_ENSURE(boost::equal(tree.cdataRange(), correctDataSet));
			}

			tree.insert(9, 8);
			{
				TEST_ENSURE(!tree.empty());
				TEST_ENSURE_OP(tree.size(), ==, 8);
				TEST_ENSURE(testInvariants(tree));

				integer correctKeySet[] = { 1, 3, 4, 5, 6, 7, 8, 9 };
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctKeySet));

				integer correctDataSet[] = { 1, 3, 4, 2, 7, 6, 5, 8 };
				TEST_ENSURE(boost::equal(tree, correctDataSet));
				TEST_ENSURE(boost::equal(tree.cdataRange(), correctDataSet));
			}

			tree.insert(2, 9);
			{
				TEST_ENSURE(!tree.empty());
				TEST_ENSURE_OP(tree.size(), ==, 9);
				TEST_ENSURE(testInvariants(tree));

				integer correctKeySet[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctKeySet));

				integer correctDataSet[] = { 1, 9, 3, 4, 2, 7, 6, 5, 8 };
				TEST_ENSURE(boost::equal(tree, correctDataSet));
				TEST_ENSURE(boost::equal(tree.cdataRange(), correctDataSet));
			}

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
				ConstIterator copyIter = tree.cbegin();
				--copyIter;
				TEST_ENSURE(copyIter == tree.cend());
			}

			// Test find().
			{
				TEST_ENSURE(
					tree.find(-1) == tree.cend());
				TEST_ENSURE(
					tree.find(1) != tree.cend() &&
					tree.find(1).key() == 1);
				TEST_ENSURE(
					tree.find(3) != tree.cend() &&
					tree.find(3).key() == 3);
				TEST_ENSURE(
					tree.find(9) != tree.cend() &&
					tree.find(9).key() == 9);
				TEST_ENSURE(
					tree.find(10) == tree.cend());
			}

			// Test lowerBound().
			{
				TEST_ENSURE(
					tree.lowerBound(-1) != tree.cend() &&
					tree.lowerBound(-1).key() == 1);
				TEST_ENSURE(
					tree.lowerBound(1) != tree.cend() &&
					tree.lowerBound(1).key() == 1);
				TEST_ENSURE(
					tree.lowerBound(3) != tree.cend() &&
					tree.lowerBound(3).key() == 3);
				TEST_ENSURE(
					tree.lowerBound(9) != tree.cend() &&
					tree.lowerBound(9).key() == 9);
				TEST_ENSURE(
					tree.lowerBound(10) == tree.cend());
			}

			// Test upperBound().
			{
				TEST_ENSURE(
					tree.upperBound(-1) != tree.cend());
				TEST_ENSURE(
					tree.upperBound(-1) != tree.cend() &&
					tree.upperBound(-1).key() == 1);
				TEST_ENSURE(
					tree.upperBound(1) != tree.cend() &&
					tree.upperBound(1).key() == 2);
				TEST_ENSURE(
					tree.upperBound(3) != tree.cend() &&
					tree.upperBound(3).key() == 4);
				TEST_ENSURE(
					tree.upperBound(9) == tree.cend());
				TEST_ENSURE(
					tree.upperBound(10) == tree.cend());
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
		testRunner().add("RedBlackTree", test);
	}

	CallFunction run(addTest);

}
