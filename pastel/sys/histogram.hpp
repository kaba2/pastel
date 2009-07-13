#ifndef PASTEL_HISTOGRAM_HPP
#define PASTEL_HISTOGRAM_HPP

#include "pastel/sys/histogram.h"
#include "pastel/sys/math_functions.h"
#include "pastel/sys/view_tools.h"

namespace Pastel
{

	template <typename Real, typename ConstIterator, typename OutputIterator>
	void computeHistogram(
		const ConstIterator& begin,
		const ConstIterator& end,
		const Real& min,
		const Real& max,
		integer bins,
		const OutputIterator& outputBegin)
	{
		ENSURE_OP(bins, >, 0);

		const Real rangeDelta = max - min;

		std::fill(
			outputBegin, outputBegin + bins, 0);

		ConstIterator iter = begin;
		integer samples = 0;

		while(iter != end)
		{
			Real value = *iter;

			if (value >= min && value <= max)
			{
				value -= min;
				value /= rangeDelta;

				const integer bin  =
					quantizeUnsigned(value, bins);

				++*(outputBegin + bin);
			}
			
			++iter;
			++samples;
		}

		// Normalize the pdf to 1.

		const Real binSize = (max - min) / bins;
		const Real factor = (Real)1 / (samples * binSize);
		
		OutputIterator outputIter = outputBegin;
		const OutputIterator outputEnd = outputBegin + bins;
		while(outputIter != outputEnd)
		{
			*outputIter *= factor;
			++outputIter;
		}
	}

	template <typename Real, typename ConstIterator, typename OutputView>
	void computeJointHistogram(
		const ConstIterator& xBegin,
		const ConstIterator& xEnd,
		const PASTEL_NO_DEDUCTION(Real)& xMin,
		const PASTEL_NO_DEDUCTION(Real)& xMax,
		const ConstIterator& yBegin,
		const ConstIterator& yEnd,
		const PASTEL_NO_DEDUCTION(Real)& yMin,
		const PASTEL_NO_DEDUCTION(Real)& yMax,
		const View<2, Real, OutputView>& output)
	{
		ENSURE_OP(output.width(), >, 0);
		ENSURE_OP(output.height(), >, 0);

		const Real xRangeDelta = xMax - xMin;
		const Real yRangeDelta = yMax - yMin;

		const integer xBins = output.width();
		const integer yBins = output.height();

		clear(0, output);

		ConstIterator xIter = xBegin;
		ConstIterator yIter = yBegin;
		integer samples = 0;

		while(xIter != xEnd)
		{
			ENSURE(yIter != yEnd);

			Real xValue = *xIter;

			if (xValue >= xMin && xValue <= xMax)
			{
				Real yValue = *yIter;

				if (yValue >= yMin && yValue <= yMax)
				{
					xValue -= xMin;
					xValue /= xRangeDelta;

					const integer xBin = 
						quantizeUnsigned(xValue, xBins);

					yValue -= yMin;
					yValue /= yRangeDelta;

					const integer yBin = 
						quantizeUnsigned(yValue, yBins);

					++output(xBin, yBin);
				}
			}
			
			++xIter;
			++yIter;
			++samples;
		}

		ENSURE(yIter == yEnd);

		// Normalize the pdf to 1.

		const Real binSize = (xRangeDelta * yRangeDelta) / (xBins * yBins);
		const Real factor = (Real)1 / (samples * binSize);

		for (integer y = 0;y < yBins;++y)
		{
			for (integer x = 0;x < xBins;++x)
			{
				output(x, y) *= factor;
			}
		}
	}

}

#endif
