// Description: Testing for alignedbox-alignedbox intersection
// DocumentationOf: intersect_alignedbox_alignedbox.h

#include "test/test_init.h"

#include "pastel/geometry/intersect/intersect_alignedbox_alignedbox.h"

#include "pastel/gfx/image_gfxrenderer.h"
#include "pastel/gfx/gfxrenderer_tools.h"
#include "pastel/gfx/image_file/pcx/savepcx.h"

#include "pastel/sys/rational.h"
#include "pastel/sys/array.h"

using Real = Rational<integer>;

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
	{
	public:
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

			REQUIRE(testPositiveCase(aBox, bBox, correctBox));
		}

		void testNormal()
		{
			AlignedBox2 aBox(0, 0, 6, 6);
			AlignedBox2 bBox(3, 2, 9, 4);
			AlignedBox2 correctBox(3, 2, 6, 4);

			REQUIRE(testPositiveCase(aBox, bBox, correctBox));
		}

		void testEmptyB()
		{
			AlignedBox2 aBox(0, 0, 6, 6);
			AlignedBox2 bBox(3, 3, 3, 3);
			AlignedBox2 correctBox = bBox;

			REQUIRE(testNegativeCase(aBox, bBox, correctBox));
		}

		void testEmptyB2()
		{
			AlignedBox2 aBox(0, 0, 6, 6);
			AlignedBox2 bBox(4, 3, 2, 1);
			AlignedBox2 correctBox = bBox;

			REQUIRE(testNegativeCase(aBox, bBox, correctBox));
		}

		void testEmptyAB()
		{
			AlignedBox2 aBox(0, 0, 0, 0);
			AlignedBox2 bBox(5, 4, 3, 2);
			AlignedBox2 correctBox(5, 4, 0, 0);

			REQUIRE(testNegativeCase(aBox, bBox, correctBox));
		}
	};

	TEST_CASE("intersect_alignedbox_alignedbox", "[intersect_alignedbox_alignedbox]")
	{
	}

}
