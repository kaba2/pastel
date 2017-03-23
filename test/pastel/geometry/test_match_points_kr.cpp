// Description: Testing for matchPointsKr
// DocumentationOf: match_points_kr.h

#include "test/test_init.h"

#include <pastel/geometry/pattern_matching/match_points_kr.h>
#include <pastel/geometry/distance/distance_point_point.h>
#include <pastel/geometry/nearestset/kdtree_nearestset.h>

#include <pastel/sys/random.h>
#include <pastel/sys/range.h>
#include <pastel/sys/rational.h>

#include <pastel/geometry/pointkdtree.h>

TEST_CASE("matchPointsKr (matchPointsKr)")
{
	//using Real = Rational<integer>;
	using Real = real;
	using Point = Vector<Real, 2>;
	using Tree_Settings = PointKdTree_Settings<Vector_Locator<Real, 2>>;
	using Tree = PointKdTree<Tree_Settings>;

	for (real sceneRatio : {1.0, 0.75, 0.65})
	{
		for (integer j = 0; j < 10; ++j)
		{
			integer kNearest = 1 + j;
			Real minMatchRatio = 0.7;
			Real maxBias = 0.1;
			Real matchingDistance2 = 0.01;

			Point translation(1 + j, 5 - 3 * j);

			// Generate a point-set.

			integer n = 10 + j * 10;
			std::vector<Point> modelSet;
			modelSet.reserve(n);

			for (integer i = 0; i < n; ++i)
			{
				Point p = 2 * randomVector<Real, 2>() - 1;
				modelSet.push_back(p);
			}

			Tree modelTree;
			modelTree.insertSet(modelSet);
			modelTree.refine();

			// Generate a translated point-set.

			integer m = n;
			std::vector<Point> sceneSet;
			sceneSet.reserve(m);

			for (integer i = 0; i < m; ++i)
			{
				sceneSet.push_back(modelSet[i] + translation);
			}

			// Randomize the order of the translated point-set.
			std::random_shuffle(sceneSet.begin(), sceneSet.end());

			// Delete points from the scene-set.
			sceneSet.resize(m * sceneRatio);
			//std::cout << sceneSet.size() << std::endl;

			Tree sceneTree;
			sceneTree.insertSet(sceneSet);
			sceneTree.refine();

			using Tree_ConstIterator = Tree::Point_ConstIterator;

			{
				std::vector<std::pair<Tree_ConstIterator, Tree_ConstIterator>> pairSet;

				// Match model-set to scene-set.
				auto result = matchPointsKr(
					kdTreeNearestSet(modelTree), 
					kdTreeNearestSet(sceneTree),
					PASTEL_TAG(matchingMode), MatchPointsKr_MatchingMode::Maximum,
					PASTEL_TAG(kNearest), kNearest,
					PASTEL_TAG(minMatchRatio), minMatchRatio,
					PASTEL_TAG(matchingDistance2), matchingDistance2,
					PASTEL_TAG(maxBias), maxBias,
					PASTEL_TAG(report), pushBackOutput(pairSet));

				if (sceneRatio > minMatchRatio)
				{
					REQUIRE(result.success);
					REQUIRE(pairSet.size() >= modelSet.size() * minMatchRatio);

					// Check that the point-pairs are within given
					// matching distance from each other.
					RANGES_FOR(auto&& pairing, pairSet)
					{
						Real actualDistance = distance2(pairing.first->point(), evaluate(pairing.second->point() + result.translation));
						REQUIRE(actualDistance <= matchingDistance2);
					}
				}
				else
				{
					// We removed so many points from scene-set,
					// that there cannot exist a match with the
					// given minimum matching ratio.
					REQUIRE(!result.success);
				}
			}

			{
				std::vector<std::pair<Tree_ConstIterator, Tree_ConstIterator>> pairSet;

				// Match scene-set to model-set.
				auto result = matchPointsKr(
					kdTreeNearestSet(sceneTree), kdTreeNearestSet(modelTree),
					PASTEL_TAG(matchingMode), MatchPointsKr_MatchingMode::Maximum,
					PASTEL_TAG(kNearest), kNearest,
					PASTEL_TAG(minMatchRatio), minMatchRatio,
					PASTEL_TAG(matchingDistance2), matchingDistance2,
					PASTEL_TAG(maxBias), maxBias,
					PASTEL_TAG(report), pushBackOutput(pairSet));

				REQUIRE(result.success);
				REQUIRE(pairSet.size() >= sceneSet.size() * minMatchRatio);

				// Check that the point-pairs are within given
				// matching distance from each other.
				RANGES_FOR(auto&& pairing, pairSet)
				{
					Real actualDistance = distance2(pairing.first->point(), evaluate(pairing.second->point() + result.translation));
					REQUIRE(actualDistance <= matchingDistance2);
				}
			}
		}
	}
}

