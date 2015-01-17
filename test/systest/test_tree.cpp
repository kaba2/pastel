// Description: Testing for Tree
// DocumentationOf: tree.h

#include "test_pastelsys.h"

#include "pastel/sys/tree.h"
#include "pastel/sys/predicate.h"

#include <boost/range/algorithm/equal.hpp>

#include <iostream>

using namespace Pastel;

namespace
{

	class Test
		: public TestSuite
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

		Test()
			: TestSuite(&testReport())
		{
		}

		virtual void run()
		{
			test();
			testInsert();
			testNoData();
			testData();
			testIterator<ConstIterator>();
			testIterator<Iterator>();
		}

		template <int N>
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
				TEST_ENSURE(tree.empty());
				TEST_ENSURE_OP(tree.size(), ==, 0);
				TEST_ENSURE_OP(tree.sentinelCount(), ==, 1);
			}
			Iterator aIter = tree.insertRoot(0);
			{
				TEST_ENSURE_OP(tree.size(), ==, 1);
				TEST_ENSURE(!tree.empty());
				TEST_ENSURE_OP(tree.sentinelCount(), ==, 3);

				integer correctSet[] = {0};
				TEST_ENSURE(same(tree, correctSet));
			}

			Iterator bIter = tree.insert(aIter, false, 1);
			{
				TEST_ENSURE_OP(tree.size(), ==, 2);
				TEST_ENSURE_OP(tree.sentinelCount(), ==, 4);

				integer correctSet[] = {1, 0};
				TEST_ENSURE(same(tree, correctSet));
			}

			Iterator cIter = tree.insert(bIter, true, 2);
			{
				TEST_ENSURE_OP(tree.size(), ==, 3);
				TEST_ENSURE_OP(tree.sentinelCount(), ==, 5);

				integer correctSet[] = {1, 2, 0};
				TEST_ENSURE(same(tree, correctSet));
			}

			tree.rotate(aIter, true);
			{
				TEST_ENSURE_OP(tree.sentinelCount(), ==, 5);

				integer correctSet[] = {1, 2, 0};
				TEST_ENSURE(same(tree, correctSet));
			}

			Tree_ copyTree(tree);
			{
				TEST_ENSURE_OP(tree.sentinelCount(), ==, 5);
				TEST_ENSURE_OP(copyTree.sentinelCount(), ==, 5);
				TEST_ENSURE(!copyTree.empty());
				TEST_ENSURE_OP(copyTree.size(), ==, 3);
				TEST_ENSURE(same(tree, copyTree));
			}

			tree.clear();
			{
				TEST_ENSURE_OP(copyTree.sentinelCount(), ==, 5);
				TEST_ENSURE_OP(tree.sentinelCount(), ==, 1);
				TEST_ENSURE(tree.empty());
				TEST_ENSURE_OP(tree.size(), ==, 0);
			}

			tree = copyTree;
			{
				TEST_ENSURE_OP(copyTree.sentinelCount(), ==, 5);
				TEST_ENSURE_OP(tree.sentinelCount(), ==, 5);
				TEST_ENSURE(!tree.empty());
				TEST_ENSURE_OP(tree.size(), ==, 3);
				TEST_ENSURE(same(tree, copyTree));
			}

			copyTree = std::move(tree);
			{
				TEST_ENSURE_OP(copyTree.sentinelCount(), ==, 2);
				TEST_ENSURE_OP(tree.sentinelCount(), ==, 4);
				integer correctSet[] = {1, 2, 0};
				TEST_ENSURE(tree.empty());
				TEST_ENSURE_OP(tree.size(), ==, 0);
				TEST_ENSURE(!copyTree.empty());
				TEST_ENSURE_OP(copyTree.size(), ==, 3);
				TEST_ENSURE(same(copyTree, correctSet));
			}

			{
				Tree_ anotherTree(std::move(copyTree));
				{
					TEST_ENSURE_OP(copyTree.sentinelCount(), ==, 1);
					TEST_ENSURE_OP(tree.sentinelCount(), ==, 4);
					TEST_ENSURE_OP(anotherTree.sentinelCount(), ==, 2);
					integer correctSet[] = {1, 2, 0};
					TEST_ENSURE(copyTree.empty());
					TEST_ENSURE_OP(copyTree.size(), ==, 0);
					TEST_ENSURE(!anotherTree.empty());
					TEST_ENSURE_OP(anotherTree.size(), ==, 3);
					TEST_ENSURE(same(anotherTree, correctSet));
				}
			}
			{
				TEST_ENSURE_OP(tree.sentinelCount(), ==, 1);
			}
		}

		void testInsert()
		{
			Tree_ tree;
			Iterator aIter = tree.insertRoot(0);
			Iterator bIter = tree.insert(aIter, false, 1);
			Iterator cIter = tree.insert(bIter, true, 2);
			{
				integer correctSet[] = {1, 2, 0};
				TEST_ENSURE(same(tree, correctSet));
			}
			
			Iterator newIter = tree.insert(bIter, false, tree);
			{
				integer correctSet[] = {1, 2, 0, 1, 2, 0};
				TEST_ENSURE(same(tree, correctSet));
			}

			Tree_ detached = tree.detach(newIter);
			{
				integer correctSet[] = {1, 2, 0};
				TEST_ENSURE(same(tree, correctSet));
				TEST_ENSURE_OP(tree.size(), ==, 3);
				TEST_ENSURE(same(detached, correctSet));
				TEST_ENSURE_OP(detached.size(), ==, 3);
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
				TEST_ENSURE_OP(*aIter, ==, 0);
				TEST_ENSURE_OP(aIter->label, ==, 0);
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
				TEST_ENSURE(iter == bIter);
			}
			{
				Iter iter = bIter.leftMost();
				TEST_ENSURE(iter == bIter);
			}
			{
				Iter iter = tree.end().leftMost();
				TEST_ENSURE(iter == tree.end());
			}
			{
				Iter iter = cIter.leftMost();
				TEST_ENSURE(iter == cIter);
			}
			
			// Finding the rightmost.
			{
				Iter iter = aIter.rightMost();
				TEST_ENSURE(iter == aIter);
			}
			{
				Iter iter = bIter.rightMost();
				TEST_ENSURE(iter == cIter);
			}
			{
				Iter iter = tree.end().rightMost();
				TEST_ENSURE(iter == tree.end());
			}
			{
				Iter iter = cIter.rightMost();
				TEST_ENSURE(iter == cIter);
			}

			// Finding the root.
			{
				Iter iter = cIter.root();
				TEST_ENSURE(iter == tree.root());
			}
			{
				Iter iter = bIter.root();
				TEST_ENSURE(iter == tree.root());
			}
			{
				Iter iter = aIter.root();
				TEST_ENSURE(iter == tree.root());
			}
			{
				Iter iter = tree.end().root();
				++iter;
				TEST_ENSURE(iter == tree.end());
			}
			{
				Iter iter = tree.end().root();
				TEST_ENSURE(iter == tree.end());
			}

			// Wrapping behaviour.
			{
				// The beginning wraps around to the one-past-end.
				Iter iter = tree.begin();
				--iter;
				TEST_ENSURE(iter == tree.end());
			}
			{
				// The one-past one-past-end stays where it is.
				Iter iter = tree.end();
				++iter;
				TEST_ENSURE(iter == tree.end());
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
