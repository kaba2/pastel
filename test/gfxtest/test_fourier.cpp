#include "pastelgfxtest.h"

#include "pastel/gfx/loadpcx.h"
#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/coloradapter.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/packrange.h"

#include "pastel/dsp/discretefourier.h"

#include "pastel/sys/arrayview.h"
#include "pastel/sys/view_tools.h"
#include "pastel/sys/adaptedview.h"

#include <iostream>
#include <string>

using namespace std;
using namespace Pastel;

namespace
{

	class Complex_Real_Adapter
	{
	public:
		typedef real32 Logical;
		typedef std::complex<real32> Physical;

		Logical toLogical(const Physical& physical) const
		{
			return physical.real();
		}

		Physical toPhysical(const Logical& logical) const
		{
			return Physical(logical, 0);
		}
	};

	typedef ReverseAdapter<Complex_Real_Adapter> Real_Complex_Adapter;

	class Complex_Mod_Adapter
	{
	public:
		typedef real32 Logical;
		typedef std::complex<real32> Physical;

		Logical toLogical(const Physical& physical) const
		{
			return std::abs(physical);
		}

		Physical toPhysical(const Logical& logical) const
		{
			return Physical(logical, 0);
		}
	};

	void testFourier()
	{
		Array<2, Color> image;
		loadPcx("lena.pcx", image);

		Array<2, std::complex<real32> > fourierImage(image.extent());

		/*
		Array<2, real32> grayImage(image.extent());
		copy(constAdaptedView(constArrayView(image), Luma_Color_Adapter()),
			arrayView(grayImage));
		packRange(arrayView(grayImage), 0, 1);
		saveGrayscalePcx(grayImage, "fourier_lena_gray.pcx");

		Array<2, std::complex<real32> > complexImage(image.extent());
		copy(constAdaptedView(constArrayView(grayImage), Real_Complex_Adapter()),
			arrayView(complexImage));

		discreteFourier(
			constArrayView(complexImage),
			arrayView(fourierImage));
		*/

		discreteFourier(
			constAdaptedView(
			constAdaptedView(constArrayView(image), Luma_Color_Adapter()),
			Real_Complex_Adapter()),
			arrayView(fourierImage));

		Array<2, real32> fourierReal(image.extent());
		copy(constAdaptedView(constArrayView(fourierImage), Complex_Mod_Adapter()),
			arrayView(fourierReal));

		packRange(arrayView(fourierReal), 0, 1);
		for (integer y = 0;y < image.height();++y)
		{
			for (integer x = 0;x < image.width();++x)
			{
				fourierReal(x, y) = std::pow(fourierReal(x, y), (real32)0.2);
			}
		}

		saveGrayscalePcx(fourierReal, "fourier_lena_real.pcx");
	}

	void testAdd()
	{
		gfxTestList().add("Fourier", testFourier);
	}

	CallFunction run(testAdd);

}
