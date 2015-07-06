// Description: Testing for noise
// DocumentationOf: noise.h

#include "test_pastelgfx.h"

#include "pastel/gfx/noise/gradientfield.h"
#include "pastel/gfx/image_file.h"

namespace
{

	using namespace Pastel;

	void test2d()
	{
		integer Width = 400;
		integer Height = 400;

		Array<Color, 2> image(Vector2i(Width, Height));
		Color fromColor = Color(0, 0, 0);
		Color toColor = Color(0, 0, 1);
		Color deltaColor(toColor - fromColor);

		for (integer y = 0;y < Height;++y)
		{
			for (integer x = 0;x < Width;++x)
			{
				Vector2 p((real)x / 5, (real)y / 5);

				real grad = 
					gradientField<real32, 2>()(floor(p), p - floor(p));

				image(x, y) = fromColor + grad * deltaColor;
			}
		}

		savePcx(image, "gradient_field.pcx");
	}

	void test()
	{
		test2d();
	}

	void addTest()
	{
		testRunner().add("gradientfield", test);
	}

	CallFunction run(addTest);

}
