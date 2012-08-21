// Description: Testing for range counting in PointKdTree
// DocumentationOf: count_range_pointkdtree.h

#include "pastelgeometrytest.h"

#include "pastel/geometry/count_range_pointkdtree.h"
#include "pastel/geometry/splitrule_pointkdtree.h"

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
			PointKdTree<real, 2> tree;

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
				Vector2(-1, -1),
				Vector2(-1, -2),
				Vector2(-3, -1),
				Vector2(-3, -3),
				Vector2(-5, -2),
				Vector2(-5, 1),
				Vector2(-4, 3),
				Vector2(-2, 4),
				Vector2(-2, 1),
				Vector2(-1, 1),
				Vector2(-1, 2),
				Vector2(1, 1),
				Vector2(2, 3),
				Vector2(3, 1),
				Vector2(2, -1),
				Vector2(1, -3)
			};

			tree.insertRange(range(pointSet));

			tree.refine(SlidingMidpoint_SplitRule_PointKdTree(), 1);

			// Increasing radius from the origin.
			TEST_ENSURE_OP(countRange(tree, AlignedBox2(-1, -1, 1, 1), 1), ==, 0);
			TEST_ENSURE_OP(countRange(tree, AlignedBox2(-2, -2, 2, 2), 1), ==, 3);
			TEST_ENSURE_OP(countRange(tree, AlignedBox2(-3, -3, 3, 3), 1), ==, 7);
			TEST_ENSURE_OP(countRange(tree, AlignedBox2(-4, -4, 4, 4), 1), ==, 12);
			TEST_ENSURE_OP(countRange(tree, AlignedBox2(-5, -5, 5, 5), 1), ==, 14);
			TEST_ENSURE_OP(countRange(tree, AlignedBox2(-6, -6, 6, 6), 1), ==, 16);

			// Empty box
			TEST_ENSURE_OP(countRange(tree, AlignedBox2(6, 6, -6, -6), 1), ==, 0);

			// The point with index 7.
			TEST_ENSURE_OP(countRange(tree, AlignedBox2(-4, 3, -4, 3), 1), ==, 0);
			TEST_ENSURE_OP(countRange(tree, AlignedBox2(-5, 2, -3, 4), 1), ==, 1);

			// Quarants
			TEST_ENSURE_OP(countRange(tree, AlignedBox2(-6, -6, 0, 0), 1), ==, 5);
			TEST_ENSURE_OP(countRange(tree, AlignedBox2(0, 0, 6, 6), 1), ==, 3);
			TEST_ENSURE_OP(countRange(tree, AlignedBox2(0, -6, 6, 0), 1), ==, 2);
			TEST_ENSURE_OP(countRange(tree, AlignedBox2(-6, 0, 0, 6), 1), ==, 6);
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("count_range_pointkdtree", test);
	}

	CallFunction run(addTest);

}
