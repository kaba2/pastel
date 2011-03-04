#include "pastelgeometrytest.h"

#include "pastel/math/affinetransformation_tools.h"
#include "pastel/math/uniform_sampling.h"
#include "pastel/math/conformalaffine2d_tools.h"

#include "pastel/sys/vector_pointpolicy.h"

#include "pastel/sys/vector_tools.h"

using namespace Pastel;

namespace
{

	class Test
		: public TestSuite
	{
	public:
		Test()
			: TestSuite(&geometryTestReport())
		{
		}

		virtual void run()
		{
			testNoiselessSimilarity();
		}

		void testNoiselessSimilarity()
		{
			for (integer i = 0;i < 1000;++i)
			{
				const integer points = i + 2;

				std::vector<Vector2> from;
				from.reserve(points);
				std::vector<Vector2> to;
				to.reserve(points);

				const real scale = random<real>() * 0.9 + 0.1;
				const real angle = random<real>() * 2 * constantPi<real>();
				const Vector2 translation(random<real>() * 2 - 1, random<real>() * 2 - 1);

				const ConformalAffine2 transformation(scale, angle, translation);

				for (integer i = 0;i < points;++i)
				{
					const Vector2 fromPoint(randomVectorSphere<real, 2>());
					from.push_back(fromPoint);

					const Vector2 toPoint(transformPoint(fromPoint, transformation));
					to.push_back(toPoint);
				}

				const ConformalAffine2 similarity =
					lsConformalAffine(
					range(from.begin(), from.end()), 
					range(to.begin(), to.end()),
					Vector_PointPolicy2(),
					Vector_PointPolicy2());

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
		geometryTestList().add("LeastSquares", test);
	}

	CallFunction run(addTest);

}
