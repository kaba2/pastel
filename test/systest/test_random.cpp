#include "pastelsystest.h"

#include "pastel/gfx/drawing.h"
#include "pastel/gfx/pcx.h"

#include "pastel/sys/randomdistribution.h"
#include "pastel/sys/view_all.h"

using namespace Pastel;

namespace
{

	void drawHistogram(
		const std::vector<integer>& histogram,
		const integer maxBinValue,
		Array<2, Color>& resultImage)
	{
		const integer width = histogram.size();
		const integer height = 600;

		Array<2, Color> image(width, height);

		const real scaling = (real)(height - 1) / maxBinValue;

		for (integer x = 0;x < width;++x)
		{
			drawPixel(Point2(x, histogram[x] * scaling), Color(0, 1, 0), 
				arrayView(image));
		}

		image.swap(resultImage);
	}

	integer createHistogram(
		const std::vector<real>& sampleSet,
		std::vector<integer>& resultHistogram,
		real min,
		real max,
		integer bins)
	{
		const integer samples = sampleSet.size();

		integer maxBinValue = 0;
		std::vector<integer> histogram(bins, 0);
		for (integer i = 0;i < samples;++i)
		{
			real value = sampleSet[i];

			if (value >= min && value <= max)
			{
				value -= min;
				value /= (max - min);

				integer& binValue = histogram[
					quantizeUnsigned(value, bins)];

				++binValue;
				if (binValue > maxBinValue)
				{
					maxBinValue = binValue;
				}
			}
		}

		histogram.swap(resultHistogram);
		
		return maxBinValue;
	}

	void drawDistribution(
		const std::string& name,
		const std::vector<real>& sampleSet,
		real min, real max)
	{
		const integer bins = 800;

		Array<2, Color> image;
		std::vector<integer> histogram;

		const integer maxBinValue = 
			createHistogram(sampleSet, histogram, min, max, bins);
		drawHistogram(histogram, maxBinValue, image);
		savePcx(image, "random_" + name + ".pcx");
	}

	void drawDistribution(
		const CountedPtr<RandomDistribution<1, real> >& randomDistribution,
		real min, real max)
	{
		const integer samples = 1000000;

		std::vector<real> sampleSet;
		sampleSet.reserve(samples);

		for (integer i = 0;i < samples;++i)
		{
			sampleSet.push_back(
				randomDistribution->sample()[0]);
		}

		drawDistribution(
			randomDistribution->name(),
			sampleSet, 
			min, max);
	}

	void drawDistributions()
	{
		drawDistribution(gaussianRandomDistribution<1, real>(), -2, 2);
		drawDistribution(uniformRandomDistribution<1, real>(), -2, 2);
		drawDistribution(generalizedGaussianRandomDistribution<1, real>(2, 2), -2, 2);
		drawDistribution(exponentialRandomDistribution<1, real>(), 0, 4);
		//drawDistribution(gammaRandomDistribution<1, real>(9), 0, 4);
	}

	void addTests()
	{
		sysTestList().add("Random.DrawDistributions", drawDistributions);
	}

	CallFunction run(addTests);

}
