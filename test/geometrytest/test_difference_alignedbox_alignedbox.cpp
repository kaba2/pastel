#include "pastelgeometrytest.h"

#include "pastel/geometry/difference_alignedbox_alignedbox.h"

#include "pastel/gfx/image_gfxrenderer.h"
#include "pastel/gfx/gfxrenderer_tools.h"
#include "pastel/gfx/savepcx.h"

#include "pastel/sys/rational.h"
#include "pastel/sys/array.h"

using namespace Pastel;

typedef Rational<integer> Real;

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
			testSymmetricDifference();
		}

		void testSymmetricDifference()
		{
			Array<Color> image(512, 512, Color(1));
			Image_GfxRenderer<Color> renderer(
				&image);

			renderer.setViewWindow(
				AlignedBox2(0, 0, 6, 6));
			
			std::vector<AlignedBox2> boxSet;
			symmetricDifference(
				AlignedBox2(0, 0, 4, 4),
				AlignedBox2(2, 2, 6, 6),
				std::back_inserter(boxSet));

			for (integer i = 0;i < boxSet.size();++i)
			{
				renderer.setColor(
					randomVector<real32, 3>());
				drawBox(renderer, boxSet[i]);
			}

			savePcx(image, "difference_alignedbox_alignedbox.pcx");
		}
	};

	void testBegin()
	{
		Test test;
		test.run();
	}

	void testAdd()
	{
		geometryTestList().add("DifferenceAlignedBox", testBegin);
	}

	CallFunction run(testAdd);

}
