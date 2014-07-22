// Description: Testing for iterators
// Documentation: iterator.txt

#include "test_pastelsys.h"

#include "pastel/sys/constant_iterator.h"
#include "pastel/sys/counting_iterator.h"
#include "pastel/sys/sparse_iterator.h"
#include "pastel/sys/rectangle_iterator.h"

#include "pastel/sys/second_iterator.h"

#include <boost/iterator/transform_iterator.hpp>

using namespace Pastel;

namespace
{

	class A;

	using PairSet = std::map<A, A>;
	using Pair_Iterator = Second_Iterator<PairSet::iterator, false>;
	using Pair_ConstIterator = Second_Iterator<PairSet::const_iterator, true>;

	class A
	{
	public:
		A(int a)
			: a_(a)
		{
		}

		bool operator<(const A& that) const
		{
			return a_ < that.a_;
		}

		bool operator==(const A& that) const
		{
			return a_ == that.a_;
		}

		int a_;
	};

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
			testSecond();
			testRectangle();
			testConstant();
			testSparse();
			testCounting();
		}

		void testSecond()
		{
			PairSet a;
			a.insert(std::make_pair(1, 3));
			a.insert(std::make_pair(2, 4));
			a.insert(std::make_pair(4, -2));
			a.insert(std::make_pair(5, 1));

			std::vector<A> b;
			b.push_back(3);
			b.push_back(4);
			b.push_back(-2);
			b.push_back(1);

			TEST_ENSURE(std::equal(b.begin(), b.end(), 
				Pair_Iterator(a.begin())));
			
			{
				Pair_Iterator aIter(a.begin());
				Pair_ConstIterator bIter(aIter);
				//Pair_Iterator c(b);

				TEST_ENSURE(aIter == aIter);
				TEST_ENSURE(aIter == bIter);
				TEST_ENSURE(!(aIter != aIter));
				TEST_ENSURE(!(aIter != bIter));

				++aIter;
				--aIter;
				aIter++;
				aIter--;

				++bIter;
				--bIter;
				bIter++;
				bIter--;
			}

			integer i = 0;
			std::for_each(
				Pair_Iterator(a.begin()), Pair_Iterator(a.end()),
				[&](A& a)
			{
				a.a_ = i;
				++i;
			});

			b[0] = 0;
			b[1] = 1;
			b[2] = 2;
			b[3] = 3;

			TEST_ENSURE(std::equal(b.begin(), b.end(), 
				Pair_ConstIterator(a.begin())));
		}

		void testConstant()
		{
			ConstantIterator<integer> iter;
			++iter;
			--iter;
			iter += 2;
			iter -= 2;
			iter = iter + 1;
			iter = iter - 1;
			iter == iter;
			iter != iter;
			iter < iter;
			iter > iter;
			iter <= iter;
			iter >= iter;
			iter - iter;
			
			//*iter = 2;
		}

		template <typename Iterator>
		void f(const ConstSparseIterator<Iterator>& that)
		{
		}

		void testSparse()
		{
			integer a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

			// SparseIterator

			SparseIterator<integer*> iter(a, 2);
			TEST_ENSURE_OP(*iter, ==, 0);
			++iter;
			TEST_ENSURE_OP(*iter, ==, 2);
			--iter;
			TEST_ENSURE_OP(*iter, ==, 0);
			iter += 2;
			TEST_ENSURE_OP(*iter, ==, 4);
			iter -= 2;
			TEST_ENSURE_OP(*iter, ==, 0);
			iter = iter + 1;
			TEST_ENSURE_OP(*iter, ==, 2);
			iter = iter - 1;
			TEST_ENSURE_OP(*iter, ==, 0);
			TEST_ENSURE(iter == iter);
			TEST_ENSURE(!(iter != iter));
			TEST_ENSURE(!(iter < iter));
			TEST_ENSURE(!(iter > iter));
			TEST_ENSURE(iter <= iter);
			TEST_ENSURE(iter >= iter);
			TEST_ENSURE_OP(iter - iter, ==, 0);

			// ConstSparseIterator

			ConstSparseIterator<const integer*> cIter(iter);
			TEST_ENSURE_OP(*cIter, ==, 0);
			++cIter;
			TEST_ENSURE_OP(*cIter, ==, 2);
			--cIter;
			TEST_ENSURE_OP(*cIter, ==, 0);
			cIter += 2;
			TEST_ENSURE_OP(*cIter, ==, 4);
			cIter -= 2;
			TEST_ENSURE_OP(*cIter, ==, 0);
			cIter = cIter + 1;
			TEST_ENSURE_OP(*cIter, ==, 2);
			cIter = cIter - 1;
			TEST_ENSURE_OP(*cIter, ==, 0);
			TEST_ENSURE(cIter == cIter);
			TEST_ENSURE(!(cIter != cIter));
			TEST_ENSURE(!(cIter < cIter));
			TEST_ENSURE(!(cIter > cIter));
			TEST_ENSURE(cIter <= cIter);
			TEST_ENSURE(cIter >= cIter);
			TEST_ENSURE_OP(cIter - cIter, ==, 0);

			// Co-operation

			cIter = iter;
			cIter += 2;

			TEST_ENSURE_OP(cIter - iter, ==, 2);
			TEST_ENSURE_OP(iter - cIter, ==, -2);
			TEST_ENSURE(cIter != iter);

			cIter = iter;
			TEST_ENSURE(cIter == cIter);
			TEST_ENSURE(iter == cIter);

			*iter = 2;
			TEST_ENSURE_OP(*iter, ==, 2);

			*cIter;

			//*cIter = 2;
			//f(iter);
		}

		void testRectangle()
		{
			{
				RectangleIterator<2> iter(
					Vector2i(1, 2), Vector2i(3, 4));
				
				TEST_ENSURE(iter.position() == Vector2i(1, 2));
				TEST_ENSURE(!iter.done());
				++iter;

				TEST_ENSURE(iter.position() == Vector2i(2, 2));
				TEST_ENSURE(!iter.done());
				++iter;

				TEST_ENSURE(iter.position() == Vector2i(1, 3));
				TEST_ENSURE(!iter.done());
				++iter;

				TEST_ENSURE(iter.position() == Vector2i(2, 3));
				TEST_ENSURE(!iter.done());
				++iter;

				TEST_ENSURE(iter.position() == Vector2i(1, 2));
				TEST_ENSURE(iter.done());
			}
			{
				RectangleIterator<2> iter(
					Vector2i(2, 3));
				
				TEST_ENSURE(iter.position() == Vector2i(0, 0));
				TEST_ENSURE(!iter.done());
				++iter;

				TEST_ENSURE(iter.position() == Vector2i(1, 0));
				TEST_ENSURE(!iter.done());
				++iter;

				TEST_ENSURE(iter.position() == Vector2i(0, 1));
				TEST_ENSURE(!iter.done());
				++iter;

				TEST_ENSURE(iter.position() == Vector2i(1, 1));
				TEST_ENSURE(!iter.done());
				++iter;

				TEST_ENSURE(iter.position() == Vector2i(0, 2));
				TEST_ENSURE(!iter.done());
				++iter;

				TEST_ENSURE(iter.position() == Vector2i(1, 2));
				TEST_ENSURE(!iter.done());
				++iter;

				TEST_ENSURE(iter.position() == Vector2i(0, 0));
				TEST_ENSURE(iter.done());
			}
		}

		void testCounting()
		{
			{
				std::vector<integer> a;
				a.push_back(1);
				a.push_back(2);
				a.push_back(3);
				a.push_back(4);
				a.push_back(5);
				a.push_back(6);

				TEST_ENSURE(
					std::equal(a.begin(), a.end(), 
					CountingIterator<integer>(1)));
			}
			{
				std::vector<integer> a;
				a.push_back(1);
				a.push_back(3);
				a.push_back(5);
				a.push_back(7);

				TEST_ENSURE(
					std::equal(a.begin(), a.end(), 
					SparseIterator<CountingIterator<integer> >(
					CountingIterator<integer>(1), 2)));
			}
		}
	};

	void testIterators()
	{
		Test test;
		test.run();		
	}

	void addTests()
	{
		testRunner().add("Iterators", testIterators);
	}

	CallFunction run(addTests);

}
