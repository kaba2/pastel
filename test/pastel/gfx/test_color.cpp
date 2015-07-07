// Description: Testing for Color
// DocumentationOf: color.h

#include "test/test_init.h"

#include "pastel/gfx/image_file/pcx.h"
#include "pastel/gfx/color/color_space.h"
#include "pastel/gfx/color/color_tools.h"
#include "pastel/gfx/color/color_illuminant.h"
#include "pastel/gfx/drawing.h"

#include "pastel/sys/string.h"
#include "pastel/sys/view.h"

#include <iostream>

namespace
{

	TEST_CASE("Chromaticity (Chromaticity)")
	{
		real Width = 500;
		real Height = 500;
		Array<Color, 2> image(Vector2i(Width, Height));

		for (integer i = 0;i < 20;++i)
		{
			real Y = (real)(i + 1) / 20;
			for (integer y = 0;y < Height;++y)
			{
				for (integer x = 0;x < Width;++x)
				{
					real u = (real)x / (Width - 1);
					real v = (real)y / (Height - 1);

					Color xyzColor =
						xyyToXyz(
						Color(u, v, Y));

					image(x, y) = fitColor(xyzToSrgb(xyzColor));
				}
			}

			savePcx(image, "chromaticity_" + integerToString(i, 2) + ".pcx");
		}
	}

	TEST_CASE("ChromaticAdaptation (ChromaticAdaptation)")
	{
		Color lmsObserved(
			xyzToLms(srgbToXyz(Color(142, 98, 29) / 255)));

		Color lmsDesired(
			xyzToLms(srgbToXyz(Color(1, 1, 1))));

		Matrix<real32> observedToDesired = 
			xyzToLinearSrgbTransform() * 
			lmsToXyzTransform();

		for (integer j = 0;j < 3;++j)
		{
			for (integer i = 0;i < 3;++i)
			{
				observedToDesired(i, j) *= 0.1 * lmsDesired[i] / lmsObserved[i];
			}
		}

		observedToDesired *=
			xyzToLmsTransform() * linearSrgbToXyzTransform();

		Array<Color> image;
		loadPcx("lena.pcx", image);

		transform(arrayView(image), [&](const Color& observed)
		{
			return 
				linearSrgbToSrgb(
					fitColor(
						observedToDesired * srgbToLinearSrgb(observed)
					)
				);
		});

		savePcx(image, "chromatic_adaptation.pcx");
	}

}
