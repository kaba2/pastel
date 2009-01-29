#include "pastelgfxtest.h"

#include "pastel/sys/arrayview.h"

#include "pastel/math/uniformsampling.h"

#include "pastel/geometry/poissondiskpattern.h"

#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/filter_all.h"
#include "pastel/gfx/filterform.h"
#include "pastel/gfx/reconstructimage.h"
#include "pastel/gfx/loadpcx.h"
#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/image_tools.h"
#include "pastel/gfx/nearestreconstructor.h"
#include "pastel/gfx/filterreconstructor.h"

#include <boost/bind.hpp>

using namespace Pastel;

namespace
{

	void testReconstructionOld()
	{
		LinearArray<2, Color> image;

		loadPcx("ScMod_Tiff_Veeco.pcx", image);

		const integer width = image.width();
		const integer height = image.height();

		LinearArray<2, bool> mask(
			width, height, false);

		for (integer y = 0;y < height;++y)
		{
			for (integer x = 0;x < width;++x)
			{
				const bool black =
					(image(x, y)[0] == 0) &&
					(image(x, y)[1] == 0) &&
					(image(x, y)[2] == 0);
				mask(x, y) = black;
			}
		}

		LinearArray<2, Color> maskImage(width, height);
		for (integer y = 0;y < height;++y)
		{
			for (integer x = 0;x < width;++x)
			{
				maskImage(x, y) =
					mask(x, y) ? Color(1) : Color(0);
			}
		}

		savePcx(maskImage, "reconstructionmask.pcx");

		const integer HoleWidth = 33;
		LinearArray<2, real> filter(
			HoleWidth, HoleWidth);
		setFilter(boost::bind(gaussian<real>, _1, (real)0.1), arrayView(filter));

		LinearArray<2, Color> result;
		reconstructImage(image, mask, filter, result);

		savePcx(result, "reconstructionresult.pcx");
	}

	class ReportFunctor
	{
	public:
		explicit ReportFunctor(
			std::vector<Point2>& positionList)
			: positionList_(positionList)
		{
		}

		void operator()(const Point2& position) const
		{
			positionList_.push_back(position);
		}

		std::vector<Point2>& positionList_;
	};

	void testReconstruction()
	{
		std::vector<Point2> positionList;

		ReportFunctor reportFunctor(
			positionList);

		poissonDiskPattern(AlignedBox2(0, 0, 1, 1),
			0.01, reportFunctor);

		const integer points = positionList.size();

		std::vector<Color> dataList;
		dataList.reserve(points);

		for (integer i = 0;i < points;++i)
		{
			const real t = 10 * dot(asVector(positionList[i]));
			const real value = (std::sin(t * t * t) + 1) / 2;

			dataList.push_back(
				hsvToRgb(Color(t * t * t / 20, 1, value)));
		}

		LinearArray<2, Color> image(500, 500);

		reconstructNearest(positionList,
			dataList,
			AlignedBox2(0, 0, 1, 1),
			arrayView(image));

		savePcx(image, "testreconstruct_nearest.pcx");

		TriangleFilter filter;

		reconstructFilter(positionList,
			dataList,
			AlignedBox2(0, 0, 1, 1),
			filter,
			10,
			arrayView(image));

		savePcx(image, "testreconstruct_filter.pcx");
	}

	void testBegin()
	{
		//testReconstructionOld();
		testReconstruction();
	}

	void testAdd()
	{
		gfxTestList().add("Reconstruct", testBegin);
	}

	CallFunction run(testAdd);

}
