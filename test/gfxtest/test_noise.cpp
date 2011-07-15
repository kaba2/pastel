// Description: Testing for noise
// DocumentationOf: noise.h

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

		Array<Color, 2> image(Width, Height);
		Color fromColor = Color(0, 0, 0);
		Color toColor = Color(0, 0, 1);
		Color deltaColor(toColor - fromColor);

		for (integer y = 0;y < Height;++y)
		{
			for (integer x = 0;x < Width;++x)
			{
				image(x, y) = fromColor +
					perlinNoise<real>((real)x / 5) * deltaColor;
			}
		}

		savePcx(image, "output/perlin_noise_1d.pcx");
	}

	void test2d()
	{
		const integer Width = 400;
		const integer Height = 400;

		Array<Color, 2> image(Width, Height);
		Color fromColor = Color(0, 0, 0);
		Color toColor = Color(0, 0, 1);
		Color deltaColor(toColor - fromColor);

		for (integer y = 0;y < Height;++y)
		{
			for (integer x = 0;x < Width;++x)
			{
				image(x, y) = fromColor +
					perlinNoise(Vector2((real)x / 5,
					(real)y / 5)) * deltaColor;
			}
		}

		savePcx(image, "output/perlin_noise_2d.pcx");
	}

	void test3d()
	{
		const integer Width = 400;
		const integer Height = 400;

		Array<Color, 2> image(Width, Height);
		Color fromColor = Color(0, 0, 0);
		Color toColor = Color(0, 0, 1);
		Color deltaColor(toColor - fromColor);

		for (integer y = 0;y < Height;++y)
		{
			for (integer x = 0;x < Width;++x)
			{
				image(x, y) = fromColor +
					perlinNoise(Vector3((real)x / 5,
					(real)y / 5, 0)) * deltaColor;
			}
		}

		savePcx(image, "output/perlin_noise_3d.pcx");
	}

	void testSimplex2d()
	{
		const integer Width = 512;
		const integer Height = 512;

		Array<Color, 2> image(Width, Height);
		Color fromColor = Color(0, 0, 0);
		Color toColor = Color(0, 0, 1);
		Color deltaColor(toColor - fromColor);

		for (integer y = 0;y < Height;++y)
		{
			for (integer x = 0;x < Width;++x)
			{
				image(x, y) = fromColor +
					simplexNoise(Vector2((real)x / 30,
					(real)y / 30)) * deltaColor;
			}
		}

		savePcx(image, "output/simplex_noise_2d.pcx");
	}

	void testSimplex3d()
	{
		const integer Width = 512;
		const integer Height = 512;

		Array<Color, 2> image(Width, Height);
		Color fromColor = Color(0, 0, 0);
		Color toColor = Color(0, 0, 1);
		Color deltaColor(toColor - fromColor);

		for (integer y = 0;y < Height;++y)
		{
			for (integer x = 0;x < Width;++x)
			{
				image(x, y) = fromColor +
					simplexNoise(Vector3((real)x / 30, (real)y / 30, 0)) * deltaColor;
			}
		}

		savePcx(image, "output/simplex_noise_3d.pcx");
	}

	void test()
	{
		testSimplex2d();
		testSimplex3d();

		test1d();
		test2d();
		test3d();
	}

	void addTest()
	{
		testRunner().add("Noise", test);
	}

	CallFunction run(addTest);

}
