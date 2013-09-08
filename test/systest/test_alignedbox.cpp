// Description: Testing for AlignedBox
// Documentation: unit_testing.txt

#include "test_pastelsys.h"

#include <pastel/sys/alignedbox.h>

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

		using Real = real;

		virtual void run()
		{
			test1d();
			test2d();
			test3d();
			test4d();
		}

		void test1d()
		{
			AlignedBox<Real, 1> a;
			{
				TEST_ENSURE_OP(a.width(), ==, -infinity<Real>());
			}

			a.set(1, 2);
			{
				TEST_ENSURE_OP(a.min().x(), ==, 1);
				TEST_ENSURE_OP(a.max().x(), ==, 2);
				TEST_ENSURE_OP(a.width(), ==, 1);
			}
		}

		void test2d()
		{
			AlignedBox<Real, 2> a;
			{
				TEST_ENSURE_OP(a.width(), ==, -infinity<Real>());
				TEST_ENSURE_OP(a.height(), ==, -infinity<Real>());
			}

			a.set(1, 2, 3, 5);
			{
				TEST_ENSURE_OP(a.min().x(), ==, 1);
				TEST_ENSURE_OP(a.min().y(), ==, 2);
				TEST_ENSURE_OP(a.max().x(), ==, 3);
				TEST_ENSURE_OP(a.max().y(), ==, 5);
				TEST_ENSURE_OP(a.width(), ==, 2);
				TEST_ENSURE_OP(a.height(), ==, 3);
			}
		}

		void test3d()
		{
			AlignedBox<Real, 3> a;
			{
				TEST_ENSURE_OP(a.width(), ==, -infinity<Real>());
				TEST_ENSURE_OP(a.height(), ==, -infinity<Real>());
				TEST_ENSURE_OP(a.depth(), ==, -infinity<Real>());
			}

			a.set(1, 2, 3, 5, 7, 9);
			{
				TEST_ENSURE_OP(a.min().x(), ==, 1);
				TEST_ENSURE_OP(a.min().y(), ==, 2);
				TEST_ENSURE_OP(a.min().z(), ==, 3);
				TEST_ENSURE_OP(a.max().x(), ==, 5);
				TEST_ENSURE_OP(a.max().y(), ==, 7);
				TEST_ENSURE_OP(a.max().z(), ==, 9);
				TEST_ENSURE_OP(a.width(), ==, 4);
				TEST_ENSURE_OP(a.height(), ==, 5);
				TEST_ENSURE_OP(a.depth(), ==, 6);
			}
		}

		void test4d()
		{
			AlignedBox<Real, 4> a;
			{
				TEST_ENSURE_OP(a.width(), ==, -infinity<Real>());
				TEST_ENSURE_OP(a.height(), ==, -infinity<Real>());
				TEST_ENSURE_OP(a.depth(), ==, -infinity<Real>());
			}

			a.set(1, 2, 3, 4, 5, 7, 9, 11);
			{
				TEST_ENSURE_OP(a.min().x(), ==, 1);
				TEST_ENSURE_OP(a.min().y(), ==, 2);
				TEST_ENSURE_OP(a.min().z(), ==, 3);
				TEST_ENSURE_OP(a.min().w(), ==, 4);
				TEST_ENSURE_OP(a.max().x(), ==, 5);
				TEST_ENSURE_OP(a.max().y(), ==, 7);
				TEST_ENSURE_OP(a.max().z(), ==, 9);
				TEST_ENSURE_OP(a.max().w(), ==, 11);
				TEST_ENSURE_OP(a.width(), ==, 4);
				TEST_ENSURE_OP(a.height(), ==, 5);
				TEST_ENSURE_OP(a.depth(), ==, 6);
				TEST_ENSURE_OP(a.extent().w(), ==, 7);
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
		testRunner().add("AlignedBox", test);
	}

	CallFunction run(addTest);

}
