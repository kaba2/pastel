#include "pastelgfxtest.h"

#include "pastel/sys/string_tools.h"
#include "pastel/sys/arrayview.h"
#include "pastel/sys/view_tools.h"
#include "pastel/sys/indexextender_all.h"

#include "pastel/gfx/loadpcx.h"
#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/color_tools.h"

#include "pastel/gfx/resample.h"
#include "pastel/gfx/filter_all.h"

using namespace Pastel;

namespace
{

	void testResample()
	{
		const integer SmallWidth = 100;
		const integer SmallHeight = 100;
		const integer BigWidth = 512;
		const integer BigHeight = 512;

		LinearArray<2, Color> image;
		LinearArray<2, Color> smallImage(SmallWidth, SmallHeight);
		LinearArray<2, Color> bigImage(BigWidth, BigHeight);
		ArrayExtender<2, Color> wrapper(mirrorExtender());

		loadPcx("lena.pcx", image);

		MitchellFilterRef mitchellFilter(mitchellFilter());
		BoxFilterRef boxFilter(boxFilter());
		TriangleFilterRef triangleFilter(triangleFilter());
		LanczosFilterRef lanczosFilter(lanczosFilter());

		resample(constArrayView(image), wrapper,
			mitchellFilter, arrayView(smallImage));
		transform(arrayView(smallImage), fitColor);
		savePcx(smallImage, "testresample_down_cubic.pcx");

		resample(constArrayView(smallImage), wrapper,
			mitchellFilter, arrayView(bigImage));
		transform(arrayView(bigImage), fitColor);
		savePcx(bigImage, "testresample_up_cubic.pcx");

		resample(constArrayView(image), wrapper,
			boxFilter, arrayView(smallImage));
		transform(arrayView(smallImage), fitColor);
		savePcx(smallImage, "testresample_down_box.pcx");

		resample(constArrayView(smallImage), wrapper,
			boxFilter, arrayView(bigImage));
		transform(arrayView(bigImage), fitColor);
		savePcx(bigImage, "testresample_up_box.pcx");

		resample(constArrayView(image), wrapper,
			triangleFilter, arrayView(smallImage));
		transform(arrayView(smallImage), fitColor);
		savePcx(smallImage, "testresample_down_triangle.pcx");

		resample(constArrayView(smallImage), wrapper,
			triangleFilter, arrayView(bigImage));
		transform(arrayView(bigImage), fitColor);
		savePcx(bigImage, "testresample_up_triangle.pcx");

		resample(constArrayView(image), wrapper,
			lanczosFilter, arrayView(smallImage));
		transform(arrayView(smallImage), fitColor);
		savePcx(smallImage, "testresample_down_lanczos.pcx");

		resample(constArrayView(image), wrapper,
			lanczosFilter, arrayView(bigImage));
		transform(arrayView(bigImage), fitColor);
		savePcx(bigImage, "testresample_non_lanczos.pcx");

		resample(constArrayView(smallImage), wrapper,
			lanczosFilter, arrayView(bigImage));
		transform(arrayView(bigImage), fitColor);
		savePcx(bigImage, "testresample_up_lanczos.pcx");

		GaussianFilterRef gaussianFilter(gaussianFilter());

		resample(constArrayView(image), wrapper,
			gaussianFilter, arrayView(smallImage));
		transform(arrayView(smallImage), fitColor);
		savePcx(smallImage, "testresample_down_gaussian.pcx");

		resample(constArrayView(smallImage), wrapper,
			gaussianFilter, arrayView(bigImage));
		transform(arrayView(bigImage), fitColor);
		savePcx(bigImage, "testresample_up_gaussian.pcx");

		/*
		loadPcx("grid.pcx", image);

		resample(image, BigWidth, BigHeight, wrapper,
			lanczosFilter, bigImage);
		transform(arrayView(bigImage), fitColor);
		savePcx(bigImage, "testresample_grid_up_lanczos.pcx");

		for (integer i = 1;i < 100;++i)
		{
			resample(image, i, i, wrapper,
				lanczosFilter, bigImage);
			transform(arrayView(bigImage), fitColor);
			savePcx(bigImage, "testresample_grid_down_" + integerToString(i, 2) + "_lanczos.pcx");
		}
		*/
	}

	void testBegin()
	{
		testResample();
	}

	void testAdd()
	{
		gfxTestList().add("Resample", testBegin);
	}

	CallFunction run(testAdd);

}
