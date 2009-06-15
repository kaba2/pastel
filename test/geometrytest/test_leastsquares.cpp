#include "pastelgeometrytest.h"

#include "pastel/math/affinetransformation_tools.h"
#include "pastel/sys/vector_tools.h"
#include "pastel/sys/random_vector.h"

using namespace Pastel;

namespace
{

	void testNoiselessSimilarity()
	{
		for (integer i = 0;i < 1000;++i)
		{
			const integer points = i + 2;

			std::vector<Point2> from;
			from.reserve(points);
			std::vector<Point2> to;
			to.reserve(points);

			const real scale = random<real>() * 0.9 + 0.1;
			const real angle = random<real>() * 2 * constantPi<real>();
			const Vector2 translation(random<real>() * 2 - 1, random<real>() * 2 - 1);

			const AffineTransformation2 transformation =
				similarityTransformation(scale, angle, translation);

			for (integer i = 0;i < points;++i)
			{
				const Point2 fromPoint(randomVectorSphere<2, real>());
				from.push_back(fromPoint);

				const Point2 toPoint(fromPoint * transformation);
				to.push_back(toPoint);
			}

			const Tuple<4, real> parameters =
				similarityTransformation(from, to);

			REPORT(mabs(parameters[0] - scale) > 0.001);
			REPORT(mabs(parameters[1] - angle) > 0.001);
			REPORT(mabs(parameters[2] - translation[0]) > 0.001);
			REPORT(mabs(parameters[3] - translation[1]) > 0.001);
		}
	}

	void testBegin()
	{
		testNoiselessSimilarity();
	}

	void testAdd()
	{
		geometryTestList().add("LeastSquares", testBegin);
	}

	CallFunction run(testAdd);

}
