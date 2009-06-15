#include "pastelsystest.h"

#include "pastel/gfx/drawing.h"
#include "pastel/gfx/pcx.h"
#include "pastel/gfx/color_tools.h"

#include "pastel/sys/randomdistribution.h"
#include "pastel/sys/view_all.h"

using namespace Pastel;

namespace
{

	template <typename OutputView>
	void drawHistogram(
		const std::vector<real>& histogram,
		real yMax,
		const View<2, Color, OutputView>& image)
	{
		const integer width = image.width();
		const integer height = image.height();

		const real scaling = (real)(height - 1) / yMax;

		Color color = hsvToRgb(
			Color(random<real32>(), 1, 1));

		for (integer x = 0;x < width;++x)
		{
			drawPixel(Point2(x, histogram[x] * scaling), color, 
				image);
		}
	}

	integer createHistogram(
		const std::vector<real>& sampleSet,
		std::vector<real>& resultHistogram,
		real min,
		real max,
		integer bins)
	{
		const integer samples = sampleSet.size();

		integer maxBinValue = 0;
		std::vector<real> histogram(bins, 0);
		for (integer i = 0;i < samples;++i)
		{
			real value = sampleSet[i];

			if (value >= min && value <= max)
			{
				value -= min;
				value /= (max - min);

				real& binValue = histogram[
					quantizeUnsigned(value, bins)];

				++binValue;
				if (binValue > maxBinValue)
				{
					maxBinValue = binValue;
				}
			}
		}

		// Normalize the probability mass distribution
		// to 1.

		const real binSize = (max - min) / bins;
		const real factor = 1 / (binSize * samples);
		for (integer i = 0;i < bins;++i)
		{
			histogram[i] *= factor;
		}

		histogram.swap(resultHistogram);
		
		return maxBinValue;
	}

	template <typename OutputView>
	void drawDistribution(
		const View<2, Color, OutputView>& image,
		const std::string& name,
		const std::vector<real>& sampleSet,
		real min, real max,
		real yMax)
	{
		std::vector<real> histogram;

		const integer maxBinValue = 
			createHistogram(sampleSet, histogram, min, max, image.width());
		drawHistogram(histogram, yMax, image);
	}

	template <typename OutputView>
	void drawDistribution(
		const View<2, Color, OutputView>& image,
		const CountedPtr<RandomDistribution<1, real> >& randomDistribution,
		real min, real max,
		real yMax)
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
			image,
			randomDistribution->name(),
			sampleSet,
			min, max, yMax);
	}

	void drawDistributions()
	{
		Array<2, Color> image(800, 600);

		clear(Color(0), arrayView(image));
		drawDistribution(arrayView(image), 
			scale(gaussianRandomDistribution<1, real>(), Vector1(1)), -5, 5, 1);
		drawDistribution(arrayView(image), 
			scale(gaussianRandomDistribution<1, real>(), Vector1(std::sqrt(0.2))), -5, 5, 1);
		drawDistribution(arrayView(image), 
			scale(gaussianRandomDistribution<1, real>(), Vector1(std::sqrt(5.0))), -5, 5, 1);
		savePcx(image, "random_gaussian.pcx");

		clear(Color(0), arrayView(image));
		drawDistribution(arrayView(image), 
			uniformRandomDistribution<1, real>(), -3, 3, 1);
		savePcx(image, "random_uniform.pcx");

		clear(Color(0), arrayView(image));
		drawDistribution(arrayView(image), 
			generalizedGaussianRandomDistribution<1, real>(0.5, 1), -3, 3, 0.6);
		drawDistribution(arrayView(image), 
			generalizedGaussianRandomDistribution<1, real>(1, 1), -3, 3, 0.6);
		drawDistribution(arrayView(image), 
			generalizedGaussianRandomDistribution<1, real>(1.5, 1), -3, 3, 0.6);
		drawDistribution(arrayView(image), 
			generalizedGaussianRandomDistribution<1, real>(2, 1), -3, 3, 0.6);
		drawDistribution(arrayView(image), 
			generalizedGaussianRandomDistribution<1, real>(3, 1), -3, 3, 0.6);
		drawDistribution(arrayView(image), 
			generalizedGaussianRandomDistribution<1, real>(8, 1), -3, 3, 0.6);
		savePcx(image, "random_generalized_gaussian.pcx");

		clear(Color(0), arrayView(image));
		drawDistribution(arrayView(image), 
			scale(exponentialRandomDistribution<1, real>(), Vector1(2)), 0, 5, 1.5);
		drawDistribution(arrayView(image), 
			scale(exponentialRandomDistribution<1, real>(), Vector1(1)), 0, 5, 1.5);
		drawDistribution(arrayView(image), 
			scale(exponentialRandomDistribution<1, real>(), Vector1((real)2 / 3)), 0, 5, 1.5);
		savePcx(image, "random_exponential.pcx");

		clear(Color(0), arrayView(image));
		drawDistribution(arrayView(image), 
			scale(gammaRandomDistribution<1, real>(1), Vector1(2)), 0, 20, 0.5);
		drawDistribution(arrayView(image), 
			scale(gammaRandomDistribution<1, real>(2), Vector1(2)), 0, 20, 0.5);
		drawDistribution(arrayView(image), 
			scale(gammaRandomDistribution<1, real>(3), Vector1(2)), 0, 20, 0.5);
		drawDistribution(arrayView(image), 
			scale(gammaRandomDistribution<1, real>(5), Vector1(1)), 0, 20, 0.5);
		drawDistribution(arrayView(image), 
			scale(gammaRandomDistribution<1, real>(9), Vector1(0.5)), 0, 20, 0.5);
		savePcx(image, "random_gamma.pcx");
	}

	void testGamma()
	{
		const integer width = 800;
		const integer height = 600;
		Array<2, Color> image(width, height);
		
		const real xMin = -5;
		const real xMax = 5;
		const real xDelta = xMax - xMin;
		const real yMin = -5;
		const real yMax = 5;
		const real yDelta = yMax - yMin;
		const real yScaling = (height - 1) / yDelta;

		real yPrevious = 1;
		for (integer x = 0;x < width;++x)
		{
			const real y =
				(gamma<real>(
				xMin + xDelta * dequantizeUnsigned(x, width - 1)) - yMin) * yScaling;
			
			drawSegment(
				Segment2(Point2(x - 1 + 0.5, yPrevious),
				Point2(x + 0.5, y)),
				Color(0, 1, 0),
				arrayView(image));

			yPrevious = y;
		}

		savePcx(image, "gamma.pcx");
	}

	void addTests()
	{
		sysTestList().add("Random.DrawDistributions", drawDistributions);
		sysTestList().add("Gamma", testGamma);
	}

	CallFunction run(addTests);

}
