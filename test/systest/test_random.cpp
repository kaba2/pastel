#include "pastelsystest.h"

#include "pastel/gfx/draw.h"
#include "pastel/gfx/pcx.h"
#include "pastel/gfx/color_space.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/image_gfxrenderer.h"
#include "pastel/gfx/gfxrenderer_tools.h"

#include "pastel/sys/randomdistribution.h"
#include "pastel/sys/random.h"
#include "pastel/sys/view_all.h"
#include "pastel/sys/histogram.h"

#include <boost/bind.hpp>

using namespace Pastel;

namespace
{

	template <typename Function>
	void drawGraph(
		const AlignedBox2& region,
		const Function& function,
		const GfxRenderer<Color>& renderer)
	{
		const integer bins = 500;

		const AlignedBox2 window = renderer.viewWindow();

		const Vector2 extent = region.extent();
		const real xScaling = window.extent()[0] / bins;
		const real yScaling = window.extent()[1] / extent.y();

		real yPrevious = 0;
		for (integer x = 0;x <= bins;++x)
		{
			const real t = 
				region.min().x() + extent.x() * dequantizeUnsignedMatchEnds(x, bins + 1);

			const real y =
				window.min().y() + (function(t) - region.min().y()) * yScaling;

			const real xPosition = window.min().x() + (real)x * xScaling;

			if (x > 0)
			{
				drawFatSegment(
					renderer,
					Segment2(Vector2(xPosition - xScaling, yPrevious),
					Vector2(xPosition, y)),
					0.005, 0.005);
				
				/*
				drawSegment(
					Segment2(Vector2(xPosition - xScaling, yPrevious),
					Vector2(xPosition, y)),
					color,
					image);
				*/
			}

			yPrevious = y;
		}
	}

	void drawHistogram(
		const std::vector<real>& histogram,
		real yMax,
		const GfxRenderer<Color>& renderer)
	{
		const AlignedBox2 window = renderer.viewWindow();

		const integer bins = histogram.size();

		const real xMin = window.min().x();
		const real yMin = window.min().y();
		const real xDelta = window.extent()[0] / bins;

		const real scaling = window.extent()[1] / yMax;

		real xPosition = xMin;
		for (integer x = 0;x < bins;++x)
		{
			const real yPosition = yMin + histogram[x] * scaling;

			drawCircle(renderer,
				Sphere2(Vector2(xPosition + xDelta / 2,
				yPosition), 0.01));

			xPosition += xDelta;
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

	template <typename RandomGenerator, typename RandomPdf>
	void drawDistribution(
		const AlignedBox2& region,
		const RandomGenerator& randomGenerator,
		const RandomPdf& randomPdf,
		GfxRenderer<Color>& renderer)
	{
		const integer samples = 100000;

		std::vector<real> sampleSet;
		sampleSet.reserve(samples);

		for (integer i = 0;i < samples;++i)
		{
			sampleSet.push_back(
				randomGenerator());
		}

		const integer bins = 100;

		std::vector<real> histogram(bins);
		computeHistogram(
			forwardRange(sampleSet.begin(), sampleSet.end()),
			region.min().x(), region.max().x(), bins,
			histogram.begin());

		/*
		const integer maxBinValue = 
			createHistogram(sampleSet, histogram, 
			region.min().x(), region.max().x(), image.width());
		*/

		Color color = hsvToRgb(
			Color(random<real32>(), 1, 0.75));

		renderer.setColor(color * 0.75);
		drawHistogram(histogram, region.max().y(), renderer);

		renderer.setColor(color);
		renderer.setFilled(true);
		drawGraph(region, randomPdf, renderer);
	}

	void drawDistributions()
	{
		const integer superSample = 4;
		const integer width = 512;
		const integer height = 384;
		Array<Color, 2> superImage(width * superSample, height * superSample);
		Array<Color, 2> image(width, height);

		Image_GfxRenderer<Color> renderer(&superImage);

		Color background(1);

		{
			AlignedBox2 region(-5, 0, 5, 1);

			renderer.setColor(background);
			renderer.clear();
			drawDistribution(
				region,
				boost::bind(randomGaussian<real>, 1),
				boost::bind(gaussianPdf<real>, _1, 1),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGaussian<real>, std::sqrt(0.2)),
				boost::bind(gaussianPdf<real>, _1, std::sqrt(0.2)),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGaussian<real>, std::sqrt(5.0)),
				boost::bind(gaussianPdf<real>, _1, std::sqrt(5.0)),
				renderer);

			resample<Color>(
				constArrayView(superImage),
				ArrayExtender<2, Color>(clampExtender()),
				lanczosFilter(2),
				arrayView(image));
			transform(arrayView(image), fitColor);
			savePcx(image, "random_gaussian.pcx");
		}

		{
			AlignedBox2 region(-3, 0, 3, 0.6);

			renderer.setColor(background);
			renderer.clear();
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<real>, 0.5, 1),
				boost::bind(generalizedGaussianPdf<real>, _1, 0.5, 1),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<real>, 1, 1),
				boost::bind(generalizedGaussianPdf<real>, _1, 1, 1),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<real>, 1.5, 1),
				boost::bind(generalizedGaussianPdf<real>, _1, 1.5, 1),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<real>, 2, 1),
				boost::bind(generalizedGaussianPdf<real>, _1, 2, 1),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<real>, 3, 1),
				boost::bind(generalizedGaussianPdf<real>, _1, 3, 1),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<real>, 8, 1),
				boost::bind(generalizedGaussianPdf<real>, _1, 8, 1),
				renderer);

			resample<Color>(
				constArrayView(superImage),
				ArrayExtender<2, Color>(clampExtender()),
				lanczosFilter(2),
				arrayView(image));
			transform(arrayView(image), fitColor);
			savePcx(image, "random_generalized_gaussian.pcx");
		}

		{
			AlignedBox2 region(-3, 0, 3, 1);

			renderer.setColor(background);
			renderer.clear();
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<real>, 0.5, 
				varianceToGeneralizedGaussianScale<real>(0.5, 1)),
				boost::bind(generalizedGaussianPdf<real>, _1, 0.5, 
				varianceToGeneralizedGaussianScale<real>(0.5, 1)),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<real>, 1, 
				varianceToGeneralizedGaussianScale<real>(1, 1)),
				boost::bind(generalizedGaussianPdf<real>, _1, 1, 
				varianceToGeneralizedGaussianScale<real>(1, 1)),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<real>, 1.5, 
				varianceToGeneralizedGaussianScale<real>(1.5, 1)),
				boost::bind(generalizedGaussianPdf<real>, _1, 1.5, 
				varianceToGeneralizedGaussianScale<real>(1.5, 1)),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<real>, 2, 
				varianceToGeneralizedGaussianScale<real>(2, 1)),
				boost::bind(generalizedGaussianPdf<real>, _1, 2, 
				varianceToGeneralizedGaussianScale<real>(2, 1)),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<real>, 3, 
				varianceToGeneralizedGaussianScale<real>(3, 1)),
				boost::bind(generalizedGaussianPdf<real>, _1, 3, 
				varianceToGeneralizedGaussianScale<real>(3, 1)),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<real>, 8, 
				varianceToGeneralizedGaussianScale<real>(8, 1)),
				boost::bind(generalizedGaussianPdf<real>, _1, 8, 
				varianceToGeneralizedGaussianScale<real>(8, 1)),
				renderer);

			resample<Color>(
				constArrayView(superImage),
				ArrayExtender<2, Color>(clampExtender()),
				lanczosFilter(2),
				arrayView(image));
			transform(arrayView(image), fitColor);
			savePcx(image, "random_generalized_gaussian2.pcx");
		}

		{
			AlignedBox2 region(0, 0, 5, 1.5);

			renderer.setColor(background);
			renderer.clear();
			drawDistribution(
				region,
				boost::bind(randomExponential<real>, 0.5),
				boost::bind(exponentialPdf<real>, _1, 0.5),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomExponential<real>, 1),
				boost::bind(exponentialPdf<real>, _1, 1),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomExponential<real>, 1.5),
				boost::bind(exponentialPdf<real>, _1, 1.5),
				renderer);

			resample<Color>(
				constArrayView(superImage),
				ArrayExtender<2, Color>(clampExtender()),
				lanczosFilter(2),
				arrayView(image));
			transform(arrayView(image), fitColor);
			savePcx(image, "random_exponential.pcx");
		}
		
		{
			AlignedBox2 region(0, 0, 20, 0.5);

			renderer.setColor(background);
			renderer.clear();
			drawDistribution(
				region,
				boost::bind(randomGamma<real>, 1, 2),
				boost::bind(gammaPdf<real>, _1, 1, 2),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGamma<real>, 2, 2),
				boost::bind(gammaPdf<real>, _1, 2, 2),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGamma<real>, 3, 2),
				boost::bind(gammaPdf<real>, _1, 3, 2),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGamma<real>, 5, 1),
				boost::bind(gammaPdf<real>, _1, 5, 1),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGamma<real>, 9, 0.5),
				boost::bind(gammaPdf<real>, _1, 9, 0.5),
				renderer);

			resample<Color>(
				constArrayView(superImage),
				ArrayExtender<2, Color>(clampExtender()),
				lanczosFilter(2),
				arrayView(image));
			transform(arrayView(image), fitColor);
			savePcx(image, "random_gamma.pcx");
		}

		{
			AlignedBox2 region(0, 0, 3, 2);

			renderer.setColor(background);
			renderer.clear();
			drawDistribution(
				region,
				boost::bind(randomLogNormal<real>, 0, (real)1 / 8),
				boost::bind(logNormalPdf<real>, _1, 0, (real)1 / 8),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomLogNormal<real>, 0, (real)1 / 4),
				boost::bind(logNormalPdf<real>, _1, 0, (real)1 / 4),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomLogNormal<real>, 0, (real)1 / 2),
				boost::bind(logNormalPdf<real>, _1, 0, (real)1 / 2),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomLogNormal<real>, 0, 1),
				boost::bind(logNormalPdf<real>, _1, 0, 1),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomLogNormal<real>, 0, (real)3 / 2),
				boost::bind(logNormalPdf<real>, _1, 0, (real)3 / 2),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomLogNormal<real>, 0, 10),
				boost::bind(logNormalPdf<real>, _1, 0, 10),
				renderer);

			resample<Color>(
				constArrayView(superImage),
				ArrayExtender<2, Color>(clampExtender()),
				lanczosFilter(2),
				arrayView(image));
			transform(arrayView(image), fitColor);
			savePcx(image, "random_lognormal.pcx");
		}

		{
			AlignedBox2 region(-10, 0, 10, 0.5);

			renderer.setColor(background);
			renderer.clear();
			drawDistribution(
				region,
				boost::bind(randomLaplace<real>, 1),
				boost::bind(laplacePdf<real>, _1, 1),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomLaplace<real>, 2),
				boost::bind(laplacePdf<real>, _1, 2),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomLaplace<real>, 4),
				boost::bind(laplacePdf<real>, _1, 4),
				renderer);

			resample<Color>(
				constArrayView(superImage),
				ArrayExtender<2, Color>(clampExtender()),
				lanczosFilter(2),
				arrayView(image));
			transform(arrayView(image), fitColor);
			savePcx(image, "random_laplace.pcx");
		}

		{
			AlignedBox2 region(-5, 0, 5, 0.7);

			renderer.setColor(background);
			renderer.clear();
			drawDistribution(
				region,
				boost::bind(randomCauchy<real>, 0.5),
				boost::bind(cauchyPdf<real>, _1, 0.5),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomCauchy<real>, 1),
				boost::bind(cauchyPdf<real>, _1, 1),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomCauchy<real>, 2),
				boost::bind(cauchyPdf<real>, _1, 2),
				renderer);

			resample<Color>(
				constArrayView(superImage),
				ArrayExtender<2, Color>(clampExtender()),
				lanczosFilter(2),
				arrayView(image));
			transform(arrayView(image), fitColor);
			savePcx(image, "random_cauchy.pcx");
		}

		{
			AlignedBox2 region(0, 0, 1, 2.6);

			renderer.setColor(background);
			renderer.clear();
			drawDistribution(
				region,
				boost::bind(randomBeta<real>, 0.5, 0.5),
				boost::bind(betaPdf<real>, _1, 0.5, 0.5),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomBeta<real>, 5, 1),
				boost::bind(betaPdf<real>, _1, 5, 1),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomBeta<real>, 1, 3),
				boost::bind(betaPdf<real>, _1, 1, 3),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomBeta<real>, 2, 2),
				boost::bind(betaPdf<real>, _1, 2, 2),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomBeta<real>, 2, 5),
				boost::bind(betaPdf<real>, _1, 2, 5),
				renderer);

			resample<Color>(
				constArrayView(superImage),
				ArrayExtender<2, Color>(clampExtender()),
				lanczosFilter(2),
				arrayView(image));
			transform(arrayView(image), fitColor);
			savePcx(image, "random_beta.pcx");
		}

		{
			AlignedBox2 region(0, 0, 8, 1);

			renderer.setColor(background);
			renderer.clear();
			drawDistribution(
				region,
				boost::bind(randomChiSquared<real>, 1),
				boost::bind(chiSquaredPdf<real>, _1, 1),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomChiSquared<real>, 2),
				boost::bind(chiSquaredPdf<real>, _1, 2),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomChiSquared<real>, 3),
				boost::bind(chiSquaredPdf<real>, _1, 3),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomChiSquared<real>, 4),
				boost::bind(chiSquaredPdf<real>, _1, 4),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomChiSquared<real>, 5),
				boost::bind(chiSquaredPdf<real>, _1, 5),
				renderer);

			resample<Color>(
				constArrayView(superImage),
				ArrayExtender<2, Color>(clampExtender()),
				lanczosFilter(2),
				arrayView(image));
			transform(arrayView(image), fitColor);
			savePcx(image, "random_chisquared.pcx");
		}

		{
			AlignedBox2 region(-1, 0, 1, 2);

			renderer.setColor(background);
			renderer.clear();
			drawDistribution(
				region,
				boost::bind(randomTriangle<real>, 1, 1),
				boost::bind(trianglePdf<real>, _1, 1, 1),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomTriangle<real>, 0.5, 0.5),
				boost::bind(trianglePdf<real>, _1, 0.5, 0.5),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomTriangle<real>, 0.75, 0.75),
				boost::bind(trianglePdf<real>, _1, 0.75, 0.75),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomTriangle<real>, 0.5, 0.75),
				boost::bind(trianglePdf<real>, _1, 0.5, 0.75),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomTriangle<real>, 0.1, 1),
				boost::bind(trianglePdf<real>, _1, 0.1, 1),
				renderer);

			resample<Color>(
				constArrayView(superImage),
				ArrayExtender<2, Color>(clampExtender()),
				lanczosFilter(2),
				arrayView(image));
			transform(arrayView(image), fitColor);
			savePcx(image, "random_triangle.pcx");
		}

		/*
		drawDistribution(renderer, 
			uniformRandomDistribution<real, 1>(), -3, 3, 1);
		savePcx(image, "random_uniform.pcx");

		clear(background, arrayView(image));
		drawDistribution(renderer, 
			scale(gammaRandomDistribution<real, 1>(1), Vector1(2)), 0, 20, 0.5);
		drawDistribution(renderer, 
			scale(gammaRandomDistribution<real, 1>(2), Vector1(2)), 0, 20, 0.5);
		drawDistribution(renderer, 
			scale(gammaRandomDistribution<real, 1>(3), Vector1(2)), 0, 20, 0.5);
		drawDistribution(renderer, 
			scale(gammaRandomDistribution<real, 1>(5), Vector1(1)), 0, 20, 0.5);
		drawDistribution(renderer, 
			scale(gammaRandomDistribution<real, 1>(9), Vector1(0.5)), 0, 20, 0.5);
		savePcx(image, "random_gamma.pcx");
		*/
	}

	void testGamma()
	{
		const integer width = 800;
		const integer height = 600;
		Array<Color, 2> image(width, height);
		Image_GfxRenderer<Color> renderer(&image);

		renderer.setColor(Color(0, 1, 0));

		drawGraph(
			AlignedBox2(-5, -5, 5, 5),
			gamma<real>,
			renderer);

		renderer.setColor(Color(0, 0, 1));

		drawGraph(
			AlignedBox2(-5, -5, 5, 5),
			lnGamma<real>,
			renderer);

		savePcx(image, "gamma.pcx");
	}

	void addTests()
	{
		sysTestList().add("Random.DrawDistributions", drawDistributions);
		sysTestList().add("Gamma", testGamma);
	}

	CallFunction run(addTests);

}
