// Description: More testing for Textures
// DocumentationOf: textures.h

#include "test_pastelgfx.h"

#include <pastel/gfx/color.h>
#include <pastel/gfx/texture.h>
#include <pastel/gfx/drawing.h>
#include <pastel/gfx/image_gfxrenderer.h>
#include <pastel/gfx/gfxrenderer_tools.h>

using namespace Pastel;

namespace
{

	class Test
		: public TestSuite
	{
	public:
		Test()
			: TestSuite(&testReport())
		{
		}

		virtual void run()
		{
			testTransform();
			testRadial();
		}

		void testTransform()
		{
			Texture<Color>* inputTexture = 
				gfxStorage().get<EwaImage_Texture<Color>*>("lena_texture");
			ENSURE(inputTexture);

			AffineTransformation<real> transform = affineRotation<real>(2, 0, 1, constantPi<real>() / 6);
			transform.translation() += 0.25;

			Transform_Texture<Color> distortedTexture =
				transformTexture(*inputTexture, transform);

			Array<Color> image(Vector2i(512, 512));
			drawBox(AlignedBox2(Vector2(0, 0), Vector2(image.extent())),
				distortedTexture,
				arrayView(image));
			
			savePcx(image, "transform_texture.pcx");
		}

		void testRadial()
		{
			EwaImage_Texture<Color>& texture = 
				*gfxStorage().get<EwaImage_Texture<Color>*>("lena_texture");			

			Array<Color, 2> image(Vector2i(768, 256));

			real minRadius = 0.25;
			real maxRadius = 0.5;
			real angleBegin = constantPi<real>() / 10;

			const real angleEnd = 1.2423 * constantPi<real>();

			Radial_Texture<Color> distortedTexture =
				radialTexture(texture, Vector2(0.5),
				Vector2(minRadius, angleBegin), Vector2(maxRadius, angleEnd));

			AffineTransformation<real> textureFlip(
				matrix2x2<real>(0, 1,
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
					transformPoint(affineRotation<real>(2, 0, 1, angleBegin), Vector2(minRadius, 0)),
					transformPoint(affineRotation<real>(2, 0, 1, angleBegin), Vector2(maxRadius, 0))));
				drawSegment(renderer, Vector2(0.5) + Segment2(
					transformPoint(affineRotation<real>(2, 0, 1, angleEnd), Vector2(minRadius, 0)),
					transformPoint(affineRotation<real>(2, 0, 1, angleEnd), Vector2(maxRadius, 0))));

				savePcx(image, "radial_texture_input.pcx");
			}
		}
	
	};

	void testTextureExamples()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("Texture_Examples", testTextureExamples);
	}

	CallFunction run(addTest);

}