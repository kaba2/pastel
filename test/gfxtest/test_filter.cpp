#include "pastelgfxtest.h"

#include <pastel/gfx/pcx.h>
#include <pastel/gfx/color_tools.h>
#include <pastel/gfx/drawing.h>
#include <pastel/gfx/filter_all.h>

#include <pastel/sys/view_all.h>
#include <pastel/sys/lineararray.h>

using namespace Pastel;

namespace
{

	void drawFilter(
		const Filter& filter,
		const Color& color,
		LinearArray<2, Color>& image)
	{
		const integer Samples = 100;

		const integer width = image.width();
		const integer height = image.height();
		const real maxFilterRadius = 2;

		Point2 previous;
		for (integer i = 0;i < width;++i)
		{
			const real t = (((real)i / (width - 1)) - 0.5) * 2 * maxFilterRadius;
			const Point2 current(i, ((filter(t) / filter(0) + 1) / 2) * (height * 0.75));

			if (i != 0)
			{
				drawSegment(
					Segment2(previous, current), color, arrayView(image));
			}

			previous = current;
		}
	}

	void testFilters()
	{
		const integer Width = 500;
		const integer Height = 500;

		LinearArray<2, Color> image(Width, Height);

		/*
		drawFilter(GaussianFilter(1), randomRgbColor(), image);
		drawFilter(GaussianFilter(2), randomRgbColor(), image);
		drawFilter(TriangleFilter(), randomRgbColor(), image);
		drawFilter(BoxFilter(), randomRgbColor(), image);
		drawFilter(MitchellFilter(), randomRgbColor(), image);
		drawFilter(CubicFilter(), randomRgbColor(), image);
		drawFilter(LanczosFilter(2), randomRgbColor(), image);
		*/

		const real b = 0.65;
		const real c = (1 - b) / 2;
		drawFilter(MitchellFilter(b, c), randomRgbColor(), image);
		drawFilter(GaussianFilter(2), Color(1), image);

		savePcx(image, "test_filter.pcx");
	}

	void testCubicFilter()
	{
		const integer Width = 500;
		const integer Height = 500;

		LinearArray<2, Color> image(Width, Height);

		const real Range = 3;
		const integer Steps = 20;
		for (integer i = 0;i < Steps;++i)
		{
			drawFilter(CubicFilter(
				linear((real)-Range, (real)(1 + Range), (real)i / (Steps - 1))), randomRgbColor(), image);
		}

		savePcx(image, "test_filter_cubic.pcx");
	}

	void testMitchellFilter()
	{
		const integer Width = 500;
		const integer Height = 500;

		LinearArray<2, Color> image(Width, Height);

		const real bMin = 0;
		const real bMax = 0.65;

		const integer Steps = 5;
		for (integer i = 0;i < Steps;++i)
		{
			const real b = linear(bMin, bMax, (real)i / (Steps - 1));
			const real c = (1 - b) / 2;

			drawFilter(MitchellFilter(b, c), randomRgbColor(), image);
		}

		savePcx(image, "test_filter_mitchell.pcx");
	}

	void testBegin()
	{
		testFilters();
		testCubicFilter();
		testMitchellFilter();
	}

	void testAdd()
	{
		gfxTestList().add("Filter", testBegin);
	}

	CallFunction run(testAdd);

}
