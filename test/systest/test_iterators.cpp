// Description: Testing for iterators
// DocumentationOf: iterator.txt

#include "pastelsystest.h"

#include "pastel/sys/constantiterator.h"
#include "pastel/sys/countingiterator.h"
#include "pastel/sys/sparseiterator.h"

using namespace Pastel;

namespace
{

	class Iterator_Test
		: public TestSuite
	{
	public:
		Iterator_Test()
			: TestSuite(&sysTestReport())
		{
		}

		virtual void run()
		{
			testConstant();
			testSparse();
			testCounting();
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
		Iterator_Test test;
		test.run();		
	}

	void addTests()
	{
		sysTestList().add("iterators", testIterators);
	}

	CallFunction run(addTests);

}
