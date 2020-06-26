// Description: Testing for drawing
// DocumentationOf: drawing.h

#include "test/test_init.h"

#include "pastel/sys/view/extendedconstviews.h"
#include "pastel/sys/view.h"

#include "pastel/gfx/image_file.h"
#include "pastel/gfx/color.h"
#include "pastel/gfx/drawing.h"
#include "pastel/gfx/image_gfxrenderer.h"
#include "pastel/gfx/gfxrenderer_tools.h"
#include "pastel/gfx/texture.h"
#include "pastel/gfx/mipmap.h"
#include "pastel/gfx/filter.h"

#include "pastel/math/sampling/uniform_sampling.h"

#include <boost/lambda/lambda.hpp>
using namespace boost::lambda;

namespace
{

	void testTextureCase(const Texture<Color>& texture)
	{
		integer width = 512;
		integer height = 512;
		Array<Color> image(Vector2i(width, height));

		drawBox(
			AlignedBox2(0, 0, width, height),
			texture,
			arrayView(image));

		savePcx(image, "texture_" + texture.fullName() + ".pcx");
	}

}

TEST_CASE("Texture (Texture)")
{
	Color colorSet[] = 
	{
		Color(1, 0, 0),
		Color(0, 1, 0),
		Color(0, 0, 1),
		Color(1, 1, 1)
	};

	LinearColor_Texture<Color> smooth =
		linearColorTexture<Color, 2>(
		range(colorSet));

	testTextureCase(
		mixTexture(
		subTexture(smooth, AlignedBox2(-1, -1, 2, 2)),
		colorTexture<Color, 2>(Color(0)), 
		subTexture(noiseTexture<2>(), 
		AlignedBox2(0, 0, 0.2, 0.2))));

	testTextureCase(
		mixTexture(
		colorTexture<Color, 2>(Color(0)),
		subTexture(smooth, AlignedBox2(-1, -1, 2, 2)),
		subTexture(turbulenceTexture<2>(),
		AlignedBox2(0, 0, 0.1, 0.1))));
}

TEST_CASE("View (View)")
{
	Array<Color, 2> image(Vector2i(500, 500), Color(0));

	clear(Color(1), subView(arrayView(image), AlignedBox2i(10, 20, 100, 110)));
	clear(Color(1), sparseView(subView(arrayView(image), AlignedBox2i(110, 120, 200, 210)), Vector2i(0), Vector2i(2, 2)));

	clear(Color(0, 0, 1), rowView(arrayView(image), 0, Vector2i(10, 10)));
	clear(Color(0, 0, 1), rowView(arrayView(image), 1, Vector2i(10, 10)));

	copy(constSubView(constArrayView(image), AlignedBox2i(0, 0, 200, 200)),
		subView(arrayView(image), AlignedBox2i(300, 300, 500, 500)));

	savePcx(image, "drawing_view.pcx");
}

TEST_CASE("Binary (Binary)")
{
	Array<bool, 2> image(Vector2i(500, 500), false);

	clear(false, arrayView(image));

	drawBox(AlignedBox2(100, 100, 200, 200), true,
		arrayView(image));

	drawSegment(Segment2(Vector2(250, 50), Vector2(300, 30)),
		true, arrayView(image));

	saveBinaryPcx(image, "drawing_binary.pcx");
}

TEST_CASE("FloodFill (FloodFill)")
{
	Array<Color, 2> image(Vector2i(500, 500), Color(0));

	Image_GfxRenderer<Color> renderer;

	renderer.setImage(&image);
	renderer.setViewWindow(AlignedBox2(-1, -1, 1, 1));
	renderer.setFilled(false);

	renderer.setColor(randomRgbColor());
	drawCircle(renderer, Sphere2(Vector2(0), 0.5), 20);

	renderer.setColor(randomRgbColor());
	drawTriangle(renderer, Triangle2(Vector2(0), Vector2(0, -1), Vector2(0.5)));

	floodFill(0, 0, randomRgbColor(), arrayView(image));

	savePcx(image, "drawing_floodfill.pcx");
}

namespace
{

	template <typename Real, integer N>
	Vector<Real, N> randomPoint(
		const AlignedBox<Real, N>& region)
	{
		return region.min() + randomVector<Real, N>() * region.extent();
	}

}

TEST_CASE("PerspectiveTriangle (PerspectiveTriangle)")
{
	Array<Color, 2> image(Vector2i(500, 500));

	Array<Color, 2> texture;
	loadPcx("lena.pcx", texture);

	//std::cout << "Computing mipmaps.." << std::endl;
	MipMap<Color, 2> mipMap(constArrayView(texture));
	transform(mipMap, fitColor);

	MipImage_Texture<Color> sampler(mipMap);

	//std::cout << "Rendering.." << std::endl;

	AlignedBox3 region(
		Vector3(-100, -100, 0),
		Vector3(600, 600, 10));

	for (integer i = 0;i < 5;++i)
	{
		drawTriangle(
			Triangle3(
				randomPoint(region),
				randomPoint(region),
				randomPoint(region)),
			Triangle2(
				Vector2(0), Vector2(1, 0), Vector2(0.5, 1)),
			sampler,
			arrayView(image));
	}

	savePcx(image, "drawing_perspectivetriangle.pcx");
}

TEST_CASE("EwaPerspectiveTriangle (EwaPerspectiveTriangle)")
{
	integer superSample = 4;

	const integer width = 512 * superSample;
	const integer height = 512 * superSample;

	Array<Color, 2> image(Vector2i(width, height));

	Array<Color, 2> textureImage;
	loadPcx("lena.pcx", textureImage);

	//std::cout << "Rendering.." << std::endl;

	AlignedBox3 region(
		Vector3(-width / 4, -height / 4, 0),

		Vector3(width * 1.25, height * 1.25, 10));

	MipMap<Color, 2> mipMap(constArrayView(textureImage));
	EwaImage_Texture<Color> texture(mipMap);
	transform(mipMap, fitColor);

	for (integer i = 0;i < 5;++i)
	{
		drawTriangle(
			Triangle3(
				randomPoint(region),
				randomPoint(region),
				randomPoint(region)),
			Triangle2(
				Vector2(0), Vector2(1, 0), Vector2(0.5, 1)),
			texture,
			arrayView(image));
	}

	Array<Color> smallImage(Vector2i(width / superSample, height / superSample));
	resample<Color>(
		constArrayView(image),
		ArrayExtender<2, Color>(clampExtender()),
		lanczosFilter(2),
		arrayView(smallImage));
	transform(arrayView(smallImage), fitColor);

	savePcx(smallImage, "drawing_ewaperspectivetriangle.pcx");
}

TEST_CASE("TextureTriangle (TextureTriangle)")
{
	Array<Color, 2> image(Vector2i(500, 500));

	Array<Color, 2> texture;
	loadPcx("lena.pcx", texture);

	//std::cout << "Computing mipmaps.." << std::endl;
	MipMap<Color, 2> mipMap(constArrayView(texture));
	transform(mipMap, fitColor);

	MipImage_Texture<Color> sampler(mipMap);

	//std::cout << "Rendering.." << std::endl;

	AlignedBox2 region(
		Vector2(-100, -100),
		Vector2(600, 600));

	for (integer i = 0;i < 5;++i)
	{
		drawTriangle(
			Triangle2(
				randomPoint(region),
				randomPoint(region),
				randomPoint(region)),
			Triangle2(
				Vector2(0), Vector2(1, 0), Vector2(0.5, 1)),
			sampler,
			arrayView(image));
	}

	savePcx(image, "drawing_texturetriangle.pcx");
}

TEST_CASE("EwaTriangle (EwaTriangle)")
{
	Array<Color, 2> image(Vector2i(500, 500));

	Array<Color, 2> textureImage;
	loadPcx("lena.pcx", textureImage);

	//std::cout << "Rendering.." << std::endl;

	AlignedBox2 region(
		Vector2(-100, -100),
		Vector2(600, 600));

	MipMap<Color, 2> mipMap(constArrayView(textureImage));
	EwaImage_Texture<Color> texture(mipMap);
	transform(mipMap, fitColor);

	Triangle_Filter ewaFilter;
	for (integer i = 0;i < 5;++i)
	{
		drawTriangle(
			Triangle2(
				randomPoint(region),
				randomPoint(region),
				randomPoint(region)),
			Triangle2(
				Vector2(0), Vector2(1, 0), Vector2(0.5, 1)),
			texture,
			arrayView(image));
	}

	savePcx(image, "drawing_ewaimagetexturetriangle.pcx");
}

TEST_CASE("SolidTriangle (SolidTriangle)")
{
	Array<Color, 2> image(Vector2i(500, 500));

	Array<Color, 2> texture;
	loadPcx("lena.pcx", texture);

	//std::cout << "Rendering.." << std::endl;

	AlignedBox2 region(
		Vector2(-100, -100),
		Vector2(600, 600));

	for (integer i = 0;i < 5;++i)
	{
		drawTriangle(
			Triangle2(
				randomPoint(region),
				randomPoint(region),
				randomPoint(region)),
			randomRgbColor(),
			arrayView(image),
			transparentColorMixer<Color>(0.75));
	}

	savePcx(image, "drawing_solidtriangle.pcx");
}

TEST_CASE("Boxes (Boxes)")
{
	Array<Color, 2> image(Vector2i(640, 480));

	Image_GfxRenderer<Color> renderer;

	renderer.setImage(&image);
	renderer.setViewWindow(AlignedBox2(0, 0, 640, 480));

	Vector2 a(0, 0);
	Vector2 b(33.3, 45.6);
	Vector2 c(634.4, 200.1);
	Vector2 d(33.3, 0);
	Vector2 e(634.4, 45.6);

	renderer.setColor(randomRgbColor());
	drawBox(renderer, AlignedBox2(a, b));
	renderer.setColor(randomRgbColor());
	drawBox(renderer, AlignedBox2(b, c));
	renderer.setColor(randomRgbColor());
	drawBox(renderer, AlignedBox2(d, e));

	savePcx(image, "drawing_boxes.pcx");
}

TEST_CASE("MoreCircles (MoreCircles)")
{
	Array<Color, 2> image(Vector2i(640, 480));

	Image_GfxRenderer<Color> renderer;
	renderer.setImage(&image);
	renderer.setViewWindow(AlignedBox2(0, 0, 640, 480));
	renderer.setColor(randomRgbColor());

	for (integer i = 0;i < 10;++i)
	{
		for (integer j = 0;j < 10;++j)
		{
			drawCircle(renderer, Sphere2(Vector2(i * 64.1 + 32, j * 48 + 24), 20 + j * 0.1));
		}
	}

	savePcx(image, "drawing_morecircles.pcx");
}

TEST_CASE("Circles (Circles)")
{
	Array<Color, 2> image(Vector2i(640, 480));

	Image_GfxRenderer<Color> renderer;
	renderer.setImage(&image);
	renderer.setViewWindow(AlignedBox2(0, 0, 640, 480));

	Vector2 a(320, 240);

	renderer.setColor(randomRgbColor());
	drawCircle(renderer, Sphere2(a, 100));
	renderer.setColor(randomRgbColor());
	drawCircle(renderer, Sphere2(a, 99));
	renderer.setColor(randomRgbColor());
	drawCircle(renderer, Sphere2(a, 98));
	renderer.setColor(randomRgbColor());
	drawCircle(renderer, Sphere2(a, 96));
	renderer.setColor(randomRgbColor());
	drawCircle(renderer, Sphere2(a, 94));
	renderer.setColor(randomRgbColor());
	drawCircle(renderer, Sphere2(a, 91));
	renderer.setColor(randomRgbColor());
	drawCircle(renderer, Sphere2(a, 88));
	renderer.setColor(randomRgbColor());
	drawCircle(renderer, Sphere2(a, 84));
	renderer.setColor(randomRgbColor());
	drawCircle(renderer, Sphere2(a, 80));
	renderer.setColor(randomRgbColor());
	drawCircle(renderer, Sphere2(a, 75));
	renderer.setColor(randomRgbColor());
	drawCircle(renderer, Sphere2(a, 70));
	renderer.setColor(randomRgbColor());
	drawCircle(renderer, Sphere2(a, 64));
	renderer.setColor(randomRgbColor());
	drawCircle(renderer, Sphere2(a, 58));
	renderer.setColor(randomRgbColor());
	drawCircle(renderer, Sphere2(a, 51));
	renderer.setColor(randomRgbColor());
	drawCircle(renderer, Sphere2(a, 44));
	renderer.setColor(randomRgbColor());
	drawCircle(renderer, Sphere2(a, 36));
	renderer.setColor(randomRgbColor());
	drawCircle(renderer, Sphere2(a, 28));
	renderer.setColor(randomRgbColor());
	drawCircle(renderer, Sphere2(a, 19));
	renderer.setColor(randomRgbColor());
	drawCircle(renderer, Sphere2(a, 10));

	Vector2 b(55.3, 45.7);

	renderer.setColor(randomRgbColor());
	drawCircle(renderer, Sphere2(b, 20.1));
	renderer.setColor(randomRgbColor());
	drawCircle(renderer, Sphere2(b, 19.5));

	Vector2 c(-50, 480);

	renderer.setColor(randomRgbColor());
	drawCircle(renderer, Sphere2(c, 200));

	renderer.setColor(randomRgbColor());
	drawCircle(renderer, Sphere2(c, 45));

	savePcx(image, "drawing_circles.pcx");
}

TEST_CASE("Lines (Lines)")
{
	Array<Color, 2> image(Vector2i(640, 480));

	integer steps = 128;

	Vector2 center(50.5, 240.5);
	dreal radius = 200;

	dreal alphaStep = 2 * constantPi<dreal>() / steps;

	for (integer i = 0;i < steps;++i)
	{
		Vector2 to(
			center.x() + std::cos(alphaStep * i) * radius,
			center.y() + std::sin(alphaStep * i) * radius);

		/*
		Vector2 to(
			std::floor(center.x() + std::cos(alphaStep * i) * radius) + 0.5,
			std::floor(center.y() + std::sin(alphaStep * i) * radius) + 0.5);
		*/

		drawSegment(Segment2(center, to), Color(1), arrayView(image));
	}

	savePcx(image, "drawing_lines.pcx");
}

TEST_CASE("MoreLines (MoreLines)")
{
	Array<Color, 2> image(Vector2i(640, 480));

	integer lines = 500;

	for (integer i = 0;i < lines;++i)
	{

		const Vector2 from(random<dreal>() * 1000 - 180, random<dreal>() * 740 - 180);
		const Vector2 to(random<dreal>() * 1000 - 180, random<dreal>() * 740 - 180);
		drawSegment(Segment2(from, to), randomRgbColor(), arrayView(image));
	}

	savePcx(image, "drawing_morelines.pcx");
}

TEST_CASE("EvenMoreLines (EvenMoreLines)")
{
	integer XLines = 5;
	integer YLines = 5;
	integer BoxWidth = 100;
	integer BoxHeight = 25;
	integer Border = 10;
	dreal YStart = 0.1;
	dreal YEnd = 0.9;
	dreal XYStep = (dreal)(YEnd - YStart) / (XLines - 1);
	dreal YYStep = (dreal)(YEnd - YStart) / (YLines - 1);

	Array<Color, 2> image(Vector2i(XLines* BoxWidth, YLines * BoxHeight));

	for (integer y = 0;y < YLines;++y)
	{
		for (integer x = 0;x < XLines;++x)
		{
			Vector2 from(
				x * BoxWidth + Border + 0.5,
				y * BoxHeight + Border + (YStart + x * XYStep));
			Vector2 to(
				x * BoxWidth + (BoxWidth - Border) + 0.5 ,
				y * BoxHeight + (BoxHeight - Border) + (YStart + y * YYStep));

			drawSegment(Segment2(from, to), Color(1), arrayView(image));
		}
	}

	savePcx(image, "drawing_evenmorelines.pcx");
}
