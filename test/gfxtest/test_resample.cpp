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

#include "pastel/dsp/resample.h"
#include "pastel/dsp/filter_all.h"

#include "pastel/device/timer.h"

using namespace Pastel;

namespace
{

	void testPerformance()
	{
		Array<2, Color> image;
		loadPcx("lena.pcx", image);

		Array<2, Color> largeImage(2500, 2500);

		Timer timer;
		timer.setStart();

		resample<Color>(constArrayView(image), 
			clampExtender(),
			lanczosFilter(3),
			arrayView(largeImage));

		timer.store();

		log() << timer.seconds() << logNewLine;

		savePcx(largeImage, "output/resample_performance.pcx");
	}

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

		saveGrayscalePcx(imageSlice, "output/resample_3.pcx");
	}

	void testResample2(
		const std::string& testName,
		const Array<2, uint32>& image,
		const ConstFilterRef& filter)
	{
		const integer SmallWidth = 100;
		const integer SmallHeight = 100;
		const integer BigWidth = 512;
		const integer BigHeight = 512;

		Array<2, uint32> smallImage(SmallWidth, SmallHeight);
		Array<2, uint32> bigImage(BigWidth, BigHeight);
		ArrayExtender<2, Color> wrapper(mirrorExtender());

		resample<Color>(constRgb888View(image), wrapper,
			filter, rgb888View(smallImage));
		transform(rgb888View(smallImage), fitColor);
		savePcx(smallImage, "output/resample_" + testName + "_down_" + filter->name() + ".pcx");

		resample<Color>(constRgb888View(smallImage), wrapper,
			filter, rgb888View(bigImage));
		transform(rgb888View(bigImage), fitColor);
		savePcx(bigImage, "output/resample_" + testName + "_up_" + filter->name() + ".pcx");

	}

	void testResample2Text()
	{
		Array<2, uint32> image;
		loadPcx("resample_text.pcx", image);

		const integer VerySmallWidth = 100;
		const integer VerySmallHeight = 100;
		const integer SmallWidth = 200;
		const integer SmallHeight = 200;
		const integer BigWidth = 800;
		const integer BigHeight = 800;

		const ConstFilterRef filter = lanczosFilter(3);

		Array<2, uint32> verySmallImage(VerySmallWidth, VerySmallHeight);
		Array<2, uint32> smallImage(SmallWidth, SmallHeight);
		Array<2, uint32> bigImage(BigWidth, BigHeight);
		ArrayExtender<2, Color> wrapper(mirrorExtender());

		resample<Color>(constRgb888View(image), wrapper,
			filter, rgb888View(smallImage));
		transform(rgb888View(smallImage), fitColor);
		savePcx(smallImage, "output/resample_text_sample.pcx");

		resample<Color>(constRgb888View(image), wrapper,
			filter, rgb888View(verySmallImage));
		resample<Color>(constRgb888View(verySmallImage), wrapper,
			filter, rgb888View(smallImage));
		transform(rgb888View(smallImage), fitColor);
		savePcx(smallImage, "output/resample_text_blurry.pcx");

		resample<Color>(constRgb888View(image), wrapper,
			filter, rgb888View(bigImage));
		transform(rgb888View(bigImage), fitColor);
		savePcx(bigImage, "output/resample_text_reconstruct.pcx");

		resample<Color>(constRgb888View(smallImage), wrapper,
			filter, rgb888View(bigImage));
		transform(rgb888View(bigImage), fitColor);
		savePcx(bigImage, "output/resample_text_filter.pcx");
	}

	void testResample2()
	{
		Array<2, uint32> image;
		loadPcx("lena.pcx", image);

		std::string testName = "lena";

		testResample2(testName, image, mitchellFilter());
		testResample2(testName, image, boxFilter());
		testResample2(testName, image, triangleFilter());
		testResample2(testName, image, lanczosFilter());

		/*
		loadPcx("grid.pcx", image);

		resample<Color>(image, BigWidth, BigHeight, wrapper,
			lanczosFilter, bigImage);
		transform(rgb888View(bigImage), fitColor);
		savePcx(bigImage, "resample_grid_up_lanczos.pcx");

		for (integer i = 1;i < 100;++i)
		{
			resample<Color>(image, i, i, wrapper,
				lanczosFilter, bigImage);
			transform(rgb888View(bigImage), fitColor);
			savePcx(bigImage, "resample_grid_down_" + integerToString(i, 2) + "_lanczos.pcx");
		}
		*/
	}

	void testAdd()
	{
		gfxTestList().add("Resample.2D", testResample2);
		gfxTestList().add("Resample.2D.Text", testResample2Text);
		gfxTestList().add("Resample.3D", testResample3);
		gfxTestList().add("Resample.Performance", testPerformance);
	}

	CallFunction run(testAdd);

}
