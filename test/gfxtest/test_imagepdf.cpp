#include "pastelgfxtest.h"

#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/loadpcx.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/noise.h"
#include "pastel/gfx/summedareatable.h"
#include "pastel/gfx/imagepdf.h"
#include "pastel/gfx/draw.h"

#include "pastel/dsp/filter_all.h"
#include "pastel/dsp/mipmap.h"

#include "pastel/sys/array.h"
#include "pastel/sys/view_all.h"
#include "pastel/sys/string_tools.h"

#include "pastel/device/timer.h"

#include <set>
#include <list>
#include <vector>
#include <algorithm>

#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

using namespace boost::lambda;

using namespace Pastel;

namespace
{

	void testImagePdf()
	{
		Array<Color, 2> colorImage;
		loadPcx("imagepdf_input.pcx", colorImage);

		Array<real32, 2> image(colorImage.extent());

		visit(constArrayView(colorImage), arrayView(image), _2 = bind(luma, _1));

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

	void testAdd()
	{
		gfxTestList().add("ImagePdf", testImagePdf);
	}

	CallFunction run(testAdd);

}
