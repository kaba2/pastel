#include "pastelgeometrytest.h"

#include "pastel/geometry/area.h"
#include "pastel/math/rational.h"

using namespace Pastel;

typedef Rational<integer> Real;

namespace
{

	class Area_Test
		: public TestSuite
	{
	public:
		Area_Test()
			: TestSuite(&geometryTestReport())
		{
		}

		virtual void run()
		{
			testSphereArea();
			testBoxArea();
			testPolygonArea();
		}

		void testSphereArea()
		{
		}

		void testBoxArea()
		{
			TEST_ENSURE_OP(
				absoluteError<real>(boxArea(Vector1(2)), 0), <, 0.001);
			TEST_ENSURE_OP(
				relativeError<real>(boxArea(Vector2(2, 2)), 8), <, 0.001);
			TEST_ENSURE_OP(
				relativeError<real>(boxArea(Vector2(3, 6)), 18), <, 0.001);
			TEST_ENSURE_OP(
				relativeError<real>(boxArea(Vector2(6, 3)), 18), <, 0.001);
			TEST_ENSURE_OP(
				relativeError<real>(boxArea(Vector2(0, 6)), 12), <, 0.001);
			TEST_ENSURE_OP(
				relativeError<real>(boxArea(Vector2(6, 0)), 12), <, 0.001);

			TEST_ENSURE_OP(
				relativeError<real>(boxArea(Vector3(1, 2, 3)), 22), <, 0.001);
			TEST_ENSURE_OP(
				relativeError<real>(boxArea(Vector3(3, 2, 1)), 22), <, 0.001);
			TEST_ENSURE_OP(
				relativeError<real>(boxArea(Vector3(2, 3, 1)), 22), <, 0.001);
			TEST_ENSURE_OP(
				relativeError<real>(boxArea(Vector3(0, 2, 3)), 12), <, 0.001);
			TEST_ENSURE_OP(
				absoluteError<real>(boxArea(Vector3(3, 0, 0)), 0), <, 0.001);
			TEST_ENSURE_OP(
				absoluteError<real>(boxArea(Vector3(0, 3, 0)), 0), <, 0.001);
			TEST_ENSURE_OP(
				absoluteError<real>(boxArea(Vector3(0, 0, 3)), 0), <, 0.001);
		}

		void testPolygonArea()
		{
			/*
			{
				// Triangle
				Polygon<Real, 2> p;
				p.pushBack(Point<Real, 2>(Real(0, 1), Real(0, 1)));
				p.pushBack(Point<Real, 2>(Real(1, 1), Real(0, 1)));
				p.pushBack(Point<Real, 2>(Real(1, 1), Real(2, 1)));

				REPORT(signedArea(p) != Real(1, 1));
			}
			{
				// Square
				Polygon<Real, 2> p;
				p.pushBack(Point<Real, 2>(Real(0, 1), Real(0, 1)));
				p.pushBack(Point<Real, 2>(Real(1, 1), Real(0, 1)));
				p.pushBack(Point<Real, 2>(Real(1, 1), Real(1, 1)));
				p.pushBack(Point<Real, 2>(Real(0, 1), Real(1, 1)));

				REPORT(signedArea(p) != Real(1, 1));
			}
			{
				// Reversed square
				Polygon<Real, 2> p;
				p.pushBack(Point<Real, 2>(Real(0, 1), Real(0, 1)));
				p.pushBack(Point<Real, 2>(Real(0, 1), Real(1, 1)));
				p.pushBack(Point<Real, 2>(Real(1, 1), Real(1, 1)));
				p.pushBack(Point<Real, 2>(Real(1, 1), Real(0, 1)));

				REPORT(signedArea(p) != Real(-1, 1));
			}
			{
				// Translated square
				Polygon<Real, 2> p;
				p.pushBack(Point<Real, 2>(Real(0 + 2, 1), Real(0 - 3, 1)));
				p.pushBack(Point<Real, 2>(Real(1 + 2, 1), Real(0 - 3, 1)));
				p.pushBack(Point<Real, 2>(Real(1 + 2, 1), Real(1 - 3, 1)));
				p.pushBack(Point<Real, 2>(Real(0 + 2, 1), Real(1 - 3, 1)));

				REPORT(signedArea(p) != Real(1, 1));
			}
			{
				// Six sided polygon.
				Polygon<Real, 2> p;
				p.pushBack(Point<Real, 2>(Real(0, 1), Real(3, 1)));
				p.pushBack(Point<Real, 2>(Real(2, 1), Real(2, 1)));
				p.pushBack(Point<Real, 2>(Real(3, 1), Real(0, 1)));
				p.pushBack(Point<Real, 2>(Real(2, 1), Real(-2, 1)));
				p.pushBack(Point<Real, 2>(Real(0, 1), Real(-3, 1)));
				p.pushBack(Point<Real, 2>(Real(-2, 1), Real(-2, 1)));
				p.pushBack(Point<Real, 2>(Real(-3, 1), Real(0, 1)));
				p.pushBack(Point<Real, 2>(Real(-2, 1), Real(2, 1)));

				REPORT(signedArea(p) != Real(24, 1));
			}
			{
				// Half of the six sided polygon.
				Polygon<Real, 2> p;
				p.pushBack(Point<Real, 2>(Real(0, 1), Real(3, 1)));
				p.pushBack(Point<Real, 2>(Real(2, 1), Real(2, 1)));
				p.pushBack(Point<Real, 2>(Real(3, 1), Real(0, 1)));
				p.pushBack(Point<Real, 2>(Real(2, 1), Real(-2, 1)));
				p.pushBack(Point<Real, 2>(Real(0, 1), Real(-3, 1)));

				REPORT(signedArea(p) != Real(12, 1));
			}
			{
				// Quarter of the six sided polygon.
				Polygon<Real, 2> p;
				p.pushBack(Point<Real, 2>(Real(0, 1), Real(3, 1)));
				p.pushBack(Point<Real, 2>(Real(2, 1), Real(2, 1)));
				p.pushBack(Point<Real, 2>(Real(3, 1), Real(0, 1)));

				REPORT(signedArea(p) != Real(6, 1));
			}
			{
				// Translated quarter of the six sided polygon.
				Polygon<Real, 2> p;
				p.pushBack(Point<Real, 2>(Real(0 - 5, 1), Real(3 + 15, 1)));
				p.pushBack(Point<Real, 2>(Real(2 - 5, 1), Real(2 + 15, 1)));
				p.pushBack(Point<Real, 2>(Real(3 - 5, 1), Real(0 + 15, 1)));

				REPORT(signedArea(p) != Real(6, 1));
			}
			*/
		}
	};


	void testBegin()
	{
		Area_Test test;
		test.run();
	}

	void testAdd()
	{
		geometryTestList().add("Area", testBegin);
	}

	CallFunction run(testAdd);

}
