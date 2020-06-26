// Description: More testing for Textures
// DocumentationOf: textures.h

#include "test/test_init.h"

#include <test/pastel/gfx/test_pastelgfx.h>

#include <pastel/gfx/color.h>
#include <pastel/gfx/texture.h>
#include <pastel/gfx/drawing.h>
#include <pastel/gfx/image_file/pcx.h>
#include <pastel/gfx/image_gfxrenderer.h>
#include <pastel/gfx/gfxrenderer_tools.h>

TEST_CASE("Transform (TextureExample)")
{
	Texture<Color>* inputTexture = 
		gfxStorage().get<EwaImage_Texture<Color>*>("lena_texture");
	ENSURE(inputTexture);

	AffineTransformation<dreal> transform = affineRotation<dreal>(2, 0, 1, constantPi<dreal>() / 6);
	transform.translation() += 0.25;

	Transform_Texture<Color> distortedTexture =
		transformTexture(*inputTexture, transform);

	Array<Color> image(Vector2i(512, 512));
	drawBox(AlignedBox2(Vector2(0, 0), Vector2(image.extent())),
		distortedTexture,
		arrayView(image));

	savePcx(image, "transform_texture.pcx");
}

TEST_CASE("Radial (TextureExample)")
{
	EwaImage_Texture<Color>& texture = 
		*gfxStorage().get<EwaImage_Texture<Color>*>("lena_texture");			

	Array<Color, 2> image(Vector2i(768, 256));

	dreal minRadius = 0.25;
	dreal maxRadius = 0.5;
	dreal angleBegin = constantPi<dreal>() / 10;

	const dreal angleEnd = 1.2423 * constantPi<dreal>();

	Radial_Texture<Color> distortedTexture =
		radialTexture(texture, Vector2(0.5),
		Vector2(minRadius, angleBegin), Vector2(maxRadius, angleEnd));

	AffineTransformation<dreal> textureFlip(
		matrix2x2<dreal>(0, 1,
				1, 0),
		Vector2(0, 0));

	Transform_Texture<Color> flippedTexture = 
		transformTexture(distortedTexture, textureFlip);

	drawBox(
		AlignedBox2(0, 0, image.width(), image.height()),
		flippedTexture,
		arrayView(image));

	transform(arrayView(image), fitColor);

	savePcx(image, "radial_texture.pcx");

	{
		Array<Color, 2> image(Vector2i(512, 512));

		drawBox(
			AlignedBox2(0, 0, image.width(), image.height()),
			texture,
			arrayView(image));

		Image_GfxRenderer<Color> renderer;
		renderer.setImage(&image);
		renderer.setFilled(false);
		renderer.setColor(Color(0, 1, 0));
		renderer.setViewWindow(
			AlignedBox2(0, 0, 1, 1));

		drawCircle(renderer, Sphere2(Vector2(0.5), minRadius), 40);
		drawCircle(renderer, Sphere2(Vector2(0.5), maxRadius), 40);
		drawSegment(renderer, Vector2(0.5) + Segment2(
			transformPoint(affineRotation<dreal>(2, 0, 1, angleBegin), Vector2(minRadius, 0)),
			transformPoint(affineRotation<dreal>(2, 0, 1, angleBegin), Vector2(maxRadius, 0))));
		drawSegment(renderer, Vector2(0.5) + Segment2(
			transformPoint(affineRotation<dreal>(2, 0, 1, angleEnd), Vector2(minRadius, 0)),
			transformPoint(affineRotation<dreal>(2, 0, 1, angleEnd), Vector2(maxRadius, 0))));

		savePcx(image, "radial_texture_input.pcx");
	}
}

