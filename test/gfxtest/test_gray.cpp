#include "pastelgfxtest.h"

#include "pastel/sys/view_tools.h"
#include "pastel/sys/arrayview.h"

#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/loadpcx.h"
#include "pastel/gfx/color_tools.h"

using namespace Pastel;

namespace
{

	Color luma(const Color& rgb)
	{
		return Color(rgbLuma(rgb));
	}

	Color lightness(const Color& rgb)
	{
		Color lab(xyzToLab(srgbToXyz(rgb)));
		lab[1] = 0;
		lab[2] = 0;
		return xyzToSrgb(labToXyz(lab));
	}

	void testGray()
	{
		LinearArray<2, Color> image;
		loadPcx("lena.pcx", image);

		LinearArray<2, Color> transformed;

		transform(constArrayView(image),
			arrayView(transformed), luma);

		savePcx(transformed, "testgray_luma.pcx");

		transform(constArrayView(image),
			arrayView(transformed), lightness);

		savePcx(transformed, "testgray_lightness.pcx");
	}

	void testBegin()
	{
		testGray();
	}

	void testAdd()
	{
		gfxTestList().add("Gray", testBegin);
	}

	CallFunction run(testAdd);

}
