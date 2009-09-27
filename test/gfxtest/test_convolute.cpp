#include "pastelgfxtest.h"

#include "pastel/sys/arrayview.h"

#include "pastel/math/uniform_sampling.h"

#include "pastel/gfx/pcx.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/draw.h"
#include "pastel/gfx/image_gfxrenderer.h"
#include "pastel/gfx/gfxrenderer_tools.h"
#include "pastel/gfx/mipimage_texture.h"
#include "pastel/dsp/filterform.h"
#include "pastel/gfx/convolute.h"
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

		setFilter(bartlettWindow<real32>, arrayView(filter));

		saveGrayscalePcx(filter, "output/convolution_filter.pcx");

		const integer points = 1000;

		for (integer i = 0;i < points;++i)
		{
			input(
				randomVector<real, 2>() * Vector2(extent - 1)) = randomRgbColor();
		}

		savePcx(input, "output/convolution_input.pcx");

		Array<Color, 2> output(extent);

		convolute(constArrayView(input),
			constArrayView(filter),
			arrayView(output));

		transform(
			arrayView(output),
			fitColor);

		savePcx(output, "output/convolution_output.pcx");
	}

	void testConvolute3()
	{
		Vector<integer, 3> extent(200);
		Vector<integer, 3> filterExtent(31);

		Array<Color, 3> input(extent, Color(0));
		Array<real32, 3> filter(filterExtent, 1);

		//setFilter(bartlettWindow<real32>, arrayView(filter));

		const integer points = 1000;

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

	void testAdd()
	{
		gfxTestList().add("Convolution.2D", testConvolute2);
		gfxTestList().add("Convolution.3D", testConvolute3);
	}

	CallFunction run(testAdd);

}
