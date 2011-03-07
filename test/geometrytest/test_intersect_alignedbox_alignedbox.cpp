// Description: Testing for alignedbox-alignedbox intersection
// DocumentationOf: intersect_alignedbox_alignedbox.h

#include "pastelgeometrytest.h"

#include "pastel/geometry/intersect_alignedbox_alignedbox.h"

#include "pastel/gfx/image_gfxrenderer.h"
#include "pastel/gfx/gfxrenderer_tools.h"
#include "pastel/gfx/savepcx.h"

#include "pastel/sys/rational.h"
#include "pastel/sys/array.h"
#include "pastel/sys/ranges.h"

using namespace Pastel;

typedef Rational<integer> Real;

namespace
{

	bool equalBox(
		const AlignedBox2& aBox, const AlignedBox2& bBox)
	{
		return 
			aBox.min() == bBox.min() &&
			aBox.minTopology() == bBox.minTopology() &&
			aBox.max() == bBox.max() &&
			aBox.maxTopology() == bBox.maxTopology();
	}

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
			testHollow();
			testNormal();
			testEmptyB();
			testEmptyB2();
			testEmptyAB();
		}

		bool testPositiveCase(
			const AlignedBox2& aBox,
			const AlignedBox2& bBox,
			const AlignedBox2& correctBox)
		{
			AlignedBox2 cBox;
			return intersect(aBox, bBox, cBox) &&
				equalBox(cBox, correctBox);
		}

		bool testNegativeCase(
			const AlignedBox2& aBox,
			const AlignedBox2& bBox,
			const AlignedBox2& correctBox)
		{
			AlignedBox2 cBox;
			return !intersect(aBox, bBox, cBox) &&
				equalBox(cBox, correctBox);
		}

		void testHollow()
		{
			AlignedBox2 aBox(0, 0, 6, 6);
			aBox.maxTopology().set(Topology::Closed);
			AlignedBox2 bBox(0, 0, 6, 6);
			bBox.minTopology().set(Topology::Open);
			AlignedBox2 correctBox = bBox;

			TEST_ENSURE(testPositiveCase(aBox, bBox, correctBox));
		}

		void testNormal()
		{
			AlignedBox2 aBox(0, 0, 6, 6);
			AlignedBox2 bBox(3, 2, 9, 4);
			AlignedBox2 correctBox(3, 2, 6, 4);
			
			TEST_ENSURE(testPositiveCase(aBox, bBox, correctBox));
		}

		void testEmptyB()
		{
			AlignedBox2 aBox(0, 0, 6, 6);
			AlignedBox2 bBox(3, 3, 3, 3);
			AlignedBox2 correctBox = bBox;
			
			TEST_ENSURE(testNegativeCase(aBox, bBox, correctBox));
		}

		void testEmptyB2()
		{
			AlignedBox2 aBox(0, 0, 6, 6);
			AlignedBox2 bBox(4, 3, 2, 1);
			AlignedBox2 correctBox = bBox;
			
			TEST_ENSURE(testNegativeCase(aBox, bBox, correctBox));
		}

		void testEmptyAB()
		{
			AlignedBox2 aBox(0, 0, 0, 0);
			AlignedBox2 bBox(5, 4, 3, 2);
			AlignedBox2 correctBox(5, 4, 0, 0);
			
			TEST_ENSURE(testNegativeCase(aBox, bBox, correctBox));
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		geometryTestList().add("intersect_alignedbox_alignedbox", test);
	}

	CallFunction run(addTest);

}
