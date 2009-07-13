// Description: Tools to compute histograms
// Documentation: math.txt

#ifndef PASTEL_HISTOGRAM_H
#define PASTEL_HISTOGRAM_H

#include "pastel/sys/view.h"

namespace Pastel
{

	template <typename Real, typename ConstIterator, typename OutputIterator>
	void computeHistogram(
		const ConstIterator& begin,
		const ConstIterator& end,
		const Real& min,
		const Real& max,
		integer bins,
		const OutputIterator& outputBegin);

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
		const View<2, Real, OutputView>& output);

}

#include "pastel/sys/histogram.hpp"

#endif
