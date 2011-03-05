#include "pastelgeometrytest.h"

#include "pastel/geometry/overlaps_alignedbox_alignedbox.h"

#include "pastel/sys/rational.h"
#include "pastel/sys/rational_tools.h"

#include <iostream>
using std::cout;
using std::endl;

using namespace Pastel;

namespace
{

	typedef Rational<integer> Real;

	class Test
		: public TestSuite
	{
	public:
		Test()
			: TestSuite(&geometryTestReport())
		{
		}

		virtual void run()
		{
			testNegative();
			testPositive();
			testSingular();
			testGrazing();
		}

		void testGrazing()
		{
			// Nine aligned boxes have been placed
			// in a square grid like this:
			// g h i
			// d e f
			// a b c
			// Their boundaries touch each other.
			// Depending on the topology, there
			// either should or should not be
			// intersections.

			AlignedBox<Real, 2> a(
				Vector<Real, 2>(0, 0),
				Vector<Real, 2>(1, 1));

			AlignedBox<Real, 2> b(
				Vector<Real, 2>(1, 0),
				Vector<Real, 2>(2, 1));

			AlignedBox<Real, 2> c(
				Vector<Real, 2>(2, 0),
				Vector<Real, 2>(3, 1));

			AlignedBox<Real, 2> d(
				Vector<Real, 2>(0, 1),
				Vector<Real, 2>(1, 2));

			AlignedBox<Real, 2> e(
				Vector<Real, 2>(1, 1),
				Vector<Real, 2>(2, 2));

			AlignedBox<Real, 2> f(
				Vector<Real, 2>(2, 1),
				Vector<Real, 2>(3, 2));

			AlignedBox<Real, 2> g(
				Vector<Real, 2>(0, 2),
				Vector<Real, 2>(1, 3));

			AlignedBox<Real, 2> h(
				Vector<Real, 2>(1, 2),
				Vector<Real, 2>(2, 3));

			AlignedBox<Real, 2> i(
				Vector<Real, 2>(2, 2),
				Vector<Real, 2>(3, 3));

			TEST_ENSURE(overlaps(e, a));
			TEST_ENSURE(overlaps(e, b));
			TEST_ENSURE(overlaps(e, c));
			TEST_ENSURE(overlaps(e, d));

			TEST_ENSURE(overlaps(e, f));
			TEST_ENSURE(overlaps(e, g));
			TEST_ENSURE(overlaps(e, h));
			TEST_ENSURE(overlaps(e, i));

			TEST_ENSURE(overlaps(a, e));
			TEST_ENSURE(overlaps(b, e));
			TEST_ENSURE(overlaps(c, e));
			TEST_ENSURE(overlaps(d, e));

			TEST_ENSURE(overlaps(f, e));
			TEST_ENSURE(overlaps(g, e));
			TEST_ENSURE(overlaps(h, e));
			TEST_ENSURE(overlaps(i, e));

			e.maxTopology().set(Topology::Open);

			TEST_ENSURE(overlaps(e, a));
			TEST_ENSURE(overlaps(e, b));
			TEST_ENSURE(!overlaps(e, c));
			TEST_ENSURE(overlaps(e, d));

			TEST_ENSURE(!overlaps(e, f));
			TEST_ENSURE(!overlaps(e, g));
			TEST_ENSURE(!overlaps(e, h));
			TEST_ENSURE(!overlaps(e, i));

			TEST_ENSURE(overlaps(a, e));
			TEST_ENSURE(overlaps(b, e));
			TEST_ENSURE(!overlaps(c, e));
			TEST_ENSURE(overlaps(d, e));

			TEST_ENSURE(!overlaps(f, e));
			TEST_ENSURE(!overlaps(g, e));
			TEST_ENSURE(!overlaps(h, e));
			TEST_ENSURE(!overlaps(i, e));

			e.minTopology().set(Topology::Open);

			TEST_ENSURE(!overlaps(e, a));
			TEST_ENSURE(!overlaps(e, b));
			TEST_ENSURE(!overlaps(e, c));
			TEST_ENSURE(!overlaps(e, d));

			TEST_ENSURE(!overlaps(e, f));
			TEST_ENSURE(!overlaps(e, g));
			TEST_ENSURE(!overlaps(e, h));
			TEST_ENSURE(!overlaps(e, i));

			TEST_ENSURE(!overlaps(a, e));
			TEST_ENSURE(!overlaps(b, e));
			TEST_ENSURE(!overlaps(c, e));
			TEST_ENSURE(!overlaps(d, e));

			TEST_ENSURE(!overlaps(f, e));
			TEST_ENSURE(!overlaps(g, e));
			TEST_ENSURE(!overlaps(h, e));
			TEST_ENSURE(!overlaps(i, e));

			e.maxTopology().set(Topology::Closed);

			TEST_ENSURE(!overlaps(e, a));
			TEST_ENSURE(!overlaps(e, b));
			TEST_ENSURE(!overlaps(e, c));
			TEST_ENSURE(!overlaps(e, d));

			TEST_ENSURE(overlaps(e, f));
			TEST_ENSURE(!overlaps(e, g));
			TEST_ENSURE(overlaps(e, h));
			TEST_ENSURE(overlaps(e, i));

			TEST_ENSURE(!overlaps(a, e));
			TEST_ENSURE(!overlaps(b, e));
			TEST_ENSURE(!overlaps(c, e));
			TEST_ENSURE(!overlaps(d, e));

			TEST_ENSURE(overlaps(f, e));
			TEST_ENSURE(!overlaps(g, e));
			TEST_ENSURE(overlaps(h, e));
			TEST_ENSURE(overlaps(i, e));
		}

		void testNegative()
		{
			// Nine non-intersecting aligned boxes have been placed
			// in a square grid like this:
			// g h i
			// d e f
			// a b c
			// Make sure the algorithm agrees that e
			// does not intersect any other aligned box.

			AlignedBox<Real, 2> a(
				Vector<Real, 2>(0, 0),
				Vector<Real, 2>(1, 1));

			AlignedBox<Real, 2> b(
				Vector<Real, 2>(2, 0),
				Vector<Real, 2>(3, 1));

			AlignedBox<Real, 2> c(
				Vector<Real, 2>(4, 0),
				Vector<Real, 2>(5, 1));

			AlignedBox<Real, 2> d(
				Vector<Real, 2>(0, 2),
				Vector<Real, 2>(1, 3));

			AlignedBox<Real, 2> e(
				Vector<Real, 2>(2, 2),
				Vector<Real, 2>(3, 3));

			AlignedBox<Real, 2> f(
				Vector<Real, 2>(4, 2),
				Vector<Real, 2>(5, 3));

			AlignedBox<Real, 2> g(
				Vector<Real, 2>(0, 4),
				Vector<Real, 2>(1, 5));

			AlignedBox<Real, 2> h(
				Vector<Real, 2>(2, 4),
				Vector<Real, 2>(3, 5));

			AlignedBox<Real, 2> i(
				Vector<Real, 2>(4, 4),
				Vector<Real, 2>(5, 5));

			TEST_ENSURE(!overlaps(e, a));
			TEST_ENSURE(!overlaps(e, b));
			TEST_ENSURE(!overlaps(e, c));
			TEST_ENSURE(!overlaps(e, d));

			TEST_ENSURE(!overlaps(e, f));
			TEST_ENSURE(!overlaps(e, g));
			TEST_ENSURE(!overlaps(e, h));
			TEST_ENSURE(!overlaps(e, i));

			TEST_ENSURE(!overlaps(a, e));
			TEST_ENSURE(!overlaps(b, e));
			TEST_ENSURE(!overlaps(c, e));
			TEST_ENSURE(!overlaps(d, e));

			TEST_ENSURE(!overlaps(f, e));
			TEST_ENSURE(!overlaps(g, e));
			TEST_ENSURE(!overlaps(h, e));
			TEST_ENSURE(!overlaps(i, e));
		}

		void testPositive()
		{
			// B fully contained in A.
			{
				AlignedBox<Real, 2> a(
					Vector<Real, 2>(0, 0),
					Vector<Real, 2>(10, 20));

				AlignedBox<Real, 2> b(
					Vector<Real, 2>(5, 4),
					Vector<Real, 2>(8, 15));

				TEST_ENSURE(overlaps(a, b));
				TEST_ENSURE(overlaps(b, a));
			}

			// A overlaps B
			{
				AlignedBox<Real, 2> a(
					Vector<Real, 2>(0, 0),
					Vector<Real, 2>(10, 20));

				AlignedBox<Real, 2> b(
					Vector<Real, 2>(6, 3),
					Vector<Real, 2>(15, 18));

				TEST_ENSURE(overlaps(a, b));
				TEST_ENSURE(overlaps(b, a));
			}

			// A overlaps B
			{
				AlignedBox<Real, 2> a(
					Vector<Real, 2>(0, 0),
					Vector<Real, 2>(10, 20));

				AlignedBox<Real, 2> b(
					Vector<Real, 2>(1, -5),
					Vector<Real, 2>(8, 25));
				TEST_ENSURE(overlaps(a, b));
				TEST_ENSURE(overlaps(b, a));
			}
		}

		void testSingular()
		{
			// Boxes share a corner point.
			{
				AlignedBox<Real, 2> a(
					Vector<Real, 2>(0, 0),
					Vector<Real, 2>(1, 1));

				AlignedBox<Real, 2> b(
					Vector<Real, 2>(1, 1),
					Vector<Real, 2>(2, 2));

				TEST_ENSURE(overlaps(a, b));
				TEST_ENSURE(overlaps(b, a));
			}

			// Boxes share an edge
			{
				AlignedBox<Real, 2> a(
					Vector<Real, 2>(0, 0),
					Vector<Real, 2>(1, 1));

				AlignedBox<Real, 2> b(
					Vector<Real, 2>(1, 0),
					Vector<Real, 2>(2, 1));

				TEST_ENSURE(overlaps(a, b));
				TEST_ENSURE(overlaps(b, a));
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
		geometryTestList().add("overlaps_alignedbox_alignedbox", test);
	}

	CallFunction run(addTest);

}
