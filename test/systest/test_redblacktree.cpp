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

		template <typename Settings_>
		class Counting_Customization
		{
		protected:
			using Fwd = RedBlackTree_Fwd<Settings_>;
			PASTEL_FWD(Iterator);
			PASTEL_FWD(Propagation);

			Counting_Customization() {}
			void swap(Counting_Customization& that) {}
			void onClear() {}
			void onInsert(const Iterator& element) {}
			void onErase(const Iterator& element) {}
			void onSpliceFrom(const Iterator& element) {}
			void onSplice(const Iterator& element) {}

			void updatePropagation(
				const Iterator& iter,
				Propagation& propagation)
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
			RedBlack_Settings<uinteger, Data, LessThan, integer, MultipleKeys>;

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

			testManyThings<Set>();
			testManyThings<MultiSet>();
			testManyThings<Map>();
			testManyThings<MultiMap>();
		}

		template <typename Tree>
		void testManyThings()
		{
			testConstruction<Tree>();
			testInsert<Tree>();
			testBlackHeight<Tree>();
			testErase<Tree>();
			testRandom<Tree>();
			testSplice<Tree>();
			testFind<Tree>();
			testLowerBound<Tree>();
			testUpperBound<Tree>();
			testJoin<Tree>();
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
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctSet));
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
				TEST_ENSURE(boost::equal(copy.ckeyRange(), correctSet));

				TEST_ENSURE(testInvariants(tree));
				TEST_ENSURE(!tree.hasSeparateSentinels());
				TEST_ENSURE(tree.sharesBottom());
				TEST_ENSURE(tree.sharesBottom(tree));
				TEST_ENSURE(tree.sharesBottom(copy));
				TEST_ENSURE_OP(tree.size(), == , 7);
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctSet));
			}
			{
				Tree tree{ 1, 2, 3, 4, 5, 6, 7 };
				integer correctSet[] = { 1, 2, 3, 4, 5, 6, 7 };

				Tree copy{ 11, 12, 13, 14, 15, 16, 17 };
				copy = tree;
				ENSURE(testInvariants(copy));
				TEST_ENSURE(testInvariants(copy));
				TEST_ENSURE(!copy.hasSeparateSentinels());
				TEST_ENSURE(!copy.sharesBottom());
				TEST_ENSURE(copy.sharesBottom(copy));
				TEST_ENSURE(!copy.sharesBottom(tree));
				TEST_ENSURE_OP(copy.size(), == , 7);
				TEST_ENSURE(boost::equal(copy.ckeyRange(), correctSet));

				TEST_ENSURE(testInvariants(tree));
				TEST_ENSURE(!tree.hasSeparateSentinels());
				TEST_ENSURE(!tree.sharesBottom());
				TEST_ENSURE(tree.sharesBottom(tree));
				TEST_ENSURE(!tree.sharesBottom(copy));
				TEST_ENSURE_OP(tree.size(), == , 7);
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctSet));
			}
			{
				Tree tree{ 1, 2, 3, 4, 5, 6, 7 };
				integer correctSet[] = { 1, 2, 3, 4, 5, 6, 7 };

				Tree moved(std::move(tree));
				ENSURE(testInvariants(moved));
				ENSURE(testInvariants(tree));
				TEST_ENSURE(testInvariants(moved));
				TEST_ENSURE(moved.hasSeparateSentinels());
				TEST_ENSURE(moved.sharesBottom());
				TEST_ENSURE(moved.sharesBottom(moved));
				TEST_ENSURE(moved.sharesBottom(tree));
				TEST_ENSURE_OP(moved.size(), == , 7);
				TEST_ENSURE(boost::equal(moved.ckeyRange(), correctSet));

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
				TEST_ENSURE(boost::equal(moved.ckeyRange(), correctSet));

				TEST_ENSURE(testInvariants(tree));
				TEST_ENSURE(!tree.hasSeparateSentinels());
				TEST_ENSURE(tree.sharesBottom());
				TEST_ENSURE(tree.sharesBottom(tree));
				TEST_ENSURE(tree.sharesBottom(moved));
				TEST_ENSURE_OP(tree.size(), == , 0);
				TEST_ENSURE(tree.empty());
			}

			/*
			{
				Tree tree { { 1, 1 }, { 2, 4 }, { 3, 9 }, { 4, 16 }, { 5, 25 }, { 6, 36 }, { 7, 49 } };
				TEST_ENSURE(testInvariants(tree));
				TEST_ENSURE_OP(tree.size(), == , 7);

				integer keySet[] = { 1, 2, 3, 4, 5, 6, 7 };
				TEST_ENSURE(boost::equal(tree.ckeyRange(), keySet));

				integer dataSet[] = { 1, 4, 9, 16, 25, 36, 49 };
				TEST_ENSURE(boost::equal(tree.cdataRange(), dataSet));
			}
			*/
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
					TEST_ENSURE(boost::equal(tree.ckeyRange(), copy.ckeyRange()));
				}
			}
		}

		template <typename Tree>
		void testBlackHeight()
		{
			using ConstIterator = typename Tree::ConstIterator;

			Tree tree;
			{
				TEST_ENSURE(testInvariants(tree));
				TEST_ENSURE_OP(tree.blackHeight(), == , 0);
			}

			std::vector<uinteger> keySet = 
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
			std::vector<integer> blackHeightSet = 
				{ 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3 };
			ASSERT_OP(keySet.size(), == , blackHeightSet.size());

			for (integer i = 0; i < keySet.size(); ++i)
			{
				tree.insert(keySet[i]);
				TEST_ENSURE(testInvariants(tree));
				TEST_ENSURE_OP(tree.blackHeight(), == , blackHeightSet[i]);
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
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctSet));
			}

			tree.insert(5);
			{
				TEST_ENSURE(!tree.empty());
				TEST_ENSURE_OP(tree.size(), == , 2);
				TEST_ENSURE(testInvariants(tree));

				integer correctSet[] = { 1, 5 };
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctSet));
			}

			tree.insert(3);
			{
				TEST_ENSURE(!tree.empty());
				TEST_ENSURE_OP(tree.size(), == , 3);
				TEST_ENSURE(testInvariants(tree));

				integer correctSet[] = { 1, 3, 5 };
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctSet));
			}

			tree.insert(4);
			{
				TEST_ENSURE(!tree.empty());
				TEST_ENSURE_OP(tree.size(), == , 4);
				TEST_ENSURE(testInvariants(tree));

				integer correctSet[] = { 1, 3, 4, 5 };
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctSet));
			}

			tree.insert(8);
			{
				TEST_ENSURE(!tree.empty());
				TEST_ENSURE_OP(tree.size(), == , 5);
				TEST_ENSURE(testInvariants(tree));

				integer correctSet[] = { 1, 3, 4, 5, 8 };
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctSet));
			}

			tree.insert(7);
			{
				TEST_ENSURE(!tree.empty());
				TEST_ENSURE_OP(tree.size(), == , 6);
				TEST_ENSURE(testInvariants(tree));

				integer correctSet[] = { 1, 3, 4, 5, 7, 8 };
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctSet));
			}

			tree.insert(6);
			{
				TEST_ENSURE(!tree.empty());
				TEST_ENSURE_OP(tree.size(), == , 7);
				TEST_ENSURE(testInvariants(tree));

				integer correctSet[] = { 1, 3, 4, 5, 6, 7, 8 };
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctSet));
			}

			tree.insert(9);
			{
				TEST_ENSURE(!tree.empty());
				TEST_ENSURE_OP(tree.size(), == , 8);
				TEST_ENSURE(testInvariants(tree));

				integer correctSet[] = { 1, 3, 4, 5, 6, 7, 8, 9 };
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctSet));
			}

			tree.insert(2);
			{
				TEST_ENSURE(!tree.empty());
				TEST_ENSURE_OP(tree.size(), == , 9);
				TEST_ENSURE(testInvariants(tree));

				integer correctSet[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctSet));
			}

			integer correctSet[] =
			{
				1, 2, 3, 4, 5, 6, 7, 8, 9
			};

			{
				TEST_ENSURE_OP(boost::distance(tree), == , 9);
				TEST_ENSURE_OP(boost::distance(tree.ckeyRange()), == , 9);
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctSet));
			}

			{
				ConstIterator copyIter = tree.cend();
				++copyIter;
				TEST_ENSURE(copyIter == tree.cbegin());
			}

			{
				TEST_ENSURE(boost::equal(
					tree.ckeyRange() | boost::adaptors::reversed,
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
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctSet));
				TEST_ENSURE(testInvariants(tree));
			}

			tree.erase(0);
			{
				integer correctSet[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
				TEST_ENSURE_OP(tree.size(), == , 9);
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctSet));
				TEST_ENSURE(testInvariants(tree));
			}
			tree.erase(4);
			{
				integer correctSet[] = { 1, 2, 3, 5, 6, 7, 8, 9 };
				TEST_ENSURE_OP(tree.size(), == , 8);
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctSet));
				TEST_ENSURE(testInvariants(tree));
			}

			tree.erase(7);
			{
				integer correctSet[] = { 1, 2, 3, 5, 6, 8, 9 };
				TEST_ENSURE_OP(tree.size(), == , 7);
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctSet));
				TEST_ENSURE(testInvariants(tree));
			}

			tree.erase(1);
			{
				integer correctSet[] = { 2, 3, 5, 6, 8, 9 };
				TEST_ENSURE_OP(tree.size(), == , 6);
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctSet));
				TEST_ENSURE(testInvariants(tree));
			}

			tree.erase(9);
			{
				integer correctSet[] = { 2, 3, 5, 6, 8};
				TEST_ENSURE_OP(tree.size(), == , 5);
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctSet));
				TEST_ENSURE(testInvariants(tree));
			}

			tree.erase(5);
			{
				integer correctSet[] = { 2, 3, 6, 8 };
				TEST_ENSURE_OP(tree.size(), == , 4);
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctSet));
				TEST_ENSURE(testInvariants(tree));
			}

			tree.erase(3);
			{
				integer correctSet[] = { 2, 6, 8 };
				TEST_ENSURE_OP(tree.size(), == , 3);
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctSet));
				TEST_ENSURE(testInvariants(tree));
			}

			tree.erase(2);
			{
				integer correctSet[] = { 6, 8 };
				TEST_ENSURE_OP(tree.size(), == , 2);
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctSet));
				TEST_ENSURE(testInvariants(tree));
			}

			tree.erase(6);
			{
				integer correctSet[] = { 8 };
				TEST_ENSURE_OP(tree.size(), == , 1);
				TEST_ENSURE(boost::equal(tree.ckeyRange(), correctSet));
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
				TEST_ENSURE(boost::equal(a.ckeyRange(), aCorrectSet));

				TEST_ENSURE(testInvariants(b));
				TEST_ENSURE_OP(b.size(), == , 6);
				integer bCorrectSet[] = { 1, 2, 7, 8, 9, 10 };
				TEST_ENSURE(boost::equal(b.ckeyRange(), bCorrectSet));
			}

			{
				a.splice(b, b.find(10));

				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE_OP(a.size(), == , 9);
				integer aCorrectSet[] = { 0, 1, 4, 5, 6, 9, 10, 15, 20 };
				TEST_ENSURE(boost::equal(a.ckeyRange(), aCorrectSet));

				TEST_ENSURE(testInvariants(b));
				TEST_ENSURE_OP(b.size(), == , 5);
				integer bCorrectSet[] = { 1, 2, 7, 8, 9};
				TEST_ENSURE(boost::equal(b.ckeyRange(), bCorrectSet));
			}
		}

		template <typename Tree>
		void print(const Tree& tree, integer height)
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
					std::cout << entry.first.key();
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
					std::cout << "-B ";
				}

				nodeSet.push_back(
					std::make_pair(entry.first.left(), entry.second + 1));

				ConstIterator right = entry.first.isSentinel() ?
					tree.cend() : entry.first.right();
				nodeSet.push_back(
					std::make_pair(right, entry.second + 1));
			}
			std::cout << std::endl;

			for (auto&& key : tree.ckeyRange())
			{
				std::cout << key << ", ";
			}
			std::cout << std::endl;
		}

		template <typename Tree>
		void testFind()
		{
			Tree tree{ 2, 4, 4, 5, 5, 5, 5, 9, 15, 20 };
			TEST_ENSURE(testInvariants(tree));

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

			notFound(0);
			notFound(1);
			found(2);
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
			Tree tree{ 2, 4, 4, 5, 5, 5, 5, 9, 15, 20 };
			TEST_ENSURE(testInvariants(tree));

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

			found(0, 2);
			found(1, 2);
			found(2, 2);
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
			Tree tree{ 2, 4, 4, 5, 5, 5, 5, 9, 15, 20 };
			TEST_ENSURE(testInvariants(tree));

			auto found = [&](integer that, integer bound)
			{
				bool wasFound =
					tree.upperBound(that) != tree.cend();
				bool isCorrect = wasFound &&
					tree.upperBound(that).key() == bound;
				TEST_ENSURE(wasFound && isCorrect);
			};

			auto notFound = [&](integer that)
			{
				TEST_ENSURE(tree.upperBound(that) == tree.cend());
			};

			found(0, 2);
			found(1, 2);
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

		template <typename Tree>
		void testJoin()
		{
			Tree aTree = {1, 2, 3, 4, 5};
			TEST_ENSURE(testInvariants(aTree));
			
			Tree bTree;
			TEST_ENSURE(testInvariants(bTree));
	
			{
				bTree.useBottomFrom(aTree);
				TEST_ENSURE(testInvariants(bTree));
				TEST_ENSURE(bTree.sharesBottom(aTree));
			}

			{
				bTree = {6, 7, 8};
				TEST_ENSURE(testInvariants(bTree));
			}

			/*
			{
				aTree.join(bTree);
				TEST_ENSURE(testInvariants(aTree));
				TEST_ENSURE(testInvariants(bTree));
				TEST_ENSURE(bTree.empty());

				integer correctSet[] = 
					{1, 2, 3, 4, 5, 6, 7, 8};
				TEST_ENSURE(boost::equal(aTree, correctSet));
			}
			*/
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
