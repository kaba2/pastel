// Description: Testing for image quantization
// DocumentationOf: image_quantization.h

#include "test/test_init.h"

#include "pastel/gfx/quantization.h"

#include "pastel/gfx/image_file.h"
#include "pastel/gfx/image_tools.h"

namespace
{

	class Test
	{
	public:
		virtual void run()
		{
			testErrorDiffusion();
			testRounding();
		}

		void testErrorDiffusion()
		{
			real32 Quantization = 0.2;

			Array<Color, 2> image;
			loadPcx("lena.pcx", image);

			quantizeErrorDiffusion(image, Color(Quantization));
			savePcx(image, "lena_diffusion_quantized.pcx");
		}

		void testRounding()
		{
			real32 Quantization = 0.2;

			Array<Color, 2> image;
			loadPcx("lena.pcx", image);

			quantizeRounding(image, Color(Quantization));
			savePcx(image, "lena_rounding_quantized.pcx");
		}
	};

}
