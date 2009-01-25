#include "pastelgfxtest.h"

#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/loadpcx.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/imagegfxrenderer.h"
#include "pastel/gfx/gfxrenderer_tools.h"
#include "pastel/gfx/erode.h"
#include "pastel/gfx/dilate.h"
#include "pastel/gfx/structuringelement.h"

#include "pastel/sys/arrayview.h"
#include "pastel/sys/borderedconstview.h"

using namespace Pastel;

namespace
{

	void testStructuringElementLarge()
	{
		const integer width = 200;
		const integer height = 100;

		LinearArray<2, bool> image(width, height, false);

		ellipsoidElement(arrayView(image), Vector2(93.734));

		saveBinaryPcx(image, "test_structuringelement_ellipsoid.pcx");

		ellipsoidElement(arrayView(image), Vector2(93.123));

		saveBinaryPcx(image, "test_structuringelement_ellipsoid2.pcx");

		diamondElement(arrayView(image), Vector2(93.734));

		saveBinaryPcx(image, "test_structuringelement_diamond.pcx");

		diamondElement(arrayView(image), Vector2(93.123));

		saveBinaryPcx(image, "test_structuringelement_diamond2.pcx");

		pEllipsoidElement(arrayView(image), 1.5, Vector2(93.734));

		saveBinaryPcx(image, "test_structuringelement_pellipsoid.pcx");

		pEllipsoidElement(arrayView(image), 1.5, Vector2(93.123));

		saveBinaryPcx(image, "test_structuringelement_pellipsoid2.pcx");

		LinearArray<3, bool> volumeImage(100, 200, 10, false);
		pEllipsoidElement(arrayView(volumeImage), 1.5, Vector3(53.23, 93.734, 9.5));
	}

	void testStructuringElementSmall()
	{
		const integer width = 5;
		const integer height = 5;

		LinearArray<2, bool> image(width, height, false);

		ellipsoidElement(arrayView(image), Vector2(1));

		saveBinaryPcx(image, "test_structuringelement_small_ellipsoid.pcx");

		ellipsoidElement(arrayView(image), Vector2(2));

		saveBinaryPcx(image, "test_structuringelement_small_ellipsoid2.pcx");

		ellipsoidElement(arrayView(image), Vector2(3));

		saveBinaryPcx(image, "test_structuringelement_small_ellipsoid3.pcx");

		ellipsoidElement(arrayView(image), Vector2(4));

		saveBinaryPcx(image, "test_structuringelement_small_ellipsoid4.pcx");

		ellipsoidElement(arrayView(image), Vector2(5));

		saveBinaryPcx(image, "test_structuringelement_small_ellipsoid5.pcx");



		diamondElement(arrayView(image), Vector2(1));

		saveBinaryPcx(image, "test_structuringelement_small_diamond.pcx");

		diamondElement(arrayView(image), Vector2(2));

		saveBinaryPcx(image, "test_structuringelement_small_diamond2.pcx");

		diamondElement(arrayView(image), Vector2(3));

		saveBinaryPcx(image, "test_structuringelement_small_diamond3.pcx");

		diamondElement(arrayView(image), Vector2(4));

		saveBinaryPcx(image, "test_structuringelement_small_diamond4.pcx");

		diamondElement(arrayView(image), Vector2(5));

		saveBinaryPcx(image, "test_structuringelement_small_diamond5.pcx");



		pEllipsoidElement(arrayView(image), 1.5, Vector2(1));

		saveBinaryPcx(image, "test_structuringelement_small_pEllipsoid.pcx");

		pEllipsoidElement(arrayView(image), 1.5, Vector2(2));

		saveBinaryPcx(image, "test_structuringelement_small_pEllipsoid2.pcx");

		pEllipsoidElement(arrayView(image), 1.5, Vector2(3));

		saveBinaryPcx(image, "test_structuringelement_small_pEllipsoid3.pcx");

		pEllipsoidElement(arrayView(image), 1.5, Vector2(4));

		saveBinaryPcx(image, "test_structuringelement_small_pEllipsoid4.pcx");

		pEllipsoidElement(arrayView(image), 1.5, Vector2(5));

		saveBinaryPcx(image, "test_structuringelement_small_pEllipsoid5.pcx");
	}

	void testErode()
	{
		const integer width = 500;
		const integer height = 500;

		LinearArray<2, bool> image(width, height, false);
		
		ImageGfxRenderer<bool> renderer(&image);

		renderer.setColor(true);
		renderer.setFilled(true);

		drawCircle(renderer, Sphere2(Point2(0), 0.5), 50);

		LinearArray<2, bool> element(31, 21, false);
		
		ImageGfxRenderer<bool> elementRenderer(&element);
		
		elementRenderer.setColor(true);
		elementRenderer.setFilled(true);

		drawTriangle(elementRenderer, Triangle2(Point2(-1, -1), Point2(0, 1), Point2(1, -1)));

		saveBinaryPcx(image, "testerode_input.pcx");
		saveBinaryPcx(element, "testerode_element.pcx");

		LinearArray<2, bool> output(width, height, false);

		erode(borderedConstView(constArrayView(image), false), constArrayView(element), arrayView(output));

		saveBinaryPcx(output, "testerode_output.pcx");

		for (integer y = 0;y < 500;++y)
		{
			for (integer x = 0;x < 500;++x)
			{
				image(x, y) = image(x,y) && !output(x, y);
			}
		}

		saveBinaryPcx(image, "testerode_difference.pcx");

		dilate(constArrayView(output), constArrayView(element), 15, 10, arrayView(image));

		saveBinaryPcx(image, "testdilate_output.pcx");
	}

	void testBegin()
	{
		testStructuringElementLarge();
		testStructuringElementSmall();
		testErode();
	}

	void testAdd()
	{
		gfxTestList().add("Erode", testBegin);
	}

	CallFunction run(testAdd);

}
