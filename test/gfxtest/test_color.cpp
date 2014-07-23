// Description: Testing for Color
// DocumentationOf: color.h

#include "test_pastelgfx.h"

#include "pastel/gfx/pcx.h"
#include "pastel/gfx/color_space.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/color_illuminant.h"
#include "pastel/gfx/drawing.h"

#include "pastel/sys/string_algorithms.h"
#include "pastel/sys/views.h"

#include <iostream>

using namespace Pastel;

namespace
{

	void testChromaticity()
	{
		real Width = 500;
		const real Height = 500;
		Array<Color, 2> image(Vector2i(Width, Height));

		for (integer i = 0;i < 20;++i)
		{
			const real Y = (real)(i + 1) / 20;
			for (integer y = 0;y < Height;++y)
			{
				for (integer x = 0;x < Width;++x)
				{
					real u = (real)x / (Width - 1);
					real v = (real)y / (Height - 1);

					const Color xyzColor =
						xyyToXyz(
						Color(u, v, Y));

					image(x, y) = fitColor(xyzToSrgb(xyzColor));
				}
			}

			savePcx(image, "chromaticity_" + integerToString(i, 2) + ".pcx");
		}
	}

	class TransformVisitor
	{
	public:
		TransformVisitor()
			: transform_(3, 3)
		{
			const Color lmsObserved(
				xyzToLms(srgbToXyz(Color(142, 98, 29) / 255)));

			const Color lmsDesired(
				xyzToLms(srgbToXyz(Color(1, 1, 1))));

			transform_ = 

				 xyzToLinearSrgbTransform() * lmsToXyzTransform();

			for (integer j = 0;j < 3;++j)
			{
				for (integer i = 0;i < 3;++i)
				{
					transform_(i, j) *= 0.1 * lmsDesired[i] / lmsObserved[i];
				}
			}

			transform_ *=
				xyzToLmsTransform() * linearSrgbToXyzTransform();
		}

		Color operator()(
			const Color& that) const
		{
			return linearSrgbToSrgb(
				fitColor(transform_ * srgbToLinearSrgb(that)));
		}

	private:
		Matrix<real32> transform_;
	};

	void testChromaticAdaptation()
	{
		Array<Color> image;
		loadPcx("lena.pcx", image);

		TransformVisitor transformVisitor;
		transform(arrayView(image), transformVisitor);

		savePcx(image, "chromatic_adaptation.pcx");
	}

	void addTest()
	{
		testRunner().add("Color.ChromaticAdaptation", testChromaticAdaptation);
	}

	CallFunction run(addTest);

}
