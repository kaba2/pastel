// Description: Testing for brute-force nearest neighbors
// DocumentationOf: search_nearest_bruteforce.h

#include "test_pastelgeometry.h"

#include <pastel/geometry/search_nearest_bruteforce.h>

#include <pastel/math/normbijection/normbijections.h>

#include <pastel/sys/vector.h>
#include <pastel/sys/input.h>
#include <pastel/sys/output.h>
#include <pastel/sys/indicator.h>
#include <pastel/sys/locator.h>
#include <pastel/sys/iterator/counting_iterator.h>

#include <vector>
#include <list>

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
			Euclidean_NormBijection<real> normBijection;

			/*
			 0   |
			     |2  3
			  1  | 4
				 |5
			--6--+--7---
			     |  8
			9ABC D
			     |    E
				 |
			*/

			using PointSet = std::list<Vector2>;
			using Point_Iterator = PointSet::iterator;
			using Locator = Vector_Locator<real, 2>;
			
			PointSet pointSet =
			{
				// 0
				{-4, 4},
				// 1
				{ -3, 2 },
				// 2
				{ 1, 3 },
				// 3
				{ 4, 3 },
				// 4
				{ 2, 2 },
				// 5
				{ 1, 1 },
				// 6
				{ -3, 0 },
				// 7
				{ 3, 0 },
				// 8
				{ 3, -1 },
				// 9
				{ -5, -2 },
				// A
				{ -4, -2 },
				// B
				{ -3, -2 },
				// C
				{ -2, -2 },
				// D
				{ 0, -2 },
				// E
				{ 5, -3 }
			};

			/*
			Nearest neighbors:
			0 -> 1 (1^2 + 2^2 = 5)
			1 -> 6 (0^2 + 2^2 = 4)
			2 -> 4 (1^2 + 1^2 = 2)
			3 -> 4 (2^2 + 1^2 = 5)
			4 -> 2 or 5 (1^2 + 1^2 = 2)
			5 -> 4 (1^2 + 1^2 = 2)
			6 -> 1 or B (0^2 + 2^2 = 4)
			7 -> 8 (0^2 + 1^2 = 1)
			8 -> 7 (0^2 + 1^2 = 1)
			9 -> A (1^2 + 0^2 = 1)
			A -> 9 or B (1^2 + 0^2 = 1)
			B -> A or C (1^2 + 0^2 = 1)
			C -> B (1^2 + 0^2 = 1)
			D -> C (2^2 + 0^2 = 4)
			E -> 8 (2^2 + 2^2 = 8)
			*/

			std::vector<real> distanceSet =
			{
				5, 4, 2, 5, 2, 2, 4, 1, 1, 1, 1, 1, 1, 4, 8
			};

			auto aNearestSet = bruteForceNearestSet(
				rangeInput(pointSet));

			PASTEL_CONCEPT_CHECK(decltype(aNearestSet), NearestSet_Concept);

			auto bNearestSet = bruteForceNearestSet(
					locationSet(rangeInput(countingRange(pointSet)), indirectLocator<Point_Iterator>(Locator()))
			);

			PASTEL_CONCEPT_CHECK(decltype(bNearestSet), NearestSet_Concept);
			
			integer j = 0;
			for (auto i = pointSet.begin(); i != pointSet.end(); ++i)
			{
				{
					std::pair<real, Vector2> result =
						searchNearest(addConst(aNearestSet), *i);

					real distance2 = result.first;

					TEST_ENSURE(distance2 == 0);
				}

				{

					std::pair<real, Point_Iterator> result =
						searchNearest(
							addConst(bNearestSet),
							*i,
							Null_Output(),
							predicateIndicator(i, NotEqualTo()),
							normBijection
						);

					real distance2 = result.first;

					TEST_ENSURE(distance2 == distanceSet[j]);
				}
				++j;
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
		testRunner().add("search_nearest_bruteforce", test);
	}

	CallFunction run(addTest);

}
