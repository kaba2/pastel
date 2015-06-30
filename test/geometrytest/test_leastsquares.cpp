// Description: Testing for least-squares conformal affine transformation
// DocumentationOf: conformalaffine2d.h

#include "test_pastelgeometry.h"

#include "pastel/math/affine/affine_transformation.h"
#include "pastel/math/sampling/uniform_sampling.h"
#include "pastel/math/conformalaffine2d/conformalaffine2d_tools.h"
#include "pastel/math/conformalaffine2d/conformalaffine2d_least_squares.h"

#include "pastel/sys/locator.h"
#include "pastel/sys/vector/vector_tools.h"
#include "pastel/sys/input.h"
#include "pastel/sys/set.h"

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
			testNoiselessSimilarity();
		}

		void testNoiselessSimilarity()
		{
			for (integer points = 0;points < 1000;++points)
			{
				std::vector<Vector2> from;
				from.reserve(points);
				std::vector<Vector2> to;
				to.reserve(points);
				
				real scale = random<real>() * 0.9 + 0.1;
				real angle = random<real>() * 2 * constantPi<real>();
				Vector2 translation(random<real>() * 2 - 1, random<real>() * 2 - 1);

				if (points <= 1)
				{
					scale = 1;
					angle = 0;
				}

				if (points == 0)
				{
					translation = 0;
				}

				ConformalAffine2D<real> transformation(scale, angle, translation);

				for (integer j = 0;j < points;++j)
				{
					Vector2 fromPoint = randomVectorSphere<real, 2>();
					from.push_back(fromPoint);

					Vector2 toPoint = transformPoint(transformation, fromPoint);
					to.push_back(toPoint);
				}

				ConformalAffine2D<real> similarity =
					lsConformalAffine(rangeSet(from), rangeSet(to));

				TEST_ENSURE(absoluteError<real>(similarity.scaling(), scale) <= 0.001);
				TEST_ENSURE(absoluteError<real>(similarity.rotation(), angle) <= 0.001);
				TEST_ENSURE(norm(similarity.translation() - translation) <= 0.001);
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
		testRunner().add("LeastSquares", test);
	}

	CallFunction run(addTest);

}
