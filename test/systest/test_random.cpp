#include "pastelsystest.h"

#include "pastel/gfx/drawing.h"
#include "pastel/gfx/pcx.h"
#include "pastel/gfx/color_tools.h"

#include "pastel/sys/randomdistribution.h"
#include "pastel/sys/random_pdf.h"
#include "pastel/sys/view_all.h"
#include "pastel/sys/histogram.h"

#include <boost/bind.hpp>

using namespace Pastel;

namespace
{

	template <typename OutputView, typename Function>
	void drawGraph(
		const AlignedBox2& region,
		const Function& function,
		const Color& color,
		const View<2, Color, OutputView>& image)
	{
		const integer width = image.width();
		const integer height = image.height();

		const Vector2 extent = region.extent();
		const real yScaling = (height - 1) / extent.y();

		real yPrevious = 0;
		for (integer x = 0;x < width;++x)
		{
			const real t = 
				region.min().x() + extent.x() * dequantizeUnsigned(x, width - 1);

			const real y =
				(function(t) - region.min().y()) * yScaling;

			if (x > 0)
			{
				drawSegment(
					Segment2(Vector2(x - 1 + 0.5, yPrevious),
					Vector2(x + 0.5, y)),
					color,
					image);
			}

			yPrevious = y;
		}
	}

	template <typename OutputView>
	void drawHistogram(
		const std::vector<real>& histogram,
		real yMax,
		const Color& color,
		const View<2, Color, OutputView>& image)
	{
		const integer width = image.width();
		const integer height = image.height();

		const real scaling = (real)(height - 1) / yMax;

		for (integer x = 0;x < width;++x)
		{
			drawPixel(Vector2(x, histogram[x] * scaling), color, 
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

	template <typename OutputView, typename RandomGenerator, typename RandomPdf>
	void drawDistribution(
		const AlignedBox2& region,
		const RandomGenerator& randomGenerator,
		const RandomPdf& randomPdf,
		const View<2, Color, OutputView>& image)
	{
		const integer samples = 1000000;

		std::vector<real> sampleSet;
		sampleSet.reserve(samples);

		for (integer i = 0;i < samples;++i)
		{
			sampleSet.push_back(
				randomGenerator());
		}

		std::vector<real> histogram(image.width());
		computeHistogram(
			sampleSet.begin(), sampleSet.end(),
			region.min().x(), region.max().x(), image.width(),
			histogram.begin());

		/*
		const integer maxBinValue = 
			createHistogram(sampleSet, histogram, 
			region.min().x(), region.max().x(), image.width());
		*/

		Color color = hsvToRgb(
			Color(random<real32>(), 1, 1));

		drawHistogram(histogram, region.max().y(), color, image);
		drawGraph(region, randomPdf, color, image);
	}

	void drawDistributions()
	{
		Array<Color, 2> image(800, 600);

		{
			AlignedBox2 region(-5, 0, 5, 1);

			clear(Color(0), arrayView(image));
			drawDistribution(
				region,
				boost::bind(randomGaussian<real>, 0, 1),
				boost::bind(gaussianPdf<real>, _1, 0, 1),
				arrayView(image));
			drawDistribution(
				region,
				boost::bind(randomGaussian<real>, 0, std::sqrt(0.2)),
				boost::bind(gaussianPdf<real>, _1, 0, std::sqrt(0.2)),
				arrayView(image));
			drawDistribution(
				region,
				boost::bind(randomGaussian<real>, 0, std::sqrt(5.0)),
				boost::bind(gaussianPdf<real>, _1, 0, std::sqrt(5.0)),
				arrayView(image));
			savePcx(image, "random_gaussian.pcx");
		}

		{
			AlignedBox2 region(-3, 0, 3, 0.6);

			clear(Color(0), arrayView(image));
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<real>, 0.5, 1),
				boost::bind(generalizedGaussianPdf<real>, _1, 0.5, 1),
				arrayView(image));
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<real>, 1, 1),
				boost::bind(generalizedGaussianPdf<real>, _1, 1, 1),
				arrayView(image));
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<real>, 1.5, 1),
				boost::bind(generalizedGaussianPdf<real>, _1, 1.5, 1),
				arrayView(image));
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<real>, 2, 1),
				boost::bind(generalizedGaussianPdf<real>, _1, 2, 1),
				arrayView(image));
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<real>, 3, 1),
				boost::bind(generalizedGaussianPdf<real>, _1, 3, 1),
				arrayView(image));
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<real>, 8, 1),
				boost::bind(generalizedGaussianPdf<real>, _1, 8, 1),
				arrayView(image));
			savePcx(image, "random_generalized_gaussian.pcx");
		}

		{
			AlignedBox2 region(-3, 0, 3, 1);

			clear(Color(0), arrayView(image));
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<real>, 0.5, 
				varianceToGeneralizedGaussianScale<real>(0.5, 1)),
				boost::bind(generalizedGaussianPdf<real>, _1, 0.5, 
				varianceToGeneralizedGaussianScale<real>(0.5, 1)),
				arrayView(image));
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<real>, 1, 
				varianceToGeneralizedGaussianScale<real>(1, 1)),
				boost::bind(generalizedGaussianPdf<real>, _1, 1, 
				varianceToGeneralizedGaussianScale<real>(1, 1)),
				arrayView(image));
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<real>, 1.5, 
				varianceToGeneralizedGaussianScale<real>(1.5, 1)),
				boost::bind(generalizedGaussianPdf<real>, _1, 1.5, 
				varianceToGeneralizedGaussianScale<real>(1.5, 1)),
				arrayView(image));
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<real>, 2, 
				varianceToGeneralizedGaussianScale<real>(2, 1)),
				boost::bind(generalizedGaussianPdf<real>, _1, 2, 
				varianceToGeneralizedGaussianScale<real>(2, 1)),
				arrayView(image));
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<real>, 3, 
				varianceToGeneralizedGaussianScale<real>(3, 1)),
				boost::bind(generalizedGaussianPdf<real>, _1, 3, 
				varianceToGeneralizedGaussianScale<real>(3, 1)),
				arrayView(image));
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<real>, 8, 
				varianceToGeneralizedGaussianScale<real>(8, 1)),
				boost::bind(generalizedGaussianPdf<real>, _1, 8, 
				varianceToGeneralizedGaussianScale<real>(8, 1)),
				arrayView(image));
			savePcx(image, "random_generalized_gaussian2.pcx");
		}

		{
			AlignedBox2 region(0, 0, 5, 1.5);

			clear(Color(0), arrayView(image));
			drawDistribution(
				region,
				boost::bind(randomExponential<real>, 0.5),
				boost::bind(exponentialPdf<real>, _1, 0.5),
				arrayView(image));
			drawDistribution(
				region,
				boost::bind(randomExponential<real>, 1),
				boost::bind(exponentialPdf<real>, _1, 1),
				arrayView(image));
			drawDistribution(
				region,
				boost::bind(randomExponential<real>, 1.5),
				boost::bind(exponentialPdf<real>, _1, 1.5),
				arrayView(image));
			savePcx(image, "random_exponential.pcx");
		}
		
		{
			AlignedBox2 region(0, 0, 20, 0.5);

			clear(Color(0), arrayView(image));
			drawDistribution(
				region,
				boost::bind(randomGamma<real>, 1, 2),
				boost::bind(gammaPdf<real>, _1, 1, 2),
				arrayView(image));
			drawDistribution(
				region,
				boost::bind(randomGamma<real>, 2, 2),
				boost::bind(gammaPdf<real>, _1, 2, 2),
				arrayView(image));
			drawDistribution(
				region,
				boost::bind(randomGamma<real>, 3, 2),
				boost::bind(gammaPdf<real>, _1, 3, 2),
				arrayView(image));
			drawDistribution(
				region,
				boost::bind(randomGamma<real>, 5, 1),
				boost::bind(gammaPdf<real>, _1, 5, 1),
				arrayView(image));
			drawDistribution(
				region,
				boost::bind(randomGamma<real>, 9, 0.5),
				boost::bind(gammaPdf<real>, _1, 9, 0.5),
				arrayView(image));
			savePcx(image, "random_gamma.pcx");
		}

		/*
		drawDistribution(arrayView(image), 
			uniformRandomDistribution<1, real>(), -3, 3, 1);
		savePcx(image, "random_uniform.pcx");

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
		*/
	}

	void testGamma()
	{
		const integer width = 800;
		const integer height = 600;
		Array<Color, 2> image(width, height);

		drawGraph(
			AlignedBox2(-5, -5, 5, 5),
			gamma<real>,
			Color(0, 1, 0),
			arrayView(image));

		drawGraph(
			AlignedBox2(-5, -5, 5, 5),
			lnGamma<real>,
			Color(0, 0, 1),
			arrayView(image));

		savePcx(image, "gamma.pcx");
	}

	void addTests()
	{
		sysTestList().add("Random.DrawDistributions", drawDistributions);
		sysTestList().add("Gamma", testGamma);
	}

	CallFunction run(addTests);

}
