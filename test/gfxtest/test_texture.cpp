#include "pastelgfxtest.h"

#include "pastel/sys/array.h"
#include "pastel/sys/view_visit.h"
#include "pastel/sys/extendedconstview_all.h"
#include "pastel/sys/view_tools.h"
#include "pastel/sys/indexextender_all.h"

#include "pastel/gfx/colormixers.h"
#include "pastel/gfx/textures.h"
#include "pastel/gfx/drawing.h"
#include "pastel/gfx/pcx.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/dsp/mipmap_tools.h"
#include "pastel/dsp/ripmap_tools.h"
#include "pastel/dsp/filter_all.h"

#include "pastel/sys/string_tools.h"
#include "pastel/sys/keyvalue.h"

#include "pastel/geometry/planar_projection.h"

using namespace Pastel;

#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

using namespace boost::lambda;

#include <iostream>

using namespace std;

namespace
{

	void testEwaMagnification()
	{
		integer Width = 400;
		integer Height = 400;

		Array<Color, 2> textureImage;

		loadPcx("lena.pcx", textureImage);

		MipMap<2, Color> mipMap(constArrayView(textureImage),
			ArrayExtender<2, Color>(clampExtender()));
		transform(mipMap, fitColor);

		Array<Color, 2> image(Width * 2, Height * 2);

		const AlignedBox2 textureBox(0.4, 0.45, 0.55, 0.60);

		const AlignedBox2 box(0, 0, Width, Height);

		{
			NearestImageTexture<Color> texture(textureImage,
				ArrayExtender<2, Color>(clampExtender()));

			drawTexturedBox(
				box + Vector2(0, Height),
				texture,
				arrayView(image),
				textureBox);
		}
		{
			BilinearImageTexture<Color> texture(textureImage);

			drawTexturedBox(
				box + Vector2(Width, Height),
				texture,
				arrayView(image),
				textureBox);
		}
		{
			EwaImageTexture<Color> texture(mipMap,
				ArrayExtender<2, Color>(clampExtender()),
				gaussianFilter(1));

			drawTexturedBox(
				box + Vector2(0, 0),
				texture,
				arrayView(image),
				textureBox);
		}
		{
			EwaImageTexture<Color> texture(mipMap,
				ArrayExtender<2, Color>(clampExtender()),
				lanczosFilter(2));

			drawTexturedBox(
				box + Vector2(Width, 0),
				texture,
				arrayView(image),
				textureBox);
		}

		transform(arrayView(image), fitColor);

		savePcx(image, "output/texture_ewa_magnification.pcx");
	}

	void testEwaMagnification2()
	{
		integer Width = 400;
		integer Height = 400;

		Array<Color, 2> textureImage(40, 40);

		const integer CheckerWidth = 20;
		const integer CheckerHeight = 20;

		for (integer y = 0;y < textureImage.height();++y)
		{
			for (integer x = 0;x < textureImage.width();++x)
			{
				const integer xMod = mod(x, 2 * CheckerWidth);
				const integer yMod = mod(y, 2 * CheckerHeight);

				if ((xMod < CheckerWidth) ^
					(yMod >= CheckerHeight))
				{
					textureImage(x, y) = Color(1);
				}
				else
				{
					textureImage(x, y) = Color(0);
				}
			}
		}

		/*
		Array<Color, 2> textureImage;

		loadPcx("texture_checker.pcx", textureImage);
		//loadPcx("lena.pcx", textureImage);
		*/

		MipMap<2, Color> mipMap(constArrayView(textureImage),
			ArrayExtender<2, Color>(clampExtender()));
		transform(mipMap, fitColor);

		Array<Color, 2> image(Width, Height);

		std::vector<Texture<Color>::Ptr> textureList;
		textureList.push_back(
			Texture<Color>::Ptr(new EwaImageTexture<Color>(mipMap,
				ArrayExtender<2, Color>(clampExtender()),
				gaussianFilter(1))));
		textureList.push_back(
			Texture<Color>::Ptr(new EwaImageTexture<Color>(mipMap,
				ArrayExtender<2, Color>(clampExtender()),
				gaussianFilter(2))));
		textureList.push_back(
			Texture<Color>::Ptr(new EwaImageTexture<Color>(mipMap,
				ArrayExtender<2, Color>(clampExtender()),
				lanczosFilter(2))));
		textureList.push_back(
			Texture<Color>::Ptr(new BilinearImageTexture<Color>(textureImage,
				ArrayExtender<2, Color>(clampExtender()))));
		textureList.push_back(
			Texture<Color>::Ptr(new NearestImageTexture<Color>(textureImage,
				ArrayExtender<2, Color>(clampExtender()))));

		const AlignedBox2 textureBox(0.4, 0.4, 0.6, 0.6);
		const AlignedBox2 box(0, 0, Width, Height);

		integer textures = textureList.size();
		for (integer i = 0;i < textures;++i)
		{
			drawTexturedBox(
				box + Vector2(0, 0),
				*textureList[i],
				arrayView(image),
				textureBox);

			transform(arrayView(image), fitColor);
			savePcx(image, "output/texture_checker3_" + textureList[i]->name() +
				"_" + integerToString(i) + ".pcx");
		}
	}

	void testSamplers()
	{
		Array<Color, 2> image(750, 500);
		Array<Color, 2> textureImage;

		loadPcx("lena.pcx", textureImage);

		MipMap<2, Color> mipMap(constArrayView(textureImage),
			ArrayExtender<2, Color>(clampExtender()));
		transform(mipMap, fitColor);

		MipImageTexture<Color> mipmapSampler(mipMap);

		drawTexturedBox(
			AlignedBox2(0, 400, 100, 500),
			bilinearColorTexture<Color>(Color(1, 1, 1), Color(1, 0, 0), Color(0, 1, 0), Color(0, 0, 1)),
			arrayView(image));

		drawTexturedBox(
			AlignedBox2(150, 400, 250, 500),
			constantColorTexture<Color>(Color(0.5, 0, 1)),
			arrayView(image));

		// Comparison of filtering techniques.

		drawTexturedBox(
			AlignedBox2(0, 250, 100, 350),
			nearestImageTexture(textureImage),
			arrayView(image));

		drawTexturedBox(
			AlignedBox2(150, 250, 250, 350),
			mipmapSampler,
			arrayView(image));

		EwaImageTexture<Color> texture(mipMap);

		drawTexturedBox(
			AlignedBox2(450, 250, 550, 350),
			texture,
			arrayView(image));

		// Something else.

		/*
		drawTexturedBox(
			AlignedBox2(0, 0, 200, 200),
			nearestImageTexture(clampedConstView(constSubView(constArrayView(textureImage), Rectangle2(100, 100, 150, 150)))),
			arrayView(image));

		drawTexturedBox(
			AlignedBox2(250, 0, 450, 200),
			bilinearImageTexture(clampedConstView(constSubView(constArrayView(textureImage), Rectangle2(100, 100, 150, 150)))),
			arrayView(image));
		*/

		drawTexturedBox(
			AlignedBox2(100, 100, 300, 225),
			mipmapSampler,
			arrayView(image),
			AlignedBox2(0, 0, 1, 1),
			transparentColorMixer<Color>(0.25));

		drawView(
			constSubView(constArrayView(textureImage), Rectangle2(200, 200, 300, 300)),
			Point2i(300, 400),
			arrayView(image));

		savePcx(image, "output/textures_texturebox.pcx");

		clear(Color(0), arrayView(image));

		drawTexturedBox(
			AlignedBox2(0, 0, 500, 500),
			nearestImageTexture(textureImage, ArrayExtender<2, Color>(clampExtender())),
			arrayView(image),
			AlignedBox2(0.5, 0.5, 5.0, 5.0));

		savePcx(image, "output/textures_clampedtexturebox.pcx");

		drawTexturedBox(
			AlignedBox2(0, 0, 500, 500),
			nearestImageTexture(textureImage, ArrayExtender<2, Color>(repeatExtender())),
			arrayView(image),
			AlignedBox2(0.5, 0.5, 5.0, 5.0));

		savePcx(image, "output/textures_repeatedtexturebox.pcx");

		drawTexturedBox(
			AlignedBox2(0, 0, 500, 500),
			nearestImageTexture(textureImage, ArrayExtender<2, Color>(mirrorExtender())),
			arrayView(image),
			AlignedBox2(0.5, 0.5, 5.0, 5.0));

		savePcx(image, "output/textures_mirroredtexturebox.pcx");

		drawTexturedBox(
			AlignedBox2(0, 0, 500, 500),
			nearestImageTexture(textureImage),
			arrayView(image),
			AlignedBox2(0.5, 0.5, 5.0, 5.0));

		savePcx(image, "output/textures_borderedtexturebox.pcx");
	}

	void drawTestQuad(
		const Tuple<Point2, 4>& quad,
		const Tuple<Point2, 4>& textureQuad,
		const Texture<Color>& texture,
		const std::string& testName,
		const std::string& name)
	{
		real yMax = -infinity<real>();
		for (integer i = 0;i < 4;++i)
		{
			if (quad[i].y() > yMax)
			{
				yMax = quad[i].y();
			}
		}

		Array<Color, 2> image(500, ceil(yMax));

		drawProjectiveQuad(
			quad,
			texture,
			arrayView(image),
			textureQuad);

		savePcx(image, "output/texture_" + testName + "_" + name + ".pcx");
	}

	void testChecker()
	{
		Array<Color, 2> image(500, 500);
		Array<Color, 2> textureImage(40, 40);

		const integer CheckerWidth = 20;
		const integer CheckerHeight = 20;

		for (integer y = 0;y < textureImage.height();++y)
		{
			for (integer x = 0;x < textureImage.width();++x)
			{
				const integer xMod = mod(x, 2 * CheckerWidth);
				const integer yMod = mod(y, 2 * CheckerHeight);

				if ((xMod < CheckerWidth) ^
					(yMod >= CheckerHeight))
				{
					textureImage(x, y) = Color(1);
				}
				else
				{
					textureImage(x, y) = Color(0);
				}
			}
		}

		savePcx(textureImage, "texture_checker.pcx");

		MipMap<2, Color> mipMap(constArrayView(textureImage),
			ArrayExtender<2, Color>(repeatExtender()), boxFilter());
		transform(mipMap, fitColor);

		RipMap<2, Color> ripMap(constArrayView(textureImage));
		transform(ripMap, fitColor);

		Array<Color, 2> lenaTextureImage;
		loadPcx("lena.pcx", lenaTextureImage);

		MipMap<2, Color> lenaMipMap(constArrayView(lenaTextureImage));
		transform(lenaMipMap, fitColor);

		RipMap<2, Color> lenaRipMap(constArrayView(lenaTextureImage));
		transform(lenaRipMap, fitColor);

		ConstIndexExtenderPtr extender = repeatExtender();

		EwaImageTexture<Color> textureEwaGaussian1(
			mipMap, ArrayExtender<2, Color>(extender), gaussianFilter(1));
		EwaImageTexture<Color> textureEwaGaussian2(
			mipMap, ArrayExtender<2, Color>(extender), gaussianFilter(2));
		EwaImageTexture<Color> textureEwaLanczos2(
			mipMap, ArrayExtender<2, Color>(extender), lanczosFilter(2));
		EwaImageTexture<Color> textureEwaMitchell(
			mipMap, ArrayExtender<2, Color>(extender), mitchellFilter());
		EwaImageTexture<Color> textureEwaBox(
			mipMap, ArrayExtender<2, Color>(extender), boxFilter());
		EwaImageTexture<Color> textureEwaTriangle(
			mipMap, ArrayExtender<2, Color>(extender), triangleFilter());
		MipImageTexture<Color> textureMip(mipMap);
		RipImageTexture<Color> textureRip(ripMap);

		std::vector<KeyValue<std::string, Texture<Color>*> > textureList;

		textureList.push_back(keyValue(std::string("ewa_gaussian1"), (Texture<Color>*)&textureEwaGaussian1));
		textureList.push_back(keyValue(std::string("ewa_gaussian2"), (Texture<Color>*)&textureEwaGaussian2));
		textureList.push_back(keyValue(std::string("ewa_lanczos2"), (Texture<Color>*)&textureEwaLanczos2));
		textureList.push_back(keyValue(std::string("ewa_mitchell"), (Texture<Color>*)&textureEwaMitchell));
		textureList.push_back(keyValue(std::string("ewa_box"), (Texture<Color>*)&textureEwaBox));
		textureList.push_back(keyValue(std::string("ewa_triangle"), (Texture<Color>*)&textureEwaTriangle));
		textureList.push_back(keyValue(std::string("mip"), (Texture<Color>*)&textureMip));
		textureList.push_back(keyValue(std::string("rip"), (Texture<Color>*)&textureRip));

		const integer textures = textureList.size();

		{
			Tuple<Point2, 4> quad(
				Point2(250, 0),
				Point2(500, 150),
				Point2(250, 175),
				Point2(0, 150));

			Tuple<Point2, 4> textureQuad(
				Point2(0, 0),
				Point2(2, 0),
				Point2(2, 2),
				Point2(0, 2));

			for (integer i = 0;i < textures;++i)
			{
				drawTestQuad(quad, textureQuad, *textureList[i].value(),
					"checker1", textureList[i].key());
			}
		}

		/*
		MipMap<2, Color> distortMipMap(constArrayView(image));
		EwaImageTexture<Color> distortTexture(distortMipMap);
		transform(distortMipMap, fitColor);

		clear(Color(0), arrayView(image));

		drawProjectiveQuad(
			Tuple<Point2, 4>(
			Point2(0, 0),
			Point2(500, 0),
			Point2(500, 500),
			Point2(0, 500)),
			distortTexture,
			arrayView(image),
			Tuple<Point2, 4>(
			Point2(0.5, 0),
			Point2(1, (real)150 / 500),
			Point2(0.5, (real)175 / 500),
			Point2(0, (real)150 / 500)));

		savePcx(image, "output/texture_checker1_distorted2x.pcx");
		*/

		mipMap.swap(lenaMipMap);
		ripMap.swap(lenaRipMap);

		for (integer i = 0;i < textures;++i)
		{
			Tuple<Point2, 4> quad(
				Point2(250, 0),
				Point2(500, 150),
				Point2(250, 175),
				Point2(0, 150));

			Tuple<Point2, 4> textureQuad(
				Point2(0, 0),
				Point2(1, 0),
				Point2(1, 1),
				Point2(0, 1));

			drawTestQuad(quad, textureQuad, *textureList[i].value(),
				"lena1", textureList[i].key());
		}

		/*
			MipMap<2, Color> distortMipMap(constArrayView(image));
			EwaImageTexture<Color> distortTexture(distortMipMap);
			transform(distortMipMap, fitColor);

			clear(Color(0), arrayView(image));

			drawProjectiveQuad(
				Tuple<Point2, 4>(
				Point2(0, 0),
				Point2(500, 0),
				Point2(500, 500),
				Point2(0, 500)),
				distortTexture,
				arrayView(image),
				Tuple<Point2, 4>(
				Point2(0.5, 0),
				Point2(1, (real)150 / 500),
				Point2(0.5, (real)175 / 500),
				Point2(0, (real)150 / 500)));

			savePcx(image, "output/texture_lena1_ewa_distorted2x.pcx");
		*/

		mipMap.swap(lenaMipMap);
		ripMap.swap(lenaRipMap);

		{
			Tuple<Point2, 4> quad(
				Point2(0, 0),
				Point2(500, 0),
				Point2(255, 150),
				Point2(245, 150));

			Tuple<Point2, 4> textureQuad(
				Point2(0, 0),
				Point2(20, 0),
				Point2(20, 800),
				Point2(0, 800));

			for (integer i = 0;i < textures;++i)
			{
				drawTestQuad(quad, textureQuad, *textureList[i].value(),
					"checker2", textureList[i].key());
			}
		}

		/*
		{
			clear(Color(0), arrayView(image));

			Tuple<Point2, 4> quad(
				Point2(0, 0),
				Point2(500, 0),
				Point2(300, 250),
				Point2(200, 250));

			Tuple<Point2, 4> textureQuad(
				Point2(0, 0),
				Point2(10, 0),
				Point2(10, 100),
				Point2(0, 100));

			clear(Color(0), arrayView(image));

			EwaImageTexture<Color> texture(mipMap);

			drawProjectiveQuad(
				quad,
				mixTexture(
				constantColorTexture<Color>(Color(0)),
				transformTexture(
				mixTexture(
				bilinearImageTexture(repeatedConstView(constArrayView(lenaTextureImage))),
				constantColorTexture<Color>(Color(1)),
				bilinearColorTexture<real>(0, 0, 1, 1)),
				AffineTransformation2(Matrix2() * 10, Vector2())),
				texture),
				arrayView(image),
				textureQuad);

			savePcx(image, "output/texture_texture_mix.pcx");
		}
		*/
	}

	void testMix()
	{
		Array<Color, 2> aTexture;
		loadPcx("lena.pcx", aTexture);

		Array<Color, 2> bTexture;
		loadPcx("kodak_test_images/kodim19.pcx", bTexture);

		Array<Color, 2> image(750, 750, Color(0));

		MipMap<2, Color> aMipMap(constArrayView(aTexture));
		transform(aMipMap, fitColor);

		MipMap<2, Color> bMipMap(constArrayView(bTexture));
		transform(bMipMap, fitColor);

		const integer iconSize = 250;
		const integer iconSpace = iconSize;

		const AlignedBox2 iconBox(0, 0, iconSize, iconSize);

		drawTexturedBox(
			iconBox + Vector2(0, 0),
			combineTexture(
			mipImageTexture(aMipMap),
			mipImageTexture(bMipMap),
			transparentColorMixer<Color>(0.5)),
			arrayView(image));

		drawTexturedBox(
			iconBox + Vector2(iconSpace, 0),
			combineTexture(
			mipImageTexture(aMipMap),
			mipImageTexture(bMipMap),
			functorColorMixer<Color>(ret<Color>(_1 - _2))),
			arrayView(image));

		drawTexturedBox(
			iconBox + Vector2(iconSpace, iconSpace),
			combineTexture(
			mipImageTexture(aMipMap),
			mipImageTexture(bMipMap),
			functorColorMixer<Color>(ret<Color>(_1 + _2))),
			arrayView(image));

		drawTexturedBox(
			iconBox + Vector2(0, iconSpace),
			combineTexture(
			mipImageTexture(aMipMap),
			mipImageTexture(bMipMap),
			functorColorMixer<Color>(ret<Color>(_1 * _2))),
			arrayView(image));

		drawTexturedBox(
			iconBox + Vector2(0, iconSpace * 2),
			mipImageTexture(aMipMap),
			arrayView(image));

		drawTexturedBox(
			iconBox + Vector2(iconSpace * 2, 0),
			mipImageTexture(bMipMap),
			arrayView(image));

		savePcx(image, "output/texture_mix.pcx");
	}

	void testBegin()
	{
		//testMix();
		testSamplers();
		testChecker();
		testEwaMagnification();
		testEwaMagnification2();
	}

	void testAdd()
	{
		gfxTestList().add("Texture", testBegin);
	}

	CallFunction run(testAdd);

}
