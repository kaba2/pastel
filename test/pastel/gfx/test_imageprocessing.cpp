// Description: Testing for image processing
// DocumentationOf: image_eroding.h

#include "test/test_init.h"

#include "pastel/gfx/image_file.h"
#include "pastel/gfx/color.h"
#include "pastel/gfx/image_gfxrenderer.h"
#include "pastel/gfx/gfxrenderer_tools.h"
#include "pastel/gfx/image_processing.h"

#include "pastel/sys/view.h"

TEST_CASE("StructuringElementLarge (StructuringElementLarge)")
{
	integer width = 200;
	integer height = 100;

	Array<bool, 2> image(Vector2i(width, height), false);

	ellipsoidElement(arrayView(image), Vector2(93.734));

	saveBinaryPcx(image, "structuring_element_ellipsoid.pcx");

	ellipsoidElement(arrayView(image), Vector2(93.123));

	saveBinaryPcx(image, "structuring_element_ellipsoid2.pcx");

	diamondElement(arrayView(image), Vector2(93.734));

	saveBinaryPcx(image, "structuring_element_diamond.pcx");

	diamondElement(arrayView(image), Vector2(93.123));

	saveBinaryPcx(image, "structuring_element_diamond2.pcx");

	pEllipsoidElement(arrayView(image), 1.5, Vector2(93.734));

	saveBinaryPcx(image, "structuring_element_pellipsoid.pcx");

	pEllipsoidElement(arrayView(image), 1.5, Vector2(93.123));

	saveBinaryPcx(image, "structuring_element_pellipsoid2.pcx");

	Array<bool, 3> volumeImage(Vector3i(100, 200, 10), false);
	pEllipsoidElement(arrayView(volumeImage), 1.5, Vector3(53.23, 93.734, 9.5));
}

TEST_CASE("StructuringElementSmall (StructuringElementSmall)")
{
	integer width = 5;
	integer height = 5;

	Array<bool, 2> image(Vector2i(width, height), false);

	ellipsoidElement(arrayView(image), Vector2(1));

	saveBinaryPcx(image, "structuring_element_small_ellipsoid.pcx");

	ellipsoidElement(arrayView(image), Vector2(2));

	saveBinaryPcx(image, "structuring_element_small_ellipsoid2.pcx");

	ellipsoidElement(arrayView(image), Vector2(3));

	saveBinaryPcx(image, "structuring_element_small_ellipsoid3.pcx");

	ellipsoidElement(arrayView(image), Vector2(4));

	saveBinaryPcx(image, "structuring_element_small_ellipsoid4.pcx");

	ellipsoidElement(arrayView(image), Vector2(5));

	saveBinaryPcx(image, "structuring_element_small_ellipsoid5.pcx");

	diamondElement(arrayView(image), Vector2(1));

	saveBinaryPcx(image, "structuring_element_small_diamond.pcx");

	diamondElement(arrayView(image), Vector2(2));

	saveBinaryPcx(image, "structuring_element_small_diamond2.pcx");

	diamondElement(arrayView(image), Vector2(3));

	saveBinaryPcx(image, "structuring_element_small_diamond3.pcx");

	diamondElement(arrayView(image), Vector2(4));

	saveBinaryPcx(image, "structuring_element_small_diamond4.pcx");

	diamondElement(arrayView(image), Vector2(5));

	saveBinaryPcx(image, "structuring_element_small_diamond5.pcx");

	pEllipsoidElement(arrayView(image), 1.5, Vector2(1));

	saveBinaryPcx(image, "structuring_element_small_pEllipsoid.pcx");

	pEllipsoidElement(arrayView(image), 1.5, Vector2(2));

	saveBinaryPcx(image, "structuring_element_small_pEllipsoid2.pcx");

	pEllipsoidElement(arrayView(image), 1.5, Vector2(3));

	saveBinaryPcx(image, "structuring_element_small_pEllipsoid3.pcx");

	pEllipsoidElement(arrayView(image), 1.5, Vector2(4));

	saveBinaryPcx(image, "structuring_element_small_pEllipsoid4.pcx");

	pEllipsoidElement(arrayView(image), 1.5, Vector2(5));

	saveBinaryPcx(image, "structuring_element_small_pEllipsoid5.pcx");
}

TEST_CASE("Erode (Erode)")
{
	integer width = 500;
	integer height = 500;

	Array<bool, 2> image(Vector2i(width, height), false);

	Image_GfxRenderer<bool> renderer(&image);

	renderer.setColor(true);
	renderer.setFilled(true);

	drawCircle(renderer, Sphere2(Vector2(0), 0.5), 50);

	Array<bool, 2> element(Vector2i(31, 21), false);

	Image_GfxRenderer<bool> elementRenderer(&element);

	elementRenderer.setColor(true);
	elementRenderer.setFilled(true);

	drawTriangle(elementRenderer, Triangle2(Vector2(-1, -1), Vector2(0, 1), Vector2(1, -1)));

	saveBinaryPcx(image, "erode_input.pcx");
	saveBinaryPcx(element, "erode_element.pcx");

	Array<bool, 2> output(Vector2i(width, height), false);

	erode(borderedConstView(constArrayView(image), false), constArrayView(element), arrayView(output));

	saveBinaryPcx(output, "erode_output.pcx");

	for (integer y = 0;y < 500;++y)
	{
		for (integer x = 0;x < 500;++x)
		{
			image(x, y) = image(x,y) && !output(x, y);
		}
	}

	saveBinaryPcx(image, "erode_difference.pcx");

	dilate(constArrayView(output), constArrayView(element), 15, 10, arrayView(image));

	saveBinaryPcx(image, "dilate_output.pcx");
}

