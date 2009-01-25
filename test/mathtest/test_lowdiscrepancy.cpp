#include "pastelmathtest.h"

#include "pastel/sys/arrayview.h"

#include "pastel/math/lowdiscrepancy.h"
#include "pastel/sys/point.h"

#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/drawing.h"
#include "pastel/gfx/color_tools.h"

using namespace Pastel;

namespace
{

	void testHalton()
	{
		log() << "- halton" << logNewLine;

		const integer Width = 500;
		const integer Height = 500;

		LinearArray<2, Color> image(Width, Height);

		Color color;
		Color aColor(1, 0, 0);
		Color bColor(0, 0, 1);

		for (integer i = 0;i < 100;++i)
		{
			const Point2 point = haltonSequence<real>(i, Point<2, integer>(3, 2));

			color = linear(aColor, bColor, (real)i / 100000);

			drawPixel(asPoint(asVector(point) * Vector<2, real>(Width, Height)), color, arrayView(image));
		}

		savePcx(image, "testlowdiscrepancy_halton_000100.pcx");

		for (integer i = 100;i < 1000;++i)
		{
			const Point2 point = haltonSequence<real>(i, Point<2, integer>(3, 2));

			color = linear(aColor, bColor, (real)i / 100000);

			drawPixel(asPoint(asVector(point) * Vector<2, real>(Width, Height)), color, arrayView(image));
		}

		savePcx(image, "testlowdiscrepancy_halton_001000.pcx");

		for (integer i = 1000;i < 2000;++i)
		{
			const Point2 point = haltonSequence<real>(i, Point<2, integer>(3, 2));

			color = linear(aColor, bColor, (real)i / 100000);

			drawPixel(asPoint(asVector(point) * Vector<2, real>(Width, Height)), color, arrayView(image));
		}

		savePcx(image, "testlowdiscrepancy_halton_002000.pcx");

		for (integer i = 1000;i < 10000;++i)
		{
			const Point2 point = haltonSequence<real>(i, Point<2, integer>(3, 2));

			color = linear(aColor, bColor, (real)i / 100000);

			drawPixel(asPoint(asVector(point) * Vector<2, real>(Width, Height)), color, arrayView(image));
		}

		savePcx(image, "testlowdiscrepancy_halton_010000.pcx");

		for (integer i = 10000;i < 20000;++i)
		{
			const Point2 point = haltonSequence<real>(i, Point<2, integer>(3, 2));

			color = linear(aColor, bColor, (real)i / 100000);

			drawPixel(asPoint(asVector(point) * Vector<2, real>(Width, Height)), color, arrayView(image));
		}

		savePcx(image, "testlowdiscrepancy_halton_020000.pcx");

		for (integer i = 20000;i < 100000;++i)
		{
			const Point2 point = haltonSequence<real>(i, Point<2, integer>(3, 2));

			color = linear(aColor, bColor, (real)i / 100000);

			drawPixel(asPoint(asVector(point) * Vector<2, real>(Width, Height)), color, arrayView(image));
		}

		savePcx(image, "testlowdiscrepancy_halton_100000.pcx");
	}

	void testHammersley()
	{
		log() << "- hammersley" << logNewLine;

		const integer Width = 500;
		const integer Height = 500;

		LinearArray<2, Color> image(Width, Height);

		Color color;
		Color aColor(1, 0, 0);
		Color bColor(0, 0, 1);

		for (integer i = 0;i < 100;++i)
		{
			const Point2 point = hammersleySequence<real>(i, Point<1, integer>(2), 100000);

			color = linear(aColor, bColor, (real)i / 100000);

			drawPixel(asPoint(asVector(point) * Vector<2, real>(Width, Height)), color, arrayView(image));
		}

		savePcx(image, "testlowdiscrepancy_hammersley_000100.pcx");

		for (integer i = 100;i < 1000;++i)
		{
			const Point2 point = hammersleySequence<real>(i, Point<1, integer>(2), 100000);

			color = linear(aColor, bColor, (real)i / 100000);

			drawPixel(asPoint(asVector(point) * Vector<2, real>(Width, Height)), color, arrayView(image));
		}

		savePcx(image, "testlowdiscrepancy_hammersley_001000.pcx");

		for (integer i = 1000;i < 2000;++i)
		{
			const Point2 point = hammersleySequence<real>(i, Point<1, integer>(2), 100000);

			color = linear(aColor, bColor, (real)i / 100000);

			drawPixel(asPoint(asVector(point) * Vector<2, real>(Width, Height)), color, arrayView(image));
		}

		savePcx(image, "testlowdiscrepancy_hammersley_002000.pcx");

		for (integer i = 1000;i < 10000;++i)
		{
			const Point2 point = hammersleySequence<real>(i, Point<1, integer>(2), 100000);

			color = linear(aColor, bColor, (real)i / 100000);

			drawPixel(asPoint(asVector(point) * Vector<2, real>(Width, Height)), color, arrayView(image));
		}

		savePcx(image, "testlowdiscrepancy_hammersley_010000.pcx");

		for (integer i = 10000;i < 20000;++i)
		{
			const Point2 point = hammersleySequence<real>(i, Point<1, integer>(2), 100000);

			color = linear(aColor, bColor, (real)i / 100000);

			drawPixel(asPoint(asVector(point) * Vector<2, real>(Width, Height)), color, arrayView(image));
		}

		savePcx(image, "testlowdiscrepancy_hammersley_020000.pcx");

		for (integer i = 20000;i < 100000;++i)
		{
			const Point2 point = hammersleySequence<real>(i, Point<1, integer>(2), 100000);

			color = linear(aColor, bColor, (real)i / 100000);

			drawPixel(asPoint(asVector(point) * Vector<2, real>(Width, Height)), color, arrayView(image));
		}

		savePcx(image, "testlowdiscrepancy_hammersley_100000.pcx");
	}

	void testBegin()
	{
		testHalton();
		testHammersley();
	}

	void testAdd()
	{
		mathTestList().add("LowDiscrepancy", testBegin);
	}

	CallFunction run(testAdd);

}
