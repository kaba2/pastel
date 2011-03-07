// Description: Testing for Color
// DocumentationOf: color.h

#include "pastelgfxtest.h"

#include "pastel/gfx/pcx.h"
#include "pastel/gfx/color_space.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/color_illuminant.h"
#include "pastel/gfx/draw.h"

#include "pastel/sys/string_algorithms.h"
#include "pastel/sys/views.h"

#include <iostream>

using namespace Pastel;

namespace
{

	void testConstants()
	{
		std::cout << "Linear sRGB to XYZ transformation: " << std::endl
			<< linearSrgbToXyzTransform() << std::endl;
		std::cout << std::endl;
		std::cout << "XYZ to linear sRGB transformation: " << std::endl
			<< inverse(linearSrgbToXyzTransform()) << std::endl;

		std::cout << "CIE illuminant E XYZ coordinates: " << std::endl
			<< xyzIlluminantE() << std::endl;
		std::cout << std::endl;

		std::cout << "CIE illuminant D50 XYZ coordinates: " << std::endl
			<< xyzIlluminantD50() << std::endl;
		std::cout << std::endl;

		std::cout << "CIE illuminant D65 XYZ coordinates: " << std::endl
			<< xyzIlluminantD65() << std::endl;
		std::cout << std::endl;

		std::cout << "Rgb-to-luma weights: " << std::endl
			<< lumaWeights() << std::endl;
		std::cout << std::endl;

		/*
		const integer Width = 500;
		const integer Height = 500;

		Array<bool, 2> image(Width, Height);
		for (integer x = 0;x < Width;++x)
		{
			const real t = (real)x / (Width - 1);
			drawPixel(Vector2(x, LabDetail::labFunction(t) * Height), true, arrayView(image));
		}

		saveBinaryPcx(image, "output/lab_function.pcx");
		*/
	}

	void testChromaticity()
	{
		const real Width = 500;
		const real Height = 500;
		Array<Color, 2> image(Width, Height);

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

			savePcx(image, "output/chromaticity_" + integerToString(i, 2) + ".pcx");
		}
	}

	class TransformVisitor
	{
	public:
		TransformVisitor()
			: transform_()
		{
			const Color lmsObserved(
				xyzToLms(srgbToXyz(Color(142, 98, 29) / 255)));

			const Color lmsDesired(
				xyzToLms(srgbToXyz(Color(1, 1, 1))));

			transform_ =
				linearSrgbToXyzTransform() * xyzToLmsTransform();

			for (integer j = 0;j < 3;++j)
			{
				for (integer i = 0;i < 3;++i)
				{
					transform_(i, j) *= 0.1 * lmsDesired[j] / lmsObserved[j];
				}
			}

			transform_ *= 
				lmsToXyzTransform() * xyzToLinearSrgbTransform();

			std::cout << transform_ << std::endl;
		}

		Color operator()(
			const Color& that) const
		{
			return linearSrgbToSrgb(
				fitColor(srgbToLinearSrgb(that) * transform_));
		}

	private:
		Matrix<real32, 3, 3> transform_;
	};

	void testChromaticAdaptation()
	{
		Array<Color, 2> image;
		loadPcx("lena.pcx", image);

		const TransformVisitor transformVisitor;
		transform(arrayView(image), transformVisitor);

		savePcx(image, "output/chromatic_adaptation.pcx");
	}

	void addTest()
	{
		gfxTestList().add("Color.Constants", testConstants);
		gfxTestList().add("Color.ChromaticAdaptation", testChromaticAdaptation);
	}

	CallFunction run(addTest);

}
