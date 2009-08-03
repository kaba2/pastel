#include "pastelgfxtest.h"

#include "pastel/sys/extendedconstview_all.h"
#include "pastel/sys/view_all.h"

#include "pastel/gfx/pcx.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/drawing.h"
#include "pastel/gfx/imagegfxrenderer.h"
#include "pastel/gfx/gfxrenderer_tools.h"
#include "pastel/dsp/mipmap_tools.h"
#include "pastel/dsp/filter_all.h"

#include "pastel/sys/random_vector.h"

#include "pastel/gfx/colormixers.h"
#include "pastel/gfx/textures.h"

#include <boost/lambda/lambda.hpp>
using namespace boost::lambda;

using namespace Pastel;

namespace
{

	void testDistortion()
	{
		Array<Color, 2> textureImage;
		loadPcx("lena.pcx", textureImage);

		MipMap<2, Color> mipMap(constArrayView(textureImage));
		EwaImageTexture<Color> texture(mipMap);
		transform(mipMap, fitColor);

		Array<Color, 2> image(500, 100);

		distortAnnulusToAlignedBox(
			texture,
			Point2(0.5),
			0.25,
			0.5,
			0,
			2 * constantPi<real>(),
			arrayView(image),
			AlignedBox2(0, 0, 500, 100),
			assignColorMixer<Color>());

		transform(arrayView(image), fitColor);

		savePcx(image, "output/distortion.pcx");

		Image_GfxRenderer<Color> renderer;
		renderer.setImage(&textureImage);
		renderer.setFilled(false);
		renderer.setColor(Color(0, 1, 0));
		renderer.setViewWindow(
			AlignedBox2(0, 0, 1, 1));

		drawCircle(renderer, Sphere2(Point2(0.5), 0.25), 40);
		drawCircle(renderer, Sphere2(Point2(0.5), 0.5), 40);
		drawSegment(renderer, Segment2(Point2(0.6, 0.5), Point2(0.7, 0.5)));

		savePcx(textureImage, "output/distortion_texture.pcx");
	}

	void testView()
	{
		Array<Color, 2> image(500, 500, Color(0));

		clear(Color(1), subView(arrayView(image), Rectangle2(10, 20, 100, 110)));
		clear(Color(1), sparseView(subView(arrayView(image), Rectangle2(110, 120, 200, 210)), Point2i(0), Vector2i(2, 2)));

		clear(Color(0, 0, 1), rowView(arrayView(image), 0, Point2i(10, 10)));
		clear(Color(0, 0, 1), rowView(arrayView(image), 1, Point2i(10, 10)));

		copy(constSubView(constArrayView(image), Rectangle2(0, 0, 200, 200)),
			subView(arrayView(image), Rectangle2(300, 300, 500, 500)));

		savePcx(image, "output/drawing_view.pcx");
	}

	void testBinary()
	{
		Array<bool, 2> image(500, 500, false);

		clear(false, arrayView(image));

		drawBox(AlignedBox2(100, 100, 200, 200), true,
			arrayView(image));

		drawSegment(Segment2(Point2(250, 50), Point2(300, 30)),
			true, arrayView(image));

		saveBinaryPcx(image, "output/drawing_binary.pcx");
	}

	void testFloodFill()
	{
		Array<Color, 2> image(500, 500, Color(0));

		Image_GfxRenderer<Color> renderer;
		renderer.setImage(&image);
		renderer.setViewWindow(AlignedBox2(-1, -1, 1, 1));
		renderer.setFilled(false);

		renderer.setColor(randomRgbColor());
		drawCircle(renderer, Sphere2(Point2(0), 0.5), 20);

		renderer.setColor(randomRgbColor());
		drawTriangle(renderer, Triangle2(Point2(0), Point2(0, -1), Point2(0.5)));

		floodFill(0, 0, randomRgbColor(), arrayView(image));

		savePcx(image, "output/drawing_floodfill.pcx");
	}

	template <int N, typename Real>
	Point<Real, N> randomPoint(
		const AlignedBox<N, Real>& region)
	{
		return region.min() + randomVector<N, Real>() * region.extent();
	}

	void testPerspectiveTriangle()
	{
		Array<Color, 2> image(500, 500);

		Array<Color, 2> texture;
		loadPcx("lena.pcx", texture);

		log() << "Computing mipmaps.." << logNewLine;
		MipMap<2, Color> mipMap(constArrayView(texture));
		transform(mipMap, fitColor);

		MipImageTexture<Color> sampler(mipMap);

		log() << "Rendering.." << logNewLine;

		const AlignedBox3 region(
			Point3(-100, -100, 0),
			Point3(600, 600, 10));

		for (integer i = 0;i < 5;++i)
		{
			drawTriangle(
				Triangle3(
					randomPoint(region),
					randomPoint(region),
					randomPoint(region)),
				Triangle2(
					Point2(0), Point2(1, 0), Point2(0.5, 1)),
				sampler,
				arrayView(image));
		}

		savePcx(image, "output/drawing_perspectivetriangle.pcx");
	}

	void testEwaPerspectiveTriangle()
	{
		Array<Color, 2> image(500, 500);

		Array<Color, 2> textureImage;
		loadPcx("lena.pcx", textureImage);

		log() << "Rendering.." << logNewLine;

		const AlignedBox3 region(
			Point3(-100, -100, 0),
			Point3(600, 600, 10));

		MipMap<2, Color> mipMap(constArrayView(textureImage));
		EwaImageTexture<Color> texture(mipMap);
		transform(mipMap, fitColor);

		for (integer i = 0;i < 5;++i)
		{
			drawTriangle(
				Triangle3(
					randomPoint(region),
					randomPoint(region),
					randomPoint(region)),
				Triangle2(
					Point2(0), Point2(1, 0), Point2(0.5, 1)),
				texture,
				arrayView(image));
		}

		savePcx(image, "output/drawing_ewaperspectivetriangle.pcx");
	}

	void testTextureTriangle()
	{
		Array<Color, 2> image(500, 500);

		Array<Color, 2> texture;
		loadPcx("lena.pcx", texture);

		log() << "Computing mipmaps.." << logNewLine;
		MipMap<2, Color> mipMap(constArrayView(texture));
		transform(mipMap, fitColor);

		MipImageTexture<Color> sampler(mipMap);

		log() << "Rendering.." << logNewLine;

		const AlignedBox2 region(
			Point2(-100, -100),
			Point2(600, 600));

		for (integer i = 0;i < 5;++i)
		{
			drawTriangle(
				Triangle2(
					randomPoint(region),
					randomPoint(region),
					randomPoint(region)),
				Triangle2(
					Point2(0), Point2(1, 0), Point2(0.5, 1)),
				sampler,
				arrayView(image));
		}

		savePcx(image, "output/drawing_texturetriangle.pcx");
	}

	void testEwaTriangle()
	{
		Array<Color, 2> image(500, 500);

		Array<Color, 2> textureImage;
		loadPcx("lena.pcx", textureImage);

		log() << "Rendering.." << logNewLine;

		const AlignedBox2 region(
			Point2(-100, -100),
			Point2(600, 600));

		MipMap<2, Color> mipMap(constArrayView(textureImage));
		EwaImageTexture<Color> texture(mipMap);
		transform(mipMap, fitColor);

		TriangleFilter ewaFilter;
		for (integer i = 0;i < 5;++i)
		{
			drawTriangle(
				Triangle2(
					randomPoint(region),
					randomPoint(region),
					randomPoint(region)),
				Triangle2(
					Point2(0), Point2(1, 0), Point2(0.5, 1)),
				texture,
				arrayView(image));
		}

		savePcx(image, "output/drawing_ewaimagetexturetriangle.pcx");
	}

	void testSolidTriangle()
	{
		Array<Color, 2> image(500, 500);

		Array<Color, 2> texture;
		loadPcx("lena.pcx", texture);

		log() << "Rendering.." << logNewLine;

		const AlignedBox2 region(
			Point2(-100, -100),
			Point2(600, 600));

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

		savePcx(image, "output/drawing_solidtriangle.pcx");
	}


	void testBoxes()
	{
		Array<Color, 2> image(640, 480);

		Image_GfxRenderer<Color> renderer;
		renderer.setImage(&image);
		renderer.setViewWindow(AlignedBox2(0, 0, 640, 480));

		Point2 a(0, 0);
		Point2 b(33.3, 45.6);
		Point2 c(634.4, 200.1);
		Point2 d(33.3, 0);
		Point2 e(634.4, 45.6);

		renderer.setColor(randomRgbColor());
		drawBox(renderer, AlignedBox2(a, b));
		renderer.setColor(randomRgbColor());
		drawBox(renderer, AlignedBox2(b, c));
		renderer.setColor(randomRgbColor());
		drawBox(renderer, AlignedBox2(d, e));

		savePcx(image, "output/drawing_boxes.pcx");
	}

	void testMoreCircles()
	{
		Array<Color, 2> image(640, 480);

		Image_GfxRenderer<Color> renderer;
		renderer.setImage(&image);
		renderer.setViewWindow(AlignedBox2(0, 0, 640, 480));
		renderer.setColor(randomRgbColor());

		for (integer i = 0;i < 10;++i)
		{
			for (integer j = 0;j < 10;++j)
			{
				drawCircle(renderer, Sphere2(Point2(i * 64.1 + 32, j * 48 + 24), 20 + j * 0.1));
			}
		}

		savePcx(image, "output/drawing_morecircles.pcx");
	}

	void testCircles()
	{
		Array<Color, 2> image(640, 480);

		Image_GfxRenderer<Color> renderer;
		renderer.setImage(&image);
		renderer.setViewWindow(AlignedBox2(0, 0, 640, 480));

		Point2 a(320, 240);

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

		const Point2 b(55.3, 45.7);

		renderer.setColor(randomRgbColor());
		drawCircle(renderer, Sphere2(b, 20.1));
		renderer.setColor(randomRgbColor());
		drawCircle(renderer, Sphere2(b, 19.5));

		const Point2 c(-50, 480);

		renderer.setColor(randomRgbColor());
		drawCircle(renderer, Sphere2(c, 200));

		renderer.setColor(randomRgbColor());
		drawCircle(renderer, Sphere2(c, 45));

		savePcx(image, "output/drawing_circles.pcx");
	}

	void testLines()
	{
		Array<Color, 2> image(640, 480);

		const integer steps = 128;

		const Point2 center(50.5, 240.5);
		const real radius = 200;
		real alphaStep = 2 * constantPi<real>() / steps;

		for (integer i = 0;i < steps;++i)
		{
			const Point2 to(
				center.x() + std::cos(alphaStep * i) * radius,
				center.y() + std::sin(alphaStep * i) * radius);

			/*
			const Point2 to(
				std::floor(center.x() + std::cos(alphaStep * i) * radius) + 0.5,
				std::floor(center.y() + std::sin(alphaStep * i) * radius) + 0.5);
			*/

			drawSegment(Segment2(center, to), Color(1), arrayView(image));
		}

		savePcx(image, "output/drawing_lines.pcx");
	}

	void testMoreLines()
	{
		Array<Color, 2> image(640, 480);

		const integer lines = 500;

		for (integer i = 0;i < lines;++i)
		{
			const Point2 from(random<real>() * 1000 - 180, random<real>() * 740 - 180);
			const Point2 to(random<real>() * 1000 - 180, random<real>() * 740 - 180);
			drawSegment(Segment2(from, to), randomRgbColor(), arrayView(image));
		}

		savePcx(image, "output/drawing_morelines.pcx");
	}

	void testEvenMoreLines()
	{
		const integer XLines = 5;
		const integer YLines = 5;
		const integer BoxWidth = 100;
		const integer BoxHeight = 25;
		const integer Border = 10;
		const real YStart = 0.1;
		const real YEnd = 0.9;
		const real XYStep = (real)(YEnd - YStart) / (XLines - 1);
		const real YYStep = (real)(YEnd - YStart) / (YLines - 1);

		Array<Color, 2> image(XLines* BoxWidth, YLines * BoxHeight);

		for (integer y = 0;y < YLines;++y)
		{
			for (integer x = 0;x < XLines;++x)
			{
				const Point2 from(
					x * BoxWidth + Border + 0.5,
					y * BoxHeight + Border + (YStart + x * XYStep));
				const Point2 to(
					x * BoxWidth + (BoxWidth - Border) + 0.5 ,
					y * BoxHeight + (BoxHeight - Border) + (YStart + y * YYStep));

				drawSegment(Segment2(from, to), Color(1), arrayView(image));
			}
		}

		savePcx(image, "output/drawing_evenmorelines.pcx");
	}

	void testAdd()
	{
		gfxTestList().add("Drawing.Distortion", testDistortion);
		gfxTestList().add("Drawing.View", testView);
		gfxTestList().add("Drawing.Binary", testBinary);
		gfxTestList().add("Drawing.FloodFill", testFloodFill);
		gfxTestList().add("Drawing.Lines", testLines);
		gfxTestList().add("Drawing.MoreLines", testMoreLines);
		gfxTestList().add("Drawing.EvenMoreLines", testEvenMoreLines);
		gfxTestList().add("Drawing.SolidTriangle", testSolidTriangle);
		gfxTestList().add("Drawing.TextureTriangle", testTextureTriangle);
		gfxTestList().add("Drawing.EwaTriangle", testEwaTriangle);
		gfxTestList().add("Drawing.PerspectiveTriangle", testPerspectiveTriangle);
		gfxTestList().add("Drawing.EwaPerspectiveTriangle", testEwaPerspectiveTriangle);
		gfxTestList().add("Drawing.Boxes", testBoxes);
		gfxTestList().add("Drawing.Circles", testCircles);
		gfxTestList().add("Drawing.MoreCircles", testMoreCircles);
	}

	CallFunction run(testAdd);

}
