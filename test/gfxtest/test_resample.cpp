#include "pastelgfxtest.h"

#include "pastel/sys/string_tools.h"
#include "pastel/sys/arrayview.h"
#include "pastel/sys/view_tools.h"
#include "pastel/sys/indexextender_all.h"
#include "pastel/sys/sliceview.h"

#include "pastel/gfx/loadpcx.h"
#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/image_tools.h"

#include "pastel/gfx/resample.h"
#include "pastel/gfx/filter_all.h"

using namespace Pastel;

namespace
{

	void testResample3()
	{
		Array<3, real> image(20, 30, 40, 0);
		Array<3, real> smallImage(40, 30, 20, 0);

		resample<real>(
			constArrayView(image), 
			ArrayExtender<3, real>(mirrorExtender()),
			boxFilter(),
			arrayView(smallImage));

		Array<2, real> imageSlice(
			shrink(smallImage.extent(), 0));

		copy(constSliceView(constArrayView(smallImage), 0, 0),
			arrayView(imageSlice));

		saveGrayscalePcx(imageSlice, "testresample_3.pcx");
	}

	void testResample2()
	{
		const integer SmallWidth = 100;
		const integer SmallHeight = 100;
		const integer BigWidth = 512;
		const integer BigHeight = 512;

		Array<2, uint32> image;
		Array<2, uint32> smallImage(SmallWidth, SmallHeight);
		Array<2, uint32> bigImage(BigWidth, BigHeight);
		ArrayExtender<2, Color> wrapper(mirrorExtender());

		loadPcx("lena.pcx", image);

		MitchellFilterRef mitchellFilter(mitchellFilter());
		BoxFilterRef boxFilter(boxFilter());
		TriangleFilterRef triangleFilter(triangleFilter());
		LanczosFilterRef lanczosFilter(lanczosFilter());

		resample<Color>(constRgb888View(image), wrapper,
			mitchellFilter, rgb888View(smallImage));
		transform(rgb888View(smallImage), fitColor);
		savePcx(smallImage, "testresample_down_cubic.pcx");

		resample<Color>(constRgb888View(smallImage), wrapper,
			mitchellFilter, rgb888View(bigImage));
		transform(rgb888View(bigImage), fitColor);
		savePcx(bigImage, "testresample_up_cubic.pcx");

		resample<Color>(constRgb888View(image), wrapper,
			boxFilter, rgb888View(smallImage));
		transform(rgb888View(smallImage), fitColor);
		savePcx(smallImage, "testresample_down_box.pcx");

		resample<Color>(constRgb888View(smallImage), wrapper,
			boxFilter, rgb888View(bigImage));
		transform(rgb888View(bigImage), fitColor);
		savePcx(bigImage, "testresample_up_box.pcx");

		resample<Color>(constRgb888View(image), wrapper,
			triangleFilter, rgb888View(smallImage));
		transform(rgb888View(smallImage), fitColor);
		savePcx(smallImage, "testresample_down_triangle.pcx");

		resample<Color>(constRgb888View(smallImage), wrapper,
			triangleFilter, rgb888View(bigImage));
		transform(rgb888View(bigImage), fitColor);
		savePcx(bigImage, "testresample_up_triangle.pcx");

		resample<Color>(constRgb888View(image), wrapper,
			lanczosFilter, rgb888View(smallImage));
		transform(rgb888View(smallImage), fitColor);
		savePcx(smallImage, "testresample_down_lanczos.pcx");

		resample<Color>(constRgb888View(image), wrapper,
			lanczosFilter, rgb888View(bigImage));
		transform(rgb888View(bigImage), fitColor);
		savePcx(bigImage, "testresample_non_lanczos.pcx");

		resample<Color>(constRgb888View(smallImage), wrapper,
			lanczosFilter, rgb888View(bigImage));
		transform(rgb888View(bigImage), fitColor);
		savePcx(bigImage, "testresample_up_lanczos.pcx");

		GaussianFilterRef gaussianFilter(gaussianFilter());

		resample<Color>(constRgb888View(image), wrapper,
			gaussianFilter, rgb888View(smallImage));
		transform(rgb888View(smallImage), fitColor);
		savePcx(smallImage, "testresample_down_gaussian.pcx");

		resample<Color>(constRgb888View(smallImage), wrapper,
			gaussianFilter, rgb888View(bigImage));
		transform(rgb888View(bigImage), fitColor);
		savePcx(bigImage, "testresample_up_gaussian.pcx");

		/*
		loadPcx("grid.pcx", image);

		resample<Color>(image, BigWidth, BigHeight, wrapper,
			lanczosFilter, bigImage);
		transform(rgb888View(bigImage), fitColor);
		savePcx(bigImage, "testresample_grid_up_lanczos.pcx");

		for (integer i = 1;i < 100;++i)
		{
			resample<Color>(image, i, i, wrapper,
				lanczosFilter, bigImage);
			transform(rgb888View(bigImage), fitColor);
			savePcx(bigImage, "testresample_grid_down_" + integerToString(i, 2) + "_lanczos.pcx");
		}
		*/
	}

	void testAdd()
	{
		gfxTestList().add("Resample.2D", testResample2);
		gfxTestList().add("Resample.3D", testResample3);
	}

	CallFunction run(testAdd);

}
