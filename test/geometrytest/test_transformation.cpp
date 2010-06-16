#include "pastelgeometrytest.h"

#include "pastel/math/conformalaffine2d_tools.h"
#include "pastel/math/uniform_sampling.h"

#include "pastel/sys/vector_tools.h"

using namespace Pastel;

namespace
{

	void testSimilaritySimple()
	{
		for (integer i = 0;i < 10000;++i)
		{
			const ConformalAffine2 transformation(
				random<real>() * 2 + 1,
				random<real>() * 2 * constantPi<real>(),
				Vector2(random<real>() * 2 - 1, random<real>() * 2 - 1));

			const Vector2 aFrom(random<real>(), random<real>());
			const Vector2 bFrom(random<real>(), random<real>());

			const Vector2 aTo(transformPoint(aFrom, transformation));
			const Vector2 bTo(transformPoint(bFrom, transformation));

			const ConformalAffine2 matchedTransformation =
				conformalAffine(aFrom, bFrom, aTo, bTo);

			const real scalingDelta = absoluteError<real>(
				matchedTransformation.scaling(), 
				transformation.scaling());
			const real angleDelta = absoluteError<real>(
				matchedTransformation.rotation(), 
				transformation.rotation());
			const real tDelta = norm(
				matchedTransformation.translation() - 
				transformation.translation());

			REPORT_OP(scalingDelta, >, 0.001);
			REPORT_OP(angleDelta, >, 0.001);
			REPORT_OP(tDelta, >,  0.001);
		}

	}

	void testSimilarityLs()
	{
		for (integer i = 0;i < 10000;++i)
		{
			const ConformalAffine2 transformation(
				random<real>() * 2 + 1,
				random<real>() * 2 * constantPi<real>(),
				Vector2(random<real>() * 2 - 1, random<real>() * 2 - 1));

			std::vector<Vector2> pattern;
			std::vector<Vector2> transformedPattern;

			for (integer i = 0;i < 1000;++i)
			{
				pattern.push_back(randomVector<real, 2>());
				transformedPattern.push_back(
					transformPoint(pattern.back(), transformation));
			}

			const ConformalAffine2 matchedTransformation =
				lsConformalAffine(pattern, transformedPattern);

			const real scalingDelta = absoluteError<real>(
				matchedTransformation.scaling(), 
				transformation.scaling());
			const real angleDelta = absoluteError<real>(
				matchedTransformation.rotation(), 
				transformation.rotation());
			const real tDelta = norm(
				matchedTransformation.translation() - 
				transformation.translation());

			REPORT_OP(scalingDelta, >, 0.001);
			REPORT_OP(angleDelta, >, 0.001);
			REPORT_OP(tDelta, >,  0.001);
		}

	}

	void testBegin()
	{
		testSimilaritySimple();
		testSimilarityLs();
	}

	void testAdd()
	{
		geometryTestList().add("Transformation", testBegin);
	}

	CallFunction run(testAdd);

}
