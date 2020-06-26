// Description: Random distributions example
// DocumentationOf: random.h

#include "pastel_example.h"

#include "pastel/gfx/drawing.h"
#include "pastel/gfx/pcx.h"
#include "pastel/gfx/color_space.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/image_gfxrenderer.h"
#include "pastel/gfx/gfxrenderer_tools.h"

#include "pastel/sys/random.h"
#include "pastel/sys/views.h"
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
		const dreal xScaling = window.extent()[0] / bins;
		const dreal yScaling = window.extent()[1] / extent.y();

		dreal yPrevious = 0;
		for (integer x = 0;x <= bins;++x)
		{
			const dreal t = 
				region.min().x() + extent.x() * dequantizeUnsignedMatchEnds(x, bins + 1);

			const dreal y =
				window.min().y() + (function(t) - region.min().y()) * yScaling;

			const dreal xPosition = window.min().x() + (dreal)x * xScaling;

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
		const std::vector<dreal>& histogram,
		dreal yMax,
		const GfxRenderer<Color>& renderer)
	{
		const AlignedBox2 window = renderer.viewWindow();

		const integer bins = histogram.size();

		const dreal xMin = window.min().x();
		const dreal yMin = window.min().y();
		const dreal xDelta = window.extent()[0] / bins;

		const dreal scaling = window.extent()[1] / yMax;

		dreal xPosition = xMin;
		for (integer x = 0;x < bins;++x)
		{
			const dreal yPosition = yMin + histogram[x] * scaling;

			drawCircle(renderer,
				Sphere2(Vector2(xPosition + xDelta / 2,
				yPosition), 0.01));

			xPosition += xDelta;
		}
	}

	integer createHistogram(
		const std::vector<dreal>& sampleSet,
		std::vector<dreal>& resultHistogram,
		dreal min,
		dreal max,
		integer bins)
	{
		const integer samples = sampleSet.size();

		integer maxBinValue = 0;
		std::vector<dreal> histogram(bins, 0);
		for (integer i = 0;i < samples;++i)
		{
			dreal value = sampleSet[i];

			if (value >= min && value <= max)
			{
				value -= min;
				value /= (max - min);

				dreal& binValue = histogram[
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

		const dreal binSize = (max - min) / bins;
		const dreal factor = 1 / (binSize * samples);
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

		std::vector<dreal> sampleSet;
		sampleSet.reserve(samples);

		for (integer i = 0;i < samples;++i)
		{
			sampleSet.push_back(
				randomGenerator());
		}

		const integer bins = 100;

		std::vector<dreal> histogram(bins);
		computeHistogram(
			range(sampleSet.begin(), sampleSet.end()),
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
		Array<Color, 2> superImage(Vector2i(width * superSample, height * superSample));
		Array<Color, 2> image(Vector2i(width, height));

		Image_GfxRenderer<Color> renderer(&superImage);

		Color background(1);

		{
			AlignedBox2 region(-5, 0, 5, 1);

			renderer.setColor(background);
			renderer.clear();
			drawDistribution(
				region,
				boost::bind(randomGaussian<dreal>, 1),
				boost::bind(gaussianPdf<dreal>, _1, 1),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGaussian<dreal>, std::sqrt(0.2)),
				boost::bind(gaussianPdf<dreal>, _1, std::sqrt(0.2)),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGaussian<dreal>, std::sqrt(5.0)),
				boost::bind(gaussianPdf<dreal>, _1, std::sqrt(5.0)),
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
				boost::bind(randomGeneralizedGaussian<dreal>, 0.5, 1),
				boost::bind(generalizedGaussianPdf<dreal>, _1, 0.5, 1),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<dreal>, 1, 1),
				boost::bind(generalizedGaussianPdf<dreal>, _1, 1, 1),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<dreal>, 1.5, 1),
				boost::bind(generalizedGaussianPdf<dreal>, _1, 1.5, 1),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<dreal>, 2, 1),
				boost::bind(generalizedGaussianPdf<dreal>, _1, 2, 1),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<dreal>, 3, 1),
				boost::bind(generalizedGaussianPdf<dreal>, _1, 3, 1),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<dreal>, 8, 1),
				boost::bind(generalizedGaussianPdf<dreal>, _1, 8, 1),
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
				boost::bind(randomGeneralizedGaussian<dreal>, 0.5, 
				varianceToGeneralizedGaussianScale<dreal>(0.5, 1)),
				boost::bind(generalizedGaussianPdf<dreal>, _1, 0.5, 
				varianceToGeneralizedGaussianScale<dreal>(0.5, 1)),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<dreal>, 1, 
				varianceToGeneralizedGaussianScale<dreal>(1, 1)),
				boost::bind(generalizedGaussianPdf<dreal>, _1, 1, 
				varianceToGeneralizedGaussianScale<dreal>(1, 1)),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<dreal>, 1.5, 
				varianceToGeneralizedGaussianScale<dreal>(1.5, 1)),
				boost::bind(generalizedGaussianPdf<dreal>, _1, 1.5, 
				varianceToGeneralizedGaussianScale<dreal>(1.5, 1)),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<dreal>, 2, 
				varianceToGeneralizedGaussianScale<dreal>(2, 1)),
				boost::bind(generalizedGaussianPdf<dreal>, _1, 2, 
				varianceToGeneralizedGaussianScale<dreal>(2, 1)),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<dreal>, 3, 
				varianceToGeneralizedGaussianScale<dreal>(3, 1)),
				boost::bind(generalizedGaussianPdf<dreal>, _1, 3, 
				varianceToGeneralizedGaussianScale<dreal>(3, 1)),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGeneralizedGaussian<dreal>, 8, 
				varianceToGeneralizedGaussianScale<dreal>(8, 1)),
				boost::bind(generalizedGaussianPdf<dreal>, _1, 8, 
				varianceToGeneralizedGaussianScale<dreal>(8, 1)),
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
				boost::bind(randomExponential<dreal>, 0.5),
				boost::bind(exponentialPdf<dreal>, _1, 0.5),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomExponential<dreal>, 1),
				boost::bind(exponentialPdf<dreal>, _1, 1),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomExponential<dreal>, 1.5),
				boost::bind(exponentialPdf<dreal>, _1, 1.5),
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
				boost::bind(randomGamma<dreal>, 1, 2),
				boost::bind(gammaPdf<dreal>, _1, 1, 2),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGamma<dreal>, 2, 2),
				boost::bind(gammaPdf<dreal>, _1, 2, 2),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGamma<dreal>, 3, 2),
				boost::bind(gammaPdf<dreal>, _1, 3, 2),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGamma<dreal>, 5, 1),
				boost::bind(gammaPdf<dreal>, _1, 5, 1),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomGamma<dreal>, 9, 0.5),
				boost::bind(gammaPdf<dreal>, _1, 9, 0.5),
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
				boost::bind(randomLogNormal<dreal>, 0, (dreal)1 / 8),
				boost::bind(logNormalPdf<dreal>, _1, 0, (dreal)1 / 8),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomLogNormal<dreal>, 0, (dreal)1 / 4),
				boost::bind(logNormalPdf<dreal>, _1, 0, (dreal)1 / 4),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomLogNormal<dreal>, 0, (dreal)1 / 2),
				boost::bind(logNormalPdf<dreal>, _1, 0, (dreal)1 / 2),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomLogNormal<dreal>, 0, 1),
				boost::bind(logNormalPdf<dreal>, _1, 0, 1),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomLogNormal<dreal>, 0, (dreal)3 / 2),
				boost::bind(logNormalPdf<dreal>, _1, 0, (dreal)3 / 2),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomLogNormal<dreal>, 0, 10),
				boost::bind(logNormalPdf<dreal>, _1, 0, 10),
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
				boost::bind(randomLaplace<dreal>, 1),
				boost::bind(laplacePdf<dreal>, _1, 1),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomLaplace<dreal>, 2),
				boost::bind(laplacePdf<dreal>, _1, 2),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomLaplace<dreal>, 4),
				boost::bind(laplacePdf<dreal>, _1, 4),
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
				boost::bind(randomCauchy<dreal>, 0.5),
				boost::bind(cauchyPdf<dreal>, _1, 0.5),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomCauchy<dreal>, 1),
				boost::bind(cauchyPdf<dreal>, _1, 1),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomCauchy<dreal>, 2),
				boost::bind(cauchyPdf<dreal>, _1, 2),
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
				boost::bind(randomBeta<dreal>, 0.5, 0.5),
				boost::bind(betaPdf<dreal>, _1, 0.5, 0.5),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomBeta<dreal>, 5, 1),
				boost::bind(betaPdf<dreal>, _1, 5, 1),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomBeta<dreal>, 1, 3),
				boost::bind(betaPdf<dreal>, _1, 1, 3),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomBeta<dreal>, 2, 2),
				boost::bind(betaPdf<dreal>, _1, 2, 2),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomBeta<dreal>, 2, 5),
				boost::bind(betaPdf<dreal>, _1, 2, 5),
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
				boost::bind(randomChiSquared<dreal>, 1),
				boost::bind(chiSquaredPdf<dreal>, _1, 1),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomChiSquared<dreal>, 2),
				boost::bind(chiSquaredPdf<dreal>, _1, 2),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomChiSquared<dreal>, 3),
				boost::bind(chiSquaredPdf<dreal>, _1, 3),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomChiSquared<dreal>, 4),
				boost::bind(chiSquaredPdf<dreal>, _1, 4),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomChiSquared<dreal>, 5),
				boost::bind(chiSquaredPdf<dreal>, _1, 5),
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
				boost::bind(randomTriangle<dreal>, 1, 1),
				boost::bind(trianglePdf<dreal>, _1, 1, 1),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomTriangle<dreal>, 0.5, 0.5),
				boost::bind(trianglePdf<dreal>, _1, 0.5, 0.5),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomTriangle<dreal>, 0.75, 0.75),
				boost::bind(trianglePdf<dreal>, _1, 0.75, 0.75),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomTriangle<dreal>, 0.5, 0.75),
				boost::bind(trianglePdf<dreal>, _1, 0.5, 0.75),
				renderer);
			drawDistribution(
				region,
				boost::bind(randomTriangle<dreal>, 0.1, 1),
				boost::bind(trianglePdf<dreal>, _1, 0.1, 1),
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
			uniformRandomDistribution<dreal, 1>(), -3, 3, 1);
		savePcx(image, "random_uniform.pcx");

		clear(background, arrayView(image));
		drawDistribution(renderer, 
			scale(gammaRandomDistribution<dreal, 1>(1), Vector1(2)), 0, 20, 0.5);
		drawDistribution(renderer, 
			scale(gammaRandomDistribution<dreal, 1>(2), Vector1(2)), 0, 20, 0.5);
		drawDistribution(renderer, 
			scale(gammaRandomDistribution<dreal, 1>(3), Vector1(2)), 0, 20, 0.5);
		drawDistribution(renderer, 
			scale(gammaRandomDistribution<dreal, 1>(5), Vector1(1)), 0, 20, 0.5);
		drawDistribution(renderer, 
			scale(gammaRandomDistribution<dreal, 1>(9), Vector1(0.5)), 0, 20, 0.5);
		savePcx(image, "random_gamma.pcx");
		*/
	}

	void testGamma()
	{
		const integer width = 800;
		const integer height = 600;
		Array<Color, 2> image(Vector2i(width, height));
		Image_GfxRenderer<Color> renderer(&image);

		renderer.setColor(Color(0, 1, 0));

		drawGraph(
			AlignedBox2(-5, -5, 5, 5),
			gamma<dreal>,
			renderer);

		renderer.setColor(Color(0, 0, 1));

		drawGraph(
			AlignedBox2(-5, -5, 5, 5),
			lnGamma<dreal>,
			renderer);

		savePcx(image, "gamma.pcx");
	}

	void addTests()
	{
		testRunner().add("Random", drawDistributions);
		testRunner().add("Gamma", testGamma);
	}

	CallFunction run(addTests);

}
