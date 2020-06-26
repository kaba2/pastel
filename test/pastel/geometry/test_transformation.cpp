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
		ConformalAffine2D<dreal> transformation(
			random<dreal>() * 2 + 1,
			random<dreal>() * 2 * constantPi<dreal>(),
			Vector2(random<dreal>() * 2 - 1, random<dreal>() * 2 - 1));

		Vector2 aFrom(random<dreal>(), random<dreal>());
		Vector2 bFrom(random<dreal>(), random<dreal>());

		Vector2 aTo(transformPoint(transformation, aFrom));
		Vector2 bTo(transformPoint(transformation, bFrom));

		ConformalAffine2D<dreal> matchedTransformation =
			conformalAffine(aFrom, bFrom, aTo, bTo);

		dreal scalingDelta = absoluteError<dreal>(
			matchedTransformation.scaling(), 
			transformation.scaling());
		dreal angleDelta = absoluteError<dreal>(
			matchedTransformation.rotation(), 
			transformation.rotation());
		dreal tDelta = norm(
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
		ConformalAffine2D<dreal> transformation(
			random<dreal>() * 2 + 1,
			random<dreal>() * 2 * constantPi<dreal>(),
			Vector2(random<dreal>() * 2 - 1, random<dreal>() * 2 - 1));

		std::vector<Vector2> pattern;
		std::vector<Vector2> transformedPattern;

		for (integer i = 0;i < 1000;++i)
		{
			pattern.push_back(randomVector<dreal, 2>());
			transformedPattern.push_back(
				transformPoint(transformation, pattern.back()));
		}

		ConformalAffine2D<dreal> matchedTransformation =
			lsConformalAffine(
				locationSet(
					pattern,
					Vector_Locator<dreal, 2>()
				),
				locationSet(
					transformedPattern,
					Vector_Locator<dreal, 2>()
				));

		dreal scalingDelta = absoluteError<dreal>(
			matchedTransformation.scaling(), 
			transformation.scaling());
		dreal angleDelta = absoluteError<dreal>(
			matchedTransformation.rotation(), 
			transformation.rotation());
		dreal tDelta = norm(
			matchedTransformation.translation() - 
			transformation.translation());

		REQUIRE(scalingDelta <= 0.001);
		REQUIRE(angleDelta <= 0.001);
		REQUIRE(tDelta <=  0.001);
	}
}
