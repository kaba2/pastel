// Description: More testing for conformal affine transformations
// DocumentationOf: conformalaffine2d.h

#include "test/test_init.h"

#include "pastel/math/conformalaffine2d/conformalaffine2d_tools.h"
#include "pastel/math/conformalaffine2d/conformalaffine2d_least_squares.h"
#include "pastel/math/sampling/uniform_sampling.h"

#include "pastel/sys/set.h"
#include "pastel/sys/vector/vector_tools.h"
#include "pastel/sys/vector/vector_locator.h"

TEST_CASE("SimilaritySimple (Transformation)")
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

		REQUIRE(scalingDelta <= 0.001);
		REQUIRE(angleDelta <= 0.001);
		REQUIRE(tDelta <=  0.001);
	}

}

TEST_CASE("SimilarityLs (Transformation)")
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
				locationSet(
					rangeSet(pattern),
					Vector_Locator<real, 2>()
				),
				locationSet(
					rangeSet(transformedPattern),
					Vector_Locator<real, 2>()
				));

		real scalingDelta = absoluteError<real>(
			matchedTransformation.scaling(), 
			transformation.scaling());
		real angleDelta = absoluteError<real>(
			matchedTransformation.rotation(), 
			transformation.rotation());
		real tDelta = norm(
			matchedTransformation.translation() - 
			transformation.translation());

		REQUIRE(scalingDelta <= 0.001);
		REQUIRE(angleDelta <= 0.001);
		REQUIRE(tDelta <=  0.001);
	}
}
