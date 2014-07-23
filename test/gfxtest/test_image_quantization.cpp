// Description: Testing for image quantization
// DocumentationOf: image_quantization.h

#include "test_pastelgfx.h"

#include "pastel/gfx/image_quantization.h"

#include "pastel/gfx/loadpcx.h"
#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/image_tools.h"

using namespace Pastel;

namespace
{

	class Test
		: public TestSuite
	{
	public:
		Test()
			: TestSuite(&testReport())
		{
		}

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

	void test()
	{
		Test test;
		test.run();
	}

	void addTests()
	{
		testRunner().add("image_quantization", test);
	}

	CallFunction run(addTests);

}
