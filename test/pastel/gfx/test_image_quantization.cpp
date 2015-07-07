// Description: Testing for image quantization
// DocumentationOf: image_quantization.h

#include "test/test_init.h"

#include "pastel/gfx/quantization.h"

#include "pastel/gfx/image_file.h"
#include "pastel/gfx/image_tools.h"

TEST_CASE("ErrorDiffusion (ErrorDiffusion)")
{
	real32 Quantization = 0.2;

	Array<Color, 2> image;
	loadPcx("lena.pcx", image);

	quantizeErrorDiffusion(image, Color(Quantization));
	savePcx(image, "lena_diffusion_quantized.pcx");
}

TEST_CASE("Rounding (Rounding)")
{
	real32 Quantization = 0.2;

	Array<Color, 2> image;
	loadPcx("lena.pcx", image);

	quantizeRounding(image, Color(Quantization));
	savePcx(image, "lena_rounding_quantized.pcx");
}

