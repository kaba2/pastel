// Description: Testing for bounded array
// DocumentationOf: bounded_array.h

#include "test_pastelsys.h"

#include <pastel/sys/bounded_array.h>

using namespace Pastel;
using namespace std;

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
		}

		void test()
		{
			using Set = BoundedArray<BoundedArray_Settings<integer, 3>>;
			using RealSet = BoundedArray<BoundedArray_Settings<real, 2>>;
			Set a;
			{
				TEST_ENSURE(a.empty());
			}
			{
				a.emplaceBack(1);
				TEST_ENSURE(!a.empty());
				TEST_ENSURE(!a.full());
				TEST_ENSURE_OP(a.size(), ==, 1);
				TEST_ENSURE(a.front() == 1);
				TEST_ENSURE(a.back() == 1);
				TEST_ENSURE(a[0] == 1);
			}
			{
				a.emplaceBack(2);
				TEST_ENSURE(!a.empty());
				TEST_ENSURE(!a.full());
				TEST_ENSURE_OP(a.size(), ==, 2);
				TEST_ENSURE(a.front() == 1);
				TEST_ENSURE(a.back() == 2);
				TEST_ENSURE(a[0] == 1);
				TEST_ENSURE(a[1] == 2);
			}
			{
				a.emplaceBack(3);
				TEST_ENSURE(!a.empty());
				TEST_ENSURE(a.full());
				TEST_ENSURE_OP(a.size(), ==, 3);
				TEST_ENSURE(a.front() == 1);
				TEST_ENSURE(a.back() == 3);
				TEST_ENSURE(a[0] == 1);
				TEST_ENSURE(a[1] == 2);
				TEST_ENSURE(a[2] == 3);
			}
			{
				Set b = { 5, 3, 4 };

				std::vector<integer> correctSet = { 5, 3, 4 };
				TEST_ENSURE(boost::equal(b.range(), range(correctSet.begin(), correctSet.end())));
				TEST_ENSURE(boost::equal(b.range(), correctSet));
				TEST_ENSURE(boost::equal(b, correctSet));

				auto i = b.begin();
				TEST_ENSURE(*i == 5);
				TEST_ENSURE(i[0] == 5);
				TEST_ENSURE(i[1] == 3);
				TEST_ENSURE(i[2] == 4);

				++i;
				TEST_ENSURE(*i == 3);
				TEST_ENSURE(i[0] == 3);
				TEST_ENSURE(i[1] == 4);

				++i;
				TEST_ENSURE(*i == 4);
				TEST_ENSURE(i[0] == 4);

				++i;
				TEST_ENSURE(i == b.end());
			}
			{
				Set b = { 5, 3, 4 };
				TEST_ENSURE_OP(b.size(), ==, 3);
				TEST_ENSURE_OP(b.back(), ==, 4);

				b.popBack();
				TEST_ENSURE_OP(b.size(), ==, 2);
				TEST_ENSURE_OP(b.back(), ==, 3);

				b.popBack();
				TEST_ENSURE_OP(b.size(), ==, 1);
				TEST_ENSURE_OP(b.back(), ==, 5);

				b.popBack();
				TEST_ENSURE_OP(b.size(), ==, 0);
				TEST_ENSURE(b.empty());
			}
			{
				Set b = { 5, 3, 4 };
				b.clear();

				TEST_ENSURE(b.empty());
				TEST_ENSURE_OP(b.size(), ==, 0);
			}

			{
				Set a = { 1, 2 };
				Set b = { 5, 3, 4 };
				
				a.swap(b);
				TEST_ENSURE_OP(a.size(), ==, 3);
				TEST_ENSURE_OP(b.size(), ==, 2);

				TEST_ENSURE_OP(a[0], ==, 5);
				TEST_ENSURE_OP(a[1], ==, 3);
				TEST_ENSURE_OP(a[2], ==, 4);

				TEST_ENSURE_OP(b[0], ==, 1);
				TEST_ENSURE_OP(b[1], ==, 2);
			}
			{
				RealSet a = { 0.5, 1.5 };
				Set b(a);
				RealSet c(b);
				Set d(b);
				
				Set correct = { 0, 1 };
				TEST_ENSURE(boost::equal(b, correct));
				TEST_ENSURE(boost::equal(c, correct));
				TEST_ENSURE(boost::equal(d, correct));
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
		testRunner().add("bounded_array", test);
	}

	CallFunction run(addTest);

}
