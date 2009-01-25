#include "pastelgeometrytest.h"

#include "pastel/geometry/kdtree_tools.h"
#include "pastel/geometry/distance_point_point.h"
#include "pastel/geometry/poissondiskpattern.h"

#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/drawing.h"
#include "pastel/gfx/imagegfxrenderer.h"
#include "pastel/gfx/gfxrenderer_tools.h"
#include "pastel/gfx/color_tools.h"

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
		explicit PoissonDiskReporter(LinearArray<2, Color>& data)
			: data_(data)
		{
		}

		void operator()(const Point1& point) const
		{
			clear(Color(1), rowView<1>(arrayView(data_), Point<2, integer>(point[0], 0)));
		}

		void operator()(const Point2& point) const
		{
			drawPixel(point, Color(1), arrayView(data_));
		}

		void operator()(const Point3& point) const
		{
			drawPixel(shrink(point), Color(1 - (point.z() / 500)), arrayView(data_));
		}
	
		void operator()(const Point4& point) const
		{
			drawPixel(shrink(shrink(point)), hsvToRgb(Color(point.w() / 100,
				1, 1 - (point.z() / 500))), arrayView(data_));
		}

	private:
		LinearArray<2, Color>& data_;
	};

	void testPoissonDiskPattern()
	{
		log() << "- poisson disk pattern" << logNewLine;

		const integer width = 500;
		const integer height = 500;

		LinearArray<2, Color> image;

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
			AlignedBox2(Point2(0), Point2(width, height)),
			5, poissonDiskReporter);

		savePcx(image, "testpointpattern_poissondisk_2d.pcx");

		clear(Color(0), arrayView(image));

		poissonDiskPattern(
			AlignedBox3(Point3(0), Point3(width, height, 500)),
			35, poissonDiskReporter);

		savePcx(image, "testpointpattern_poissondisk_3d.pcx");

		clear(Color(0), arrayView(image));

		poissonDiskPattern(
			AlignedBox4(Point4(0), Point4(width, height, 500, 500)),
			70, poissonDiskReporter);

		savePcx(image, "testpointpattern_poissondisk_4d.pcx");
	}

	void testBegin()
	{
		testPoissonDiskPattern();
	}

	void testAdd()
	{
		geometryTestList().add("PointPattern", testBegin);
	}

	CallFunction run(testAdd);

}
