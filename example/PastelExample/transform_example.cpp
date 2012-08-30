// Description: Signal transform example
// Documentation: signal_transforms.txt

#include "pastel_example.h"

#include "pastel/gfx/loadpcx.h"
#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/coloradapter.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/packrange.h"

#include "pastel/gfx/signal_transforms.h"

#include "pastel/sys/arrayview.h"
#include "pastel/sys/view_tools.h"
#include "pastel/sys/adaptedview.h"
#include "pastel/sys/subarray_for_each.h"

#include <iostream>
#include <string>

using namespace Pastel;
using namespace std;

namespace
{

	template <
		typename Transform_Algorithm,
		typename Inverse_Algorithm>
	void testImage(
		const Transform_Algorithm& transform,
		const Inverse_Algorithm& inverse,
		const std::string& name)
	{
		Array<real32> image;
		loadPcx("lena.pcx", image);
		
		Array<real32> tImage(image);

		forEachRowOnAllAxes(tImage(), transform);

		{
			Array<real32> sImage(tImage);

			for (integer i = 0;i < tImage.size();++i)
			{
				sImage(i) = std::log(std::abs(tImage(i)) + 1);
			}

			saveGrayscalePcx(sImage, name + "_lena.pcx", true);
		}

		for (integer y = 0;y < tImage.height();++y)
		{
			for (integer x = 0;x < tImage.width();++x)
			{
				tImage(x, y) *= std::exp(-(real32)(x * x + y * y) / 4000);
			}
		}

		forEachRowOnAllAxes(tImage(), inverse);

		saveGrayscalePcx(tImage, name + "_lena_blur.pcx", true);
	}

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
			testImage(Haar(), InverseHaar(), "haar");
			testImage(Dct(), InverseDct(), "dct");
		}

	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("SignalTransforms", test);
	}

	CallFunction run(addTest);

}
