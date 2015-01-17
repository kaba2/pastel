// Description: Testing for drawing
// DocumentationOf: drawing.h

#include "test_pastelgfx.h"

#include "pastel/sys/view/extendedconstviews.h"
#include "pastel/sys/views.h"

#include "pastel/gfx/pcx.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/drawing.h"
#include "pastel/gfx/image_gfxrenderer.h"
#include "pastel/gfx/gfxrenderer_tools.h"
#include "pastel/gfx/radial_texture.h"

#include "pastel/gfx/mipmap_tools.h"
#include "pastel/gfx/filters.h"

#include "pastel/math/uniform_sampling.h"

#include "pastel/gfx/colormixers.h"
#include "pastel/gfx/textures.h"

#include <boost/lambda/lambda.hpp>
using namespace boost::lambda;

using namespace Pastel;

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

	void testTexture()
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

	void testView()
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

	void testBinary()
	{
		Array<bool, 2> image(Vector2i(500, 500), false);

		clear(false, arrayView(image));

		drawBox(AlignedBox2(100, 100, 200, 200), true,
			arrayView(image));

		drawSegment(Segment2(Vector2(250, 50), Vector2(300, 30)),
			true, arrayView(image));

		saveBinaryPcx(image, "drawing_binary.pcx");
	}

	void testFloodFill()
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

	template <typename Real, int N>
	Vector<Real, N> randomPoint(
		const AlignedBox<Real, N>& region)
	{
		return region.min() + randomVector<Real, N>() * region.extent();
	}

	void testPerspectiveTriangle()
	{
		Array<Color, 2> image(Vector2i(500, 500));

		Array<Color, 2> texture;
		loadPcx("lena.pcx", texture);

		//log() << "Computing mipmaps.." << logNewLine;
		MipMap<Color, 2> mipMap(constArrayView(texture));
		transform(mipMap, fitColor);

		MipImage_Texture<Color> sampler(mipMap);

		//log() << "Rendering.." << logNewLine;

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

	void testEwaPerspectiveTriangle()
	{
		integer superSample = 4;

		const integer width = 512 * superSample;
		const integer height = 512 * superSample;

		Array<Color, 2> image(Vector2i(width, height));

		Array<Color, 2> textureImage;
		loadPcx("lena.pcx", textureImage);

		//log() << "Rendering.." << logNewLine;

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

	void testTextureTriangle()
	{
		Array<Color, 2> image(Vector2i(500, 500));

		Array<Color, 2> texture;
		loadPcx("lena.pcx", texture);

		//log() << "Computing mipmaps.." << logNewLine;
		MipMap<Color, 2> mipMap(constArrayView(texture));
		transform(mipMap, fitColor);

		MipImage_Texture<Color> sampler(mipMap);

		//log() << "Rendering.." << logNewLine;

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

	void testEwaTriangle()
	{
		Array<Color, 2> image(Vector2i(500, 500));

		Array<Color, 2> textureImage;
		loadPcx("lena.pcx", textureImage);

		//log() << "Rendering.." << logNewLine;

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

	void testSolidTriangle()
	{
		Array<Color, 2> image(Vector2i(500, 500));

		Array<Color, 2> texture;
		loadPcx("lena.pcx", texture);

		//log() << "Rendering.." << logNewLine;

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


	void testBoxes()
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

	void testMoreCircles()
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

	void testCircles()
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

	void testLines()
	{
		Array<Color, 2> image(Vector2i(640, 480));

		integer steps = 128;

		Vector2 center(50.5, 240.5);
		real radius = 200;

		real alphaStep = 2 * constantPi<real>() / steps;

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

	void testMoreLines()
	{
		Array<Color, 2> image(Vector2i(640, 480));

		integer lines = 500;

		for (integer i = 0;i < lines;++i)
		{

			const Vector2 from(random<real>() * 1000 - 180, random<real>() * 740 - 180);
			const Vector2 to(random<real>() * 1000 - 180, random<real>() * 740 - 180);
			drawSegment(Segment2(from, to), randomRgbColor(), arrayView(image));
		}

		savePcx(image, "drawing_morelines.pcx");
	}

	void testEvenMoreLines()
	{
		integer XLines = 5;
		integer YLines = 5;
		integer BoxWidth = 100;
		integer BoxHeight = 25;
		integer Border = 10;
		real YStart = 0.1;
		real YEnd = 0.9;
		real XYStep = (real)(YEnd - YStart) / (XLines - 1);
		real YYStep = (real)(YEnd - YStart) / (YLines - 1);


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

	void addTest()
	{
		testRunner().add("Drawing.Texture", testTexture);
		testRunner().add("Drawing.View", testView);
		testRunner().add("Drawing.Binary", testBinary);
		testRunner().add("Drawing.FloodFill", testFloodFill);
		testRunner().add("Drawing.Lines", testLines);
		testRunner().add("Drawing.MoreLines", testMoreLines);
		testRunner().add("Drawing.EvenMoreLines", testEvenMoreLines);
		testRunner().add("Drawing.SolidTriangle", testSolidTriangle);
		testRunner().add("Drawing.TextureTriangle", testTextureTriangle);
		testRunner().add("Drawing.EwaTriangle", testEwaTriangle);
		testRunner().add("Drawing.PerspectiveTriangle", testPerspectiveTriangle);
		testRunner().add("Drawing.EwaPerspectiveTriangle", testEwaPerspectiveTriangle);
		testRunner().add("Drawing.Boxes", testBoxes);
		testRunner().add("Drawing.Circles", testCircles);
		testRunner().add("Drawing.MoreCircles", testMoreCircles);
	}

	CallFunction run(addTest);

}
