#include "pastelgeometrytest.h"

#include "pastel/geometry/pointkdtree_tools.h"
#include "pastel/geometry/distance_point_point.h"
#include "pastel/geometry/poissondiskpattern.h"

#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/draw.h"
#include "pastel/gfx/image_gfxrenderer.h"
#include "pastel/gfx/gfxrenderer_tools.h"
#include "pastel/gfx/color_space.h"

#include "pastel/sys/random.h"
#include "pastel/sys/arrayview.h"
#include "pastel/sys/view_tools.h"
#include "pastel/sys/view_all.h"

#include "pastel/sys/vector_tools.h"

using namespace Pastel;

namespace
{

	class PoissonDiskReporter
	{
	public:
		explicit PoissonDiskReporter(Array<Color, 2>& data)
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
		const integer width = 500;
		const integer height = 500;

		Array<Color, 2> image;

		PoissonDiskReporter poissonDiskReporter(image);

		image.setExtent(width, 100, Color(0));
		clear(Color(0), arrayView(image));

		poissonDiskPattern(
			AlignedBox1(0, width),
			5, poissonDiskReporter);

		savePcx(image, "testpointpattern_poissondisk_1d.pcx");

		image.setExtent(width, height, Color(0));
		clear(Color(0), arrayView(image));

		poissonDiskPattern(
			AlignedBox2(Vector2(0), Vector2(width, height)),
			5, poissonDiskReporter);

		savePcx(image, "testpointpattern_poissondisk_2d.pcx");

		clear(Color(0), arrayView(image));

		poissonDiskPattern(
			AlignedBox3(Vector3(0), Vector3(width, height, 500)),
			35, poissonDiskReporter);

		savePcx(image, "testpointpattern_poissondisk_3d.pcx");

		clear(Color(0), arrayView(image));

		poissonDiskPattern(
			AlignedBox4(Vector4(0), Vector4(width, height, 500, 500)),
			70, poissonDiskReporter);

		savePcx(image, "testpointpattern_poissondisk_4d.pcx");
	}

	void test()
	{
		testPoissonDiskPattern();
	}

	void addTest()
	{
		geometryTestList().add("PointPattern", test);
	}

	CallFunction run(addTest);

}
