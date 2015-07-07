// Description: Testing for red-black tree
// DocumentationOf: redblacktree.h

#include "test/test_init.h"

#include <pastel/sys/redblacktree.h>
#include <pastel/sys/random/random_uniform.h>
#include <pastel/sys/random/random_integer.h>
#include <pastel/sys/downfilter.h>

#include <boost/range/adaptor/reversed.hpp> 

#include <algorithm>
#include <iostream>
#include <list>

namespace
{

	class Test
	{
	public:
		class Propagation
		{
		public:
			integer blackHeight;
			bool marked;
		};

		template <typename Settings_>
		class Map_Counting_Customization
			: public Empty_RedBlackTree_Customization<Settings_>
		{
		protected:
			using Fwd = RedBlackTree_Fwd<Settings_>;
			PASTEL_FWD(Iterator);
			PASTEL_FWD(Propagation_Class);

			Map_Counting_Customization() {}

			void updatePropagation(
				const Iterator& iter,
				Propagation_Class& propagation)
			{
				propagation.marked =
					iter.data().marked ||
					iter.left().propagation().marked ||
					iter.right().propagation().marked;
				propagation.blackHeight = 
					std::max(iter.left().propagation().blackHeight, iter.right().propagation().blackHeight) +
					iter.black();
			}

		private:
			Map_Counting_Customization(const Map_Counting_Customization& that) = delete;
			Map_Counting_Customization(Map_Counting_Customization&& that) = delete;
			Map_Counting_Customization& operator=(Map_Counting_Customization) = delete;
		};

		template <typename Settings_>
		class Set_Counting_Customization
			: public Empty_RedBlackTree_Customization<Settings_>
		{
		protected:
			using Fwd = RedBlackTree_Fwd<Settings_>;
			PASTEL_FWD(Iterator);
			PASTEL_FWD(Propagation_Class);

			Set_Counting_Customization() {}

			void updatePropagation(
				const Iterator& iter,
				Propagation_Class& propagation)
			{
				propagation.blackHeight =
					std::max(iter.left().propagation().blackHeight, iter.right().propagation().blackHeight) +
					iter.black();
			}

		private:
			Set_Counting_Customization(const Set_Counting_Customization& that) = delete;
			Set_Counting_Customization(Set_Counting_Customization&& that) = delete;
			Set_Counting_Customization& operator=(Set_Counting_Customization) = delete;
		};

		template <typename Data_, bool MultipleKeys>
		using Counting_Settings =
			RedBlackTree_Set_Settings<integer, Data_, LessThan, Propagation, void, MultipleKeys>;

		class Data
		{
		public:
			Data(integer value_ = 0, bool marked_ = false)
				: value(value_)
				, marked(marked_)
			{
			}

			operator integer() const
			{
				return value;
			}

			integer value;
			bool marked;
		};

		using Set_Settings = Counting_Settings<void, false>;
		using MultiSet_Settings = Counting_Settings<void, true>;
		using Map_Settings = Counting_Settings<Data, false>;
		using MultiMap_Settings = Counting_Settings<Data, true>;

		using Set = RedBlackTree<Set_Settings, Set_Counting_Customization>;
		using MultiSet = RedBlackTree<MultiSet_Settings, Set_Counting_Customization>;
		using Map = RedBlackTree<Map_Settings, Map_Counting_Customization>;
		using MultiMap = RedBlackTree<MultiMap_Settings, Map_Counting_Customization>;

		virtual void run()
		{
			testSet();
			testMultiSet();
			testMap();
			testMultiMap();
			testVoidSet();

			testMapFilteredIterator<Map>();
			testMapFilteredIterator<MultiMap>();

			testMapFilteredSearch<Map>();
			testMapFilteredSearch<MultiMap>();

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

		void testVoidSet()
		{
			using Set = RedBlackTree_Set<void>;
			Set tree;
			REQUIRE(testInvariants(tree));
			tree.insert(nullptr);
		}

		template <typename Tree>
		void testMapFilteredIterator()
		{
			using ConstIterator = typename Tree::ConstIterator;

			Tree map
			{ 
				{ 2, { 0, false } },
				{ 4, { 0, false } },
				{ 5, { 0, true } },
				{ 6, { 0, false } },
				{ 9, { 0, false } },
				{ 10, { 0, true } },
				{ 14, { 0, true } },
				{ 16, { 0, false } },
				{ 19, { 0, false } },
				{ 20, { 0, true } }
			};

			auto onlyMarked = indicatorDownFilter(
				[](const ConstIterator& that) {return that.data().marked;},
				[](const ConstIterator& that) {return that.propagation().marked;}
			);

			{
				auto test = [&](const ConstIterator& i, integer correct)
				{
					return !i.isSentinel() &&
						i.key() == correct;
				};

				ConstIterator i = map.cbegin();
				REQUIRE(test(i, 2));

				i = i.next(onlyMarked);
				REQUIRE(test(i, 5));

				i = i.next(onlyMarked);
				REQUIRE(test(i, 10));

				i = i.next(onlyMarked);
				REQUIRE(test(i, 14));

				i = i.next(onlyMarked);
				REQUIRE(test(i, 20));

				i = i.next(onlyMarked);
				REQUIRE(i == map.cend());

				i = i.prev(onlyMarked);
				REQUIRE(test(i, 20));

				i = i.prev(onlyMarked);
				REQUIRE(test(i, 14));

				i = i.prev(onlyMarked);
				REQUIRE(test(i, 10));

				i = i.prev(onlyMarked);
				REQUIRE(test(i, 5));

				i = i.prev(onlyMarked);
				REQUIRE(i == map.cend());

				i = map.begin();
				REQUIRE(test(i, 2));

				++i;
				REQUIRE(test(i, 4));

				++i;
				REQUIRE(test(i, 5));

				++i;
				REQUIRE(test(i, 6));

				++i;
				REQUIRE(test(i, 9));

				++i;
				REQUIRE(test(i, 10));

				++i;
				REQUIRE(test(i, 14));

				++i;
				REQUIRE(test(i, 16));

				++i;
				REQUIRE(test(i, 19));

				++i;
				REQUIRE(test(i, 20));

				++i;
				REQUIRE(i == map.cend());

				--i;
				REQUIRE(test(i, 20));

				--i;
				REQUIRE(test(i, 19));

				--i;
				REQUIRE(test(i, 16));

				--i;
				REQUIRE(test(i, 14));

				--i;
				REQUIRE(test(i, 10));

				--i;
				REQUIRE(test(i, 9));

				--i;
				REQUIRE(test(i, 6));

				--i;
				REQUIRE(test(i, 5));

				--i;
				REQUIRE(test(i, 4));

				--i;
				REQUIRE(test(i, 2));

				--i;
				REQUIRE(i == map.cend());
			}
		}

		template <typename Tree>
		void testMapFilteredSearch()
		{
			using ConstIterator = typename Tree::ConstIterator;

			Tree map
			{ 
				{ 2, { 0, false } },
				{ 4, { 0, false } },
				{ 5, { 0, true } },
				{ 6, { 0, false } },
				{ 9, { 0, false } },
				{ 10, { 0, true } },
				{ 14, { 0, true } },
				{ 16, { 0, false } },
				{ 19, { 0, false } },
				{ 20, { 0, true } }
			};

			auto onlyMarked = indicatorDownFilter(
				[](const ConstIterator& that) {return that.data().marked; },
				[](const ConstIterator& that) {return that.propagation().marked; }
			);

			{
				auto test = [&](integer i)
				{
					ConstIterator iter = 
						map.find(i, onlyMarked);

					return !iter.isSentinel() &&
						iter.key() == i &&
						map.exists(i, onlyMarked);
				};

				REQUIRE(!test(2));
				REQUIRE(!test(4));
				REQUIRE(!test(6));
				REQUIRE(!test(9));
				REQUIRE(!test(16));
				REQUIRE(!test(19));

				REQUIRE(test(5));
				REQUIRE(test(10));
				REQUIRE(test(14));
				REQUIRE(test(20));
			}
			{
				auto test = [&](integer i, integer correct)
				{
					ConstIterator iter = 
						map.lowerBound(i, onlyMarked);

					return !iter.isSentinel() &&
						iter.key() == correct;
				};

				REQUIRE(test(-1, 5));
				REQUIRE(test(0, 5));
				REQUIRE(test(1, 5));
				REQUIRE(test(2, 5));
				REQUIRE(test(3, 5));
				REQUIRE(test(4, 5));
				REQUIRE(test(5, 5));
				REQUIRE(test(6, 10));
				REQUIRE(test(7, 10));
				REQUIRE(test(8, 10));
				REQUIRE(test(9, 10));
				REQUIRE(test(10, 10));
				REQUIRE(test(11, 14));
				REQUIRE(test(12, 14));
				REQUIRE(test(13, 14));
				REQUIRE(test(14, 14));
				REQUIRE(test(15, 20));
				REQUIRE(test(16, 20));
				REQUIRE(test(17, 20));
				REQUIRE(test(18, 20));
				REQUIRE(test(19, 20));
				REQUIRE(test(20, 20));
			}

			{
				auto test = [&](integer i, integer correct)
				{
					ConstIterator iter = 
						map.upperBound(i, onlyMarked);

					return !iter.isSentinel() &&
						iter.key() == correct;
				};

				REQUIRE(test(-1, 5));
				REQUIRE(test(0, 5));
				REQUIRE(test(1, 5));
				REQUIRE(test(2, 5));
				REQUIRE(test(3, 5));
				REQUIRE(test(4, 5));
				REQUIRE(test(5, 10));
				REQUIRE(test(6, 10));
				REQUIRE(test(7, 10));
				REQUIRE(test(8, 10));
				REQUIRE(test(9, 10));
				REQUIRE(test(10, 14));
				REQUIRE(test(11, 14));
				REQUIRE(test(12, 14));
				REQUIRE(test(13, 14));
				REQUIRE(test(14, 20));
				REQUIRE(test(15, 20));
				REQUIRE(test(16, 20));
				REQUIRE(test(17, 20));
				REQUIRE(test(18, 20));
				REQUIRE(test(19, 20));
				REQUIRE(map.upperBound(20) == map.cend());
			}
		}

		void testSet()
		{
			{
				Set tree;
				auto test = [&](integer that)
				{
					auto iterAndNew = tree.insert(that);
					REQUIRE(testInvariants(tree));
					REQUIRE(iterAndNew.second);
					REQUIRE(*iterAndNew.first == that);
					REQUIRE(tree.find(that) == iterAndNew.first);
					REQUIRE(tree.lowerBound(that) == iterAndNew.first);
					REQUIRE(tree.exists(that));
				};

				test(1);
				REQUIRE(tree.size() == 1);

				test(5);
				REQUIRE(tree.size() == 2);

				test(3);
				REQUIRE(tree.size() == 3);

				auto iterAndNew = tree.insert(1);
				REQUIRE(!iterAndNew.second);
				REQUIRE(*iterAndNew.first == 1);
				REQUIRE(tree.size() == 3);
			}
			{
				Set tree({ 4, 2, 1, 1, 1, 3 });
				integer correctSet[] = { 1, 2, 3, 4 };
				REQUIRE(tree.size() == 4);
				REQUIRE(boost::equal(tree, correctSet));
			}
			{
				Set tree;
				tree = { 4, 2, 1, 1, 1, 3 };

				integer correctSet[] = { 1, 2, 3, 4 };
				REQUIRE(tree.size() == 4);
				REQUIRE(boost::equal(tree, correctSet));
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
					REQUIRE(testInvariants(tree));
					REQUIRE(*iter == that);
					REQUIRE(tree.find(that) == iter);
					REQUIRE(tree.lowerBound(that) == iter);
					REQUIRE(tree.exists(that));
				};

				test(1);
				REQUIRE(tree.size() == 1);

				test(5);
				REQUIRE(tree.size() == 2);

				test(3);
				REQUIRE(tree.size() == 3);
			}
			{
				Tree tree;

				auto test = [&](integer that)
				{
					auto iter = tree.insert(that);
					REQUIRE(testInvariants(tree));
					REQUIRE(*iter == that);
					REQUIRE(iter == std::prev(tree.cend()));
				};

				test(1);
				REQUIRE(tree.size() == 1);

				test(1);
				REQUIRE(tree.size() == 2);

				test(1);
				REQUIRE(tree.size() == 3);

				test(5);
				REQUIRE(tree.size() == 4);

				test(5);
				REQUIRE(tree.size() == 5);

				test(5);
				REQUIRE(tree.size() == 6);
			}
			{
				Tree aTree = { 1, 1, 2, 3, 4, 5, 5, 5 };
				REQUIRE(testInvariants(aTree));
				REQUIRE(aTree.size() == 8);

				Tree bTree;
				bTree.useBottomFrom(aTree);
				bTree = { 5, 5, 6, 7, 7, 8 };
				REQUIRE(testInvariants(bTree));
				REQUIRE(bTree.size() == 6);

				aTree.join(bTree);
				REQUIRE(testInvariants(aTree));
				REQUIRE(testInvariants(bTree));
				REQUIRE(aTree.size() == 14);
				REQUIRE(bTree.empty());

				integer correctSet[] =
					{ 1, 1, 2, 3, 4, 5, 5, 5, 5, 5, 6, 7, 7, 8 };
				REQUIRE(boost::equal(aTree.crange().dereferenceKey(), correctSet));
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
					REQUIRE(testInvariants(tree));

					REQUIRE(test(0, 0));
					REQUIRE(test(1, 0));
					REQUIRE(test(2, 0));
					REQUIRE(test(3, 1));
					REQUIRE(test(4, 1));
					REQUIRE(test(5, 3));
					REQUIRE(test(6, 7));
					REQUIRE(test(7, 7));
					REQUIRE(test(8, 7));
					REQUIRE(test(9, 7));
					REQUIRE(test(10, 8));
					REQUIRE(test(11, 8));
					REQUIRE(test(12, 8));
					REQUIRE(test(13, 8));
					REQUIRE(test(14, 8));
					REQUIRE(test(15, 8));
					REQUIRE(test(16, 9));
					REQUIRE(test(17, 9));
					REQUIRE(test(18, 9));
					REQUIRE(test(19, 9));
					REQUIRE(test(20, 9));
					REQUIRE(test(21, 10));
					REQUIRE(test(22, 10));
				}

				{
					tree = { 3, 4, 5, 5, 5, 5, 5, 5, 5, 6, 7 };
					REQUIRE(testInvariants(tree));

					REQUIRE(test(0, 0));
					REQUIRE(test(1, 0));
					REQUIRE(test(2, 0));
					REQUIRE(test(3, 0));
					REQUIRE(test(4, 1));
					REQUIRE(test(5, 2));
					REQUIRE(test(6, 9));
					REQUIRE(test(7, 10));
					REQUIRE(test(8, 11));
					REQUIRE(test(9, 11));
				}
			}
		}

		void testMap()
		{
			using Tree = Map;

			{
				Tree tree{ { 1, 1 }, { 2, 4 }, { 3, 9 }, { 4, 16 }, { 5, 25 }, { 6, 36 }, { 7, 49 } };
				REQUIRE(testInvariants(tree));
				REQUIRE(tree.size() == 7);

				integer keySet[] = { 1, 2, 3, 4, 5, 6, 7 };
				REQUIRE(boost::equal(tree.crange().dereferenceKey(), keySet));

				integer dataSet[] = { 1, 4, 9, 16, 25, 36, 49 };
				REQUIRE(boost::equal(tree.crange().dereferenceData(), dataSet));

				REQUIRE(*tree.begin().dereferenceKey() == 1);
				REQUIRE(*tree.begin().dereferenceData() == 1);
				REQUIRE(*std::next(tree.begin().dereferenceKey()) == 2);
				REQUIRE(*std::next(tree.begin().dereferenceData()) == 4);
			}
			{
				Map tree;
				auto a = tree.insert(5).first;

				*a = 4;
				REQUIRE(*a == 4);

				auto b = tree.insert(1, *a).first;
				REQUIRE(*b == 4);

			}
		}

		void testMultiMap()
		{
			using Tree = MultiMap;

			{
				Tree tree { { 1, 1 }, { 2, 4 }, { 3, 9 }, { 4, 16 }, { 5, 25 }, { 6, 36 }, { 7, 49 } };
				REQUIRE(testInvariants(tree));
				REQUIRE(tree.size() == 7);

				integer keySet[] = { 1, 2, 3, 4, 5, 6, 7 };
				REQUIRE(boost::equal(tree.crange().dereferenceKey(), keySet));

				integer dataSet[] = { 1, 4, 9, 16, 25, 36, 49 };
				REQUIRE(boost::equal(tree.crange().dereferenceData(), dataSet));
			}
		}

		template <typename Tree>
		void testConstruction()
		{
			{
				Tree tree;
				REQUIRE(testInvariants(tree));
				REQUIRE(!tree.hasSeparateSentinels());
				REQUIRE(!tree.sharesBottom());
				REQUIRE(tree.sharesBottom(tree));
				REQUIRE(tree.size() == 0);
				REQUIRE(tree.empty());
			}

			{
				Tree tree({ 1, 2, 3, 4, 5, 6, 7 });
				integer correctSet[] = { 1, 2, 3, 4, 5, 6, 7 };

				REQUIRE(tree.extremum(false) == tree.cbegin());
				REQUIRE(tree.extremum(false).key() == 1);
				REQUIRE(tree.extremum(true) == tree.clast());
				REQUIRE(tree.extremum(true).key() == 7);

				REQUIRE(testInvariants(tree));
				REQUIRE(!tree.hasSeparateSentinels());
				REQUIRE(!tree.sharesBottom());
				REQUIRE(tree.sharesBottom(tree));
				REQUIRE(tree.size() == 7);
				REQUIRE(boost::equal(tree.crange().dereferenceKey(), correctSet));
			}
			{
				Tree tree{ 1, 2, 3, 4, 5, 6, 7 };
				integer correctSet[] = { 1, 2, 3, 4, 5, 6, 7 };

				Tree copy(tree);
				REQUIRE(testInvariants(copy));
				REQUIRE(copy.hasSeparateSentinels());
				REQUIRE(copy.sharesBottom());
				REQUIRE(copy.sharesBottom(copy));
				REQUIRE(copy.sharesBottom(tree));
				REQUIRE(copy.size() == 7);
				REQUIRE(boost::equal(copy.crange().dereferenceKey(), correctSet));

				REQUIRE(testInvariants(tree));
				REQUIRE(!tree.hasSeparateSentinels());
				REQUIRE(tree.sharesBottom());
				REQUIRE(tree.sharesBottom(tree));
				REQUIRE(tree.sharesBottom(copy));
				REQUIRE(tree.size() == 7);
				REQUIRE(boost::equal(tree.crange().dereferenceKey(), correctSet));
			}
			{
				Tree tree{ 1, 2, 3, 4, 5, 6, 7 };
				integer correctSet[] = { 1, 2, 3, 4, 5, 6, 7 };

				Tree copy{ 11, 12, 13, 14, 15, 16, 17 };
				copy = tree;
				REQUIRE(testInvariants(copy));
				REQUIRE(!copy.hasSeparateSentinels());
				REQUIRE(!copy.sharesBottom());
				REQUIRE(copy.sharesBottom(copy));
				REQUIRE(!copy.sharesBottom(tree));
				REQUIRE(copy.size() == 7);
				REQUIRE(boost::equal(copy.crange().dereferenceKey(), correctSet));

				REQUIRE(testInvariants(tree));
				REQUIRE(!tree.hasSeparateSentinels());
				REQUIRE(!tree.sharesBottom());
				REQUIRE(tree.sharesBottom(tree));
				REQUIRE(!tree.sharesBottom(copy));
				REQUIRE(tree.size() == 7);
				REQUIRE(boost::equal(tree.crange().dereferenceKey(), correctSet));
			}
			{
				Tree tree{ 1, 2, 3, 4, 5, 6, 7 };
				integer correctSet[] = { 1, 2, 3, 4, 5, 6, 7 };

				Tree moved(std::move(tree));
				REQUIRE(testInvariants(moved));
				REQUIRE(moved.hasSeparateSentinels());
				REQUIRE(moved.sharesBottom());
				REQUIRE(moved.sharesBottom(moved));
				REQUIRE(moved.sharesBottom(tree));
				REQUIRE(moved.size() == 7);
				REQUIRE(boost::equal(moved.crange().dereferenceKey(), correctSet));

				REQUIRE(testInvariants(tree));
				REQUIRE(!tree.hasSeparateSentinels());
				REQUIRE(tree.sharesBottom());
				REQUIRE(tree.sharesBottom(tree));
				REQUIRE(tree.sharesBottom(moved));
				REQUIRE(tree.size() == 0);
				REQUIRE(tree.empty());
			}
			{
				Tree tree{ 1, 2, 3, 4, 5, 6, 7 };
				integer correctSet[] = { 1, 2, 3, 4, 5, 6, 7 };
				Tree moved{ 11, 12, 13, 14, 15, 16, 17 };
				moved = std::move(tree);
				REQUIRE(testInvariants(moved));
				REQUIRE(moved.hasSeparateSentinels());
				REQUIRE(moved.sharesBottom());
				REQUIRE(moved.sharesBottom(moved));
				REQUIRE(moved.sharesBottom(tree));
				REQUIRE(moved.size() == 7);
				REQUIRE(boost::equal(moved.crange().dereferenceKey(), correctSet));

				REQUIRE(testInvariants(tree));
				REQUIRE(!tree.hasSeparateSentinels());
				REQUIRE(tree.sharesBottom());
				REQUIRE(tree.sharesBottom(tree));
				REQUIRE(tree.sharesBottom(moved));
				REQUIRE(tree.size() == 0);
				REQUIRE(tree.empty());
			}
			{
				Tree tree;
				Tree copy(tree);
				REQUIRE(testInvariants(tree));
				REQUIRE(testInvariants(copy));
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

			// The next element from the end-node is 
			// the first element of the tree.
			++iter;
			REQUIRE(iter.key() == 1);

			// The previous element from the first element
			// is the end-node.
			--iter;
			REQUIRE(iter == aTree.cend());

			// The previous element from the end-node is
			// the last element.
			--iter;
			REQUIRE(iter.key() == 4);

			// The next element from the last node is the
			// end-node.
			++iter;
			REQUIRE(iter == aTree.cend());

			// A const-iterator can be explicitly converted to an iterator,
			// provided one has a mutable reference to the tree.
			Iterator bIter = aTree.cast(iter);

			// An iterator can be converted to a const-iterator.
			iter = bIter;
			REQUIRE(iter == bIter);

			ConstIterator cIter = bIter;
			unused(cIter);
		}

		template <typename Tree>
		void testRandom()
		{
			Tree tree;

			std::list<integer> dataSet;

			integer treeSizeSet[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 100};
			integer treeSizes = sizeof(treeSizeSet) / sizeof(integer);

			for (integer k = 0;k < treeSizes;++k)
			{
				integer treeSize = treeSizeSet[k];
				tree.clear();
				dataSet.clear();

				for (integer i = 0;i < 4 * treeSize;++i)
				{
					uinteger n = randomUinteger() % 100;
					dataSet.push_back(n);

					tree.insert(n);
					REQUIRE(testInvariants(tree));

					if (tree.size() > treeSize)
					{
						tree.erase(dataSet.front());
						REQUIRE(testInvariants(tree));
						dataSet.pop_front();
					}
				}

				{
					Tree copy(tree);
					REQUIRE(testInvariants(copy));
					REQUIRE(tree.size() == copy.size());
					REQUIRE(boost::equal(tree.crange().dereferenceKey(), copy.crange().dereferenceKey()));
				}
			}
		}

		template <typename Tree>
		void testInsert()
		{
			using ConstIterator = typename Tree::ConstIterator;

			Tree tree;
			{
				REQUIRE(tree.empty());
				REQUIRE(tree.size() == 0);
				REQUIRE(testInvariants(tree));
			}

			tree.insert(1);
			{
				REQUIRE(!tree.empty());
				REQUIRE(tree.size() == 1);
				REQUIRE(testInvariants(tree));

				integer correctSet[] = { 1 };
				REQUIRE(boost::equal(tree.crange().dereferenceKey(), correctSet));
			}

			tree.insert(5);
			{
				REQUIRE(!tree.empty());
				REQUIRE(tree.size() == 2);
				REQUIRE(testInvariants(tree));

				integer correctSet[] = { 1, 5 };
				REQUIRE(boost::equal(tree.crange().dereferenceKey(), correctSet));
			}

			tree.insert(3);
			{
				REQUIRE(!tree.empty());
				REQUIRE(tree.size() == 3);
				REQUIRE(testInvariants(tree));

				integer correctSet[] = { 1, 3, 5 };
				REQUIRE(boost::equal(tree.crange().dereferenceKey(), correctSet));
			}

			tree.insert(4);
			{
				REQUIRE(!tree.empty());
				REQUIRE(tree.size() == 4);
				REQUIRE(testInvariants(tree));

				integer correctSet[] = { 1, 3, 4, 5 };
				REQUIRE(boost::equal(tree.crange().dereferenceKey(), correctSet));
			}

			tree.insert(8);
			{
				REQUIRE(!tree.empty());
				REQUIRE(tree.size() == 5);
				REQUIRE(testInvariants(tree));

				integer correctSet[] = { 1, 3, 4, 5, 8 };
				REQUIRE(boost::equal(tree.crange().dereferenceKey(), correctSet));
			}

			tree.insert(7);
			{
				REQUIRE(!tree.empty());
				REQUIRE(tree.size() == 6);
				REQUIRE(testInvariants(tree));

				integer correctSet[] = { 1, 3, 4, 5, 7, 8 };
				REQUIRE(boost::equal(tree.crange().dereferenceKey(), correctSet));
			}

			tree.insert(6);
			{
				REQUIRE(!tree.empty());
				REQUIRE(tree.size() == 7);
				REQUIRE(testInvariants(tree));

				integer correctSet[] = { 1, 3, 4, 5, 6, 7, 8 };
				REQUIRE(boost::equal(tree.crange().dereferenceKey(), correctSet));
			}

			tree.insert(9);
			{
				REQUIRE(!tree.empty());
				REQUIRE(tree.size() == 8);
				REQUIRE(testInvariants(tree));

				integer correctSet[] = { 1, 3, 4, 5, 6, 7, 8, 9 };
				REQUIRE(boost::equal(tree.crange().dereferenceKey(), correctSet));
			}

			tree.insert(2);
			{
				REQUIRE(!tree.empty());
				REQUIRE(tree.size() == 9);
				REQUIRE(testInvariants(tree));

				integer correctSet[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
				REQUIRE(boost::equal(tree.crange().dereferenceKey(), correctSet));
			}

			integer correctSet[] =
			{
				1, 2, 3, 4, 5, 6, 7, 8, 9
			};

			{
				REQUIRE(boost::distance(tree) == 9);
				REQUIRE(boost::distance(tree.crange().dereferenceKey()) == 9);
				REQUIRE(boost::equal(tree.crange().dereferenceKey(), correctSet));
			}

			{
				ConstIterator copyIter = tree.cend();
				++copyIter;
				REQUIRE(copyIter == tree.cbegin());
			}

			{
				REQUIRE(boost::equal(
					tree.crange().dereferenceKey() | boost::adaptors::reversed,
					correctSet | boost::adaptors::reversed));
			}

			{
				ConstIterator copyIter = tree.cbegin();
				--copyIter;
				REQUIRE(copyIter == tree.cend());
			}
		}

		template <typename Tree>
		void testErase()
		{
			Tree tree{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			{
				integer correctSet[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
				REQUIRE(tree.size() == 9);
				REQUIRE(boost::equal(tree.crange().dereferenceKey(), correctSet));
				REQUIRE(testInvariants(tree));
			}

			tree.erase(0);
			{
				integer correctSet[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
				REQUIRE(tree.size() == 9);
				REQUIRE(boost::equal(tree.crange().dereferenceKey(), correctSet));
				REQUIRE(testInvariants(tree));
			}

			tree.erase(4);
			{
				integer correctSet[] = { 1, 2, 3, 5, 6, 7, 8, 9 };
				REQUIRE(tree.size() == 8);
				REQUIRE(boost::equal(tree.crange().dereferenceKey(), correctSet));
				REQUIRE(testInvariants(tree));
			}

			tree.erase(7);
			{
				integer correctSet[] = { 1, 2, 3, 5, 6, 8, 9 };
				REQUIRE(tree.size() == 7);
				REQUIRE(boost::equal(tree.crange().dereferenceKey(), correctSet));
				REQUIRE(testInvariants(tree));
			}

			tree.erase(1);
			{
				integer correctSet[] = { 2, 3, 5, 6, 8, 9 };
				REQUIRE(tree.size() == 6);
				REQUIRE(boost::equal(tree.crange().dereferenceKey(), correctSet));
				REQUIRE(testInvariants(tree));
			}

			tree.erase(9);
			{
				integer correctSet[] = { 2, 3, 5, 6, 8};
				REQUIRE(tree.size() == 5);
				REQUIRE(boost::equal(tree.crange().dereferenceKey(), correctSet));
				REQUIRE(testInvariants(tree));
			}

			tree.erase(5);
			{
				integer correctSet[] = { 2, 3, 6, 8 };
				REQUIRE(tree.size() == 4);
				REQUIRE(boost::equal(tree.crange().dereferenceKey(), correctSet));
				REQUIRE(testInvariants(tree));
			}

			tree.erase(3);
			{
				integer correctSet[] = { 2, 6, 8 };
				REQUIRE(tree.size() == 3);
				REQUIRE(boost::equal(tree.crange().dereferenceKey(), correctSet));
				REQUIRE(testInvariants(tree));
			}

			tree.erase(2);
			{
				integer correctSet[] = { 6, 8 };
				REQUIRE(tree.size() == 2);
				REQUIRE(boost::equal(tree.crange().dereferenceKey(), correctSet));
				REQUIRE(testInvariants(tree));
			}

			tree.erase(6);
			{
				integer correctSet[] = { 8 };
				REQUIRE(tree.size() == 1);
				REQUIRE(boost::equal(tree.crange().dereferenceKey(), correctSet));
				REQUIRE(testInvariants(tree));
			}

			tree.erase(8);
			{
				REQUIRE(tree.size() == 0);
				REQUIRE(tree.empty());
				REQUIRE(testInvariants(tree));
			}
		}

		template <typename Tree>
		void testSplice()
		{
			Tree a{ 0, 1, 4, 5, 9, 15, 20 };
			REQUIRE(testInvariants(a));

			Tree b{ 1, 2, 6, 7, 8, 9, 10 };
			REQUIRE(testInvariants(b));

			{
				a.splice(b, b.find(6));

				REQUIRE(testInvariants(a));
				REQUIRE(a.size() == 8);
				integer aCorrectSet[] = { 0, 1, 4, 5, 6, 9, 15, 20 };
				REQUIRE(boost::equal(a.crange().dereferenceKey(), aCorrectSet));

				REQUIRE(testInvariants(b));
				REQUIRE(b.size() == 6);
				integer bCorrectSet[] = { 1, 2, 7, 8, 9, 10 };
				REQUIRE(boost::equal(b.crange().dereferenceKey(), bCorrectSet));
			}

			{
				a.splice(b, b.find(10));

				REQUIRE(testInvariants(a));
				REQUIRE(a.size() == 9);
				integer aCorrectSet[] = { 0, 1, 4, 5, 6, 9, 10, 15, 20 };
				REQUIRE(boost::equal(a.crange().dereferenceKey(), aCorrectSet));

				REQUIRE(testInvariants(b));
				REQUIRE(b.size() == 5);
				integer bCorrectSet[] = { 1, 2, 7, 8, 9};
				REQUIRE(boost::equal(b.crange().dereferenceKey(), bCorrectSet));
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
			REQUIRE(testInvariants(tree));

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

			REQUIRE(notFound(0));
			REQUIRE(notFound(1));
			REQUIRE(notFound(3));
			REQUIRE(notFound(6));
			REQUIRE(notFound(7));
			REQUIRE(notFound(8));
			REQUIRE(notFound(10));
			REQUIRE(notFound(11));
			REQUIRE(notFound(12));
			REQUIRE(notFound(13));
			REQUIRE(notFound(14));
			REQUIRE(notFound(16));
			REQUIRE(notFound(17));
			REQUIRE(notFound(18));
			REQUIRE(notFound(19));
			REQUIRE(notFound(21));

			REQUIRE(test(2));
			REQUIRE(test(4));
			REQUIRE(test(5));
			REQUIRE(test(9));
			REQUIRE(test(15));
			REQUIRE(test(20));
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
			REQUIRE(testInvariants(tree));

			auto test = [&](integer that, integer bound)
			{
				return tree.upperBound(that) != tree.cend() &&
					tree.upperBound(that).key() == bound;
			};

			auto notFound = [&](integer that)
			{
				return tree.upperBound(that) == tree.cend();
			};

			REQUIRE(test(0, 2));
			REQUIRE(test(1, 2));
			REQUIRE(test(2, 4));
			REQUIRE(test(3, 4));
			REQUIRE(test(4, 5));
			REQUIRE(test(5, 9));
			REQUIRE(test(6, 9));
			REQUIRE(test(7, 9));
			REQUIRE(test(8, 9));
			REQUIRE(test(9, 15));
			REQUIRE(test(10, 15));
			REQUIRE(test(11, 15));
			REQUIRE(test(12, 15));
			REQUIRE(test(13, 15));
			REQUIRE(test(14, 15));
			REQUIRE(test(15, 20));
			REQUIRE(test(16, 20));
			REQUIRE(test(17, 20));
			REQUIRE(test(18, 20));
			REQUIRE(test(19, 20));
			REQUIRE(notFound(20));
			REQUIRE(notFound(21));
		}

		template <typename Tree>
		void testJoin()
		{
			{
				Tree aTree = { 1, 2, 3, 4, 5 };
				REQUIRE(testInvariants(aTree));

				Tree bTree;
				bTree.useBottomFrom(aTree);
				REQUIRE(testInvariants(bTree));
				REQUIRE(bTree.sharesBottom(aTree));

				aTree.join(bTree);
				REQUIRE(testInvariants(aTree));
				REQUIRE(testInvariants(bTree));
				REQUIRE(bTree.empty());

				integer correctSet[] =
					{ 1, 2, 3, 4, 5 };
				REQUIRE(boost::equal(aTree.crange().dereferenceKey(), correctSet));

				bTree.join(aTree);
				REQUIRE(testInvariants(aTree));
				REQUIRE(testInvariants(bTree));
				REQUIRE(aTree.empty());
				REQUIRE(boost::equal(bTree.crange().dereferenceKey(), correctSet));
			}

			{
				Tree aTree = { 1, 2, 3, 4, 5 };
				REQUIRE(testInvariants(aTree));

				Tree bTree;
				bTree.useBottomFrom(aTree);
				bTree = { 6, 7, 8 };
				REQUIRE(testInvariants(bTree));

				bTree.join(aTree);
				REQUIRE(testInvariants(aTree));
				REQUIRE(testInvariants(bTree));
				REQUIRE(aTree.empty());

				integer correctSet[] =
					{ 1, 2, 3, 4, 5, 6, 7, 8 };
				REQUIRE(boost::equal(bTree.crange().dereferenceKey(), correctSet));
			}

			{
				Tree aTree = { 1, 2, 3, 4, 5 };
				REQUIRE(testInvariants(aTree));

				Tree bTree;
				bTree.useBottomFrom(aTree);
				bTree = { 6, 7, 8 };
				REQUIRE(testInvariants(bTree));

				aTree.join(bTree);
				REQUIRE(testInvariants(aTree));
				REQUIRE(testInvariants(bTree));
				REQUIRE(bTree.empty());

				integer correctSet[] = 
					{1, 2, 3, 4, 5, 6, 7, 8};
				REQUIRE(boost::equal(aTree.crange().dereferenceKey(), correctSet));
			}
		}

		void testMultiSplit()
		{
			using Tree = MultiMap;
			using Iterator = Tree::Iterator;
			{
				Tree tree;
				std::vector<integer> dataSet;
				integer n = 5000;
				for (integer i = 0;i < n;++i)
				{
					integer data = randomInteger(65536);
					dataSet.push_back(data);

					tree.insert(0, data);
					//REQUIRE(testInvariants(tree));
				}

				std::vector<integer> correctSet = dataSet;
				REQUIRE(boost::equal(tree.range().dereferenceData(), correctSet));

				std::vector<integer> indexSet;
				for (integer i = 0; i < 25; ++i)
				{
					indexSet.push_back(i);
				}
				for (integer i = 3000; i < 3025; ++i)
				{
					indexSet.push_back(i);
				}
				for (integer i = 4975; i < 5000; ++i)
				{
					indexSet.push_back(i);
				}
				/*
				for (integer i = 0; i < 5000; ++i)
				{
					indexSet.push_back(i);
				}
				*/

				for (integer i : indexSet)
				{
					Tree aTree = tree;
					REQUIRE(testInvariants(aTree));

					Tree bTree;

					{
						Iterator aEndBefore = aTree.end();
						Iterator aBottomBefore = aTree.bottom();
						Iterator bEndBefore = bTree.end();

						bTree = aTree.split(std::next(aTree.cbegin(), i));

						Iterator bEndAfter = bTree.end();
						Iterator aBottomAfter = aTree.bottom();
						Iterator aEndAfter = aTree.end();

						REQUIRE(aEndBefore == aEndAfter);
						REQUIRE(aBottomBefore == aBottomAfter);
						REQUIRE(bEndBefore == bEndAfter);
					}

					REQUIRE(testInvariants(aTree));
					REQUIRE(aTree.size() == i);
					REQUIRE(testInvariants(bTree));
					REQUIRE(bTree.size() == n - i);

					{
						Iterator aEndBefore = aTree.end();
						Iterator aBottomBefore = aTree.bottom();
						Iterator bEndBefore = bTree.end();
						Iterator bBottomBefore = bTree.bottom();

						aTree.join(bTree);

						Iterator bBottomAfter = bTree.bottom();
						Iterator bEndAfter = bTree.end();
						Iterator aBottomAfter = aTree.bottom();
						Iterator aEndAfter = aTree.end();

						REQUIRE(aEndBefore == aEndAfter);
						REQUIRE(aBottomBefore == aBottomAfter);
						REQUIRE(bEndBefore == bEndAfter);
						REQUIRE(bBottomBefore == bBottomAfter);
					}

					REQUIRE(testInvariants(aTree));
					REQUIRE(aTree.size() == n);
					REQUIRE(boost::equal(aTree.range().dereferenceData(), correctSet));
					REQUIRE(testInvariants(bTree));
					REQUIRE(bTree.size() == 0);
				}
			}
		}

		void testMultiJoin()
		{
			using Tree = MultiMap;
			using Iterator = Tree::Iterator;
			{
				integer n = 20;
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
							REQUIRE(testInvariants(aTree));
						}

						Tree bTree;
						bTree.useBottomFrom(aTree);

						for (integer k = 0; k < j; ++k)
						{
							integer data = randomInteger(65536);
							correctSet.push_back(data);

							bTree.insert(0, data);
							REQUIRE(testInvariants(bTree));
						}

						{
							Iterator aEndBefore = aTree.end();
							Iterator aBottomBefore = aTree.bottom();
							Iterator bEndBefore = bTree.end();
							Iterator bBottomBefore = bTree.bottom();

							aTree.join(bTree);

							Iterator bBottomAfter = bTree.bottom();
							Iterator bEndAfter = bTree.end();
							Iterator aBottomAfter = aTree.bottom();
							Iterator aEndAfter = aTree.end();

							REQUIRE(aEndBefore == aEndAfter);
							REQUIRE(aBottomBefore == aBottomAfter);
							REQUIRE(bEndBefore == bEndAfter);
							REQUIRE(bBottomBefore == bBottomAfter);
						}

						REQUIRE(testInvariants(aTree));
						REQUIRE(testInvariants(bTree));
						REQUIRE(boost::equal(aTree.crange().dereferenceData(), correctSet));
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

			REQUIRE(test(-0.10, 0));
			REQUIRE(test(0.00, 0));
			REQUIRE(test(0.10, 0));
			REQUIRE(test(0.19, 0));
			REQUIRE(test(0.20, 1));
			REQUIRE(test(0.29, 1));
			REQUIRE(test(0.30, 1));
			REQUIRE(test(0.39, 1));
			REQUIRE(test(0.40, 2));
			REQUIRE(test(0.49, 2));
			REQUIRE(test(0.50, 2));
			REQUIRE(test(0.59, 2));
			REQUIRE(test(0.60, 3));
			REQUIRE(test(0.69, 3));
			REQUIRE(test(0.70, 3));
			REQUIRE(test(0.79, 3));
			REQUIRE(test(0.80, 4));
			REQUIRE(test(0.89, 4));
			REQUIRE(test(0.90, 4));
			REQUIRE(test(0.99, 4));
			REQUIRE(test(1.00, 4));
			REQUIRE(test(1.09, 4));
			REQUIRE(test(1.10, 4));
			REQUIRE(test(1.19, 4));
		}

		template <typename Tree>
		void testMultiCount()
		{
			Tree tree = { 3, 4, 5, 5, 5, 5, 5, 5, 5, 6, 7 };

			auto test = [&](integer key, integer count)
			{
				return tree.count(key) == count;
			};

			REQUIRE(test(0, 0));
			REQUIRE(test(1, 0));
			REQUIRE(test(2, 0));
			REQUIRE(test(3, 1));
			REQUIRE(test(4, 1));
			REQUIRE(test(5, 7));
			REQUIRE(test(6, 1));
			REQUIRE(test(7, 1));
			REQUIRE(test(8, 0));
			REQUIRE(test(9, 0));
		}
	};

	TEST_CASE("RedBlackTree", "[RedBlackTree]")
	{
	}

}
