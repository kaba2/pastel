// Description: Testing for alignedbox-alignedbox containment
// DocumentationOf: contains_alignedbox_alignedbox.h

#include "test_pastelgeometry.h"

#include "pastel/geometry/contains_alignedbox_alignedbox.h"

#include "pastel/sys/rational.h"

#include <iostream>
using std::cout;
using std::endl;

using namespace Pastel;

namespace
{

	using Real = Rational<integer>;

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
			testEasy();
			testEmpty();
			testSame();
			testPartial();
		}

		void testSame()
		{
			AlignedBox<Real, 2> aBox(0, 0, 1, 1);
			AlignedBox<Real, 2> bBox(0, 0, 1, 1);

			TEST_ENSURE(contains(aBox, bBox));
			TEST_ENSURE(contains(bBox, aBox));

			aBox.maxTopology().set(Topology::Closed);
			TEST_ENSURE(contains(aBox, bBox));
			TEST_ENSURE(!contains(bBox, aBox));
		}

		void testEasy()
		{
			AlignedBox<Real, 2> aBox(0, 0, 3, 3);
			AlignedBox<Real, 2> bBox(1, 1, 2, 2);

			TEST_ENSURE(contains(aBox, bBox));
			TEST_ENSURE(!contains(bBox, aBox));
		}

		void testEmpty()
		{
			{
				AlignedBox<Real, 2> aBox(0, 0, 2, 2);
				AlignedBox<Real, 2> bBox(1, 1, 1, 1);

				TEST_ENSURE(contains(aBox, bBox));
				TEST_ENSURE(!contains(bBox, aBox));
			}
			{
				AlignedBox<Real, 2> aBox(0, 0, 2, 2);
				AlignedBox<Real, 2> bBox(-1, -3, -1, -3);

				TEST_ENSURE(contains(aBox, bBox));
				TEST_ENSURE(!contains(bBox, aBox));
			}
			{
				AlignedBox<Real, 2> aBox(0, 0, 3, 3);
				AlignedBox<Real, 2> bBox(1, 1, -1, 2);

				TEST_ENSURE(contains(aBox, bBox));
				TEST_ENSURE(!contains(bBox, aBox));
			}
			{
				AlignedBox<Real, 2> aBox(0, 0, 0, 0);
				AlignedBox<Real, 2> bBox(0, 0, 0, 0);

				TEST_ENSURE(contains(aBox, bBox));
				TEST_ENSURE(contains(bBox, aBox));
			}
			{
				AlignedBox<Real, 2> aBox(0, 0, 0, 0);
				AlignedBox<Real, 2> bBox(1, 1, 1, 1);

				TEST_ENSURE(contains(aBox, bBox));
				TEST_ENSURE(contains(bBox, aBox));
			}
		}

		void testPartial()
		{
			{
				AlignedBox<Real, 2> aBox(0, 0, 3, 3);
				AlignedBox<Real, 2> bBox(1, 1, 4, 2);

				TEST_ENSURE(!contains(aBox, bBox));
				TEST_ENSURE(!contains(bBox, aBox));
			}
			{
				AlignedBox<Real, 2> aBox(0, 0, 3, 3);
				AlignedBox<Real, 2> bBox(-1, 1, 2, 2);

				TEST_ENSURE(!contains(aBox, bBox));
				TEST_ENSURE(!contains(bBox, aBox));
			}
			{
				AlignedBox<Real, 2> aBox(0, 0, 3, 3);
				AlignedBox<Real, 2> bBox(1, 1, 2, 4);

				TEST_ENSURE(!contains(aBox, bBox));
				TEST_ENSURE(!contains(bBox, aBox));
			}
			{
				AlignedBox<Real, 2> aBox(0, 0, 3, 3);
				AlignedBox<Real, 2> bBox(1, -1, 2, 2);

				TEST_ENSURE(!contains(aBox, bBox));
				TEST_ENSURE(!contains(bBox, aBox));
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
		testRunner().add("contains_alignedbox_alignedbox", test);
	}

	CallFunction run(addTest);

}
