// Description: Smooth-step function example
// DocumentationOf: smoothstep.h

#include "pastel_example.h"

#include "pastel/math/interpolation/smoothstep.h"

#include "pastel/gfx/image_file/pcx/savepcx.h"
#include "pastel/gfx/drawing.h"
#include "pastel/gfx/color/color_tools.h"

#include "pastel/sys/random.h"
#include "pastel/sys/view/concrete_views.h"

using namespace Pastel;

namespace
{

	void testSmoothStep()
	{
		const integer Width = 500;
		const integer Height = 500;

		Array<Color, 2> image(Vector2i(Width, Height));

		const integer Points = 100;

		drawSegment(Segment2(Vector2(0, 0), Vector2(Width, Height)), Color(1, 1, 1), arrayView(image));

		for (integer i = 0;i < Points;++i)
		{
			const dreal tFrom = (dreal)i / Points;
			const dreal tTo = (dreal)(i + 1) / Points;

			const Vector2 from(Width * tFrom, Height * cubicSmoothStep(tFrom));
			const Vector2 to(Width * tTo, Height * cubicSmoothStep(tTo));

			drawSegment(
				Segment2(from, to), Color(0, 1, 0), arrayView(image));
		}

		for (integer i = 0;i < Points;++i)
		{
			const dreal tFrom = (dreal)i / Points;
			const dreal tTo = (dreal)(i + 1) / Points;

			const Vector2 from(Width * tFrom, Height * quarticSmoothStep(tFrom * tFrom));
			const Vector2 to(Width * tTo, Height * quarticSmoothStep(tTo * tTo));

			drawSegment(
				Segment2(from, to), Color(1, 0, 0), arrayView(image));
		}

		for (integer i = 0;i < Points;++i)
		{
			const dreal tFrom = (dreal)i / Points;
			const dreal tTo = (dreal)(i + 1) / Points;

			const Vector2 from(Width * tFrom, Height * quinticSmoothStep(tFrom));
			const Vector2 to(Width * tTo, Height * quinticSmoothStep(tTo));

			drawSegment(
				Segment2(from, to), Color(0, 0, 1), arrayView(image));
		}

		savePcx(image, "smoothstep.pcx");
	}

	void test()
	{
		testSmoothStep();
	}

	void addTest()
	{
		testRunner().add("SmoothStep", test);
	}

	CallFunction run(addTest);

}
