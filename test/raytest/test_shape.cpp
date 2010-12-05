#include "pastelraytest.h"

#include <pastel/ray/sphere_shape.h>
#include <pastel/ray/shape_thing.h>

#include <pastel/geometry/alignedbox.h>

#include <pastel/gfx/pcx.h>

using namespace Pastel;

namespace
{

	class Shape_Test
		: public TestSuite
	{
	public:
		Shape_Test()
			: TestSuite(&rayTestReport())
		{
		}

		virtual void run()
		{
			const integer width = 512;
			const integer height = 512;
			Array<Color> image(width, height, Color(1));

			renderShape(
				ShapePtr(new Sphere_Shape()), 
				image,
				"sphere");
		}

		void renderShape(
			const ShapePtr& shape,
			Array<Color>& image,
			const std::string& name)
		{
			const AlignedBox3 viewVolume(
				Vector3(-3), Vector3(3));

			const integer width = image.width();
			const integer height = image.height();
			for (integer y = 0;y < height;++y)
			{
				for (integer x = 0;x < width;++x)
				{
					const real u = dequantizeUnsigned(x, width);
					const real v = dequantizeUnsigned(y, height);

					const Ray3 ray(
						viewVolume.at(Vector3(u, v, 0)),
						Vector3(0, 0, 1));

					real t;
					if (shape->intersect(ray, t))
					{
						LocalGeometry local = shape->localGeometry(ray.at(t));

						const Color colorSet[] = 
						{
							Color(1, 1, 1),
							Color(0, 0, 1),
							Color(0, 1, 0),
							Color(1, 0, 0)
						};

						const Color color = 
							linear(
							evaluate(mod(local.q * Vector2(5, 10))),
							range(colorSet));

						image(x, y) = color;
					}
				}
			}

			savePcx(image, name + "_shape.pcx");
		}
	};

	void testShape()
	{
		Shape_Test test;
		test.run();
	}

	void addTest()
	{
		rayTestList().add("Shape", testShape);
	}

	CallFunction run(addTest);

}
