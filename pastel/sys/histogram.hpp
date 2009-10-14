#ifndef PASTEL_HISTOGRAM_HPP
#define PASTEL_HISTOGRAM_HPP

#include "pastel/sys/histogram.h"
#include "pastel/sys/math_functions.h"
#include "pastel/sys/view_tools.h"

namespace Pastel
{

	template <typename Real_ConstIterator, typename Real_OutputIterator>
	void computeHistogram(
		const ForwardRange<Real_ConstIterator>& dataSet,
		const typename std::iterator_traits<Real_ConstIterator>::value_type& min,
		const typename std::iterator_traits<Real_ConstIterator>::value_type& max,
		integer bins,
		const Real_OutputIterator& outputBegin)
	{
		ENSURE_OP(bins, >, 0);

		typedef typename std::iterator_traits<Real_ConstIterator>::value_type
			Real;

		const Real rangeDelta = max - min;

		std::vector<Real> hitSet(bins, 0);

		Real_ConstIterator iter = dataSet.begin();
		const Real_ConstIterator iterEnd = dataSet.end();
		integer samples = 0;

		while(iter != iterEnd)
		{
			Real value = *iter;

			if (value >= min && value <= max)
			{
				value -= min;
				value /= rangeDelta;

				const integer bin  =
					quantizeUnsigned(value, bins);

				++hitSet[bin];
			}
			
			++iter;
			++samples;
		}

		// Normalize the pdf to 1.

		const Real binSize = (max - min) / bins;
		const Real factor = (Real)1 / (samples * binSize);
		for (integer i = 0;i < bins;++i)
		{
			hitSet[i] *= factor;
		}

		std::copy(hitSet.begin(), hitSet.end(),
			outputBegin);
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
