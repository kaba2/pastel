#include "pastelgfxtest.h"

#include <pastel/gfx/pcx.h>
#include <pastel/gfx/color_tools.h>
#include <pastel/gfx/drawing.h>
#include <pastel/dsp/filter_all.h>
#include <pastel/gfx/noise.h>
#include <pastel/dsp/resample.h>
#include <pastel/gfx/colormixers.h>

#include <pastel/sys/view_all.h>
#include <pastel/sys/array.h>
#include <pastel/sys/string_tools.h>

using namespace Pastel;

namespace
{

	void drawFilter(
		const Filter& filter,
		const Color& color,
		Array<2, Color>& image)
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

	template <typename Image_View>
	void drawSignal(
		const Array<1, real>& signal,
		const ConstFilterRef& filter,
		const View<2, Color, Image_View>& image,
		const Color& color)
	{
		Array<1, real> resampledSignal(image.width());
		resample(constArrayView(signal), 
			ArrayExtender<1, real>(0),
			filter, arrayView(resampledSignal));

		const integer width = image.width();
		const integer height = image.height();

		Point2 previous;
		for (integer i = 0;i < width;++i)
		{
			const Point2 current(i, height * (0.5 * resampledSignal(i) + 0.5));
			if (i > 0)
			{
				drawSegment(Segment2(previous, current), color, image);
			}
			previous = current;
		}
	}

	void testSignal(const ConstFilterRef& filter)
	{
		const integer Width = 512;
		const integer Height = 256;

		Array<2, Color> image(Width, Height);

		Array<1, real> signal(Width);

		for (integer i = 0;i < Width;++i)
		{
			signal(i) = (noise<real>(i * 0.11) - 0.5) * 2;
		}

		drawSignal(signal, filter, arrayView(image), Color(1));

		for (integer i = Width;i >= 1;i /= 2)
		{
			clear(Color(0), arrayView(image));

			Array<1, real> resampledSignal(i);
			resample(constArrayView(signal), 
				ArrayExtender<1, real>(0),
				filter, 
				arrayView(resampledSignal));

			drawSignal(resampledSignal, filter, arrayView(image), Color(1));

			for (integer k = 0;k < i;++k)
			{
				drawVerticalLine((k + 0.5) * ((real)Width / i), 0, Height, 
					Color(0, 1, 0), arrayView(image),
					TransparentColorMixer<Color>(0.5));
			}

			savePcx(image, "test_filter_" + filter->name() + "_" + integerToString(i, 3) + ".pcx");
		}
	}

	void testSignal()
	{
		testSignal(boxFilter());
		testSignal(triangleFilter());
		testSignal(cubicFilter());
		testSignal(lanczosFilter());
	}

	void testFilters()
	{
		const integer Width = 500;
		const integer Height = 500;

		Array<2, Color> image(Width, Height);

		//drawFilter(GaussianFilter(1), hsvToRgb(Color(randomReal(), 1, 1)), image);
		drawFilter(GaussianFilter(2), hsvToRgb(Color(randomReal(), 1, 1)), image);
		drawFilter(TriangleFilter(), hsvToRgb(Color(randomReal(), 1, 1)), image);
		drawFilter(BoxFilter(), hsvToRgb(Color(randomReal(), 1, 1)), image);
		//drawFilter(MitchellFilter(), hsvToRgb(Color(randomReal(), 1, 1)), image);
		//drawFilter(CubicFilter(), hsvToRgb(Color(randomReal(), 1, 1)), image);
		drawFilter(LanczosFilter(2), hsvToRgb(Color(randomReal(), 1, 1)), image);

		/*
		const real b = 0.65;
		const real c = (1 - b) / 2;
		drawFilter(MitchellFilter(b, c), randomRgbColor(), image);
		drawFilter(GaussianFilter(2), Color(1), image);
		*/

		savePcx(image, "test_filter.pcx");
	}

	void testCubicFilter()
	{
		const integer Width = 500;
		const integer Height = 500;

		Array<2, Color> image(Width, Height);

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

		Array<2, Color> image(Width, Height);

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
		gfxTestList().add("Filter.Signal", testSignal);
	}

	CallFunction run(testAdd);

}
