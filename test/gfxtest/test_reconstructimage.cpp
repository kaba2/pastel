#include "pastelgfxtest.h"

#include "pastel/sys/arrayview.h"

#include "pastel/math/uniform_sampling.h"

#include "pastel/geometry/poissondiskpattern.h"

#include "pastel/gfx/color_tools.h"
#include "pastel/dsp/filter_all.h"
#include "pastel/dsp/filterform.h"
#include "pastel/dsp/reconstructimage.h"
#include "pastel/gfx/loadpcx.h"
#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/image_tools.h"
#include "pastel/gfx/reconstruct_nearest.h"
#include "pastel/gfx/reconstruct_filter.h"
#include "pastel/gfx/reconstruct_rbf.h"

#include "pastel/dsp/resample.h"

#include "pastel/sys/indexextender_all.h"
#include "pastel/sys/string_algorithms.h"

#include <boost/bind.hpp>

using namespace Pastel;

namespace
{

	class ReportFunctor
	{
	public:
		explicit ReportFunctor(
			std::vector<Vector2>& positionList)
			: positionList_(positionList)
		{
		}

		void operator()(const Vector2& position) const
		{
			positionList_.push_back(position);
		}

		std::vector<Vector2>& positionList_;
	};

	void generatePoisson(
			std::vector<Vector2>& positionList,
			const AlignedBox2& region,
			real poissonRadius)
	{
		std::vector<Vector2> sampleList;

		ReportFunctor reportFunctor(
			sampleList);
		poissonDiskPattern(region,
			poissonRadius, reportFunctor);

		sampleList.swap(positionList);
	}

	void generateGrid(
		std::vector<Vector2>& positionList,
		integer xPoints,
		integer yPoints)
	{
		ENSURE_OP(xPoints, >, 0);
		ENSURE_OP(yPoints, >, 0);

		std::vector<Vector2> sampleList;

		for (integer y = 0;y < yPoints;++y)
		{
			for (integer x = 0;x < xPoints;++x)
			{
				sampleList.push_back(
					Vector2(dequantizeUnsigned(x, xPoints),
					dequantizeUnsigned(y, yPoints)));
			}
		}

		sampleList.swap(positionList);
	}

	void testReconstruction(
		const Array<Color, 2>& blurTexture,
		const std::vector<Vector2>& positionList,
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

		Array<Color, 2> image(blurTexture);
		for (integer i = 0;i < points;++i)
		{
			image(
				quantizeUnsigned(positionList[i].x(), image.width()),
				quantizeUnsigned(positionList[i].y(), image.height())) = 
				Color(1);
		}

		savePcx(image, "output/reconstruct_samples_" + integerToString(k, 2) +  "_" + name + ".pcx");

		/*
		for (integer kNearest = 1;kNearest <= 3;++kNearest)
		{
			reconstructNearest(
				positionList,
				dataList,
				AlignedBox2(0, 0, 1, 1),
				arrayView(image),
				kNearest);
		}

		for (integer kNearest = 1;kNearest <= 3;++kNearest)
		{
			reconstructNearest(
				positionList,
				dataList,
				AlignedBox2(0, 0, 1, 1),
				arrayView(image),
				kNearest, 
				0);

			savePcx(image, "output/reconstruct_nearest_" + integerToString(kNearest) + "_"
				+ integerToString(k, 2) + "_" + name + ".pcx");
		}
		*/

		for (integer maxRelativeError = 0;maxRelativeError <= 2;++maxRelativeError)
		{
			reconstructNearest(
				positionList,
				dataList,
				AlignedBox2(0, 0, 1, 1),
				arrayView(image),
				1,
				maxRelativeError);

			savePcx(image, "output/reconstruct_nearest_1_"
				+ integerToString(maxRelativeError) + "_"
				+ integerToString(k, 2) + "_" + name + ".pcx");
		}

		FilterPtr filter = tableFilter(gaussianFilter());

		reconstructFilter(
			positionList,
			dataList,
			AlignedBox2(0, 0, 1, 1),
			*filter,
			sampleDistance * (image.width() + 1),
			arrayView(image));

		savePcx(image, "output/reconstruct_filter_" + integerToString(k, 2) + "_" + name + ".pcx");
	}

	void testReconstruction()
	{
		Array<Color, 2> texture;
		loadPcx("lena.pcx", texture);

		for (integer k = 4;k <= 9;++k)
		{
			const integer xPoints = 1 << k;
			const real sampleDistance = (real)1 / (xPoints + 1);

			Array<Color, 2> blurTexture(texture.extent());
			std::vector<Vector2> positionList;

			resample<Color>(
				constArrayView(texture),
				clampExtender(),
				lanczosFilter(),
				arrayView(blurTexture), 
				(texture.width() + 1) * sampleDistance);

			generatePoisson(positionList, AlignedBox2(0, 0, 1, 1), sampleDistance);

			testReconstruction(
				blurTexture, positionList, "poisson",
				sampleDistance, k);

			generateGrid(positionList, xPoints, xPoints);

			testReconstruction(
				blurTexture, positionList, "grid",
				sampleDistance, k);
		}
	}

	template <int N, typename Data>
	class Shepard_Interpolator
	{
	public:
		Shepard_Interpolator(
			const std::vector<Vector<real, N> >& positionList,
			const std::vector<Data>& dataList)
			: positionList_(positionList)
			, dataList_(dataList)
		{
		}

		Data operator()(const Vector<real, N>& position) const
		{
			const integer n = positionList_.size();

			const real radius = 16;
			const real radius2 = square(radius);

			real weightSum = 0;
			Data dataSum(0);
			for (integer i = 0;i < n;++i)
			{
				const real distance2 = dot(positionList_[i] - position);
				if (distance2 == 0)
				{
					return dataList_[i];
				}

				//const real weight = inverse(distance2 * std::sqrt(distance2));

				if (distance2 > radius2)
				{
					continue;
				}

				const real r2 = distance2 / radius2;
				const real a = -1;
				const real weight = std::sqrt(std::sqrt(inverse(a * radius2) * (inverse(r2 - 2 * sqrt(r2)) + 1)));
				
				dataSum += dataList_[i] * weight;
				weightSum += weight;
			}

			return dataSum / weightSum;
		}

	private:
		const std::vector<Vector<real, N> >& positionList_;
		const std::vector<Data>& dataList_;
	};

	void testRbf()
	{
		Array<Color, 2> colorTexture;
		loadPcx("lena.pcx", colorTexture);

		Array<real32, 2> texture(colorTexture.extent());
		transform(constArrayView(colorTexture), arrayView(texture), luma);

		saveGrayscalePcx(texture, "test_rbf_input.pcx");

		const integer xPoints = texture.width() / 16;
		const real sampleDistance = (real)texture.width() / (xPoints + 1);

		std::vector<Vector2> positionList;
		generatePoisson(positionList, 
			AlignedBox2(Vector2(0), Vector2(texture.extent())), sampleDistance);

		const integer n = positionList.size();
		std::vector<real32> dataList;
		dataList.reserve(n);

		for (integer i = 0;i < n;++i)
		{
			dataList[i] = texture(
				Vector2i(positionList[i]));
		}

		FilterPtr filter(new MultiQuadric_Rbf(sampleDistance));

		reconstructRbf(
			positionList,
			dataList,
			AlignedBox2(Vector2(0), Vector2(texture.extent())),
			filter,
			arrayView(texture));

		saveGrayscalePcx(texture, "test_rbf_output.pcx");
	}

	void testShepard()
	{
		Array<Color, 2> texture;
		loadPcx("lena.pcx", texture);

		const integer xPoints = texture.width() / 8;
		const real sampleDistance = (real)texture.width() / (xPoints + 1);

		std::vector<Vector2> positionList;
		generatePoisson(positionList, 
			AlignedBox2(Vector2(0), Vector2(texture.extent())), sampleDistance);

		const integer n = positionList.size();
		std::vector<Color> dataList;
		dataList.reserve(n);

		for (integer i = 0;i < n;++i)
		{
			dataList[i] = texture(
				Vector2i(positionList[i]));
		}

		const Shepard_Interpolator<2, Color> interpolator(
			positionList, dataList);

		const integer width = texture.width();
		const integer height = texture.height();

		for (integer y = 0;y < height;++y)
		{
			for (integer x = 0;x < width;++x)
			{
				texture(x, y) = interpolator(Vector2(x + 0.5, y + 0.5));
			}
		}

		savePcx(texture, "test_shepard.pcx");
	}

	void testAdd()
	{
		gfxTestList().add("Reconstruction", testReconstruction);
		gfxTestList().add("Rbf", testRbf);
		gfxTestList().add("Shepard", testShepard);
	}

	CallFunction run(testAdd);

	void testReconstructionOld()
	{
		Array<Color, 2> image;

		loadPcx("ScMod_Tiff_Veeco.pcx", image);

		const integer width = image.width();
		const integer height = image.height();

		Array<bool, 2> mask(
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

		Array<Color, 2> maskImage(width, height);
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
		Array<real, 2> filter(
			HoleWidth, HoleWidth);
		setFilter(boost::bind(gaussian<real>, _1, (real)0.1), arrayView(filter));

		Array<Color, 2> result;
		reconstructImage(image, mask, filter, result);

		savePcx(result, "output/reconstructionresult.pcx");
	}

}
