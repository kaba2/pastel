// Description: Testing for poisson-disk pattern
// DocumentationOf: poisson_disk_pattern.h

#include "test_pastelgeometry.h"

#include "pastel/geometry/pointkdtree/pointkdtree.h"
#include "pastel/geometry/distance/distance_point_point.h"
#include "pastel/geometry/poisson_disk_pattern.h"

#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/drawing.h"
#include "pastel/gfx/image_gfxrenderer.h"
#include "pastel/gfx/gfxrenderer_tools.h"
#include "pastel/gfx/color_space.h"

#include "pastel/sys/random.h"
#include "pastel/sys/view/arrayview.h"
#include "pastel/sys/view/view_tools.h"
#include "pastel/sys/views.h"

#include "pastel/sys/vector/vector_tools.h"

using namespace Pastel;

namespace
{

	class PoissonDiskOutput
	{
	public:
		explicit PoissonDiskOutput(Array<Color, 2>& data)
			: data_(data)
		{
		}

		void operator()(const Vector1& point) const
		{
			clear(Color(1), rowView(arrayView(data_), 1, Vector<integer, 2>(point[0], 0)));
		}

		void operator()(const Vector2& point) const
		{
			drawPixel(point, Color(1), arrayView(data_));
		}

		void operator()(const Vector3& point) const
		{
			drawPixel(shrink(point), Color(1 - (point.z() / 500)), arrayView(data_));
		}

		void operator()(const Vector4& point) const
		{
			drawPixel(shrink(shrink(point)), hsvToRgb(Color(point.w() / 100,
				1, 1 - (point.z() / 500))), arrayView(data_));
		}

	private:
		Array<Color, 2>& data_;
	};

	void testPoissonDiskPattern()
	{
		integer width = 500;
		integer height = 500;

		Array<Color, 2> image;

		PoissonDiskOutput poissonDiskOutput(image);

		image.setExtent(Vector2i(width, 100), Color(0));
		clear(Color(0), arrayView(image));

		poissonDiskPattern(
			AlignedBox1(0, width),
			5, poissonDiskOutput);

		savePcx(image, "testpointpattern_poissondisk_1d.pcx");

		image.setExtent(Vector2i(width, height), Color(0));
		clear(Color(0), arrayView(image));

		poissonDiskPattern(
			AlignedBox2(Vector2(0), Vector2(width, height)),
			5, poissonDiskOutput);

		savePcx(image, "testpointpattern_poissondisk_2d.pcx");

		clear(Color(0), arrayView(image));

		poissonDiskPattern(
			AlignedBox3(Vector3(0), Vector3(width, height, 500)),
			35, poissonDiskOutput);

		savePcx(image, "testpointpattern_poissondisk_3d.pcx");

		clear(Color(0), arrayView(image));

		poissonDiskPattern(
			AlignedBox4(Vector4(0), Vector4(width, height, 500, 500)),
			70, poissonDiskOutput);

		savePcx(image, "testpointpattern_poissondisk_4d.pcx");
	}

	void test()
	{
		testPoissonDiskPattern();
	}

	void addTest()
	{
		testRunner().add("PointPattern", test);
	}

	CallFunction run(addTest);

}
