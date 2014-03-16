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

		using Map_Settings = RedBlack_Settings<integer, integer>;
		class Counting_Customization
			: public Empty_RedBlackTree_Customization<Map_Settings>
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

		typedef RedBlackTree<Map_Settings, Counting_Customization> Counting_Map;
		typedef Counting_Map::Iterator Counting_Iterator;
		typedef Counting_Map::ConstIterator Counting_ConstIterator;

		using Set = RedBlack_Set<integer>;
		using MultiSet = RedBlack_MultiSet<integer>;
		using Map = RedBlack_Map<integer, integer>;
		using MultiMap = RedBlack_MultiMap<integer, integer>;

		virtual void run()
		{
			testSet();
			testMultiSet();

			testMapInsert<Map>();
			testMapInsert<MultiMap>();

			testRandom<Map>();
			testRandom<MultiMap>();

			testSearch<Set>();
			testSearch<MultiSet>();
			testSearch<Map>();
			testSearch<MultiMap>();
		}

		template <typename Tree>
		void testSearch()
		{
			testFind<Tree>();
			testLowerBound<Tree>();
			testUpperBound<Tree>();
		}

		void testSet()
		{
			{
				Set tree;
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
				
				auto iterAndNew = tree.insert(1);
				TEST_ENSURE(!iterAndNew.second);
				TEST_ENSURE_OP(*iterAndNew.first, == , 1);
				TEST_ENSURE_OP(tree.size(), == , 3);
			}
			{
				Set tree({ 4, 2, 1, 1, 1, 3 });
				integer correctSet[] = { 1, 2, 3, 4 };
				TEST_ENSURE_OP(tree.size(), == , 4);
				TEST_ENSURE(boost::equal(tree, correctSet));
			}
			{
				Set tree;
				tree = { 4, 2, 1, 1, 1, 3 };
				
				integer correctSet[] = { 1, 2, 3, 4 };
				TEST_ENSURE_OP(tree.size(), == , 4);
				TEST_ENSURE(boost::equal(tree, correctSet));
			}
		}

		void testMultiSet()
		{
			{
				MultiSet tree;

				auto test = [&](integer that)
				{
					auto iter = tree.insert(that);
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
				MultiSet tree;

				auto test = [&](integer that)
				{
					auto iter = tree.insert(that);
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

		template <typename Tree>
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
					Tree copyMap(tree);
					TEST_ENSURE(testInvariants(copyMap));
					TEST_ENSURE_OP(tree.size(), ==, copyMap.size());
					TEST_ENSURE(boost::equal(tree, copyMap));
					TEST_ENSURE(boost::equal(tree.ckeyRange(), copyMap.ckeyRange()));
					TEST_ENSURE(boost::equal(tree.cdataRange(), copyMap.cdataRange()));
				}
			}
		}

		template <typename Tree>
		void testMapInsert()
		{
			using ConstIterator = typename Tree::ConstIterator;

			Tree tree;
			{
				TEST_ENSURE(tree.empty());
				TEST_ENSURE_OP(tree.size(), == , 0);
				TEST_ENSURE(testInvariants(tree));
			}

			tree.insert(1, 1);
			{
				TEST_ENSURE(!tree.empty());
				TEST_ENSURE_OP(tree.size(), == , 1);
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
				TEST_ENSURE_OP(tree.size(), == , 2);
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
				TEST_ENSURE_OP(tree.size(), == , 3);
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
				TEST_ENSURE_OP(tree.size(), == , 4);
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
				TEST_ENSURE_OP(tree.size(), == , 5);
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
				TEST_ENSURE_OP(tree.size(), == , 6);
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
				TEST_ENSURE_OP(tree.size(), == , 7);
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
				TEST_ENSURE_OP(tree.size(), == , 8);
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
				TEST_ENSURE_OP(tree.size(), == , 9);
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
				TEST_ENSURE_OP(boost::distance(tree), == , 9)
					TEST_ENSURE_OP(boost::distance(tree.cdataRange()), == , 9)
					TEST_ENSURE_OP(boost::distance(tree.ckeyRange()), == , 9)

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
		}

		template <typename Tree>
		void testFind()
		{
			Tree tree{ 0, 4, 4, 5, 5, 5, 5, 9, 15, 20 };

			auto found = [&](integer that)
			{
				TEST_ENSURE(tree.exists(that));
				TEST_ENSURE(
					tree.find(that) != tree.cend() &&
					tree.find(that).key() == that);
			};

			auto notFound = [&](integer that)
			{
				TEST_ENSURE(tree.find(that) == tree.cend());
				TEST_ENSURE(!tree.exists(that));
			};

			notFound(-1);
			found(0);
			notFound(1);
			notFound(2);
			notFound(3);
			found(4);
			found(5);
			notFound(6);
			notFound(7);
			notFound(8);
			found(9);
			notFound(10);
			notFound(11);
			notFound(12);
			notFound(13);
			notFound(14);
			found(15);
			notFound(16);
			notFound(17);
			notFound(18);
			notFound(19);
			found(20);
			notFound(21);
		}

		template <typename Tree>
		void testLowerBound()
		{
			Tree tree{ 0, 4, 4, 5, 5, 5, 5, 9, 15, 20 };

			auto found = [&](integer that, integer bound)
			{
				TEST_ENSURE(
					tree.lowerBound(that) != tree.cend() &&
					tree.lowerBound(that).key() == bound);
			};

			auto notFound = [&](integer that)
			{
				TEST_ENSURE(tree.lowerBound(that) == tree.cend());
			};

			found(-1, 0);
			found(0, 0);
			found(1, 4);
			found(2, 4);
			found(3, 4);
			found(4, 4);
			found(5, 5);
			found(6, 9);
			found(7, 9);
			found(8, 9);
			found(9, 9);
			found(10, 15);
			found(11, 15);
			found(12, 15);
			found(13, 15);
			found(14, 15);
			found(15, 15);
			found(16, 20);
			found(17, 20);
			found(18, 20);
			found(19, 20);
			found(20, 20);
			notFound(21);
			notFound(22);
		}

		template <typename Tree>
		void testUpperBound()
		{
			Tree tree{ 0, 4, 4, 5, 5, 5, 5, 9, 15, 20 };

			auto found = [&](integer that, integer bound)
			{
				bool wasFound = 
					tree.upperBound(that) != tree.cend() &&
					tree.upperBound(that).key() == bound;
				TEST_ENSURE(wasFound);
				if (!wasFound)
				{
					std::cout << that << ", " 
						<< tree.upperBound(that).key() << ", " 
						<< bound << std::endl;
				}
			};

			auto notFound = [&](integer that)
			{
				TEST_ENSURE(tree.upperBound(that) == tree.cend());
			};

			found(-1, 0);
			found(0, 4);
			found(1, 4);
			found(2, 4);
			found(3, 4);
			found(4, 5);
			found(5, 9);
			found(6, 9);
			found(7, 9);
			found(8, 9);
			found(9, 15);
			found(10, 15);
			found(11, 15);
			found(12, 15);
			found(13, 15);
			found(14, 15);
			found(15, 20);
			found(16, 20);
			found(17, 20);
			found(18, 20);
			found(19, 20);
			notFound(20);
			notFound(21);
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
