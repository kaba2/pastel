// Description: Low-discrepancy sequence example
// DocumentationOf: lowdiscrepancy.h

#include "pastel_example.h"

#include "pastel/sys/view/arrayview.h"
#include "pastel/sys/vector.h"

#include "pastel/math/low_discrepancy/lowdiscrepancy.h"

#include "pastel/gfx/image_file/pcx/savepcx.h"
#include "pastel/gfx/drawing.h"
#include "pastel/gfx/color/color_tools.h"

using namespace Pastel;

namespace
{

	void testHalton()
	{
		const integer Width = 500;
		const integer Height = 500;

		Array<Color, 2> image(Vector2i(Width, Height));

		Color color;
		Color aColor = Color(1, 0, 0);
		Color bColor = Color(0, 0, 1);

		for (integer i = 0;i < 100;++i)
		{
			const Vector2 point = haltonSequence<dreal>(i, Vector<integer, 2>(3, 2));

			color = linear(aColor, bColor, (dreal)i / 100000);

			drawPixel(point * Vector2(Width, Height), color, arrayView(image));
		}

		savePcx(image, "testlowdiscrepancy_halton_000100.pcx");

		for (integer i = 100;i < 1000;++i)
		{
			const Vector2 point = haltonSequence<dreal>(i, Vector<integer, 2>(3, 2));

			color = linear(aColor, bColor, (dreal)i / 100000);

			drawPixel(point * Vector2(Width, Height), color, arrayView(image));
		}

		savePcx(image, "testlowdiscrepancy_halton_001000.pcx");

		for (integer i = 1000;i < 2000;++i)
		{
			const Vector2 point = haltonSequence<dreal>(i, Vector<integer, 2>(3, 2));

			color = linear(aColor, bColor, (dreal)i / 100000);

			drawPixel(point * Vector2(Width, Height), color, arrayView(image));
		}

		savePcx(image, "testlowdiscrepancy_halton_002000.pcx");

		for (integer i = 1000;i < 10000;++i)
		{
			const Vector2 point = haltonSequence<dreal>(i, Vector<integer, 2>(3, 2));

			color = linear(aColor, bColor, (dreal)i / 100000);

			drawPixel(point * Vector2(Width, Height), color, arrayView(image));
		}

		savePcx(image, "testlowdiscrepancy_halton_010000.pcx");

		for (integer i = 10000;i < 20000;++i)
		{
			const Vector2 point = haltonSequence<dreal>(i, Vector<integer, 2>(3, 2));

			color = linear(aColor, bColor, (dreal)i / 100000);

			drawPixel(point * Vector2(Width, Height), color, arrayView(image));
		}

		savePcx(image, "testlowdiscrepancy_halton_020000.pcx");

		for (integer i = 20000;i < 100000;++i)
		{
			const Vector2 point = haltonSequence<dreal>(i, Vector<integer, 2>(3, 2));

			color = linear(aColor, bColor, (dreal)i / 100000);

			drawPixel(point * Vector2(Width, Height), color, arrayView(image));
		}

		savePcx(image, "testlowdiscrepancy_halton_100000.pcx");
	}

	void testHammersley()
	{
		const integer Width = 500;
		const integer Height = 500;

		Array<Color, 2> image(Vector2i(Width, Height));

		Color color;
		Color aColor = Color(1, 0, 0);
		Color bColor = Color(0, 0, 1);

		for (integer i = 0;i < 100;++i)
		{
			const Vector2 point = hammersleySequence<dreal>(i, Vector<integer, 1>(2), 100000);

			color = linear(aColor, bColor, (dreal)i / 100000);

			drawPixel(point * Vector2(Width, Height), color, arrayView(image));
		}

		savePcx(image, "testlowdiscrepancy_hammersley_000100.pcx");

		for (integer i = 100;i < 1000;++i)
		{
			const Vector2 point = hammersleySequence<dreal>(i, Vector<integer, 1>(2), 100000);

			color = linear(aColor, bColor, (dreal)i / 100000);

			drawPixel(point * Vector2(Width, Height), color, arrayView(image));
		}

		savePcx(image, "testlowdiscrepancy_hammersley_001000.pcx");

		for (integer i = 1000;i < 2000;++i)
		{
			const Vector2 point = hammersleySequence<dreal>(i, Vector<integer, 1>(2), 100000);

			color = linear(aColor, bColor, (dreal)i / 100000);

			drawPixel(point * Vector2(Width, Height), color, arrayView(image));
		}

		savePcx(image, "testlowdiscrepancy_hammersley_002000.pcx");

		for (integer i = 1000;i < 10000;++i)
		{
			const Vector2 point = hammersleySequence<dreal>(i, Vector<integer, 1>(2), 100000);

			color = linear(aColor, bColor, (dreal)i / 100000);

			drawPixel(point * Vector2(Width, Height), color, arrayView(image));
		}

		savePcx(image, "testlowdiscrepancy_hammersley_010000.pcx");

		for (integer i = 10000;i < 20000;++i)
		{
			const Vector2 point = hammersleySequence<dreal>(i, Vector<integer, 1>(2), 100000);

			color = linear(aColor, bColor, (dreal)i / 100000);

			drawPixel(point * Vector2(Width, Height), color, arrayView(image));
		}

		savePcx(image, "testlowdiscrepancy_hammersley_020000.pcx");

		for (integer i = 20000;i < 100000;++i)
		{
			const Vector2 point = hammersleySequence<dreal>(i, Vector<integer, 1>(2), 100000);

			color = linear(aColor, bColor, (dreal)i / 100000);

			drawPixel(point * Vector2(Width, Height), color, arrayView(image));		}

		savePcx(image, "testlowdiscrepancy_hammersley_100000.pcx");
	}

	void addTest()
	{
		testRunner().add("LowDiscrepancy.Halton", testHalton);
		testRunner().add("LowDiscrepancy.Hammersley", testHammersley);
	}

	CallFunction run(addTest);

}
