#include "pastelgeometrytest.h"

#include "pastel/geometry/area.h"
#include "pastel/math/rational.h"

using namespace Pastel;

typedef Rational<integer> Real;

namespace
{

	void testPolygon()
	{
		/*
		{
			// Triangle
			Polygon<2, Real> p;
			p.pushBack(Point<2, Real>(Real(0, 1), Real(0, 1)));
			p.pushBack(Point<2, Real>(Real(1, 1), Real(0, 1)));
			p.pushBack(Point<2, Real>(Real(1, 1), Real(2, 1)));

			REPORT(signedArea(p) != Real(1, 1));
		}
		{
			// Square
			Polygon<2, Real> p;
			p.pushBack(Point<2, Real>(Real(0, 1), Real(0, 1)));
			p.pushBack(Point<2, Real>(Real(1, 1), Real(0, 1)));
			p.pushBack(Point<2, Real>(Real(1, 1), Real(1, 1)));
			p.pushBack(Point<2, Real>(Real(0, 1), Real(1, 1)));

			REPORT(signedArea(p) != Real(1, 1));
		}
		{
			// Reversed square
			Polygon<2, Real> p;
			p.pushBack(Point<2, Real>(Real(0, 1), Real(0, 1)));
			p.pushBack(Point<2, Real>(Real(0, 1), Real(1, 1)));
			p.pushBack(Point<2, Real>(Real(1, 1), Real(1, 1)));
			p.pushBack(Point<2, Real>(Real(1, 1), Real(0, 1)));

			REPORT(signedArea(p) != Real(-1, 1));
		}
		{
			// Translated square
			Polygon<2, Real> p;
			p.pushBack(Point<2, Real>(Real(0 + 2, 1), Real(0 - 3, 1)));
			p.pushBack(Point<2, Real>(Real(1 + 2, 1), Real(0 - 3, 1)));
			p.pushBack(Point<2, Real>(Real(1 + 2, 1), Real(1 - 3, 1)));
			p.pushBack(Point<2, Real>(Real(0 + 2, 1), Real(1 - 3, 1)));

			REPORT(signedArea(p) != Real(1, 1));
		}
		{
			// Six sided polygon.
			Polygon<2, Real> p;
			p.pushBack(Point<2, Real>(Real(0, 1), Real(3, 1)));
			p.pushBack(Point<2, Real>(Real(2, 1), Real(2, 1)));
			p.pushBack(Point<2, Real>(Real(3, 1), Real(0, 1)));
			p.pushBack(Point<2, Real>(Real(2, 1), Real(-2, 1)));
			p.pushBack(Point<2, Real>(Real(0, 1), Real(-3, 1)));
			p.pushBack(Point<2, Real>(Real(-2, 1), Real(-2, 1)));
			p.pushBack(Point<2, Real>(Real(-3, 1), Real(0, 1)));
			p.pushBack(Point<2, Real>(Real(-2, 1), Real(2, 1)));

			REPORT(signedArea(p) != Real(24, 1));
		}
		{
			// Half of the six sided polygon.
			Polygon<2, Real> p;
			p.pushBack(Point<2, Real>(Real(0, 1), Real(3, 1)));
			p.pushBack(Point<2, Real>(Real(2, 1), Real(2, 1)));
			p.pushBack(Point<2, Real>(Real(3, 1), Real(0, 1)));
			p.pushBack(Point<2, Real>(Real(2, 1), Real(-2, 1)));
			p.pushBack(Point<2, Real>(Real(0, 1), Real(-3, 1)));

			REPORT(signedArea(p) != Real(12, 1));
		}
		{
			// Quarter of the six sided polygon.
			Polygon<2, Real> p;
			p.pushBack(Point<2, Real>(Real(0, 1), Real(3, 1)));
			p.pushBack(Point<2, Real>(Real(2, 1), Real(2, 1)));
			p.pushBack(Point<2, Real>(Real(3, 1), Real(0, 1)));

			REPORT(signedArea(p) != Real(6, 1));
		}
		{
			// Translated quarter of the six sided polygon.
			Polygon<2, Real> p;
			p.pushBack(Point<2, Real>(Real(0 - 5, 1), Real(3 + 15, 1)));
			p.pushBack(Point<2, Real>(Real(2 - 5, 1), Real(2 + 15, 1)));
			p.pushBack(Point<2, Real>(Real(3 - 5, 1), Real(0 + 15, 1)));

			REPORT(signedArea(p) != Real(6, 1));
		}
		*/
	}

	void testBegin()
	{
		testPolygon();
	}

	void testAdd()
	{
		geometryTestList().add("Area", testBegin);
	}

	CallFunction run(testAdd);

}
