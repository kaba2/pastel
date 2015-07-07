// Description: Testing for range counting in PointKdTree
// DocumentationOf: pointkdtree_count_range.h

#include "test/test_init.h"

#include <pastel/geometry/pointkdtree/pointkdtree_count_range.h>
#include <pastel/geometry/splitrules.h>
#include <pastel/sys/locator.h>

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
			using Settings = PointKdTree_Settings<Vector_Locator<real, 2>>;

			PointKdTree<Settings> tree;

			/*
				 ^
			     |      
			   7 |     
			 6   | C   
			     |     
			5  89|B D  
			-----x------>
			  2 0| E   
			4   1|     
			  3  |F    
			*/

			Vector2 pointSet[] = 
			{
				{ -1, -1 },
				{ -1, -2 },
				{ -3, -1 },
				{ -3, -3 },
				{ -5, -2 },
				{ -5, 1 },
				{ -4, 3 },
				{ -2, 4 },
				{ -2, 1 },
				{ -1, 1 },
				{ -1, 2 },
				{ 1, 1 },
				{ 2, 3 },
				{ 3, 1 },
				{ 2, -1 },
				{ 1, -3 }
			};

			tree.insertSet(rangeSet(pointSet));

			tree.refine(SlidingMidpoint_SplitRule(), 1);

			// Increasing radius from the origin.
			REQUIRE(countRange(tree, AlignedBox2(-1, -1, 1, 1), 1) == 0);
			REQUIRE(countRange(tree, AlignedBox2(-2, -2, 2, 2), 1) == 3);
			REQUIRE(countRange(tree, AlignedBox2(-3, -3, 3, 3), 1) == 7);
			REQUIRE(countRange(tree, AlignedBox2(-4, -4, 4, 4), 1) == 12);
			REQUIRE(countRange(tree, AlignedBox2(-5, -5, 5, 5), 1) == 14);
			REQUIRE(countRange(tree, AlignedBox2(-6, -6, 6, 6), 1) == 16);

			// Empty box
			REQUIRE(countRange(tree, AlignedBox2(6, 6, -6, -6), 1) == 0);

			// The point with index 7.
			REQUIRE(countRange(tree, AlignedBox2(-4, 3, -4, 3), 1) == 0);
			REQUIRE(countRange(tree, AlignedBox2(-5, 2, -3, 4), 1) == 1);

			// Quarants
			REQUIRE(countRange(tree, AlignedBox2(-6, -6, 0, 0), 1) == 5);
			REQUIRE(countRange(tree, AlignedBox2(0, 0, 6, 6), 1) == 3);
			REQUIRE(countRange(tree, AlignedBox2(0, -6, 6, 0), 1) == 2);
			REQUIRE(countRange(tree, AlignedBox2(-6, 0, 0, 6), 1) == 6);
		}
	};

	TEST_CASE("pointkdtree_count_range", "[pointkdtree_count_range]")
	{
	}

}
