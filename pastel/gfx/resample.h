/*!
\file
\brief Functions for resampling
*/

#ifndef PASTELGFX_RESAMPLE_H
#define PASTELGFX_RESAMPLE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/memoryview.h"
#include "pastel/sys/view.h"
#include "pastel/sys/arrayextender.h"

#include "pastel/gfx/filter.h"

namespace Pastel
{

	//! Resamples a 1-dimensional array to a different size.

	template <
		typename Input_Element,
		typename Input_View,
		typename Output_Element,
		typename Output_View>
	void resample(
		const ConstView<1, Input_Element, Input_View>& input,
		const PASTEL_NO_DEDUCTION((ArrayExtender<1, Input_Element>))& arrayExtender,
		const ConstFilterRef& filter,
		const View<1, Output_Element, Output_View>& output);

	//! Resamples a multi-dimensional array to a different size.
	/*!
	This function essentially calls the 1-dimensional
	array resampling function for each row of each axis.
	*/

	template <
		typename Computation_Element,
		int N,
		typename Input_Element,
		typename Input_View,
		typename Output_Element,
		typename Output_View>
		typename boost::enable_if_c<(N > 1), void>::type
		resample(
		const ConstView<N, Input_Element, Input_View>& input,
		const PASTEL_NO_DEDUCTION((ArrayExtender<N, Input_Element>))& arrayExtender,
		const ConstFilterRef& filter,
		const View<N, Output_Element, Output_View>& output);

}

#include "pastel/gfx/resample.hpp"

#endif
