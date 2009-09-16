#include "pastelgeometrytest.h"

#include "pastel/math/affinetransformation_tools.h"
#include "pastel/sys/vector_tools.h"
#include "pastel/sys/random_vector.h"

using namespace Pastel;

namespace
{

	void testSimilaritySimple()
	{
		for (integer i = 0;i < 10000;++i)
		{
			const Tuple<real, 4> parameter(
				random<real>() * 2 + 1,
				random<real>() * 2 * constantPi<real>(),
				random<real>() * 2 - 1,
				random<real>() * 2 - 1);

			const AffineTransformation2 transformation =
				similarityTransformation(parameter);

			const Vector2 aFrom(random<real>(), random<real>());
			const Vector2 bFrom(random<real>(), random<real>());

			const Vector2 aTo(transformPoint(aFrom, transformation));
			const Vector2 bTo(transformPoint(bFrom, transformation));

			const Tuple<real, 4> matchedParameter =
				similarityTransformation(aFrom, bFrom, aTo, bTo);

			const real scalingDelta = mabs(matchedParameter[0] - parameter[0]);
			const real angleDelta = mabs(matchedParameter[1] - parameter[1]);
			const real xDelta = mabs(matchedParameter[2] - parameter[2]);
			const real yDelta = mabs(matchedParameter[3] - parameter[3]);

			REPORT1(scalingDelta > 0.001, scalingDelta);
			REPORT1(angleDelta > 0.001, angleDelta);
			REPORT1(xDelta > 0.001, xDelta);
			REPORT1(yDelta > 0.001, yDelta);
		}

	}

	void testSimilarityLs()
	{
		for (integer i = 0;i < 10000;++i)
		{
			const Tuple<real, 4> parameter(
				random<real>() * 2 + 1,
				random<real>() * 2 * constantPi<real>(),
				random<real>() * 2 - 1,
				random<real>() * 2 - 1);

			const AffineTransformation2 transformation =
				similarityTransformation(parameter);

			std::vector<Vector2> pattern;
			std::vector<Vector2> transformedPattern;

			for (integer i = 0;i < 1000;++i)
			{
				pattern.push_back(randomVector<2, real>());
				transformedPattern.push_back(
					transformPoint(pattern.back(), transformation));
			}

			const Tuple<real, 4> matchedParameter =
				similarityTransformation(
				pattern, transformedPattern);

			const real scalingDelta = mabs(matchedParameter[0] - parameter[0]);
			const real angleDelta = mabs(matchedParameter[1] - parameter[1]);
			const real xDelta = mabs(matchedParameter[2] - parameter[2]);
			const real yDelta = mabs(matchedParameter[3] - parameter[3]);

			REPORT1(scalingDelta > 0.001, scalingDelta);
			REPORT1(angleDelta > 0.001, angleDelta);
			REPORT1(xDelta > 0.001, xDelta);
			REPORT1(yDelta > 0.001, yDelta);
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
