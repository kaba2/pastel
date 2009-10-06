#include "pastelgfxtest.h"

#include "pastel/sys/extendedconstview_all.h"
#include "pastel/sys/view_all.h"

#include "pastel/gfx/pcx.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/draw.h"
#include "pastel/gfx/image_gfxrenderer.h"
#include "pastel/gfx/gfxrenderer_tools.h"
#include "pastel/gfx/radial_texture.h"

#include "pastel/dsp/mipmap_tools.h"
#include "pastel/dsp/filter_all.h"

#include "pastel/math/uniform_sampling.h"

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
		//loadPcx("circle_text.pcx", textureImage);
		loadPcx("lena.pcx", textureImage);

		MipMap<Color, 2> mipMap(constArrayView(textureImage));
		EwaImage_Texture<Color> texture(mipMap, ArrayExtender<2, Color>(mirrorExtender()));
		//NearestImage_Texture<Color> texture(textureImage, ArrayExtender<2, Color>(mirrorExtender()));
		transform(mipMap, fitColor);

		Array<Color, 2> image(800, 256);

		/*
		drawTexturedAnnulus(
			texture,
			Vector2(0.5),
			0.25,
			0.5,
			0,
			2 * constantPi<real>(),
			arrayView(image),
			AlignedBox2(0, 0, 500, 100),
			assignColorMixer<Color>());
		*/

		const real minRadius = 0.25;
		const real maxRadius = 0.5;

		Radial_Texture<Color> distortedTexture =
			radialTexture(texture, Vector2(0.5),
			Vector2(minRadius, 2 * constantPi<real>()), Vector2(maxRadius, 0));

		/*
		Noise_Texture<> staticTexture = noiseTexture<2>();
		
		Tuple<Color, 4> colorSquare(
			Color(1, 0, 0),
			Color(0, 1, 0),
			Color(0, 0, 1),
			Color(1, 1, 1));

		LinearColor_Texture<Color> colorTexture = linearColorTexture<Color, 2>(colorSquare);

		Mix_Texture<Color, real> mixTexture(
			distortedTexture,
			colorTexture,
			staticTexture);
		*/

		drawBox(
			AlignedBox2(0, 0, image.width(), image.height()),
			distortedTexture,
			arrayView(image));

		transform(arrayView(image), fitColor);

		savePcx(image, "output/distortion.pcx");

		Image_GfxRenderer<Color> renderer;
		renderer.setImage(&textureImage);
		renderer.setFilled(false);
		renderer.setColor(Color(0, 1, 0));
		renderer.setViewWindow(
			AlignedBox2(0, 0, 1, 1));

		drawCircle(renderer, Sphere2(Vector2(0.5), minRadius), 40);
		drawCircle(renderer, Sphere2(Vector2(0.5), maxRadius), 40);
		drawSegment(renderer, Segment2(Vector2(0.5 + minRadius, 0.5), Vector2(0.5 + maxRadius, 0.5)));

		savePcx(textureImage, "output/distortion_texture.pcx");
	}

	void testTextureCase(const Texture<Color>& texture)
	{
		const integer width = 512;
		const integer height = 512;
		Array<Color> image(width, height);

		drawBox(
			AlignedBox2(0, 0, width, height),
			texture,
			arrayView(image));

		savePcx(image, "output/texture_" + texture.fullName() + ".pcx");
	}

	void testTexture()
	{
		LinearColor_Texture<Color> smooth =
			linearColorTexture<Color, 2>(
			makeTuple(
			Color(1, 0, 0),
			Color(0, 1, 0),
			Color(0, 0, 1),
			Color(1, 1, 1)));

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
		Array<Color, 2> image(500, 500, Color(0));

		clear(Color(1), subView(arrayView(image), Rectangle2(10, 20, 100, 110)));
		clear(Color(1), sparseView(subView(arrayView(image), Rectangle2(110, 120, 200, 210)), Vector2i(0), Vector2i(2, 2)));

		clear(Color(0, 0, 1), rowView(arrayView(image), 0, Vector2i(10, 10)));
		clear(Color(0, 0, 1), rowView(arrayView(image), 1, Vector2i(10, 10)));

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

		drawSegment(Segment2(Vector2(250, 50), Vector2(300, 30)),
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
		drawCircle(renderer, Sphere2(Vector2(0), 0.5), 20);

		renderer.setColor(randomRgbColor());
		drawTriangle(renderer, Triangle2(Vector2(0), Vector2(0, -1), Vector2(0.5)));

		floodFill(0, 0, randomRgbColor(), arrayView(image));

		savePcx(image, "output/drawing_floodfill.pcx");
	}

	template <typename Real, int N>
	Vector<Real, N> randomPoint(
		const AlignedBox<Real, N>& region)
	{
		return region.min() + randomVector<Real, N>() * region.extent();
	}

	void testPerspectiveTriangle()
	{
		Array<Color, 2> image(500, 500);

		Array<Color, 2> texture;
		loadPcx("lena.pcx", texture);

		log() << "Computing mipmaps.." << logNewLine;
		MipMap<Color, 2> mipMap(constArrayView(texture));
		transform(mipMap, fitColor);

		MipImage_Texture<Color> sampler(mipMap);

		log() << "Rendering.." << logNewLine;

		const AlignedBox3 region(
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

		savePcx(image, "output/drawing_perspectivetriangle.pcx");
	}

	void testEwaPerspectiveTriangle()
	{
		const integer superSample = 4;
		const integer width = 512 * superSample;
		const integer height = 512 * superSample;

		Array<Color, 2> image(width, height);

		Array<Color, 2> textureImage;
		loadPcx("lena.pcx", textureImage);

		log() << "Rendering.." << logNewLine;

		const AlignedBox3 region(
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

		Array<Color> smallImage(width / superSample, height / superSample);
		resample<Color>(
			constArrayView(image),
			ArrayExtender<2, Color>(clampExtender()),
			lanczosFilter(2),
			arrayView(smallImage));
		transform(arrayView(smallImage), fitColor);

		savePcx(smallImage, "output/drawing_ewaperspectivetriangle.pcx");
	}

	void testTextureTriangle()
	{
		Array<Color, 2> image(500, 500);

		Array<Color, 2> texture;
		loadPcx("lena.pcx", texture);

		log() << "Computing mipmaps.." << logNewLine;
		MipMap<Color, 2> mipMap(constArrayView(texture));
		transform(mipMap, fitColor);

		MipImage_Texture<Color> sampler(mipMap);

		log() << "Rendering.." << logNewLine;

		const AlignedBox2 region(
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

		savePcx(image, "output/drawing_texturetriangle.pcx");
	}

	void testEwaTriangle()
	{
		Array<Color, 2> image(500, 500);

		Array<Color, 2> textureImage;
		loadPcx("lena.pcx", textureImage);

		log() << "Rendering.." << logNewLine;

		const AlignedBox2 region(
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

		savePcx(image, "output/drawing_ewaimagetexturetriangle.pcx");
	}

	void testSolidTriangle()
	{
		Array<Color, 2> image(500, 500);

		Array<Color, 2> texture;
		loadPcx("lena.pcx", texture);

		log() << "Rendering.." << logNewLine;

		const AlignedBox2 region(
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

		savePcx(image, "output/drawing_solidtriangle.pcx");
	}


	void testBoxes()
	{
		Array<Color, 2> image(640, 480);

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
				drawCircle(renderer, Sphere2(Vector2(i * 64.1 + 32, j * 48 + 24), 20 + j * 0.1));
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

		const Vector2 b(55.3, 45.7);

		renderer.setColor(randomRgbColor());
		drawCircle(renderer, Sphere2(b, 20.1));
		renderer.setColor(randomRgbColor());
		drawCircle(renderer, Sphere2(b, 19.5));

		const Vector2 c(-50, 480);

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

		const Vector2 center(50.5, 240.5);
		const real radius = 200;
		real alphaStep = 2 * constantPi<real>() / steps;

		for (integer i = 0;i < steps;++i)
		{
			const Vector2 to(
				center.x() + std::cos(alphaStep * i) * radius,
				center.y() + std::sin(alphaStep * i) * radius);

			/*
			const Vector2 to(
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
			const Vector2 from(random<real>() * 1000 - 180, random<real>() * 740 - 180);
			const Vector2 to(random<real>() * 1000 - 180, random<real>() * 740 - 180);
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
				const Vector2 from(
					x * BoxWidth + Border + 0.5,
					y * BoxHeight + Border + (YStart + x * XYStep));
				const Vector2 to(
					x * BoxWidth + (BoxWidth - Border) + 0.5 ,
					y * BoxHeight + (BoxHeight - Border) + (YStart + y * YYStep));

				drawSegment(Segment2(from, to), Color(1), arrayView(image));
			}
		}

		savePcx(image, "output/drawing_evenmorelines.pcx");
	}

	void testAdd()
	{
		gfxTestList().add("Drawing.Texture", testTexture);
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
