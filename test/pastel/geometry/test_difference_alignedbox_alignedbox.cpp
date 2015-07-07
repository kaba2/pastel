// Description: Testing for alignedbox-alignedbox difference
// DocumentationOf: difference_alignedbox_alignedbox.h

#include "test/test_init.h"

#include "pastel/geometry/difference/difference_alignedbox_alignedbox.h"

#include "pastel/gfx/image_gfxrenderer.h"
#include "pastel/gfx/gfxrenderer_tools.h"
#include "pastel/gfx/image_file/pcx/savepcx.h"

#include "pastel/sys/rational.h"
#include "pastel/sys/array.h"
#include "pastel/sys/output.h"

#include <boost/range/algorithm/equal.hpp>

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
			testSame();
			testHole();
			testHollow();
			testCorner();
			testEmptyA();
			testEmptyB();
		}

		bool testCase(
			const AlignedBox2& aBox,
			const AlignedBox2& bBox,
			const std::vector<AlignedBox2>& correctSet) const
		{
			std::vector<AlignedBox2> boxSet;
			difference(aBox, bBox, pushBackOutput(boxSet));

			bool result = boost::equal(
				range(boxSet.begin(), boxSet.end()),
				range(correctSet.begin(), correctSet.end()),
				equalBox);

			if (!result)
			{
				for (integer i = 0;i < boxSet.size();++i)
				{
					std::cout 
						<< boxSet[i].min() << " " << std::endl
						<< boxSet[i].minTopology() << " " << std::endl
						<< boxSet[i].max() << " " << std::endl
						<< boxSet[i].maxTopology() << " " << std::endl
						<< std::endl;
				}
			}

			return result;
		}

		void testSame()
		{
			AlignedBox2 aBox(
				0, 0, 6, 6);
			AlignedBox2 bBox(
				0, 0, 6, 6);

			std::vector<AlignedBox2> correctSet;
			REQUIRE(testCase(aBox, bBox, correctSet));

			aBox.maxTopology().set(Topology::Closed);
			bBox.maxTopology().set(Topology::Closed);

			REQUIRE(testCase(aBox, bBox, correctSet));
		}

		void testCorner()
		{
			AlignedBox2 aBox(
				0, 0, 6, 6);
			AlignedBox2 bBox(
				3, 3, 6, 6);

			std::vector<AlignedBox2> correctSet;
			correctSet.push_back(
				AlignedBox2(0, 0, 3, 6));
			correctSet.push_back(
				AlignedBox2(3, 0, 6, 3));

			REQUIRE(testCase(aBox, bBox, correctSet));
		}

		void testHollow()
		{
			AlignedBox2 aBox(
				0, 0, 6, 6);
			aBox.maxTopology().set(Topology::Closed);

			AlignedBox2 bBox(
				0, 0, 6, 6);
			bBox.minTopology().set(Topology::Open);

			std::vector<AlignedBox2> correctSet;
			correctSet.push_back(
				AlignedBox2(0, 0, 0, 6));
			correctSet.back().maxTopology().set(Topology::Closed);

			correctSet.push_back(
				AlignedBox2(6, 0, 6, 6));
			correctSet.back().maxTopology().set(Topology::Closed);

			correctSet.push_back(
				AlignedBox2(0, 0, 6, 0));
			correctSet.back().minTopology().set(Topology::Open, Topology::Closed);
			correctSet.back().maxTopology().set(Topology::Open, Topology::Closed);

			correctSet.push_back(
				AlignedBox2(0, 6, 6, 6));
			correctSet.back().minTopology().set(Topology::Open, Topology::Closed);
			correctSet.back().maxTopology().set(Topology::Open, Topology::Closed);

			REQUIRE(testCase(aBox, bBox, correctSet));
		}

		void testHole()
		{
			AlignedBox2 aBox(
				0, 0, 6, 6);
			AlignedBox2 bBox(
				2, 3, 4, 5);

			std::vector<AlignedBox2> correctSet;
			correctSet.push_back(
				AlignedBox2(0, 0, 2, 6));
			correctSet.push_back(
				AlignedBox2(4, 0, 6, 6));
			correctSet.push_back(
				AlignedBox2(2, 0, 4, 3));
			correctSet.push_back(
				AlignedBox2(2, 5, 4, 6));

			REQUIRE(testCase(aBox, bBox, correctSet));
		}

		void testEmptyB()
		{
			AlignedBox2 aBox(
				0, 0, 6, 6);
			AlignedBox2 bBox(
				3, 3, 2, 2);

			std::vector<AlignedBox2> correctSet;
			correctSet.push_back(aBox);
			REQUIRE(testCase(aBox, bBox, correctSet));
		}

		void testEmptyA()
		{
			AlignedBox2 aBox(
				3, 3, 2, 2);
			AlignedBox2 bBox(
				0, 0, 6, 6);

			std::vector<AlignedBox2> correctSet;
			REQUIRE(testCase(aBox, bBox, correctSet));
		}
	};

	TEST_CASE("DifferenceAlignedBox", "[DifferenceAlignedBox]")
	{
	}

}
