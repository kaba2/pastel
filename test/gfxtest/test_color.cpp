#include "pastelgfxtest.h"

#include "pastel/gfx/pcx.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/drawing.h"

#include "pastel/sys/string_tools.h"
#include "pastel/sys/view_all.h"

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
			<< rgbLumaWeights() << std::endl;
		std::cout << std::endl;

		const integer Width = 500;
		const integer Height = 500;

		LinearArray<2, bool> image(Width, Height);
		for (integer x = 0;x < Width;++x)
		{
			const real t = (real)x / (Width - 1);
			drawPixel(Point2(x, LabDetail::labFunction(t) * Height), true, arrayView(image));
		}

		saveBinaryPcx(image, "test_color_labfunction.pcx");
	}

	void testChromaticity()
	{
		const real Width = 500;
		const real Height = 500;
		LinearArray<2, Color> image(Width, Height);

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
			
			savePcx(image, "testcolor_chromaticity_" + integerToString(i, 2) + ".pcx");
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
				linearSrgbToXyzTransform() * xyzToLmsTransform() *
				diagonal(evaluate(0.1 * lmsDesired / lmsObserved)) *
				lmsToXyzTransform() * xyzToLinearSrgbTransform();

			std::cout << transform_ << std::endl;
		}

		Color operator()(
			const Color& that) const
		{
			return linearSrgbToGammaSrgb(
				fitColor(gammaSrgbToLinearSrgb(that) * transform_));
		}

	private:
		Matrix<3, 3, real32> transform_;
	};

	void testChromaticAdaptation()
	{
		log() << "- chromatic adaptation" << logNewLine;

		LinearArray<2, Color> image;
		loadPcx("lena.pcx", image);

		const TransformVisitor transformVisitor;
		transform(arrayView(image), transformVisitor);
		
		savePcx(image, "test_color_chromatic_adaptation.pcx");
	}

	void testBegin()
	{
		testConstants();
		//testChromaticity();
		testChromaticAdaptation();
	}

	void testAdd()
	{
		gfxTestList().add("Color", testBegin);
	}

	CallFunction run(testAdd);

}
