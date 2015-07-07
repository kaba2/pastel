// Description: Testing for noise
// DocumentationOf: noise.h

#include "test/test_init.h"

#include "pastel/gfx/noise.h"
#include "pastel/gfx/image_file.h"

TEST_CASE("1d (Noise)")
{
	integer Width = 400;
	integer Height = 80;

	Array<Color, 2> image(Vector2i(Width, Height));
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

	savePcx(image, "perlin_noise_1d.pcx");
}

TEST_CASE("2d (Noise)")
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
			image(x, y) = fromColor +
				perlinNoise(Vector2((real)x / 5,

				(real)y / 5)) * deltaColor;
		}
	}

	savePcx(image, "perlin_noise_2d.pcx");
}

TEST_CASE("3d (Noise)")
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
			image(x, y) = fromColor +
				perlinNoise(Vector3((real)x / 5,

				(real)y / 5, 0)) * deltaColor;
		}
	}

	savePcx(image, "perlin_noise_3d.pcx");
}

TEST_CASE("Simplex2d (Noise)")
{
	integer Width = 512;
	integer Height = 512;

	Array<Color, 2> image(Vector2i(Width, Height));
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

	savePcx(image, "simplex_noise_2d.pcx");
}

TEST_CASE("Simplex3d (Noise)")
{
	integer Width = 512;
	integer Height = 512;

	Array<Color, 2> image(Vector2i(Width, Height));
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

	savePcx(image, "simplex_noise_3d.pcx");
}

