// Description: Testing for image pdfs
// DocumentationOf: imagepdf.h

#include "test_pastelgfx.h"

#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/loadpcx.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/noise.h"
#include "pastel/gfx/summedareatable.h"
#include "pastel/gfx/imagepdf.h"
#include "pastel/gfx/drawing.h"

#include "pastel/dsp/filters.h"
#include "pastel/dsp/mipmap.h"

#include "pastel/sys/array.h"
#include "pastel/sys/views.h"
#include "pastel/sys/string_algorithms.h"

#include "pastel/device/timer.h"

#include <set>
#include <list>
#include <vector>
#include <algorithm>

using namespace Pastel;

namespace
{

	void testImagePdf()
	{
		Array<Color> colorImage;
		loadPcx("imagepdf_input.pcx", colorImage);

		Array<real32> image(colorImage.extent());
		for (integer i = 0;i < image.size();++i)
		{
			image(i) = luma(colorImage(i));
		}

		ImagePdf<real32, 2> imagePdf(constArrayView(image));

		clear(0, arrayView(image));

		integer points = 0;

		Timer timer;
		timer.setStart();

		integer stepPoints = 10000;
		for (integer step = 0;step < 4;++step)
		{
			for (;points < stepPoints;++points)
			{
				image(imagePdf()) += 0.2;
			}

			timer.store();

			log() << "Computation took " << timer.seconds() << " seconds." << logNewLine;

			saveGrayscalePcx(image, std::string("imagepdf_output_") + integerToString(points) + std::string(".pcx"));

			stepPoints *= 10;
		}
	}

	void addTest()
	{
		testRunner().add("ImagePdf", testImagePdf);
	}

	CallFunction run(addTest);

}
