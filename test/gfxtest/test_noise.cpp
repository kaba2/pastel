#include "pastelgfxtest.h"

#include "pastel/gfx/noise.h"
#include "pastel/gfx/savepcx.h"

namespace
{

	using namespace Pastel;

	void test1d()
	{
		const integer Width = 400;
		const integer Height = 80;

		Array<2, Color> image(Width, Height);
		Color fromColor(0, 0, 0);
		Color toColor(0, 0, 1);
		Color deltaColor(toColor - fromColor);

		for (integer y = 0;y < Height;++y)
		{
			for (integer x = 0;x < Width;++x)
			{
				image(x, y) = fromColor +
					noise((real)x / 5) * deltaColor;
			}
		}

		savePcx(image, "testnoise_1d.pcx");
	}

	void test2d()
	{
		const integer Width = 400;
		const integer Height = 400;

		Array<2, Color> image(Width, Height);
		Color fromColor(0, 0, 0);
		Color toColor(0, 0, 1);
		Color deltaColor(toColor - fromColor);

		for (integer y = 0;y < Height;++y)
		{
			for (integer x = 0;x < Width;++x)
			{
				image(x, y) = fromColor +
					noise(Point2((real)x / 5,
					(real)y / 5)) * deltaColor;
			}
		}

		savePcx(image, "testnoise_2d.pcx");
	}

	void testSome()
	{
		noise(real(0));
		noise(real(0));
		noise(real(0));
		noise(real(0));
		noise(Point2(0));
		noise(Point2(0));
		noise(Point2(0));
		noise(Point2(0));
		noise(Point3(0));
		noise(Point3(0));
		noise(Point3(0));
		noise(Point3(0));
	}

	void test3d()
	{
		const integer Width = 400;
		const integer Height = 400;

		Array<2, Color> image(Width, Height);
		Color fromColor(0, 0, 0);
		Color toColor(0, 0, 1);
		Color deltaColor(toColor - fromColor);

		for (integer y = 0;y < Height;++y)
		{
			for (integer x = 0;x < Width;++x)
			{
				image(x, y) = fromColor +
					noise(Point3((real)x / 5,
					(real)y / 5, 0)) * deltaColor;
			}
		}

		savePcx(image, "testnoise_3d.pcx");
	}

	void testBegin()
	{
		test1d();
		test2d();
		test3d();
	}

	void testAdd()
	{
		gfxTestList().add("Noise", testBegin);
	}

	CallFunction run(testAdd);

}
