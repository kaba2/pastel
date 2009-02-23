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
		Array<2, Color> image;

		loadPcx("quantizeinput.pcx", image);

		quantizeErrorDiffusion(image, Quantization);

		savePcx(image, "quantizediffusionoutput.pcx");
	}

	void testRounding()
	{
		Array<2, Color> image;

		loadPcx("quantizeinput.pcx", image);

		quantizeRounding(image, Quantization);

		savePcx(image, "quantizeroundedoutput.pcx");
	}

	void quantizeTests()
	{
		gfxTestList() +=
			testErrorDiffusion,
			testRounding;
	}

	CallFunction addTests(quantizeTests);

}
*/
