#include "pastelgfxtest.h"

#include "pastel/sys/arrayview.h"

#include "pastel/math/uniformsampling.h"

#include "pastel/geometry/poissondiskpattern.h"

#include "pastel/gfx/color_tools.h"
#include "pastel/dsp/filter_all.h"
#include "pastel/dsp/filterform.h"
#include "pastel/dsp/reconstructimage.h"
#include "pastel/gfx/loadpcx.h"
#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/image_tools.h"
#include "pastel/gfx/nearestreconstructor.h"
#include "pastel/gfx/filterreconstructor.h"

#include "pastel/dsp/resample.h"

#include "pastel/sys/indexextender_all.h"
#include "pastel/sys/string_tools.h"

#include <boost/bind.hpp>

using namespace Pastel;

namespace
{

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

	void generatePoisson(
			std::vector<Point2>& positionList,
			real poissonRadius)
	{
		std::vector<Point2> sampleList;

		ReportFunctor reportFunctor(
			sampleList);
		poissonDiskPattern(AlignedBox2(0, 0, 1, 1),
			poissonRadius, reportFunctor);

		sampleList.swap(positionList);
	}

	void generateGrid(
		std::vector<Point2>& positionList,
		integer xPoints,
		integer yPoints)
	{
		ENSURE1(xPoints > 0, xPoints);
		ENSURE1(yPoints > 0, yPoints);

		std::vector<Point2> sampleList;

		for (integer y = 0;y < yPoints;++y)
		{
			for (integer x = 0;x < xPoints;++x)
			{
				sampleList.push_back(
					Point2(dequantizeUnsigned(x, xPoints),
					dequantizeUnsigned(y, yPoints)));
			}
		}

		sampleList.swap(positionList);
	}

	void testReconstruction(
		const Array<2, Color>& blurTexture,
		const std::vector<Point2>& positionList,
		const std::string& name,
		real sampleDistance,
		integer k)
	{
		const integer points = positionList.size();

		std::vector<Color> dataList;
		dataList.reserve(points);

		for (integer i = 0;i < points;++i)
		{
			const Color color = blurTexture(
				quantizeUnsigned(positionList[i].x(), blurTexture.width()),
				quantizeUnsigned(positionList[i].y(), blurTexture.height()));

			dataList.push_back(color);
		}

		Array<2, Color> image(blurTexture);
		for (integer i = 0;i < points;++i)
		{
			image(
				quantizeUnsigned(positionList[i].x(), image.width()),
				quantizeUnsigned(positionList[i].y(), image.height())) = 
				Color(1);
		}

		savePcx(image, "output/reconstruct_samples_" + integerToString(k, 2) +  "_" + name + ".pcx");

		reconstructNearest(positionList,
			dataList,
			AlignedBox2(0, 0, 1, 1),
			arrayView(image));

		savePcx(image, "output/reconstruct_nearest_" + integerToString(k, 2) + "_" + name + ".pcx");

		FilterRef filter = tableFilter(gaussianFilter());

		reconstructFilter(positionList,
			dataList,
			AlignedBox2(0, 0, 1, 1),
			*filter,
			sampleDistance * (image.width() + 1),
			arrayView(image));

		savePcx(image, "output/reconstruct_filter_" + integerToString(k, 2) + "_" + name + ".pcx");
	}

	void testReconstruction()
	{
		Array<2, Color> texture;
		loadPcx("lena.pcx", texture);

		for (integer k = 4;k <= 9;++k)
		{
			const integer xPoints = 1 << k;
			const real sampleDistance = (real)1 / (xPoints + 1);

			Array<2, Color> blurTexture(texture.extent());
			std::vector<Point2> positionList;

			resample<Color>(
				constArrayView(texture),
				clampExtender(),
				lanczosFilter(),
				arrayView(blurTexture), 
				(texture.width() + 1) * sampleDistance);

			generatePoisson(positionList, sampleDistance);

			testReconstruction(
				blurTexture, positionList, "poisson",
				sampleDistance, k);

			generateGrid(positionList, xPoints, xPoints);

			testReconstruction(
				blurTexture, positionList, "grid",
				sampleDistance, k);
		}
	}

	void testAdd()
	{
		gfxTestList().add("Reconstruction", testReconstruction);
	}

	CallFunction run(testAdd);

	void testReconstructionOld()
	{
		Array<2, Color> image;

		loadPcx("ScMod_Tiff_Veeco.pcx", image);

		const integer width = image.width();
		const integer height = image.height();

		Array<2, bool> mask(
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

		Array<2, Color> maskImage(width, height);
		for (integer y = 0;y < height;++y)
		{
			for (integer x = 0;x < width;++x)
			{
				maskImage(x, y) =
					mask(x, y) ? Color(1) : Color(0);
			}
		}

		savePcx(maskImage, "output/reconstructionmask.pcx");

		const integer HoleWidth = 33;
		Array<2, real> filter(
			HoleWidth, HoleWidth);
		setFilter(boost::bind(gaussian<real>, _1, (real)0.1), arrayView(filter));

		Array<2, Color> result;
		reconstructImage(image, mask, filter, result);

		savePcx(result, "output/reconstructionresult.pcx");
	}

}
