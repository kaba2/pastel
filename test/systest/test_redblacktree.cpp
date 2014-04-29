// Description: Testing for red-black tree
// DocumentationOf: redblacktree.h

#include "test_pastelsys.h"

#include <pastel/sys/redblacktree.h>
#include <pastel/sys/random_uniform.h>
#include <pastel/sys/random_integer.h>

#include <boost/range/adaptor/reversed.hpp> 

#include <algorithm>
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

		template <typename Settings_>
		class Counting_Customization
		{
		protected:
			using Fwd = RedBlackTree_Fwd<Settings_>;
			PASTEL_FWD(Iterator);

			Counting_Customization() {}
			void swap(Counting_Customization& that) {}
			void onConstruction() {}
			void onClear() {}
			void onInsert(const Iterator& element) {}
			void onErase(const Iterator& element) {}
			void onSpliceFrom(const Iterator& element) {}
			void onSplice(const Iterator& element) {}

			void updatePropagation(
				const Iterator& iter,
				integer& propagation)
			{
				propagation = 
					std::max(iter.left().propagation(), iter.right().propagation()) +
					iter.black();
			}

		private:
			Counting_Customization(const Counting_Customization& that) = delete;
			Counting_Customization(Counting_Customization&& that) = delete;
			Counting_Customization& operator=(Counting_Customization) = delete;
		};

		template <typename Data, bool MultipleKeys>
		using Counting_Settings =
			RedBlack_Settings<integer, Data, LessThan, integer, void, MultipleKeys>;

		using Set_Settings = Counting_Settings<void, false>;
		using MultiSet_Settings = Counting_Settings<void, true>;
		using Map_Settings = Counting_Settings<integer, false>;
		using MultiMap_Settings = Counting_Settings<integer, true>;

		using Set = RedBlackTree<Set_Settings, Counting_Customization>;
		using MultiSet = RedBlackTree<MultiSet_Settings, Counting_Customization>;
		using Map = RedBlackTree<Map_Settings, Counting_Customization>;
		using MultiMap = RedBlackTree<MultiMap_Settings, Counting_Customization>;

		virtual void run()
		{
			testSet();
			testMultiSet();
			testMap();
			testMultiMap();

			testMultiLowerBound<MultiSet>();
			testMultiLowerBound<MultiMap>();
			testMultiCount<MultiSet>();
			testMultiCount<MultiMap>();
			testMultiSplit();
			testMultiJoin();

			testManyThings<Set>();
			testManyThings<Map>();
			testManyThings<MultiSet>();
			testManyThings<MultiMap>();
		}

		template <typename Tree>
		void testManyThings()
		{
			testConstruction<Tree>();
			testIterator<Tree>();
			testInsert<Tree>();
			testErase<Tree>();
			testRandom<Tree>();
			testSplice<Tree>();
			testFind<Tree>();
			testLowerBound<Tree>();
			testUpperBound<Tree>();
			testJoin<Tree>();
			testQuantile<Tree>();
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
			using Tree = MultiSet;

			{
				Tree tree;

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
				Tree tree;

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
			{
				Tree aTree = { 1, 1, 2, 3, 4, 5, 5, 5 };
				TEST_ENSURE(testInvariants(aTree));
				TEST_ENSURE_OP(aTree.size(), == , 8);

				Tree bTree;
				bTree.useBottomFrom(aTree);
				bTree = { 5, 5, 6, 7, 7, 8 };
				TEST_ENSURE(testInvariants(bTree));
				TEST_ENSURE_OP(bTree.size(), == , 6);

				aTree.join(bTree);
				TEST_ENSURE(testInvariants(aTree));
				TEST_ENSURE(testInvariants(bTree));
				TEST_ENSURE_OP(aTree.size(), == , 14);
				TEST_ENSURE(bTree.empty());

				integer correctSet[] =
					{ 1, 1, 2, 3, 4, 5, 5, 5, 5, 5, 6, 7, 7, 8 };
				TEST_ENSURE(boost::equal(aTree.crange().dereferenceKey(), correctSet));
			}
		}

		template <typename Tree>
		void testMultiLowerBound()
		{
			{
				Tree tree;

				auto test = [&](integer that, integer index)
				{
					integer actualIndex =
						std::distance(tree.begin(), tree.lowerBound(that));

					return actualIndex == index;
				};

				{
					tree = { 2, 4, 4, 5, 5, 5, 5, 9, 15, 20 };
					TEST_ENSURE(testInvariants(tree));

					TEST_ENSURE(test(0, 0));
					TEST_ENSURE(test(1, 0));
					TEST_ENSURE(test(2, 0));
					TEST_ENSURE(test(3, 1));
					TEST_ENSURE(test(4, 1));
					TEST_ENSURE(test(5, 3));
					TEST_ENSURE(test(6, 7));
					TEST_ENSURE(test(7, 7));
					TEST_ENSURE(test(8, 7));
					TEST_ENSURE(test(9, 7));
					TEST_ENSURE(test(10, 8));
					TEST_ENSURE(test(11, 8));
					TEST_ENSURE(test(12, 8));
					TEST_ENSURE(test(13, 8));
					TEST_ENSURE(test(14, 8));
					TEST_ENSURE(test(15, 8));
					TEST_ENSURE(test(16, 9));
					TEST_ENSURE(test(17, 9));
					TEST_ENSURE(test(18, 9));
					TEST_ENSURE(test(19, 9));
					TEST_ENSURE(test(20, 9));
					TEST_ENSURE(test(21, 10));
					TEST_ENSURE(test(22, 10));
				}

				{
					tree = { 3, 4, 5, 5, 5, 5, 5, 5, 5, 6, 7 };
					TEST_ENSURE(testInvariants(tree));

					TEST_ENSURE(test(0, 0));
					TEST_ENSURE(test(1, 0));
					TEST_ENSURE(test(2, 0));
					TEST_ENSURE(test(3, 0));
					TEST_ENSURE(test(4, 1));
					TEST_ENSURE(test(5, 2));
					TEST_ENSURE(test(6, 9));
					TEST_ENSURE(test(7, 10));
					TEST_ENSURE(test(8, 11));
					TEST_ENSURE(test(9, 11));
				}
			}
		}

		void testMap()
		{
			using Tree = Map;

			{
				Tree tree{ { 1, 1 }, { 2, 4 }, { 3, 9 }, { 4, 16 }, { 5, 25 }, { 6, 36 }, { 7, 49 } };
				TEST_ENSURE(testInvariants(tree));
				TEST_ENSURE_OP(tree.size(), == , 7);

				integer keySet[] = { 1, 2, 3, 4, 5, 6, 7 };
				TEST_ENSURE(boost::equal(tree.crange().dereferenceKey(), keySet));

				integer dataSet[] = { 1, 4, 9, 16, 25, 36, 49 };
				TEST_ENSURE(boost::equal(tree.crange().dereferenceData(), dataSet));

				TEST_ENSURE_OP(*tree.begin().dereferenceKey(), == , 1);
				TEST_ENSURE_OP(*tree.begin().dereferenceData(), == , 1);
				TEST_ENSURE_OP(*std::next(tree.begin().dereferenceKey()), == , 2);
				TEST_ENSURE_OP(*std::next(tree.begin().dereferenceData()), == , 4);
			}
		}

		void testMultiMap()
		{
			using Tree = MultiMap;

			{
				Tree tree { { 1, 1 }, { 2, 4 }, { 3, 9 }, { 4, 16 }, { 5, 25 }, { 6, 36 }, { 7, 49 } };
				TEST_ENSURE(testInvariants(tree));
				TEST_ENSURE_OP(tree.size(), == , 7);

				integer keySet[] = { 1, 2, 3, 4, 5, 6, 7 };
				TEST_ENSURE(boost::equal(tree.crange().dereferenceKey(), keySet));

				integer dataSet[] = { 1, 4, 9, 16, 25, 36, 49 };
				TEST_ENSURE(boost::equal(tree.crange().dereferenceData(), dataSet));
			}
		}

		template <typename Tree>
		void testConstruction()
		{
			{
				Tree tree;
				TEST_ENSURE(testInvariants(tree));
				TEST_ENSURE(!tree.hasSeparateSentinels());
				TEST_ENSURE(!tree.sharesBottom());
				TEST_ENSURE(tree.sharesBottom(tree));
				TEST_ENSURE_OP(tree.size(), == , 0);
				TEST_ENSURE(tree.empty());
			}

			{
				Tree tree({ 1, 2, 3, 4, 5, 6, 7 });
				integer correctSet[] = { 1, 2, 3, 4, 5, 6, 7 };

				TEST_ENSURE(testInvariants(tree));
				TEST_ENSURE(!tree.hasSeparateSentinels());
				TEST_ENSURE(!tree.sharesBottom());
				TEST_ENSURE(tree.sharesBottom(tree));
				TEST_ENSURE_OP(tree.size(), == , 7);
				TEST_ENSURE(boost::equal(tree.crange().dereferenceKey(), correctSet));
			}
			{
				Tree tree{ 1, 2, 3, 4, 5, 6, 7 };
				integer correctSet[] = { 1, 2, 3, 4, 5, 6, 7 };

				Tree copy(tree);
				TEST_ENSURE(testInvariants(copy));
				TEST_ENSURE(copy.hasSeparateSentinels());
				TEST_ENSURE(copy.sharesBottom());
				TEST_ENSURE(copy.sharesBottom(copy));
				TEST_ENSURE(copy.sharesBottom(tree));
				TEST_ENSURE_OP(copy.size(), == , 7);
				TEST_ENSURE(boost::equal(copy.crange().dereferenceKey(), correctSet));

				TEST_ENSURE(testInvariants(tree));
				TEST_ENSURE(!tree.hasSeparateSentinels());
				TEST_ENSURE(tree.sharesBottom());
				TEST_ENSURE(tree.sharesBottom(tree));
				TEST_ENSURE(tree.sharesBottom(copy));
				TEST_ENSURE_OP(tree.size(), == , 7);
				TEST_ENSURE(boost::equal(tree.crange().dereferenceKey(), correctSet));
			}
			{
				Tree tree{ 1, 2, 3, 4, 5, 6, 7 };
				integer correctSet[] = { 1, 2, 3, 4, 5, 6, 7 };

				Tree copy{ 11, 12, 13, 14, 15, 16, 17 };
				copy = tree;
				TEST_ENSURE(testInvariants(copy));
				TEST_ENSURE(!copy.hasSeparateSentinels());
				TEST_ENSURE(!copy.sharesBottom());
				TEST_ENSURE(copy.sharesBottom(copy));
				TEST_ENSURE(!copy.sharesBottom(tree));
				TEST_ENSURE_OP(copy.size(), == , 7);
				TEST_ENSURE(boost::equal(copy.crange().dereferenceKey(), correctSet));

				TEST_ENSURE(testInvariants(tree));
				TEST_ENSURE(!tree.hasSeparateSentinels());
				TEST_ENSURE(!tree.sharesBottom());
				TEST_ENSURE(tree.sharesBottom(tree));
				TEST_ENSURE(!tree.sharesBottom(copy));
				TEST_ENSURE_OP(tree.size(), == , 7);
				TEST_ENSURE(boost::equal(tree.crange().dereferenceKey(), correctSet));
			}
			{
				Tree tree{ 1, 2, 3, 4, 5, 6, 7 };
				integer correctSet[] = { 1, 2, 3, 4, 5, 6, 7 };

				Tree moved(std::move(tree));
				TEST_ENSURE(testInvariants(moved));
				TEST_ENSURE(moved.hasSeparateSentinels());
				TEST_ENSURE(moved.sharesBottom());
				TEST_ENSURE(moved.sharesBottom(moved));
				TEST_ENSURE(moved.sharesBottom(tree));
				TEST_ENSURE_OP(moved.size(), == , 7);
				TEST_ENSURE(boost::equal(moved.crange().dereferenceKey(), correctSet));

				TEST_ENSURE(testInvariants(tree));
				TEST_ENSURE(!tree.hasSeparateSentinels());
				TEST_ENSURE(tree.sharesBottom());
				TEST_ENSURE(tree.sharesBottom(tree));
				TEST_ENSURE(tree.sharesBottom(moved));
				TEST_ENSURE_OP(tree.size(), == , 0);
				TEST_ENSURE(tree.empty());
			}
			{
				Tree tree{ 1, 2, 3, 4, 5, 6, 7 };
				integer correctSet[] = { 1, 2, 3, 4, 5, 6, 7 };
				Tree moved{ 11, 12, 13, 14, 15, 16, 17 };
				moved = std::move(tree);
				TEST_ENSURE(testInvariants(moved));
				TEST_ENSURE(moved.hasSeparateSentinels());
				TEST_ENSURE(moved.sharesBottom());
				TEST_ENSURE(moved.sharesBottom(moved));
				TEST_ENSURE(moved.sharesBottom(tree));
				TEST_ENSURE_OP(moved.size(), == , 7);
				TEST_ENSURE(boost::equal(moved.crange().dereferenceKey(), correctSet));

				TEST_ENSURE(testInvariants(tree));
				TEST_ENSURE(!tree.hasSeparateSentinels());
				TEST_ENSURE(tree.sharesBottom());
				TEST_ENSURE(tree.sharesBottom(tree));
				TEST_ENSURE(tree.sharesBottom(moved));
				TEST_ENSURE_OP(tree.size(), == , 0);
				TEST_ENSURE(tree.empty());
			}
			{
				Tree tree;
				Tree copy(tree);
				TEST_ENSURE(testInvariants(tree));
				TEST_ENSURE(testInvariants(copy));
			}
		}

		template <typename Tree>
		void testIterator()
		{
			using ConstIterator = typename Tree::ConstIterator;
			using Iterator = typename Tree::Iterator;

			Tree aTree{ 1, 2, 3, 4 };
			Tree bTree{ 5, 6, 7 };

			ConstIterator iter = aTree.cend();
			++iter;
			TEST_ENSURE(iter == aTree.cbegin());
			--iter;
			TEST_ENSURE(iter == aTree.cend());
			--iter;
			TEST_ENSURE(iter.key() == 4);

			Iterator bIter = aTree.cast(iter);
			iter = bIter;
			TEST_ENSURE(iter == bIter);
		}

		template <typename Tree>
		void testRandom()
		{
			Tree tree;

			std::list<integer> dataSet;

			const integer treeSizeSet[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 100};
			const integer treeSizes = sizeof(treeSizeSet) / sizeof(integer);

			for (integer k = 0;k < treeSizes;++k)
			{
				const integer treeSize = treeSizeSet[k];
				tree.clear();
				dataSet.clear();
				for (integer i = 0;i < 4 * treeSize;++i)
				{
					const uinteger n = randomUinteger() % 100;
					dataSet.push_back(n);

					tree.insert(n);
					TEST_ENSURE(testInvariants(tree));

					if (tree.size() > treeSize)
					{
						tree.erase(dataSet.front());
						TEST_ENSURE(testInvariants(tree));
						dataSet.pop_front();
					}
				}

				{
					Tree copy(tree);
					TEST_ENSURE(testInvariants(copy));
					TEST_ENSURE_OP(tree.size(), == , copy.size());
					TEST_ENSURE(boost::equal(tree.crange().dereferenceKey(), copy.crange().dereferenceKey()));
				}
			}
		}

		template <typename Tree>
		void testInsert()
		{
			using ConstIterator = typename Tree::ConstIterator;

			Tree tree;
			{
				TEST_ENSURE(tree.empty());
				TEST_ENSURE_OP(tree.size(), == , 0);
				TEST_ENSURE(testInvariants(tree));
			}

			tree.insert(1);
			{
				TEST_ENSURE(!tree.empty());
				TEST_ENSURE_OP(tree.size(), == , 1);
				TEST_ENSURE(testInvariants(tree));

				integer correctSet[] = { 1 };
				TEST_ENSURE(boost::equal(tree.crange().dereferenceKey(), correctSet));
			}

			tree.insert(5);
			{
				TEST_ENSURE(!tree.empty());
				TEST_ENSURE_OP(tree.size(), == , 2);
				TEST_ENSURE(testInvariants(tree));

				integer correctSet[] = { 1, 5 };
				TEST_ENSURE(boost::equal(tree.crange().dereferenceKey(), correctSet));
			}

			tree.insert(3);
			{
				TEST_ENSURE(!tree.empty());
				TEST_ENSURE_OP(tree.size(), == , 3);
				TEST_ENSURE(testInvariants(tree));

				integer correctSet[] = { 1, 3, 5 };
				TEST_ENSURE(boost::equal(tree.crange().dereferenceKey(), correctSet));
			}

			tree.insert(4);
			{
				TEST_ENSURE(!tree.empty());
				TEST_ENSURE_OP(tree.size(), == , 4);
				TEST_ENSURE(testInvariants(tree));

				integer correctSet[] = { 1, 3, 4, 5 };
				TEST_ENSURE(boost::equal(tree.crange().dereferenceKey(), correctSet));
			}

			tree.insert(8);
			{
				TEST_ENSURE(!tree.empty());
				TEST_ENSURE_OP(tree.size(), == , 5);
				TEST_ENSURE(testInvariants(tree));

				integer correctSet[] = { 1, 3, 4, 5, 8 };
				TEST_ENSURE(boost::equal(tree.crange().dereferenceKey(), correctSet));
			}

			tree.insert(7);
			{
				TEST_ENSURE(!tree.empty());
				TEST_ENSURE_OP(tree.size(), == , 6);
				TEST_ENSURE(testInvariants(tree));

				integer correctSet[] = { 1, 3, 4, 5, 7, 8 };
				TEST_ENSURE(boost::equal(tree.crange().dereferenceKey(), correctSet));
			}

			tree.insert(6);
			{
				TEST_ENSURE(!tree.empty());
				TEST_ENSURE_OP(tree.size(), == , 7);
				TEST_ENSURE(testInvariants(tree));

				integer correctSet[] = { 1, 3, 4, 5, 6, 7, 8 };
				TEST_ENSURE(boost::equal(tree.crange().dereferenceKey(), correctSet));
			}

			tree.insert(9);
			{
				TEST_ENSURE(!tree.empty());
				TEST_ENSURE_OP(tree.size(), == , 8);
				TEST_ENSURE(testInvariants(tree));

				integer correctSet[] = { 1, 3, 4, 5, 6, 7, 8, 9 };
				TEST_ENSURE(boost::equal(tree.crange().dereferenceKey(), correctSet));
			}

			tree.insert(2);
			{
				TEST_ENSURE(!tree.empty());
				TEST_ENSURE_OP(tree.size(), == , 9);
				TEST_ENSURE(testInvariants(tree));

				integer correctSet[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
				TEST_ENSURE(boost::equal(tree.crange().dereferenceKey(), correctSet));
			}

			integer correctSet[] =
			{
				1, 2, 3, 4, 5, 6, 7, 8, 9
			};

			{
				TEST_ENSURE_OP(boost::distance(tree), == , 9);
				TEST_ENSURE_OP(boost::distance(tree.crange().dereferenceKey()), == , 9);
				TEST_ENSURE(boost::equal(tree.crange().dereferenceKey(), correctSet));
			}

			{
				ConstIterator copyIter = tree.cend();
				++copyIter;
				TEST_ENSURE(copyIter == tree.cbegin());
			}

			{
				TEST_ENSURE(boost::equal(
					tree.crange().dereferenceKey() | boost::adaptors::reversed,
					correctSet | boost::adaptors::reversed));
			}

			{
				ConstIterator copyIter = tree.cbegin();
				--copyIter;
				TEST_ENSURE(copyIter == tree.cend());
			}
		}

		template <typename Tree>
		void testErase()
		{
			Tree tree{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			{
				integer correctSet[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
				TEST_ENSURE_OP(tree.size(), == , 9);
				TEST_ENSURE(boost::equal(tree.crange().dereferenceKey(), correctSet));
				TEST_ENSURE(testInvariants(tree));
			}

			tree.erase(0);
			{
				integer correctSet[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
				TEST_ENSURE_OP(tree.size(), == , 9);
				TEST_ENSURE(boost::equal(tree.crange().dereferenceKey(), correctSet));
				TEST_ENSURE(testInvariants(tree));
			}
			tree.erase(4);
			{
				integer correctSet[] = { 1, 2, 3, 5, 6, 7, 8, 9 };
				TEST_ENSURE_OP(tree.size(), == , 8);
				TEST_ENSURE(boost::equal(tree.crange().dereferenceKey(), correctSet));
				TEST_ENSURE(testInvariants(tree));
			}

			tree.erase(7);
			{
				integer correctSet[] = { 1, 2, 3, 5, 6, 8, 9 };
				TEST_ENSURE_OP(tree.size(), == , 7);
				TEST_ENSURE(boost::equal(tree.crange().dereferenceKey(), correctSet));
				TEST_ENSURE(testInvariants(tree));
			}

			tree.erase(1);
			{
				integer correctSet[] = { 2, 3, 5, 6, 8, 9 };
				TEST_ENSURE_OP(tree.size(), == , 6);
				TEST_ENSURE(boost::equal(tree.crange().dereferenceKey(), correctSet));
				TEST_ENSURE(testInvariants(tree));
			}

			tree.erase(9);
			{
				integer correctSet[] = { 2, 3, 5, 6, 8};
				TEST_ENSURE_OP(tree.size(), == , 5);
				TEST_ENSURE(boost::equal(tree.crange().dereferenceKey(), correctSet));
				TEST_ENSURE(testInvariants(tree));
			}

			tree.erase(5);
			{
				integer correctSet[] = { 2, 3, 6, 8 };
				TEST_ENSURE_OP(tree.size(), == , 4);
				TEST_ENSURE(boost::equal(tree.crange().dereferenceKey(), correctSet));
				TEST_ENSURE(testInvariants(tree));
			}

			tree.erase(3);
			{
				integer correctSet[] = { 2, 6, 8 };
				TEST_ENSURE_OP(tree.size(), == , 3);
				TEST_ENSURE(boost::equal(tree.crange().dereferenceKey(), correctSet));
				TEST_ENSURE(testInvariants(tree));
			}

			tree.erase(2);
			{
				integer correctSet[] = { 6, 8 };
				TEST_ENSURE_OP(tree.size(), == , 2);
				TEST_ENSURE(boost::equal(tree.crange().dereferenceKey(), correctSet));
				TEST_ENSURE(testInvariants(tree));
			}

			tree.erase(6);
			{
				integer correctSet[] = { 8 };
				TEST_ENSURE_OP(tree.size(), == , 1);
				TEST_ENSURE(boost::equal(tree.crange().dereferenceKey(), correctSet));
				TEST_ENSURE(testInvariants(tree));
			}

			tree.erase(8);
			{
				TEST_ENSURE_OP(tree.size(), == , 0);
				TEST_ENSURE(tree.empty());
				TEST_ENSURE(testInvariants(tree));
			}
		}

		template <typename Tree>
		void testSplice()
		{
			Tree a{ 0, 1, 4, 5, 9, 15, 20 };
			TEST_ENSURE(testInvariants(a));

			Tree b{ 1, 2, 6, 7, 8, 9, 10 };
			TEST_ENSURE(testInvariants(b));

			{
				a.splice(b, b.find(6));
				
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE_OP(a.size(), == , 8);
				integer aCorrectSet[] = { 0, 1, 4, 5, 6, 9, 15, 20 };
				TEST_ENSURE(boost::equal(a.crange().dereferenceKey(), aCorrectSet));

				TEST_ENSURE(testInvariants(b));
				TEST_ENSURE_OP(b.size(), == , 6);
				integer bCorrectSet[] = { 1, 2, 7, 8, 9, 10 };
				TEST_ENSURE(boost::equal(b.crange().dereferenceKey(), bCorrectSet));
			}

			{
				a.splice(b, b.find(10));

				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE_OP(a.size(), == , 9);
				integer aCorrectSet[] = { 0, 1, 4, 5, 6, 9, 10, 15, 20 };
				TEST_ENSURE(boost::equal(a.crange().dereferenceKey(), aCorrectSet));

				TEST_ENSURE(testInvariants(b));
				TEST_ENSURE_OP(b.size(), == , 5);
				integer bCorrectSet[] = { 1, 2, 7, 8, 9};
				TEST_ENSURE(boost::equal(b.crange().dereferenceKey(), bCorrectSet));
			}
		}

		template <typename Tree>
		void print(const Tree& tree, integer height, bool key = true)
		{
			using ConstIterator = typename Tree::ConstIterator;

			std::list<std::pair<ConstIterator, integer>> nodeSet;
			nodeSet.push_back(std::make_pair(tree.croot(), (integer)0));
			integer prevLevel = 0;
			while (!nodeSet.empty() && nodeSet.front().second < height)
			{
				auto entry = nodeSet.front();
				nodeSet.pop_front();

				if (entry.second != prevLevel)
				{
					std::cout << std::endl;
					prevLevel = entry.second;
				}

				if (!entry.first.isSentinel())
				{
					if (key)
					{
						std::cout << entry.first.key();
					}
					else
					{
						std::cout << entry.first.data();
					}
					if (entry.first.red())
					{
						std::cout << "R";
					}
					else
					{
						std::cout << "B";
					}
					std::cout << " ";
				}
				else
				{
					std::cout << "- ";
				}

				nodeSet.push_back(
					std::make_pair(entry.first.left(), entry.second + 1));

				ConstIterator right = entry.first.isSentinel() ?
					tree.cend() : entry.first.right();
				nodeSet.push_back(
					std::make_pair(right, entry.second + 1));
			}
			std::cout << std::endl;

			if (key)
			{
				for (auto&& key : tree.crange().dereferenceKey())
				{
					std::cout << key << ", ";
				}
			}
			else
			{
				for (auto&& data : tree.crange().dereferenceData())
				{
					std::cout << data << ", ";
				}
			}
			std::cout << std::endl;
		}

		template <typename Tree>
		void testFind()
		{
			Tree tree{ 2, 4, 4, 5, 5, 5, 5, 9, 15, 20 };
			TEST_ENSURE(testInvariants(tree));

			auto test = [&](integer that)
			{
				return tree.exists(that) &&
					tree.find(that) != tree.cend() &&
					tree.find(that).key() == that;
			};

			auto notFound = [&](integer that)
			{
				return (tree.find(that) == tree.cend()) &&
					!tree.exists(that);
			};

			TEST_ENSURE(notFound(0));
			TEST_ENSURE(notFound(1));
			TEST_ENSURE(notFound(3));
			TEST_ENSURE(notFound(6));
			TEST_ENSURE(notFound(7));
			TEST_ENSURE(notFound(8));
			TEST_ENSURE(notFound(10));
			TEST_ENSURE(notFound(11));
			TEST_ENSURE(notFound(12));
			TEST_ENSURE(notFound(13));
			TEST_ENSURE(notFound(14));
			TEST_ENSURE(notFound(16));
			TEST_ENSURE(notFound(17));
			TEST_ENSURE(notFound(18));
			TEST_ENSURE(notFound(19));
			TEST_ENSURE(notFound(21));

			TEST_ENSURE(test(2));
			TEST_ENSURE(test(4));
			TEST_ENSURE(test(5));
			TEST_ENSURE(test(9));
			TEST_ENSURE(test(15));
			TEST_ENSURE(test(20));
		}

		template <typename Tree>
		void testLowerBound()
		{
			Tree tree;
		}

		template <typename Tree>
		void testUpperBound()
		{
			Tree tree{ 2, 4, 4, 5, 5, 5, 5, 9, 15, 20 };
			TEST_ENSURE(testInvariants(tree));

			auto test = [&](integer that, integer bound)
			{
				return tree.upperBound(that) != tree.cend() &&
					tree.upperBound(that).key() == bound;
			};

			auto notFound = [&](integer that)
			{
				return tree.upperBound(that) == tree.cend();
			};

			TEST_ENSURE(test(0, 2));
			TEST_ENSURE(test(1, 2));
			TEST_ENSURE(test(2, 4));
			TEST_ENSURE(test(3, 4));
			TEST_ENSURE(test(4, 5));
			TEST_ENSURE(test(5, 9));
			TEST_ENSURE(test(6, 9));
			TEST_ENSURE(test(7, 9));
			TEST_ENSURE(test(8, 9));
			TEST_ENSURE(test(9, 15));
			TEST_ENSURE(test(10, 15));
			TEST_ENSURE(test(11, 15));
			TEST_ENSURE(test(12, 15));
			TEST_ENSURE(test(13, 15));
			TEST_ENSURE(test(14, 15));
			TEST_ENSURE(test(15, 20));
			TEST_ENSURE(test(16, 20));
			TEST_ENSURE(test(17, 20));
			TEST_ENSURE(test(18, 20));
			TEST_ENSURE(test(19, 20));
			TEST_ENSURE(notFound(20));
			TEST_ENSURE(notFound(21));
		}

		template <typename Tree>
		void testJoin()
		{
			{
				Tree aTree = { 1, 2, 3, 4, 5 };
				TEST_ENSURE(testInvariants(aTree));

				Tree bTree;
				bTree.useBottomFrom(aTree);
				TEST_ENSURE(testInvariants(bTree));
				TEST_ENSURE(bTree.sharesBottom(aTree));

				aTree.join(bTree);
				TEST_ENSURE(testInvariants(aTree));
				TEST_ENSURE(testInvariants(bTree));
				TEST_ENSURE(bTree.empty());

				integer correctSet[] =
					{ 1, 2, 3, 4, 5 };
				TEST_ENSURE(boost::equal(aTree.crange().dereferenceKey(), correctSet));

				bTree.join(aTree);
				TEST_ENSURE(testInvariants(aTree));
				TEST_ENSURE(testInvariants(bTree));
				TEST_ENSURE(aTree.empty());
				TEST_ENSURE(boost::equal(bTree.crange().dereferenceKey(), correctSet));
			}

			{
				Tree aTree = { 1, 2, 3, 4, 5 };
				TEST_ENSURE(testInvariants(aTree));

				Tree bTree;
				bTree.useBottomFrom(aTree);
				bTree = { 6, 7, 8 };
				TEST_ENSURE(testInvariants(bTree));

				bTree.join(aTree);
				TEST_ENSURE(testInvariants(aTree));
				TEST_ENSURE(testInvariants(bTree));
				TEST_ENSURE(aTree.empty());

				integer correctSet[] =
					{ 1, 2, 3, 4, 5, 6, 7, 8 };
				TEST_ENSURE(boost::equal(bTree.crange().dereferenceKey(), correctSet));
			}

			{
				Tree aTree = { 1, 2, 3, 4, 5 };
				TEST_ENSURE(testInvariants(aTree));

				Tree bTree;
				bTree.useBottomFrom(aTree);
				bTree = { 6, 7, 8 };
				TEST_ENSURE(testInvariants(bTree));

				aTree.join(bTree);
				TEST_ENSURE(testInvariants(aTree));
				TEST_ENSURE(testInvariants(bTree));
				TEST_ENSURE(bTree.empty());

				integer correctSet[] = 
					{1, 2, 3, 4, 5, 6, 7, 8};
				TEST_ENSURE(boost::equal(aTree.crange().dereferenceKey(), correctSet));
			}
		}

		void testMultiSplit()
		{
			using Tree = MultiMap;
			{
				Tree tree;
				std::vector<integer> dataSet;
				integer n = 5000;
				for (integer i = 0;i < n;++i)
				{
					integer data = randomInteger(65536);
					tree.insert(0, data);
					dataSet.push_back(data);
					//TEST_ENSURE(testInvariants(tree));
				}

				std::vector<integer> correctSet = dataSet;
				TEST_ENSURE(boost::equal(tree.range().dereferenceData(), correctSet));

				//for (integer i = 0;i < n;++i)
				integer i = 3071;
				{
					Tree aTree = tree;
					TEST_ENSURE(testInvariants(aTree));

					Tree bTree = aTree.split(std::next(aTree.cbegin(), i));
					TEST_ENSURE(testInvariants(aTree));
					TEST_ENSURE_OP(aTree.size(), == , i);
					TEST_ENSURE(testInvariants(bTree));
					TEST_ENSURE_OP(bTree.size(), == , n - i);

					if (!testInvariants(aTree))
					{
						std::cout << i << std::endl;
						testInvariants(aTree);
						//break;
					}

					aTree.join(bTree);
					TEST_ENSURE(testInvariants(aTree));
					TEST_ENSURE_OP(aTree.size(), == , n);
					TEST_ENSURE(boost::equal(aTree.range().dereferenceData(), correctSet));
					TEST_ENSURE(testInvariants(bTree));
					TEST_ENSURE_OP(bTree.size(), == , 0);
				}
			}
		}

		void testMultiJoin()
		{
			using Tree = MultiMap;
			{
				integer n = 50;
				for (integer i = 0; i < n; ++i)
				{
					for (integer j = 0; j < n; ++j)
					{
						std::vector<integer> correctSet;

						Tree aTree;
						for (integer k = 0; k < i; ++k)
						{
							integer data = randomInteger(65536);
							correctSet.push_back(data);

							aTree.insert(0, data);
							TEST_ENSURE(testInvariants(aTree));
						}

						Tree bTree;
						bTree.useBottomFrom(aTree);

						for (integer k = 0; k < j; ++k)
						{
							integer data = randomInteger(65536);
							correctSet.push_back(data);

							bTree.insert(0, data);
							TEST_ENSURE(testInvariants(bTree));
						}

						aTree.join(bTree);
						TEST_ENSURE(testInvariants(aTree));
						TEST_ENSURE(testInvariants(bTree));
						TEST_ENSURE(boost::equal(aTree.crange().dereferenceData(), correctSet));
					}
				}
			}
		}

		template <typename Tree>
		void testQuantile()
		{
			Tree tree {0, 1, 2, 3, 4};

			using ConstIterator = typename Tree::ConstIterator;

			auto test = [&](real alpha, integer correct)
			{
				ConstIterator q = quantile(tree, alpha);
				return q != tree.cend() && 
					q.key() == correct;
			};

			TEST_ENSURE(test(-0.10, 0));
			TEST_ENSURE(test(0.00, 0));
			TEST_ENSURE(test(0.10, 0));
			TEST_ENSURE(test(0.19, 0));
			TEST_ENSURE(test(0.20, 1));
			TEST_ENSURE(test(0.29, 1));
			TEST_ENSURE(test(0.30, 1));
			TEST_ENSURE(test(0.39, 1));
			TEST_ENSURE(test(0.40, 2));
			TEST_ENSURE(test(0.49, 2));
			TEST_ENSURE(test(0.50, 2));
			TEST_ENSURE(test(0.59, 2));
			TEST_ENSURE(test(0.60, 3));
			TEST_ENSURE(test(0.69, 3));
			TEST_ENSURE(test(0.70, 3));
			TEST_ENSURE(test(0.79, 3));
			TEST_ENSURE(test(0.80, 4));
			TEST_ENSURE(test(0.89, 4));
			TEST_ENSURE(test(0.90, 4));
			TEST_ENSURE(test(0.99, 4));
			TEST_ENSURE(test(1.00, 4));
			TEST_ENSURE(test(1.09, 4));
			TEST_ENSURE(test(1.10, 4));
			TEST_ENSURE(test(1.19, 4));
		}

		template <typename Tree>
		void testMultiCount()
		{
			Tree tree = { 3, 4, 5, 5, 5, 5, 5, 5, 5, 6, 7 };

			auto test = [&](integer key, integer count)
			{
				return tree.count(key) == count;
			};

			TEST_ENSURE(test(0, 0));
			TEST_ENSURE(test(1, 0));
			TEST_ENSURE(test(2, 0));
			TEST_ENSURE(test(3, 1));
			TEST_ENSURE(test(4, 1));
			TEST_ENSURE(test(5, 7));
			TEST_ENSURE(test(6, 1));
			TEST_ENSURE(test(7, 1));
			TEST_ENSURE(test(8, 0));
			TEST_ENSURE(test(9, 0));
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
