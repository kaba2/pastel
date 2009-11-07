#include "pastelraytest.h"

#include <pastel/ray/sphere_shape.h>
#include <pastel/ray/shape_thing.h>
#include <pastel/ray/set_thing.h>
#include <pastel/ray/diffuse_material.h>
#include <pastel/ray/beam_tools.h>
#include <pastel/ray/camera.h>
#include <pastel/ray/pinhole_lens.h>

#include <pastel/geometry/alignedbox.h>

#include <pastel/math/affinetransformation_tools.h>

#include <pastel/gfx/pcx.h>
#include <pastel/gfx/color_srgb.h>
#include <pastel/gfx/textures.h>

#include <pastel/sys/view_all.h>
#include <pastel/sys/indexextender_all.h>
#include <pastel/sys/random_uniform.h>

using namespace Pastel;

namespace
{

	class Thing_Test
		: public TestSuite
	{
	public:
		Thing_Test()
			: TestSuite(&rayTestReport())
		{
		}

		virtual void run()
		{
			const integer width = 512;
			const integer height = 512;
			Array<Spectrum> image(width, height, Spectrum());

			Array<Color> textureImage;
			loadPcx("lena.pcx", textureImage);

			Array<Spectrum> spectrumImage(textureImage.extent());
			for (integer i = 0;i < textureImage.size();++i)
			{
				/*
				spectrumImage(i) = Spectrum::fromXyz(
					srgbToXyz(textureImage(i)));
				*/
				spectrumImage(i) = textureImage(i);
			}

			MipMap<Spectrum> mipMap(constArrayView(spectrumImage));

			Texture<Spectrum>::Ptr texture(
				new EwaImage_Texture<Spectrum>(mipMap, 
				ArrayExtender<2, Spectrum>(repeatExtender())));

			/*
			Texture<Spectrum>::Ptr texture(
				new NearestImage_Texture<Spectrum>(spectrumImage,
				ArrayExtender<2, Spectrum>(repeatExtender())));
			*/

			/*
			const Color colorSet[] = 
			{
				Color(1, 1, 0),
				Color(1, 0, 1),
				Color(0, 0, 1),
				Color(0, 1, 1)
			};

			Texture<Spectrum>::Ptr texture(
				new LinearColor_Texture<Spectrum>(
				randomAccessRange(colorSet)));
			*/

			Diffuse_MaterialPtr material =
				Diffuse_MaterialPtr(new Diffuse_Material(texture));

			ShapePtr shape = 
				ShapePtr(new Sphere_Shape);

			const integer things = 100;
			std::vector<ThingPtr> thingSet;
			thingSet.reserve(things);
			for (integer i = 0;i < things;++i)
			{			
				Shape_ThingPtr thing = Shape_ThingPtr(new Shape_Thing(
					shape, material));
				thing->setTransformation(
					scaling3<real>(Vector3(2, 1, 1)) * translation3(evaluate((randomVector<real, 3>() - 0.5) * 30)));
				thingSet.push_back(thing);
			}

			Set_ThingPtr setThing = 
				Set_ThingPtr(new Set_Thing);
			setThing->insert(
				forwardRange(thingSet.begin(), thingSet.end()));			

			LensPtr lens = LensPtr(new Pinhole_Lens);
			CameraPtr camera = CameraPtr(
				new Camera(lens));
			camera->setPosition(Vector3(0, 0, -30));

			renderThing(
				camera,
				setThing,
				image,
				"sphere");
		}

		void renderThing(
			const CameraPtr& camera,
			const ThingPtr& thing,
			Array<Spectrum>& image,
			const std::string& name)
		{
			const AlignedBox3 viewVolume(
				Vector3(-3), Vector3(3));

			const integer width = image.width();
			const integer height = image.height();

			const real xDelta = (real)1 / width;
			const real yDelta = (real)1 / height;

			for (integer y = 0;y < height;++y)
			{
				for (integer x = 0;x < width;++x)
				{
					const real u = dequantizeUnsigned(x, width);
					const real v = dequantizeUnsigned(y, height);

					Beam beam = camera->beam(Vector2(u, v));
					/*
					beam.ray().set(
						viewVolume.at(Vector3(u, v, 0)),
						Vector3(0, 0, 1));
					*/

					LocalGeometry surface;
					real t;

					if (thing->intersect(beam.ray(), surface, t))
					{
						beam = transferToSurface(beam, surface, t);

						const Matrix3 basis(
							surface.dpDu,
							surface.dpDv,
							surface.normal);

						const Matrix3 invBasis = inverse(basis);

						/*
						const Vector2 dqDx = 
							shrink(solveLinear(basis, beam.ddx().direction()));
						const Vector2 dqDy = 
							shrink(solveLinear(basis, beam.ddy().direction()));
						*/

						Vector3 light(0, 0, -1);
						Vector3 in = normalize(light * invBasis);
						Vector3 out = normalize(-beam.ray().direction() * invBasis);

						const Vector2 dqDx = 
							shrink(beam.ddx().position() * invBasis);
						const Vector2 dqDy = 
							shrink(beam.ddy().position() * invBasis);

						image(x, y) = surface.thing->material()->brdf(
							surface.q, dqDx * xDelta, dqDy * yDelta,
							in, out);
					}
				}
			}

			Array<Color> colorImage(
				image.extent());
			for (integer i = 0;i < image.size();++i)
			{
				//colorImage(i) = xyzToSrgb(image(i).toXyz());
				colorImage(i) = image(i);
			}

			savePcx(colorImage, name + "_thing.pcx");
		}
	};

	void testThing()
	{
		Thing_Test test;
		test.run();
	}

	void addTest()
	{
		rayTestList().add("Thing", testThing);
	}

	CallFunction run(addTest);

}
