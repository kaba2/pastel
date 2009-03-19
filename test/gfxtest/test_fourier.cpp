#include "pastelgfxtest.h"

#include "pastel/gfx/loadpcx.h"
#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/coloradapter.h"
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
		typedef Complex<real32> Physical;

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

	void testFourier()
	{
		Array<2, Color> image;
		loadPcx("lena.pcx", image);

		Array<2, Complex<real32> > fourierImage(image.extent());

		discreteFourier(
			constAdaptedView(
			constAdaptedView(constArrayView(image), Color_Component_Adapter(0)),
			Real_Complex_Adapter()),
			arrayView(fourierImage));

		Array<2, real32> fourierReal(image.extent());
		copy(constAdaptedView(constArrayView(fourierImage), Complex_Real_Adapter()),
			arrayView(fourierReal));

		//packRange(arrayView(fourierReal), 0, 1);

		saveGrayscalePcx(fourierReal, "fourier_lena_real.pcx");
	}

	void testAdd()
	{
		gfxTestList().add("Fourier", testFourier);
	}

	CallFunction run(testAdd);

}
