// Description: Convolution on arrays

#ifndef PASTELGFX_CONVOLUTION_H
#define PASTELGFX_CONVOLUTION_H

#include "pastel/sys/view/view.h"

namespace Pastel
{

	//! Convolutes an image with a filter.
	/*!
	Preconditions:
	inputView.extent() == outputView.extent()

	inputView:
	The image to convolute.

	filterView:
	The filter to use.

	outputView:
	The resulting convoluted image.

	processFunctor:
	Dedices whether the input element contributes to
	the convolution result or not.
	bool processFunctor(const Input_RingElement& that);
	*/

	template <
		integer N,
		typename Input_RingElement,
		typename Input_ConstView,
		typename Filter_RingElement,
		typename Filter_ConstView,
		typename Output_RingElement,
		typename Output_View,
		typename ConvoluteProcessFunctor>
		void convolute(
		const ConstView<N, Input_RingElement, Input_ConstView>& inputView,
		const ConstView<N, Filter_RingElement, Filter_ConstView>& filterView,
		const View<N, Output_RingElement, Output_View>& outputView,
		const ConvoluteProcessFunctor& processFunctor);

	//! Convolutes an image with a filter.
	/*!
	Preconditions:
	inputView.extent() == outputView.extent()

	Calls 'convolute(inputView, filterView, outputView, skipZero)'
	where skipZero is a functor that skips zero valued input elements.
	*/

	template <
		integer N,
		typename Input_RingElement,
		typename Input_ConstView,
		typename Filter_RingElement,
		typename Filter_ConstView,
		typename Output_RingElement,
		typename Output_View>
		void convolute(
		const ConstView<N, Input_RingElement, Input_ConstView>& inputView,
		const ConstView<N, Filter_RingElement, Filter_ConstView>& filterView,
		const View<N, Output_RingElement, Output_View>& outputView);

	//! Convolutes an image with a varying size filter.
	/*!
	Preconditions:
	inputView.extent() == outputView.extent()

	inputView:
	The image to convolute.

	filterView:
	The filter to use.

	radiusView:
	Determines the filter radius at each pixel.

	outputView:
	The resulting convoluted image.
	*/

	template <
		typename Input_Element,
		typename Input_ConstView,
		typename Filter_Element,
		typename Filter_ConstView,
		typename Radius_Element,
		typename Radius_ConstView,
		typename Output_Element,
		typename Output_View,
		typename ConvoluteProcessFunctor>
		void generalizedConvolute(
		const ConstView<2, Input_Element, Input_ConstView>& inputView,
		const ConstView<2, Filter_Element, Filter_ConstView>& filterView,
		const ConstView<2, Radius_Element, Radius_ConstView>& radiusView,
		const View<2, Output_Element, Output_View>& outputView,
		ConvoluteProcessFunctor& processFunctor);

	template <
		typename Input_Element,
		typename Input_ConstView,
		typename Filter_Element,
		typename Filter_ConstView,
		typename Radius_Element,
		typename Radius_ConstView,
		typename Output_Element,
		typename Output_View>
		void generalizedConvolute(
		const ConstView<2, Input_Element, Input_ConstView>& inputView,
		const ConstView<2, Filter_Element, Filter_ConstView>& filterView,
		const ConstView<2, Radius_Element, Radius_ConstView>& radiusView,
		const View<2, Output_Element, Output_View>& outputView);

}

#include "pastel/gfx/image_processing/convolution.hpp"

#endif
