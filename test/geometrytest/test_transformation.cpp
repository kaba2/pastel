// Description: More testing for conformal affine transformations
// DocumentationOf: conformalaffine2d.h

#include "test_pastelgeometry.h"

#include "pastel/math/conformalaffine2d/conformalaffine2d_tools.h"
#include "pastel/math/conformalaffine2d/conformalaffine2d_least_squares.h"
#include "pastel/math/sampling/uniform_sampling.h"

#include "pastel/sys/vector/vector_tools.h"
#include "pastel/sys/locator/vector_locator.h"

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
			testSimilaritySimple();
			testSimilarityLs();
		}

		void testSimilaritySimple()
		{
			for (integer i = 0;i < 10000;++i)
			{
				ConformalAffine2D<real> transformation(
					random<real>() * 2 + 1,
					random<real>() * 2 * constantPi<real>(),
					Vector2(random<real>() * 2 - 1, random<real>() * 2 - 1));

				Vector2 aFrom(random<real>(), random<real>());
				Vector2 bFrom(random<real>(), random<real>());

				Vector2 aTo(transformPoint(transformation, aFrom));
				Vector2 bTo(transformPoint(transformation, bFrom));

				ConformalAffine2D<real> matchedTransformation =
					conformalAffine(aFrom, bFrom, aTo, bTo);

				real scalingDelta = absoluteError<real>(
					matchedTransformation.scaling(), 
					transformation.scaling());
				real angleDelta = absoluteError<real>(
					matchedTransformation.rotation(), 
					transformation.rotation());
				real tDelta = norm(
					matchedTransformation.translation() - 
					transformation.translation());

				TEST_ENSURE_OP(scalingDelta, <=, 0.001);
				TEST_ENSURE_OP(angleDelta, <=, 0.001);
				TEST_ENSURE_OP(tDelta, <=,  0.001);
			}

		}

		void testSimilarityLs()
		{
			for (integer i = 0;i < 10000;++i)
			{
				ConformalAffine2D<real> transformation(
					random<real>() * 2 + 1,
					random<real>() * 2 * constantPi<real>(),
					Vector2(random<real>() * 2 - 1, random<real>() * 2 - 1));

				std::vector<Vector2> pattern;
				std::vector<Vector2> transformedPattern;

				for (integer i = 0;i < 1000;++i)
				{
					pattern.push_back(randomVector<real, 2>());
					transformedPattern.push_back(
						transformPoint(transformation, pattern.back()));
				}

				ConformalAffine2D<real> matchedTransformation =
					lsConformalAffine(
					range(pattern.begin(), pattern.end()),
					range(transformedPattern.begin(), transformedPattern.end()),
					Vector_Locator<real, 2>(),
					Vector_Locator<real, 2>());

				real scalingDelta = absoluteError<real>(
					matchedTransformation.scaling(), 
					transformation.scaling());
				real angleDelta = absoluteError<real>(
					matchedTransformation.rotation(), 
					transformation.rotation());
				real tDelta = norm(
					matchedTransformation.translation() - 
					transformation.translation());

				TEST_ENSURE_OP(scalingDelta, <=, 0.001);
				TEST_ENSURE_OP(angleDelta, <=, 0.001);
				TEST_ENSURE_OP(tDelta, <=,  0.001);
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
		testRunner().add("Transformation", test);
	}

	CallFunction run(addTest);

}
