// Description: Testing for statistics
// DocumentationOf: statistics.h

#include "test_pastelmath.h"

#include <pastel/sys/input.h>
#include <pastel/sys/rational.h>
#include <pastel/sys/locator.h>
#include <pastel/sys/pointset.h>

#include <pastel/math/statistic.h>

#include <vector>
#include <list>

using namespace Pastel;

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

		using Real = Rational<integer>;

		void test()
		{
			{
				using Point = Real;
				std::vector<Point> aSet = { -1, 7, 4, -3, 7, 8 };
				std::vector<Point> bSet = { -6, 4, 3, 2, -6, -1 };

				TEST_ENSURE(allEqual(pointMean(rangeInput(aSet)), Real(11, 3)));
				TEST_ENSURE(allEqual(pointVariance(rangeInput(aSet), false), Real(322, 15)));
				TEST_ENSURE(allEqual(pointVariance(rangeInput(aSet)), Real(322, 18)));

				TEST_ENSURE(allEqual(pointMean(rangeInput(bSet)), Real(-2, 3)));
				TEST_ENSURE(allEqual(pointVariance(rangeInput(bSet), false), Real(298, 15)));
				TEST_ENSURE(allEqual(pointVariance(rangeInput(bSet)), Real(298, 18)));

				TEST_ENSURE(scalarCovariance<Real>(rangeInput(aSet), rangeInput(bSet), false) == Real(14, 15));
				TEST_ENSURE(scalarMeanSquareError<Real>(rangeInput(aSet), rangeInput(bSet)) == Real(310, 6));
			}
			{
				std::vector<Real> aSet = { -1, 7, 4, -3, 7, 8 };
				std::vector<Real> bSet = { -6, 4, 3, 2, -6, -1 };

				TEST_ENSURE(scalarMean<Real>(rangeInput(aSet)) == Real(11, 3));
				TEST_ENSURE(scalarVariance<Real>(rangeInput(aSet), false) == Real(322, 15));
				TEST_ENSURE(scalarVariance<Real>(rangeInput(aSet)) == Real(322, 18));

				TEST_ENSURE(scalarMean<Real>(rangeInput(bSet)) == Real(-2, 3));
				TEST_ENSURE(scalarVariance<Real>(rangeInput(bSet), false) == Real(298, 15));
				TEST_ENSURE(scalarVariance<Real>(rangeInput(bSet)) == Real(298, 18));

				TEST_ENSURE(scalarCovariance<Real>(rangeInput(aSet), rangeInput(bSet), false) == Real(14, 15));
				TEST_ENSURE(scalarMeanSquareError<Real>(rangeInput(aSet), rangeInput(bSet)) == Real(310, 6));
			}

			{
				using Point = Vector<Real, 2>;
				using Locator = Vector_Locator<Real, 2>;
				std::list<Point> aSet = { { -1, -6 }, { 7, 4 }, { 4, 3 }, { -3, 2 }, { 7, -6 }, { 8, -1 } };

				TEST_ENSURE(pointMean(locationSet(rangeInput(aSet), Locator())) == Point(Real(11, 3), Real(-2, 3)));
				TEST_ENSURE(pointVariance(locationSet(rangeInput(aSet), Locator()), false) == Point(Real(322, 15), Real(298, 15)));
				TEST_ENSURE(pointVariance(locationSet(rangeInput(aSet), Locator())) == Point(Real(322, 18), Real(298, 18)));
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
		testRunner().add("statistics", test);
	}

	CallFunction run(addTest);

}
