// Description: Testing for textures
// DocumentationOf: textures.h

#include "test/test_init.h"

#include <test/pastel/gfx/test_pastelgfx.h>

#include "pastel/sys/array.h"
#include "pastel/sys/view.h"
#include "pastel/sys/extender/indexextenders.h"

#include "pastel/gfx/color.h"
#include "pastel/gfx/texture.h"
#include "pastel/gfx/drawing.h"
#include "pastel/gfx/image_file.h"
#include "pastel/gfx/mipmap.h"
#include "pastel/gfx/ripmap.h"
#include "pastel/gfx/filter.h"

#include "pastel/sys/string.h"

#include "pastel/geometry/planar_projection.h"

#include <boost/bind.hpp>

#include <memory>
#include <iostream>

TEST_CASE("EwaMagnification (Texture)")
{
	integer Width = 400;
	integer Height = 400;

	auto textureImagePtr = gfxStorage().get<std::shared_ptr<Array<Color>>>("lena_image");
	ENSURE(textureImagePtr);
	Array<Color>& textureImage = *textureImagePtr;

	MipMap<Color, 2> mipMap(constArrayView(textureImage),
		ArrayExtender<2, Color>(clampExtender()));
	transform(mipMap, fitColor);

	Array<Color, 2> image(Vector2i(Width * 2, Height * 2));

	AlignedBox2 textureBox(0.4, 0.45, 0.55, 0.60);

	AlignedBox2 box(0, 0, Width, Height);

	{
		NearestImage_Texture<Color> texture(textureImage,
			ArrayExtender<2, Color>(clampExtender()));

		drawTexturedBox(
			box + Vector2(0, Height),
			texture,
			arrayView(image),
			textureBox);
	}
	{
		LinearImage_Texture<Color> texture(textureImage);

		drawTexturedBox(
			box + Vector2(Width, Height),
			texture,
			arrayView(image),
			textureBox);
	}
	{
		EwaImage_Texture<Color> texture(mipMap,
			ArrayExtender<2, Color>(clampExtender()),
			gaussianFilter(1));

		drawTexturedBox(
			box + Vector2(0, 0),
			texture,
			arrayView(image),
			textureBox);
	}
	{
		EwaImage_Texture<Color> texture(mipMap,
			ArrayExtender<2, Color>(clampExtender()),
			lanczosFilter(2));

		drawTexturedBox(
			box + Vector2(Width, 0),
			texture,
			arrayView(image),
			textureBox);
	}

	transform(arrayView(image), fitColor);

	savePcx(image, "texture_ewa_magnification.pcx");
}

TEST_CASE("EwaMagnification2 (Texture)")
{
	integer Width = 400;
	integer Height = 400;

	Array<Color, 2> textureImage(Vector2i(40, 40));

	integer CheckerWidth = 20;
	integer CheckerHeight = 20;

	for (integer y = 0;y < textureImage.height();++y)
	{
		for (integer x = 0;x < textureImage.width();++x)
		{

			const integer xMod = mod(x, 2 * CheckerWidth);
			const integer yMod = mod(y, 2 * CheckerHeight);

			if ((xMod < CheckerWidth) !=
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

	MipMap<Color, 2> mipMap(constArrayView(textureImage),
		ArrayExtender<2, Color>(clampExtender()));
	transform(mipMap, fitColor);

	Array<Color, 2> image(Vector2i(Width, Height));

	using TexturePtr = std::shared_ptr<Texture<Color>>;

	std::vector<TexturePtr> textureList;
	textureList.push_back(
		TexturePtr(new EwaImage_Texture<Color>(mipMap,
			ArrayExtender<2, Color>(clampExtender()),
			gaussianFilter(1))));
	textureList.push_back(
		TexturePtr(new EwaImage_Texture<Color>(mipMap,
			ArrayExtender<2, Color>(clampExtender()),
			gaussianFilter(2))));
	textureList.push_back(
		TexturePtr(new EwaImage_Texture<Color>(mipMap,
			ArrayExtender<2, Color>(clampExtender()),
			lanczosFilter(2))));
	textureList.push_back(
		TexturePtr(new EwaImage_Texture<Color>(mipMap,
			ArrayExtender<2, Color>(clampExtender()),
			boxFilter())));
	textureList.push_back(
		TexturePtr(new LinearImage_Texture<Color>(textureImage,
			ArrayExtender<2, Color>(clampExtender()))));
	textureList.push_back(
		TexturePtr(new NearestImage_Texture<Color>(textureImage,
			ArrayExtender<2, Color>(clampExtender()))));

	AlignedBox2 textureBox(0.4, 0.4, 0.6, 0.6);
	AlignedBox2 box(0, 0, Width, Height);

	integer textures = textureList.size();
	for (integer i = 0;i < textures;++i)
	{
		drawTexturedBox(
			box + Vector2(0, 0),

			*textureList[i],
			arrayView(image),
			textureBox);

		transform(arrayView(image), fitColor);
		savePcx(image, "texture_checker3_" + textureList[i]->name() +
			"_" + integerToString(i) + ".pcx");
	}
}

TEST_CASE("Samplers (Texture)")
{
	Array<Color, 2> image(Vector2i(750, 500));
	Array<Color, 2> textureImage;

	loadPcx("lena.pcx", textureImage);

	MipMap<Color, 2> mipMap(constArrayView(textureImage),
		ArrayExtender<2, Color>(clampExtender()));
	transform(mipMap, fitColor);

	MipImage_Texture<Color> mipmapSampler(mipMap);

	Color colorSet[] = 
	{
		Color(1, 1, 1), 
		Color(1, 0, 0), 
		Color(0, 1, 0),
		Color(0, 0, 1)
	};

	drawTexturedBox(
		AlignedBox2(0, 400, 100, 500),
		linearColorTexture<Color, 2>(
			range(colorSet)),
		arrayView(image));

	drawTexturedBox(
		AlignedBox2(150, 400, 250, 500),
		colorTexture<Color, 2>(Color(0.5, 0, 1)),
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

	EwaImage_Texture<Color> texture(mipMap);

	drawTexturedBox(
		AlignedBox2(450, 250, 550, 350),
		texture,
		arrayView(image));

	// Something else.

	/*
	drawTexturedBox(
		AlignedBox2(0, 0, 200, 200),
		nearestImageTexture(clampedConstView(constSubView(constArrayView(textureImage), AlignedBox2i(100, 100, 150, 150)))),
		arrayView(image));

	drawTexturedBox(
		AlignedBox2(250, 0, 450, 200),
		linearImageTexture(clampedConstView(constSubView(constArrayView(textureImage), AlignedBox2i(100, 100, 150, 150)))),
		arrayView(image));
	*/

	drawTexturedBox(
		AlignedBox2(100, 100, 300, 225),
		mipmapSampler,
		arrayView(image),
		AlignedBox2(0, 0, 1, 1),
		transparentColorMixer<Color>(0.25));

	drawView(
		constSubView(constArrayView(textureImage), AlignedBox2i(200, 200, 300, 300)),
		Vector2i(300, 400),
		arrayView(image));

	savePcx(image, "textures_texturebox.pcx");

	clear(Color(0), arrayView(image));

	drawTexturedBox(
		AlignedBox2(0, 0, 500, 500),
		nearestImageTexture(textureImage, ArrayExtender<2, Color>(clampExtender())),
		arrayView(image),
		AlignedBox2(0.5, 0.5, 5.0, 5.0));

	savePcx(image, "textures_clampedtexturebox.pcx");

	drawTexturedBox(
		AlignedBox2(0, 0, 500, 500),
		nearestImageTexture(textureImage, ArrayExtender<2, Color>(repeatExtender())),
		arrayView(image),
		AlignedBox2(0.5, 0.5, 5.0, 5.0));

	savePcx(image, "textures_repeatedtexturebox.pcx");

	drawTexturedBox(
		AlignedBox2(0, 0, 500, 500),
		nearestImageTexture(textureImage, ArrayExtender<2, Color>(mirrorExtender())),
		arrayView(image),
		AlignedBox2(0.5, 0.5, 5.0, 5.0));

	savePcx(image, "textures_mirroredtexturebox.pcx");

	drawTexturedBox(
		AlignedBox2(0, 0, 500, 500),
		nearestImageTexture(textureImage),
		arrayView(image),
		AlignedBox2(0.5, 0.5, 5.0, 5.0));

	savePcx(image, "textures_borderedtexturebox.pcx");
}

namespace
{

	void drawTestQuad(
		const Tuple<Vector2, 4>& quad,
		const Tuple<Vector2, 4>& textureQuad,
		const Texture<Color>& texture,
		const std::string& testName,
		const std::string& name)
	{
		dreal yMax = -(dreal)Infinity();
		for (integer i = 0;i < 4;++i)
		{
			if (quad[i].y() > yMax)
			{
				yMax = quad[i].y();
			}
		}

		Array<Color, 2> image(Vector2i(500, ceil(yMax)));

		drawProjectiveQuad(
			quad,
			texture,
			arrayView(image),
			textureQuad);

		savePcx(image, "texture_" + testName + "_" + name + ".pcx");
	}

}

TEST_CASE("Checker (Texture)")
{
	Array<Color, 2> image(Vector2i(500, 500));
	Array<Color, 2> textureImage(Vector2i(40, 40));

	integer CheckerWidth = 20;
	integer CheckerHeight = 20;

	for (integer y = 0;y < textureImage.height();++y)
	{
		for (integer x = 0;x < textureImage.width();++x)
		{

			const integer xMod = mod(x, 2 * CheckerWidth);
			const integer yMod = mod(y, 2 * CheckerHeight);

			if ((xMod < CheckerWidth) !=
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

	MipMap<Color, 2> mipMap(constArrayView(textureImage),
		ArrayExtender<2, Color>(repeatExtender()), boxFilter());
	transform(mipMap, fitColor);

	RipMap<Color, 2> ripMap(constArrayView(textureImage));
	transform(ripMap, fitColor);

	Array<Color, 2> lenaTextureImage;
	loadPcx("lena.pcx", lenaTextureImage);

	MipMap<Color, 2> lenaMipMap(constArrayView(lenaTextureImage));
	transform(lenaMipMap, fitColor);

	RipMap<Color, 2> lenaRipMap(constArrayView(lenaTextureImage));
	transform(lenaRipMap, fitColor);

	ConstIndexExtenderPtr extender = repeatExtender();

	EwaImage_Texture<Color> textureEwaGaussian1(
		mipMap, ArrayExtender<2, Color>(extender), gaussianFilter(1));
	EwaImage_Texture<Color> textureEwaGaussian2(
		mipMap, ArrayExtender<2, Color>(extender), gaussianFilter(2));
	EwaImage_Texture<Color> textureEwaLanczos2(
		mipMap, ArrayExtender<2, Color>(extender), lanczosFilter(2));
	EwaImage_Texture<Color> textureEwaMitchell(
		mipMap, ArrayExtender<2, Color>(extender), mitchellFilter());
	EwaImage_Texture<Color> textureEwaBox(
		mipMap, ArrayExtender<2, Color>(extender), boxFilter());
	EwaImage_Texture<Color> textureEwaTriangle(
		mipMap, ArrayExtender<2, Color>(extender), triangleFilter());
	MipImage_Texture<Color> textureMip(
		mipMap, ArrayExtender<2, Color>(extender));
	RipImage_Texture<Color> textureRip(
		ripMap, ArrayExtender<2, Color>(extender));

	std::vector<std::pair<std::string, Texture<Color>*> > textureList;

	textureList.emplace_back(std::string("ewa_gaussian1"), (Texture<Color>*)&textureEwaGaussian1);
	textureList.emplace_back(std::string("ewa_gaussian2"), (Texture<Color>*)&textureEwaGaussian2);
	textureList.emplace_back(std::string("ewa_lanczos2"), (Texture<Color>*)&textureEwaLanczos2);
	textureList.emplace_back(std::string("ewa_mitchell"), (Texture<Color>*)&textureEwaMitchell);
	textureList.emplace_back(std::string("ewa_box"), (Texture<Color>*)&textureEwaBox);
	textureList.emplace_back(std::string("ewa_triangle"), (Texture<Color>*)&textureEwaTriangle);
	textureList.emplace_back(std::string("mip"), (Texture<Color>*)&textureMip);
	textureList.emplace_back(std::string("rip"), (Texture<Color>*)&textureRip);

	integer textures = textureList.size();

	{
		Tuple<Vector2, 4> quad(
			Vector2(250, 0),
			Vector2(500, 150),
			Vector2(250, 175),
			Vector2(0, 150));

		Tuple<Vector2, 4> textureQuad(
			Vector2(0, 0),
			Vector2(2, 0),
			Vector2(2, 2),
			Vector2(0, 2));

		for (integer i = 0;i < textures;++i)
		{

			drawTestQuad(quad, textureQuad, *textureList[i].second,
				"checker1", textureList[i].first);
		}
	}

	/*
	MipMap<Color, 2> distortMipMap(constArrayView(image));
	EwaImage_Texture<Color> distortTexture(distortMipMap);
	transform(distortMipMap, fitColor);

	clear(Color(0), arrayView(image));

	drawProjectiveQuad(
		Tuple<Vector2, 4>(
		Vector2(0, 0),
		Vector2(500, 0),
		Vector2(500, 500),
		Vector2(0, 500)),
		distortTexture,
		arrayView(image),
		Tuple<Vector2, 4>(
		Vector2(0.5, 0),
		Vector2(1, (dreal)150 / 500),
		Vector2(0.5, (dreal)175 / 500),
		Vector2(0, (dreal)150 / 500)));

	savePcx(image, "texture_checker1_distorted2x.pcx");
	*/

	mipMap.swap(lenaMipMap);
	ripMap.swap(lenaRipMap);

	for (integer i = 0;i < textures;++i)
	{
		Tuple<Vector2, 4> quad(
			Vector2(250, 0),
			Vector2(500, 150),
			Vector2(250, 175),
			Vector2(0, 150));

		Tuple<Vector2, 4> textureQuad(
			Vector2(0, 0),
			Vector2(1, 0),
			Vector2(1, 1),
			Vector2(0, 1));

		drawTestQuad(quad, textureQuad, *textureList[i].second,
			"lena1", textureList[i].first);
	}

	/*
		MipMap<Color, 2> distortMipMap(constArrayView(image));
		EwaImage_Texture<Color> distortTexture(distortMipMap);
		transform(distortMipMap, fitColor);

		clear(Color(0), arrayView(image));

		drawProjectiveQuad(
			Tuple<Vector2, 4>(
			Vector2(0, 0),
			Vector2(500, 0),
			Vector2(500, 500),
			Vector2(0, 500)),
			distortTexture,
			arrayView(image),
			Tuple<Vector2, 4>(
			Vector2(0.5, 0),
			Vector2(1, (dreal)150 / 500),
			Vector2(0.5, (dreal)175 / 500),
			Vector2(0, (dreal)150 / 500)));

		savePcx(image, "texture_lena1_ewa_distorted2x.pcx");
	*/

	mipMap.swap(lenaMipMap);
	ripMap.swap(lenaRipMap);

	{
		Tuple<Vector2, 4> quad(
			Vector2(0, 0),
			Vector2(500, 0),
			Vector2(255, 150),
			Vector2(245, 150));

		Tuple<Vector2, 4> textureQuad(
			Vector2(0, 0),
			Vector2(20, 0),
			Vector2(20, 800),
			Vector2(0, 800));

		for (integer i = 0;i < textures;++i)
		{
			drawTestQuad(quad, textureQuad, *textureList[i].second,
				"checker2", textureList[i].first);
		}
	}

	/*
	{
		clear(Color(0), arrayView(image));

		Tuple<Vector2, 4> quad(
			Vector2(0, 0),
			Vector2(500, 0),
			Vector2(300, 250),
			Vector2(200, 250));

		Tuple<Vector2, 4> textureQuad(
			Vector2(0, 0),
			Vector2(10, 0),
			Vector2(10, 100),
			Vector2(0, 100));

		clear(Color(0), arrayView(image));

		EwaImage_Texture<Color> texture(mipMap);

		drawProjectiveQuad(
			quad,
			mixTexture(
			colorTexture<Color>(Color(0)),
			transformTexture(
			mixTexture(
			linearImageTexture(repeatedConstView(constArrayView(lenaTextureImage))),
			colorTexture<Color>(Color(1)),
			linearColorTexture<dreal>(0, 0, 1, 1)),
			AffineTransformation<dreal>(identityMatrix<Real, 2, 2>(2, 2) * 10, Vector2())),
			texture),
			arrayView(image),
			textureQuad);

		savePcx(image, "texture_texture_mix.pcx");
	}
	*/
}

TEST_CASE("Mix (Texture)")
{
	Array<Color, 2> aTexture;
	loadPcx("lena.pcx", aTexture);

	Array<Color, 2> bTexture;
	loadPcx("lena.pcx", bTexture);

	Array<Color, 2> image(Vector2i(750, 750), Color(0));

	MipMap<Color, 2> aMipMap(constArrayView(aTexture));
	transform(aMipMap, fitColor);

	MipMap<Color, 2> bMipMap(constArrayView(bTexture));
	transform(bMipMap, fitColor);

	integer iconSize = 250;
	integer iconSpace = iconSize;

	AlignedBox2 iconBox(0, 0, iconSize, iconSize);

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
		functorColorMixer<Color>(std::minus<Color>())),
		arrayView(image));

	drawTexturedBox(
		iconBox + Vector2(iconSpace, iconSpace),
		combineTexture(
		mipImageTexture(aMipMap),
		mipImageTexture(bMipMap),
		functorColorMixer<Color>(std::plus<Color>())),
		arrayView(image));

	drawTexturedBox(
		iconBox + Vector2(0, iconSpace),
		combineTexture(
		mipImageTexture(aMipMap),
		mipImageTexture(bMipMap),
		functorColorMixer<Color>(std::multiplies<Color>())),
		arrayView(image));

	drawTexturedBox(
		iconBox + Vector2(0, iconSpace * 2),
		mipImageTexture(aMipMap),
		arrayView(image));

	drawTexturedBox(
		iconBox + Vector2(iconSpace * 2, 0),
		mipImageTexture(bMipMap),
		arrayView(image));

	savePcx(image, "texture_mix.pcx");
}

