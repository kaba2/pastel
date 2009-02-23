#include "pastelgfxtest.h"

#include "pastel/sys/arrayview.h"

#include "pastel/math/uniformsampling.h"

#include "pastel/gfx/pcx.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/drawing.h"
#include "pastel/gfx/imagegfxrenderer.h"
#include "pastel/gfx/gfxrenderer_tools.h"
#include "pastel/gfx/mipimagetexture.h"
#include "pastel/gfx/filterform.h"
#include "pastel/gfx/convolute.h"
#include "pastel/gfx/packrange.h"
#include "pastel/gfx/noise.h"

using namespace Pastel;

namespace
{

	void testConvolute2()
	{
		Vector<2, integer> extent(500);
		Vector<2, integer> filterExtent(31);

		Array<2, Color> input(extent, Color(0));
		Array<2, real32> filter(filterExtent, 1);

		setFilter(bartlettWindow<real32>, arrayView(filter));

		saveGrayscalePcx(filter, "testconvolute_filter.pcx");

		const integer points = 1000;

		for (integer i = 0;i < points;++i)
		{
			input(
				asPoint(randomVector<2, real>() * Vector<2, real>((extent - 1)))) = randomRgbColor();
		}

		savePcx(input, "testconvolute_input.pcx");

		Array<2, Color> output(extent);

		convolute(constArrayView(input),
			constArrayView(filter),
			arrayView(output));

		transform(
			arrayView(output),
			fitColor);

		savePcx(output, "testconvolute_output.pcx");
	}

	void testConvolute3()
	{
		Vector<3, integer> extent(500);
		Vector<3, integer> filterExtent(31);

		Array<3, Color> input(extent, Color(0));
		Array<3, real32> filter(filterExtent, 1);

		//setFilter(bartlettWindow<real32>, arrayView(filter));

		const integer points = 1000;

		for (integer i = 0;i < points;++i)
		{
			input(
				asPoint(randomVector<3, real>() * Vector<3, real>((extent - 1)))) = randomRgbColor();
		}

		Array<3, Color> output(extent);

		convolute(constArrayView(input),
			constArrayView(filter),
			arrayView(output));

		transform(
			arrayView(output),
			fitColor);
	}

	void testBegin()
	{
		testConvolute2();
		testConvolute3();
	}

	void testAdd()
	{
		gfxTestList().add("Convolution", testBegin);
	}

	CallFunction run(testAdd);

}
