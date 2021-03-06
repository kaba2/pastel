// Description: Testing for convolution
// DocumentationOf: convolution.h

#include "test/test_init.h"

#include "pastel/sys/view/arrayview.h"

#include "pastel/math/sampling/uniform_sampling.h"

#include "pastel/gfx/image_file.h"
#include "pastel/gfx/color.h"
#include "pastel/gfx/drawing.h"
#include "pastel/gfx/image_gfxrenderer.h"
#include "pastel/gfx/gfxrenderer_tools.h"
#include "pastel/gfx/texture.h"
#include "pastel/gfx/image_processing.h"
#include "pastel/gfx/image_processing/packrange.h"
#include "pastel/gfx/noise.h"

TEST_CASE("Convolute2 (Convolute2)")
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
			randomVector<dreal, 2>() * Vector2(extent - 1)) = randomRgbColor();
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

TEST_CASE("Convolute3 (Convolute3)")
{
	Vector<integer, 3> extent(200);
	Vector<integer, 3> filterExtent(31);

	Array<Color, 3> input(extent, Color(0));
	Array<real32, 3> filter(filterExtent, 1);

	integer points = 1000;

	for (integer i = 0;i < points;++i)
	{
		input(
			randomVector<dreal, 3>() * Vector3(extent - 1)) = randomRgbColor();
	}

	Array<Color, 3> output(extent);

	convolute(constArrayView(input),
		constArrayView(filter),
		arrayView(output));

	transform(
		arrayView(output),
		fitColor);
}

