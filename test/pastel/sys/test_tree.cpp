// Description: Testing for Tree
// DocumentationOf: tree.h

#include "test/test_init.h"

#include "pastel/sys/tree.h"
#include "pastel/sys/predicate.h"

#include <boost/range/algorithm/equal.hpp>

#include <iostream>

namespace
{

	class Test
	{
	public:
		class Label
		{
		public:
			Label(integer label_)
			: label(label_)
			{
			}

			operator integer() const
			{
				return label;
			}

			integer label;
		};

		using Tree_ = Tree<Label>;
		using Iterator = Tree_::Iterator;
		using ConstIterator = Tree_::ConstIterator;
		using Range = Tree_::Range;
		using ConstRange = Tree_::ConstRange;

		virtual void run()
		{
			test();
			testInsert();
			testNoData();
			testData();
			testIterator<ConstIterator>();
			testIterator<Iterator>();
		}

		template <integer N>
		bool same(const Tree_& tree, integer (&correctSet)[N]) const
		{
			return boost::equal(range(correctSet), tree.crange(), EqualTo());
		}

		bool same(const Tree_& tree, const Tree_& that)
		{
			return boost::equal(tree.crange(), that.crange(), EqualTo());
		}

		void test()
		{
			Tree_ tree;
			{
				REQUIRE(tree.empty());
				REQUIRE(tree.size() == 0);
				REQUIRE(tree.sentinelCount() == 1);
			}
			Iterator aIter = tree.insertRoot(0);
			{
				REQUIRE(tree.size() == 1);
				REQUIRE(!tree.empty());
				REQUIRE(tree.sentinelCount() == 3);

				integer correctSet[] = {0};
				REQUIRE(same(tree, correctSet));
			}

			Iterator bIter = tree.insert(aIter, false, 1);
			{
				REQUIRE(tree.size() == 2);
				REQUIRE(tree.sentinelCount() == 4);

				integer correctSet[] = {1, 0};
				REQUIRE(same(tree, correctSet));
			}

			Iterator cIter = tree.insert(bIter, true, 2);
			unused(cIter);
			{
				REQUIRE(tree.size() == 3);
				REQUIRE(tree.sentinelCount() == 5);

				integer correctSet[] = {1, 2, 0};
				REQUIRE(same(tree, correctSet));
			}

			tree.rotate(aIter, true);
			{
				REQUIRE(tree.sentinelCount() == 5);

				integer correctSet[] = {1, 2, 0};
				REQUIRE(same(tree, correctSet));
			}

			Tree_ copyTree(tree);
			{
				REQUIRE(tree.sentinelCount() == 5);
				REQUIRE(copyTree.sentinelCount() == 5);
				REQUIRE(!copyTree.empty());
				REQUIRE(copyTree.size() == 3);
				REQUIRE(same(tree, copyTree));
			}

			tree.clear();
			{
				REQUIRE(copyTree.sentinelCount() == 5);
				REQUIRE(tree.sentinelCount() == 1);
				REQUIRE(tree.empty());
				REQUIRE(tree.size() == 0);
			}

			tree = copyTree;
			{
				REQUIRE(copyTree.sentinelCount() == 5);
				REQUIRE(tree.sentinelCount() == 5);
				REQUIRE(!tree.empty());
				REQUIRE(tree.size() == 3);
				REQUIRE(same(tree, copyTree));
			}

			copyTree = std::move(tree);
			{
				REQUIRE(copyTree.sentinelCount() == 2);
				REQUIRE(tree.sentinelCount() == 4);
				integer correctSet[] = {1, 2, 0};
				REQUIRE(tree.empty());
				REQUIRE(tree.size() == 0);
				REQUIRE(!copyTree.empty());
				REQUIRE(copyTree.size() == 3);
				REQUIRE(same(copyTree, correctSet));
			}

			{
				Tree_ anotherTree(std::move(copyTree));
				{
					REQUIRE(copyTree.sentinelCount() == 1);
					REQUIRE(tree.sentinelCount() == 4);
					REQUIRE(anotherTree.sentinelCount() == 2);
					integer correctSet[] = {1, 2, 0};
					REQUIRE(copyTree.empty());
					REQUIRE(copyTree.size() == 0);
					REQUIRE(!anotherTree.empty());
					REQUIRE(anotherTree.size() == 3);
					REQUIRE(same(anotherTree, correctSet));
				}
			}
			{
				REQUIRE(tree.sentinelCount() == 1);
			}
		}

		void testInsert()
		{
			Tree_ tree;
			Iterator aIter = tree.insertRoot(0);
			Iterator bIter = tree.insert(aIter, false, 1);
			Iterator cIter = tree.insert(bIter, true, 2);
			unused(cIter);
			{
				integer correctSet[] = {1, 2, 0};
				REQUIRE(same(tree, correctSet));
			}

			Iterator newIter = tree.insert(bIter, false, tree);
			{
				integer correctSet[] = {1, 2, 0, 1, 2, 0};
				REQUIRE(same(tree, correctSet));
			}

			Tree_ detached = tree.detach(newIter);
			{
				integer correctSet[] = {1, 2, 0};
				REQUIRE(same(tree, correctSet));
				REQUIRE(tree.size() == 3);
				REQUIRE(same(detached, correctSet));
				REQUIRE(detached.size() == 3);
			}
		}

		void testNoData()
		{
			Tree<void> tree;
			tree.insertRoot();
		}

		void testData()
		{
			Tree_ tree;
			Iterator aIter = tree.insertRoot(0);
			{
				REQUIRE(*aIter == 0);
				REQUIRE(aIter->label == 0);
			}
		}

		template <typename Iter>
		void testIterator()
		{
			Tree_ tree;
			Iter aIter = tree.insertRoot(0);
			Iter bIter = tree.insert(aIter, false, 1);
			Iter cIter = tree.insert(bIter, true, 2);

			// Finding the leftmost.
			{
				Iter iter = aIter.leftMost();
				REQUIRE(iter == bIter);
			}
			{
				Iter iter = bIter.leftMost();
				REQUIRE(iter == bIter);
			}
			{
				Iter iter = tree.end().leftMost();
				REQUIRE(iter == tree.end());
			}
			{
				Iter iter = cIter.leftMost();
				REQUIRE(iter == cIter);
			}

			// Finding the rightmost.
			{
				Iter iter = aIter.rightMost();
				REQUIRE(iter == aIter);
			}
			{
				Iter iter = bIter.rightMost();
				REQUIRE(iter == cIter);
			}
			{
				Iter iter = tree.end().rightMost();
				REQUIRE(iter == tree.end());
			}
			{
				Iter iter = cIter.rightMost();
				REQUIRE(iter == cIter);
			}

			// Finding the root.
			{
				Iter iter = cIter.root();
				REQUIRE(iter == tree.root());
			}
			{
				Iter iter = bIter.root();
				REQUIRE(iter == tree.root());
			}
			{
				Iter iter = aIter.root();
				REQUIRE(iter == tree.root());
			}
			{
				Iter iter = tree.end().root();
				++iter;
				REQUIRE(iter == tree.end());
			}
			{
				Iter iter = tree.end().root();
				REQUIRE(iter == tree.end());
			}

			// Wrapping behaviour.
			{
				// The beginning wraps around to the one-past-end.
				Iter iter = tree.begin();
				--iter;
				REQUIRE(iter == tree.end());
			}
			{
				// The one-past one-past-end stays where it is.
				Iter iter = tree.end();
				++iter;
				REQUIRE(iter == tree.end());
			}
		}

		void print(const Tree_& that)
		{
			ConstRange range = that.crange();
			while(!range.empty())
			{
				std::cout << range.front() << ", ";
				range.pop_back();
			}

			std::cout << "end." << std::endl;
		}
	};

	TEST_CASE("Tree", "[Tree]")
	{
	}

}