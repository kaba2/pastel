#include "pastelgfxtest.h"

#include "pastel/sys/array.h"
#include "pastel/sys/view_visit.h"
#include "pastel/sys/extendedconstview_all.h"
#include "pastel/sys/vector_tools.h"

#include "pastel/gfx/colormixers.h"
#include "pastel/gfx/textures.h"
#include "pastel/gfx/draw.h"
#include "pastel/gfx/pcx.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/summedareatable.h"

#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>

using namespace boost::lambda;

using namespace Pastel;

namespace
{

	void testSummedAreaTable()
	{
		Array<Color, 2> image;

		loadPcx("lena.pcx", image);

		Array<Color, 2> sumImage(image.extent());
		computeSummedAreaTable(constArrayView(image), arrayView(sumImage));

		const Color sum = sumImage(image.extent() - 1);
		for (integer i = 0;i < sumImage.size();++i)
		{
			sumImage(i) /= sum;
		}

		savePcx(sumImage, "output/summedareatable.pcx");
	}

	void testBegin()
	{
		testSummedAreaTable();
	}

	void testAdd()
	{
		gfxTestList().add("SummedAreaTable", testBegin);
	}

	CallFunction run(testAdd);

}
