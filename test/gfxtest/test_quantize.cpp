/*
#include "pastelgfxtest.h"

#include "pastel/gfx/quantize.h"

#include "pastel/gfx/loadpcx.h"
#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/image_tools.h"

using namespace Pastel;

namespace
{

	const real32 Quantization = 0.2;

	void testErrorDiffusion()
	{
		Array<Color, 2> image;

		loadPcx("quantizeinput.pcx", image);

		quantizeErrorDiffusion(image, Quantization);

		savePcx(image, "output/quantizediffusionoutput.pcx");
	}

	void testRounding()
	{
		Array<Color, 2> image;

		loadPcx("quantizeinput.pcx", image);

		quantizeRounding(image, Quantization);

		savePcx(image, "output/quantizeroundedoutput.pcx");
	}

	void quantizeTests()
	{
		testRunner() +=
			testErrorDiffusion,
			testRounding;
	}

	CallFunction addTests(quantizeTests);

}
*/
