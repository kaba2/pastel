#include "pastelraytest.h"

#include <pastel/ray/sphere_shape.h>

#include <pastel/ray/shape_thing.h>
#include <pastel/ray/set_thing.h>
#include <pastel/ray/kdtree_thing.h>

#include <pastel/ray/diffuse_material.h>
#include <pastel/ray/beam_tools.h>
#include <pastel/ray/camera.h>
#include <pastel/ray/pinhole_lens.h>
#include <pastel/ray/spherical_lens.h>

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

	class Test
		: public TestSuite
	{
	public:
		Test()
			: TestSuite(&rayTestReport())
		{
		}

		virtual void run()
		{
			const integer width = 768;
			const integer height = 768;
			Array<Spectrum> image(width, height, Spectrum());

			Array<Color> textureImage;
			loadPcx("texture.pcx", textureImage);

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

			/*
			Texture<Spectrum>::Ptr texture(
				new EwaImage_Texture<Spectrum>(mipMap, 
				ArrayExtender<2, Spectrum>(repeatExtender())));
			*/

			Texture<Spectrum>::Ptr texture(
				new MipImage_Texture<Spectrum>(mipMap, 
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
				range(colorSet)));
			*/

			Diffuse_MaterialPtr material =
				Diffuse_MaterialPtr(new Diffuse_Material(texture));

			ShapePtr shape = 
				ShapePtr(new Sphere_Shape);

			const integer things = 500;
			std::vector<ThingPtr> thingSet;
			thingSet.reserve(things);
			for (integer i = 0;i < things;++i)
			{			
				Shape_ThingPtr thing = Shape_ThingPtr(new Shape_Thing(
					shape, material));

				//scaling3<real>(randomVector<real, 3>() + 0.5) * 
				thing->setTransformation(					
					translation3(evaluate(randomVectorBall<real, 3>() * 30)));
				thingSet.push_back(thing);
			}

			/*
			Set_ThingPtr setThing = 
				Set_ThingPtr(new Set_Thing);
			setThing->insert(
				range(thingSet.begin(), thingSet.end()));			
			*/
			KdTree_ThingPtr setThing =
				KdTree_ThingPtr(new KdTree_Thing);
			setThing->insert(
				range(thingSet.begin(), thingSet.end()));
			setThing->refine();

			LensPtr lens = LensPtr(new Pinhole_Lens);
			//Spherical_LensPtr lens = Spherical_LensPtr(new Spherical_Lens);
			/*
			lens->setWindow(
				AlignedBox2(-1, -0.5, 1, 0.5) * constantPi<real>());
			*/
			CameraPtr camera = CameraPtr(
				new Camera(lens));
			camera->setPosition(Vector3(0, 0, -10));

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

					ShapeIntersection intersection;
					LocalGeometry surface;
					if (thing->intersect(beam.ray(), intersection))
					{
						LocalGeometry surface = localGeometry(intersection);

						beam = transferToSurface(beam, surface, intersection.t);

						const Matrix3 qBasis(
							surface.dpDu,
							surface.dpDv,
							surface.normal);

						const Matrix3 qInvBasis = inverse(qBasis);

						const Matrix3 lightBasis(
							normalize(surface.dpDu),
							normalize(-cross(surface.dpDu, surface.normal)),
							normalize(surface.normal));

						const Matrix3 lightInvBasis = transpose(lightBasis);
							
						Vector3 light = -surface.position;
						//Vector3 light = Vector3(0, 0, -1);
						Vector3 in = normalize(light * lightInvBasis);
						Vector3 out = normalize(-beam.ray().direction() * lightInvBasis);

						const Vector2 dqDx = 
							shrink(beam.ddx().position() * qInvBasis);
						const Vector2 dqDy = 
							shrink(beam.ddy().position() * qInvBasis);

						/*
						image(x, y) = Color(dot(normalize(light), normalize(surface.normal)));
						*/
						image(x, y) = intersection.thing->material()->brdf(
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
		Test test;
		test.run();
	}

	void addTest()
	{
		rayTestList().add("Thing", testThing);
	}

	CallFunction run(addTest);

}
