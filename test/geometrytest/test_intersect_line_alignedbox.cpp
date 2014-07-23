// Description: Testing for line-alignedbox intersection
// DocumentationOf: intersect_line_alignedbox.h

#include "test_pastelgeometry.h"

#include "pastel/geometry/intersect_line_alignedbox.h"
#include "pastel/sys/rational.h"
#include "pastel/sys/biginteger.h"

#include <iostream>

using namespace Pastel;

namespace
{

	using Real = Rational<BigInteger>;

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
			testPositive();
			testNegative();
		}

		void testPositive()
		{
			// Rays are inside the aligned box.
			// The rays are axis-aligned.
			// The aligned box is centered on origin.
			// Send rays from the origin and
			// make sure they intersect the aligned box
			// as they should.
			{
				AlignedBox<Real, 2> alignedBox(
					Vector<Real, 2>(-1),
					Vector<Real, 2>(1));

				Line<Real, 2> a(
					Vector<Real, 2>(0),
					Vector<Real, 2>(1, 0));

				Line<Real, 2> b(
					Vector<Real, 2>(0),
					Vector<Real, 2>(0, 1));

				Line<Real, 2> c(
					Vector<Real, 2>(0),
					Vector<Real, 2>(-1, 0));

				Line<Real, 2> d(
					Vector<Real, 2>(0),
					Vector<Real, 2>(0, -1));

				Vector<Real, 2> hitList;

				TEST_ENSURE(intersect(a, alignedBox, hitList));
				TEST_ENSURE(hitList[0] == -1);
				TEST_ENSURE(hitList[1] == 1);

				TEST_ENSURE(intersect(b, alignedBox, hitList));
				TEST_ENSURE(hitList[0] == -1);
				TEST_ENSURE(hitList[1] == 1);

				TEST_ENSURE(intersect(c, alignedBox, hitList));
				TEST_ENSURE(hitList[0] == -1);
				TEST_ENSURE(hitList[1] == 1);

				TEST_ENSURE(intersect(d, alignedBox, hitList));
				TEST_ENSURE(hitList[0] == -1);
				TEST_ENSURE(hitList[1] == 1);
			}
			// Rays are inside the aligned box.
			// The rays are diagonal.
			// The aligned box is centered on origin.
			// Send rays from the origin and
			// make sure they intersect the aligned box
			// as they should.
			{
				AlignedBox<Real, 2> alignedBox(
					Vector<Real, 2>(-1),
					Vector<Real, 2>(1));

				Line<Real, 2> a(
					Vector<Real, 2>(0),
					Vector<Real, 2>(Real(1, 1)));

				Line<Real, 2> b(
					Vector<Real, 2>(0),
					Vector<Real, 2>(Real(-1, 1)));

				Line<Real, 2> c(
					Vector<Real, 2>(0),
					Vector<Real, 2>(Real(-1, -1)));

				Line<Real, 2> d(
					Vector<Real, 2>(0),
					Vector<Real, 2>(Real(1, -1)));

				Vector<Real, 2> hitList;

				TEST_ENSURE(intersect(a, alignedBox, hitList));
				TEST_ENSURE(hitList[0] == -1);
				TEST_ENSURE(hitList[1] == 1);

				TEST_ENSURE(intersect(b, alignedBox, hitList));
				TEST_ENSURE(hitList[0] == -1);
				TEST_ENSURE(hitList[1] == 1);

				TEST_ENSURE(intersect(c, alignedBox, hitList));
				TEST_ENSURE(hitList[0] == -1);
				TEST_ENSURE(hitList[1] == 1);

				TEST_ENSURE(intersect(d, alignedBox, hitList));
				TEST_ENSURE(hitList[0] == -1);
				TEST_ENSURE(hitList[1] == 1);
			}

			// The aligned box is centered on origin.
			// The rays originate from the boundary
			// of the aligned box.
			// Make sure the rays
			// intersect the aligned box as they should.
			{
				AlignedBox<Real, 2> alignedBox(
					Vector<Real, 2>(-1),
					Vector<Real, 2>(1));

				Line<Real, 2> a(
					Vector<Real, 2>(
					1, Real(-1, 2)),
					Vector<Real, 2>(
					-2, 1));

				Line<Real, 2> b(
					Vector<Real, 2>(
					-1, Real(1, 2)),
					Vector<Real, 2>(
					2, -1));

				Line<Real, 2> c(
					Vector<Real, 2>(
					Real(-1, 2), -1),
					Vector<Real, 2>(
					Real(-1, 2), 1));

				Vector<Real, 2> hitList;

				TEST_ENSURE(intersect(a, alignedBox, hitList));
				TEST_ENSURE(hitList[0] == 0);
				TEST_ENSURE(hitList[1] == 1);

				TEST_ENSURE(intersect(b, alignedBox, hitList));
				TEST_ENSURE(hitList[0] == 0);
				TEST_ENSURE(hitList[1] == 1);

				TEST_ENSURE(intersect(c, alignedBox, hitList));
				TEST_ENSURE(hitList[0] == 0);
				TEST_ENSURE(hitList[1] == 1);
			}
			// These rays have origin outside the aligned boxes,
			// but still intersect the aligned box.
			{
				AlignedBox<Real, 2> alignedBox(
					Vector<Real, 2>(-1),
					Vector<Real, 2>(1));

				Line<Real, 2> a(
					Vector<Real, 2>(
					0, -2),
					Vector<Real, 2>(
					1, 4));

				Line<Real, 2> b(
					Vector<Real, 2>(
					-2, 2),
					Vector<Real, 2>(
					1, -1));

				Vector<Real, 2> hitList;

				TEST_ENSURE(intersect(a, alignedBox, hitList));
				TEST_ENSURE(hitList[0] == Real(1, 4));
				TEST_ENSURE(hitList[1] == Real(3, 4));

				TEST_ENSURE(intersect(b, alignedBox, hitList));
				TEST_ENSURE(hitList[0] == 1);
				TEST_ENSURE(hitList[1] == 3);
			}
			// These rays just touch the aligned box corners.
			{
				AlignedBox<Real, 2> alignedBox(
					Vector<Real, 2>(-1),
					Vector<Real, 2>(1));

				Line<Real, 2> a(
					Vector<Real, 2>(
					0, 3),
					Vector<Real, 2>(
					-2, -4));

				Line<Real, 2> b(
					Vector<Real, 2>(
					-5, 0),
					Vector<Real, 2>(
					4, 1));

				Line<Real, 2> c(
					Vector<Real, 2>(
					-2, 1),
					Vector<Real, 2>(
					1, 0));

				Line<Real, 2> d(
					Vector<Real, 2>(
					-1, -2),
					Vector<Real, 2>(
					0, 1));

				Vector<Real, 2> hitList;

				TEST_ENSURE(intersect(a, alignedBox, hitList));
				TEST_ENSURE(hitList[0] == Real(1, 2));
				TEST_ENSURE(hitList[1] == Real(1, 2));

				TEST_ENSURE(intersect(b, alignedBox, hitList));
				TEST_ENSURE(hitList[0] == 1);
				TEST_ENSURE(hitList[1] == 1);

				TEST_ENSURE(intersect(c, alignedBox, hitList));
				TEST_ENSURE(hitList[0] == 1);
				TEST_ENSURE(hitList[1] == 3);

				TEST_ENSURE(intersect(d, alignedBox, hitList));
				TEST_ENSURE(hitList[0] == 1);
				TEST_ENSURE(hitList[1] == 3);
			}
		}

		void testNegative()
		{
			{
				AlignedBox<Real, 2> alignedBox(
					Vector<Real, 2>(-1),
					Vector<Real, 2>(1));

				Line<Real, 2> a(
					Vector<Real, 2>(
					0, -2),
					Vector<Real, 2>(
					2, 1));

				Line<Real, 2> b(
					Vector<Real, 2>(
					-2, 2),
					Vector<Real, 2>(
					1, 0));

				Line<Real, 2> c(
					Vector<Real, 2>(
					-1, -2),
					Vector<Real, 2>(
					-1, 3));

				Line<Real, 2> d(
					Vector<Real, 2>(
					0, -5),
					Vector<Real, 2>(
					2, 6));

				Vector<Real, 2> hitList;

				TEST_ENSURE(!intersect(a, alignedBox, hitList));
				TEST_ENSURE(!intersect(b, alignedBox, hitList));
				TEST_ENSURE(!intersect(c, alignedBox, hitList));
				TEST_ENSURE(!intersect(d, alignedBox, hitList));
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
		testRunner().add("intersect_line_alignedbox", test);
	}

	CallFunction run(addTest);

}
