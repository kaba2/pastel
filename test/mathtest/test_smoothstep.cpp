#include "pastelmathtest.h"

#include "pastel/math/smoothstep.h"

#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/drawing.h"
#include "pastel/gfx/color_tools.h"

#include "pastel/sys/random.h"
#include "pastel/sys/view_all.h"

using namespace Pastel;

namespace
{

	void testSmoothStep()
	{
		const integer Width = 500;
		const integer Height = 500;

		Array<2, Color> image(Width, Height);

		const integer Points = 100;

		drawSegment(Segment2(Point2(0, 0), Point2(Width, Height)), Color(1), arrayView(image));

		for (integer i = 0;i < Points;++i)
		{
			const real tFrom = (real)i / Points;
			const real tTo = (real)(i + 1) / Points;

			const Point2 from(Width * tFrom, Height * cubicSmoothStep(tFrom));
			const Point2 to(Width * tTo, Height * cubicSmoothStep(tTo));

			drawSegment(
				Segment2(from, to), Color(0, 1, 0), arrayView(image));
		}

		for (integer i = 0;i < Points;++i)
		{
			const real tFrom = (real)i / Points;
			const real tTo = (real)(i + 1) / Points;

			const Point2 from(Width * tFrom, Height * quarticSmoothStep(tFrom * tFrom));
			const Point2 to(Width * tTo, Height * quarticSmoothStep(tTo * tTo));

			drawSegment(
				Segment2(from, to), Color(1, 0, 0), arrayView(image));
		}

		for (integer i = 0;i < Points;++i)
		{
			const real tFrom = (real)i / Points;
			const real tTo = (real)(i + 1) / Points;

			const Point2 from(Width * tFrom, Height * quinticSmoothStep(tFrom));
			const Point2 to(Width * tTo, Height * quinticSmoothStep(tTo));

			drawSegment(
				Segment2(from, to), Color(0, 0, 1), arrayView(image));
		}

		savePcx(image, "test_smoothstep.pcx");
	}

	void testBegin()
	{
		testSmoothStep();
	}

	void testAdd()
	{
		mathTestList().add("SmoothStep", testBegin);
	}

	CallFunction run(testAdd);

}
