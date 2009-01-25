#include "pastelgeometrytest.h"

#include "pastel/math/affinetransformation_tools.h"
#include "pastel/sys/vector_tools.h"
#include "pastel/math/uniformsampling.h"

using namespace Pastel;

namespace
{

	void testSimilaritySimple()
	{
		log() << "- similarity" << logNewLine;

		for (integer i = 0;i < 10000;++i)
		{
			const Tuple<4, real> parameter(
				randomReal() * 2 + 1,
				randomReal() * 2 * constantPi<real>(),
				randomReal() * 2 - 1,
				randomReal() * 2 - 1);

			const AffineTransformation2 transformation = 
				similarityTransformation(parameter);

			const Point2 aFrom(randomReal(), randomReal());
			const Point2 bFrom(randomReal(), randomReal());

			const Point2 aTo(aFrom * transformation);
			const Point2 bTo(bFrom * transformation);

			const Tuple<4, real> matchedParameter =
				similarityTransformation(aFrom, bFrom, aTo, bTo);

			const real scalingDelta = std::abs(matchedParameter[0] - parameter[0]);
			const real angleDelta = std::abs(matchedParameter[1] - parameter[1]);
			const real xDelta = std::abs(matchedParameter[2] - parameter[2]);
			const real yDelta = std::abs(matchedParameter[3] - parameter[3]);

			REPORT1(scalingDelta > 0.001, scalingDelta);
			REPORT1(angleDelta > 0.001, angleDelta);
			REPORT1(xDelta > 0.001, xDelta);
			REPORT1(yDelta > 0.001, yDelta);
		}

	}

	void testSimilarityLs()
	{
		log() << "- least squares similarity" << logNewLine;

		for (integer i = 0;i < 10000;++i)
		{
			const Tuple<4, real> parameter(
				randomReal() * 2 + 1,
				randomReal() * 2 * constantPi<real>(),
				randomReal() * 2 - 1,
				randomReal() * 2 - 1);

			const AffineTransformation2 transformation = 
				similarityTransformation(parameter);

			std::vector<Point2> pattern;
			std::vector<Point2> transformedPattern;
			
			for (integer i = 0;i < 1000;++i)
			{
				pattern.push_back(asPoint(randomVector<2, real>()));
				transformedPattern.push_back(
					pattern.back() * transformation);
			}

			const Tuple<4, real> matchedParameter =
				similarityTransformation(
				pattern, transformedPattern);

			const real scalingDelta = std::abs(matchedParameter[0] - parameter[0]);
			const real angleDelta = std::abs(matchedParameter[1] - parameter[1]);
			const real xDelta = std::abs(matchedParameter[2] - parameter[2]);
			const real yDelta = std::abs(matchedParameter[3] - parameter[3]);

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
