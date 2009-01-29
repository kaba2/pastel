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

	//! Resamples an image horizontally to a different width.
	/*!
	See the documentation for the more general resample()
	function. If you wish to resample vertically, use
	the TransponatedView class.
	*/

	template <
		typename Input_Element,
		typename Input_View,
		typename Output_Element,
		typename Output_View>
	void resampleHorizontal(
		const ConstView<2, Input_Element, Input_View>& input,
		const ConstIndexExtenderRef& indexExtender,
		const ConstFilterRef& filter,
		const View<2, Output_Element, Output_View>& output);

	//! Resamples an image to a different size.
	/*!
	This is a specialized function to save some typing when
	the same filter and index wrapper is used for resampling
	in both x and y directions. The effect is:

	resample(input, outputWidth, outputHeight,
		indexWrapper, indexWrapper,
		filter, filter, output);

	See the documentation for the more
	general resample() function.
	*/

	template <
		typename Input_Element,
		typename Input_View,
		typename Output_Element,
		typename Output_View>
		void resample(
		const ConstView<2, Input_Element, Input_View>& input,
		const ArrayExtender<2, PASTEL_NO_DEDUCTION(Input_Element)>& arrayExtender,
		const ConstFilterRef& filter,
		const View<2, Output_Element, Output_View>& output);

	//! Resamples an image to a different size.
	/*!
	The resampling is done by first resampling
	each row with a 1d filter and then resampling
	each resulting column with the same 1d filter.

	The advantage of doing this is that the
	performance of 2d filtering then becomes
	linear in filter radius (rather than quadratic).

	The disadvantage is that only separable 2d filters
	can be implemented this way. Although sufficient in most
	situations, such filters usually are not symmetric
	and thus create axis-aligned artifacts.
	There is only one filter that is both separable
	and symmetric: the gaussian filter.

	Note: this function is different from
	simply calling resampleHorizontal() and
	resampleVertical() in that this function
	performs them in an optimal order
	(w.r.t. performance).

	Note: the resampling process may yield
	negative values if the used filters have negative
	lobes. If the negative values do not make sense
	to you, you need decide what to do with them or
	use a filter that is non-negative everywhere.

	input:
	The image to be resampled.

	outputWidth:
	The width of the resulting image.

	outputHeight:
	The height of the resulting image.

	xIndexWrapper, yIndexWrapper:
	The index wrappers to use for
	x and y, respectively.
	Filters may reach over the boundaries
	of the input image. Index wrappers define
	what the image values are in such positions.
	The wrapping policy can be given separately
	for x and y axes.
	See the IndexWrapper concept.

	xFilter, yFilter:
	The 1d filters to use for resampling
	rows and columns, respectively.
	The used filter affects the resampling
	process. The best filter to use depends
	on the given task, and so the choice
	is given to the user. While a
	128-pixel-radius Lanczos filter might
	give very good frequency space behaviour,
	it is not visually good a image resampling
	filter because of serious ringing (however, the
	2-pixel-radius Lanczos filter is visually
	very pleasant).
	Remember optimality in frequency space
	is not optimality in perception.
	See the Filter class and its derived classes.

	output:
	The image to store the result to.

	Preconditions:
	outputWidth >= 0
	outputHeight >= 0

	Time complexity:
	Rule of thumb: linear in any sense

	Exception safety:
	strong
	*/

	template <
		typename Input_Element,
		typename Input_View,
		typename Output_Element,
		typename Output_View>
		void resample(
		const ConstView<2, Input_Element, Input_View>& input,
		const ArrayExtender<2, PASTEL_NO_DEDUCTION(Input_Element)> & arrayExtender,
		const ConstFilterRef& xFilter,
		const ConstFilterRef& yFilter,
		const View<2, Output_Element, Output_View>& output);

}

#include "pastel/gfx/resample.hpp"

#endif
