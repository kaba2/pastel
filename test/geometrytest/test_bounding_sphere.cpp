// Description: Testing for bounding sphere
// Documentation: unit_testing.txt

#include "test_pastelgeometry.h"

#include <pastel/geometry/bounding/bounding_sphere_pointset.h>

#include <pastel/sys/locator.h>
#include <pastel/sys/input.h>

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

		void test()
		{
			{
				auto check = [&](
					std::initializer_list<Vector2> pointSet,
					real radius,
					Vector2 position)
				{
					Sphere2 bound = boundingSphere(
						rangeInput(range(pointSet.begin(), pointSet.end())));

					return abs(bound.radius() - radius) < 0.001 &&
						norm(bound.position() - position) < 0.001;
				};

				TEST_ENSURE(check({ }, 0, { 0, 0 }));
				TEST_ENSURE(check({ {-5, 3} }, 0, { -5, 3 }));
				TEST_ENSURE(check({ {-1, 0}, {1, 0} }, 1, { 0, 0 }));
				TEST_ENSURE(check({ {-1, 0}, {1, 0}, {0, 1}, {0, -1} }, 1, { 0, 0 }));
				TEST_ENSURE(check({ {-1 - 3, 0 + 5}, {1 - 3, 0 + 5}, {0 - 3, 1 + 5}, {0 - 3, -1 + 5} }, 1, { -3, 5 }));
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
		testRunner().add("boundingSphere", test);
	}

	CallFunction run(addTest);

}
