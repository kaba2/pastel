// Description: Filter example
// DocumentationOf: filter.h

#include "pastel_example.h"

#include <pastel/gfx/pcx.h>
#include <pastel/gfx/color_space.h>
#include <pastel/gfx/color_tools.h>
#include <pastel/gfx/drawing.h>
#include <pastel/gfx/noise.h>
#include <pastel/gfx/colormixers.h>

#include <pastel/gfx/resampling.h>
#include <pastel/gfx/filters.h>
#include <pastel/gfx/table_filter.h>

#include <pastel/sys/views.h>
#include <pastel/sys/array.h>
#include <pastel/sys/string_algorithms.h>

#include <pastel/sys/vector_tools.h>

using namespace Pastel;

namespace
{

	void drawFilter(
		const ConstFilterPtr& filter,
		const Color& color,
		Array<Color, 2>& image,
		dreal maxFilterRadius = 2)
	{
		const integer width = image.width();
		const integer height = image.height();

		Vector2 previous;
		for (integer i = 0;i < width;++i)
		{
			const dreal t = (((dreal)i / (width - 1)) - 0.5) * 2 * maxFilterRadius;
			const Vector2 current(i, ((filter->evaluate(t) / filter->evaluate(0) + 1) / 2) * (height * 0.75));

			if (i != 0)
			{
				drawSegment(
					Segment2(previous, current), color, arrayView(image));
			}

			previous = current;
		}
	}

	void drawFilter2D(
		const ConstFilterPtr& xFilter,
		const ConstFilterPtr& yFilter,
		const Color& color,
		Array<Color, 2>& image,
		dreal xMaxFilterRadius = 2,
		dreal yMaxFilterRadius = 2)
	{
		const integer width = image.width();
		const integer height = image.height();
		
		const dreal normalization =
			inverse(xFilter->evaluate(0) * 
			yFilter->evaluate(0));

		for (integer y = 0;y < height;++y)
		{
			for (integer x = 0;x < width;++x)
			{
				const Vector2 uv(
					dequantizeUnsigned(x, width), 
					dequantizeUnsigned(y, height));
				
				const dreal value = 
					xFilter->evaluate(2 * (uv.x() - 0.5) * xMaxFilterRadius) * 
					yFilter->evaluate(2 * (uv.y() - 0.5) * yMaxFilterRadius);

				image(x, y) = color * value * normalization;
			}
		}
	}

	void drawRadialFilter2D(
		const ConstFilterPtr& filter,
		const Color& color,
		Array<Color, 2>& image,
		dreal xMaxFilterRadius = 2)
	{
		const integer width = image.width();
		const integer height = image.height();
		
		const dreal normalization =
			inverse(filter->evaluate(0));

		for (integer y = 0;y < height;++y)
		{
			for (integer x = 0;x < width;++x)
			{
				const Vector2 uv(
					dequantizeUnsigned(x, width), 
					dequantizeUnsigned(y, height));

				const dreal distance = norm(evaluate(2 * uv - 1));
				
				const dreal value = 
					filter->evaluate(distance * xMaxFilterRadius);

				image(x, y) = color * value * normalization;
			}
		}
	}

	template <typename Image_View>
	void drawSignal(
		const Array<dreal, 1>& signal,
		const ConstFilterPtr& filter,
		const View<2, Color, Image_View>& image,
		const Color& color)
	{
		Array<dreal, 1> resampledSignal(image.width());
		resample<dreal>(constArrayView(signal), 
			ArrayExtender<1, dreal>(0),
			filter, arrayView(resampledSignal));

		const integer width = image.width();
		const integer height = image.height();

		Vector2 previous;
		for (integer i = 0;i < width;++i)
		{
			const Vector2 current(i, height * (0.5 * resampledSignal(i) + 0.5));
			if (i > 0)
			{
				drawSegment(Segment2(previous, current), color, image);
			}
			previous = current;
		}
	}

	void testSignal(const ConstFilterPtr& filter)
	{
		const integer Width = 512;
		const integer Height = 256;

		Array<Color, 2> image(Vector2i(Width, Height));

		Array<dreal, 1> signal(Width);

		for (integer i = 0;i < Width;++i)
		{
			signal(i) = (perlinNoise<dreal>(i * 0.11) - 0.5) * 2;
		}

		drawSignal(signal, filter, arrayView(image), Color(1));

		for (integer i = Width;i >= 1;i /= 2)
		{
			clear(Color(0), arrayView(image));

			Array<dreal, 1> resampledSignal(i);
			resample<dreal>(constArrayView(signal), 
				ArrayExtender<1, dreal>(0),
				filter, 
				arrayView(resampledSignal));

			drawSignal(resampledSignal, filter, arrayView(image), Color(1));

			for (integer k = 0;k < i;++k)
			{
				drawVerticalLine((k + 0.5) * ((dreal)Width / i), 0, Height, 
					Color(0, 1, 0), arrayView(image),
					TransparentColorMixer<Color>(0.5));
			}

			savePcx(image, "filter_" + filter->name() + "_" + integerToString(i, 3) + ".pcx");
		}
	}

	void testSignal()
	{
		testSignal(boxFilter());
		testSignal(triangleFilter());
		testSignal(cubicFilter());
		testSignal(lanczosFilter());
	}

	void testTableFilter()
	{
		const integer Width = 500;
		const integer Height = 500;

		Array<Color, 2> image(Vector2i(Width, Height));

		FilterPtr filter = lanczosFilter(3);

		drawFilter(tableFilter(filter, 0), 
			hsvToRgb(Color(random<real32>(), 1, 1)), image, 4);
		drawFilter(tableFilter(filter, 1), 
			hsvToRgb(Color(random<real32>(), 1, 1)), image, 4);
		drawFilter(tableFilter(filter, 4), 
			hsvToRgb(Color(random<real32>(), 1, 1)), image, 4);
		drawFilter(tableFilter(filter, 16), 
			hsvToRgb(Color(random<real32>(), 1, 1)), image, 4);

		savePcx(image, "filter_table.pcx");
	}

	void testShape1D()
	{
		const integer Width = 500;
		const integer Height = 500;

		Array<Color, 2> image(Vector2i(Width, Height));

		drawFilter(gaussianFilter(2), hsvToRgb(Color(random<real32>(), 1, 1)), image);
		drawFilter(triangleFilter(), hsvToRgb(Color(random<real32>(), 1, 1)), image);
		drawFilter(boxFilter(), hsvToRgb(Color(random<real32>(), 1, 1)), image);
		drawFilter(mitchellFilter(), hsvToRgb(Color(random<real32>(), 1, 1)), image);
		//drawFilter(Cubic_Filter(), hsvToRgb(Color(random<real32>(), 1, 1)), image);

		savePcx(image, "filter_shape.pcx");
	}

	void testSeparableShape2D()
	{
		const integer Width = 500;
		const integer Height = 500;

		Array<Color, 2> image(Vector2i(Width, Height));

		drawFilter2D(gaussianFilter(2), gaussianFilter(2), hsvToRgb(Color(random<real32>(), 1, 1)), image);
		savePcx(image, "filter_shape_gaussian.pcx");

		drawFilter2D(triangleFilter(), triangleFilter(), hsvToRgb(Color(random<real32>(), 1, 1)), image);
		savePcx(image, "filter_shape_triangle.pcx");

		drawFilter2D(boxFilter(), boxFilter(), hsvToRgb(Color(random<real32>(), 1, 1)), image);
		savePcx(image, "filter_shape_box.pcx");

		drawFilter2D(mitchellFilter(), mitchellFilter(), hsvToRgb(Color(random<real32>(), 1, 1)), image);
		savePcx(image, "filter_shape_mitchell.pcx");

		drawFilter2D(cubicFilter(), cubicFilter(), hsvToRgb(Color(random<real32>(), 1, 1)), image);
		savePcx(image, "filter_shape_cubic.pcx");
	}

	void testRadialShape2D()
	{
		const integer Width = 500;
		const integer Height = 500;

		Array<Color, 2> image(Vector2i(Width, Height));

		drawRadialFilter2D(gaussianFilter(2), hsvToRgb(Color(random<real32>(), 1, 1)), image);
		savePcx(image, "filter_radial_shape_gaussian.pcx");

		drawRadialFilter2D(triangleFilter(), hsvToRgb(Color(random<real32>(), 1, 1)), image);
		savePcx(image, "filter_radial_shape_triangle.pcx");

		drawRadialFilter2D(boxFilter(), hsvToRgb(Color(random<real32>(), 1, 1)), image);
		savePcx(image, "filter_radial_shape_box.pcx");

		drawRadialFilter2D(mitchellFilter(), hsvToRgb(Color(random<real32>(), 1, 1)), image);
		savePcx(image, "filter_radial_shape_mitchell.pcx");

		drawRadialFilter2D(cubicFilter(), hsvToRgb(Color(random<real32>(), 1, 1)), image);
		savePcx(image, "filter_radial_shape_cubic.pcx");
	}

	void testCubicFilter()
	{
		const integer Width = 500;
		const integer Height = 500;

		Array<Color, 2> image(Vector2i(Width, Height));

		const dreal Range = 3;
		const integer Steps = 20;
		for (integer i = 0;i < Steps;++i)
		{
			drawFilter(cubicFilter(
				linear((dreal)-Range, (dreal)(1 + Range), (dreal)i / (Steps - 1))), randomRgbColor(), image);
		}

		savePcx(image, "filter_cubic.pcx");
	}

	void testMitchellFilter()
	{
		const integer Width = 500;
		const integer Height = 500;

		Array<Color, 2> image(Vector2i(Width, Height));

		const dreal bMin = 0;
		const dreal bMax = 0.65;

		const integer Steps = 5;
		for (integer i = 0;i < Steps;++i)
		{
			const dreal b = linear(bMin, bMax, (dreal)i / (Steps - 1));
			const dreal c = (1 - b) / 2;

			drawFilter(mitchellFilter(b, c), randomRgbColor(), image);
		}

		savePcx(image, "filter_mitchell.pcx");
	}

	void addTest()
	{
		testRunner().add("Filter.Shape1D", testShape1D);
		testRunner().add("Filter.SeparableShape2D", testSeparableShape2D);
		testRunner().add("Filter.RadialShape2D", testRadialShape2D);
		testRunner().add("Filter.Cubic", testCubicFilter);
		testRunner().add("Filter.Mitchell", testMitchellFilter);
		testRunner().add("Filter.Table", testTableFilter);
		testRunner().add("Filter.Signal", testSignal);
	}

	CallFunction run(addTest);

}
