// Description: Testing for bounding sphere
// Documentation: unit_testing.txt

#include "test/test_init.h"

#include <pastel/geometry/bounding/bounding_sphere_pointset.h>

#include <pastel/sys/locator.h>
#include <pastel/sys/set.h>

namespace
{

	class Test
	{
	public:
		virtual void run()
		{
			test();
		}

		void test()
		{
			{
				using Point = std::array<real, 2>;
				Point a = { {-1, 0} };
				PASTEL_CONCEPT_CHECK(Point, Point_Concept);
				PASTEL_CONCEPT_REJECT(Point, PointSet_Concept);
				Sphere<real, 2> bound = boundingSphere(a);

				REQUIRE(abs(bound.radius()) < 0.001);
				REQUIRE(norm(bound.position() - Vector2(-1, 0)) < 0.001);
			}
			{
				std::array<real, 2> a = { {-1, 0} };
				std::array<real, 2> b = { {1, 0} };
				Sphere<real, 2> bound = boundingSphere(a, b);

				REQUIRE(abs(bound.radius() - 1) < 0.001);
				REQUIRE(norm(bound.position()) < 0.001);
			}
			{
				auto check = [&](
					std::initializer_list<Vector2> pointSet,
					real radius,
					Vector2 position)
				{
					auto pointSet_ = rangeSet(pointSet);
					using PointSet = decltype(pointSet_);
					PASTEL_CONCEPT_CHECK(PointSet, PointSet_Concept);
					PASTEL_CONCEPT_REJECT(PointSet, Point_Concept);

					Sphere2 bound = boundingSphere(pointSet_);

					return abs(bound.radius() - radius) < 0.001 &&
						norm(bound.position() - position) < 0.001;
				};

				REQUIRE(check({ }, 0, { 0, 0 }));
				REQUIRE(check({ {-5, 3} }, 0, { -5, 3 }));
				REQUIRE(check({ {-1, 0}, {1, 0} }, 1, { 0, 0 }));
				REQUIRE(check({ {-1, 0}, {1, 0}, {0, 1}, {0, -1} }, 1, { 0, 0 }));
				REQUIRE(check({ {-1 - 3, 0 + 5}, {1 - 3, 0 + 5}, {0 - 3, 1 + 5}, {0 - 3, -1 + 5} }, 1, { -3, 5 }));
			}
		}
	};

	TEST_CASE("boundingSphere", "[boundingSphere]")
	{
	}

}