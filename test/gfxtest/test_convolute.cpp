// Description: Testing for convolution
// DocumentationOf: convolution.h

#include "test_pastelgfx.h"

#include "pastel/sys/view/arrayview.h"

#include "pastel/math/uniform_sampling.h"

#include "pastel/gfx/pcx.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/drawing.h"
#include "pastel/gfx/image_gfxrenderer.h"
#include "pastel/gfx/gfxrenderer_tools.h"
#include "pastel/gfx/mipimage_texture.h"
#include "pastel/gfx/convolution.h"
#include "pastel/gfx/packrange.h"
#include "pastel/gfx/noise.h"

using namespace Pastel;

namespace
{

	void testConvolute2()
	{
		Vector<integer, 2> extent(512);
		Vector<integer, 2> filterExtent(31);

		Array<Color, 2> input(extent, Color(0));
		Array<real32, 2> filter(filterExtent, 1);

		saveGrayscalePcx(filter, "convolution_filter.pcx");

		integer points = 1000;

		for (integer i = 0;i < points;++i)
		{
			input(
				randomVector<real, 2>() * Vector2(extent - 1)) = randomRgbColor();
		}

		savePcx(input, "convolution_input.pcx");

		Array<Color, 2> output(extent);

		convolute(constArrayView(input),
			constArrayView(filter),
			arrayView(output));

		transform(
			arrayView(output),
			fitColor);

		savePcx(output, "convolution_output.pcx");
	}

	void testConvolute3()
	{
		Vector<integer, 3> extent(200);
		Vector<integer, 3> filterExtent(31);

		Array<Color, 3> input(extent, Color(0));
		Array<real32, 3> filter(filterExtent, 1);

		integer points = 1000;

		for (integer i = 0;i < points;++i)
		{
			input(
				randomVector<real, 3>() * Vector3(extent - 1)) = randomRgbColor();
		}

		Array<Color, 3> output(extent);

		convolute(constArrayView(input),
			constArrayView(filter),
			arrayView(output));

		transform(
			arrayView(output),
			fitColor);
	}

	void addTest()
	{
		testRunner().add("Convolution.2D", testConvolute2);
		testRunner().add("Convolution.3D", testConvolute3);
	}

	CallFunction run(addTest);

}
