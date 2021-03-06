// Description: Histograms

#ifndef PASTELSYS_HISTOGRAM_H
#define PASTELSYS_HISTOGRAM_H

#include "pastel/sys/view/view.h"
#include "pastel/sys/range.h"

#include <iterator>

namespace Pastel
{

	template <typename Real_ConstRange, typename Real_OutputIterator>
	void computeHistogram(
		const Real_ConstRange& dataSet,
		const ranges::range_value_t<Real_ConstRange>& min,
		const ranges::range_value_t<Real_ConstRange>& max,
		integer bins,
		const Real_OutputIterator& outputBegin);

	template <typename Real, typename ConstIterator, typename OutputView>
	void computeJointHistogram(
		const ConstIterator& xBegin,
		const ConstIterator& xEnd,
		const NoDeduction<Real>& xMin,
		const NoDeduction<Real>& xMax,
		const ConstIterator& yBegin,
		const ConstIterator& yEnd,
		const NoDeduction<Real>& yMin,
		const NoDeduction<Real>& yMax,
		const View<2, Real, OutputView>& output);

}

#include "pastel/sys/histogram.hpp"

#endif
