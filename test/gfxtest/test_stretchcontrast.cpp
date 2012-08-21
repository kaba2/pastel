#include "test_pastelgfx.h"

#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/color_space.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/pcx.h"

#include "pastel/sys/string_algorithms.h"
#include "pastel/sys/views.h"

#include <iostream>

using namespace Pastel;

namespace
{

	class LightnessVisitor
	{
	public:
		LightnessVisitor(
			real& min)
			: min_(min)
		{
		}

		void operator()(
			const Color& color) const
		{
			if (color[0] < min_)
			{
				min_ = color[0];
			}
		}

	private:
		real& min_;
	};

	class ShiftVisitor
	{
	public:
		ShiftVisitor(
			real min)
			: min_(min)
		{
		}

		void operator()(
			Color& color) const
		{
			if (color[0] != 0)
			{
				color *= (1 - min_ / color[0]);
			}
		}

	private:
		real min_;
	};

	class MaxVisitor
	{
	public:
		MaxVisitor(
			real& max)
			: max_(max)
		{
		}

		void operator()(
			const Color& color) const
		{
			const real maxRgb = max(xyzToSrgb(labToXyz(color)));
			if (maxRgb > max_)
			{
				max_ = maxRgb;
			}
		}

	private:
		real& max_;
	};

	class ScaleVisitor
	{
	public:
		ScaleVisitor(
			real scale)
			: scale_(scale)
		{
		}

		void operator()(
			Color& color) const
		{
			color *= scale_;
		}

	private:
		real scale_;
	};

	void test()
	{
		Array<Color, 2> image;
		loadPcx("lena.pcx", image);

		transform(arrayView(image), srgbToXyz);
		transform(arrayView(image), xyzToLab);

		real minLightness = infinity<real>();
		LightnessVisitor lightnessVisitor(
			minLightness);
		visit(constArrayView(image), lightnessVisitor);

		ShiftVisitor shiftVisitor(
			minLightness);
		visit(arrayView(image), shiftVisitor);

		real minScale = 1;
		real maxScale = 100;

		Array<Color, 2> copyImage(image);

		while(true)
		{
			copy(constArrayView(copyImage), arrayView(image));

			const real scale = (minScale + maxScale) / 2;

			log() << "Trying scale " << scale << logNewLine;

			ScaleVisitor scaleVisitor(scale);
			visit(arrayView(image), scaleVisitor);

			real maxRgb = -infinity<real>();
			MaxVisitor maxVisitor(
				maxRgb);
			visit(constArrayView(image), maxVisitor);

			log() << "Max rgb " << maxRgb << logNewLine;

			if (mabs(maxRgb - 1) < 0.01)
			{
				break;
			}

			if (maxRgb > 1)
			{
				maxScale = scale;
			}
			else
			{
				minScale = scale;
			}
		}

		transform(arrayView(image), labToXyz);
		transform(arrayView(image), xyzToSrgb);
		transform(arrayView(image), fitColor);

		savePcx(image, "output/stretch_contrast.pcx");
	}

	void addTest()
	{
		testRunner().add("StretchContrast", test);
	}

	CallFunction run(addTest);

}
