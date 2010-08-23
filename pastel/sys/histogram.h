// Description: Histograms
// Documentation: basic_math.txt

#ifndef PASTEL_HISTOGRAM_H
#define PASTEL_HISTOGRAM_H

#include "pastel/sys/view.h"
#include "pastel/sys/forwardrange.h"

#include <iterator>

namespace Pastel
{

	template <typename Real_ConstIterator, typename Real_OutputIterator>
	void computeHistogram(
		const ForwardRange<Real_ConstIterator>& dataSet,
		const typename std::iterator_traits<Real_ConstIterator>::value_type& min,
		const typename std::iterator_traits<Real_ConstIterator>::value_type& max,
		integer bins,
		const Real_OutputIterator& outputBegin);

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
