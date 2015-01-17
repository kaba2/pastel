// Description: Algorithms for array views

#ifndef PASTELSYS_VIEW_TOOLS_H
#define PASTELSYS_VIEW_TOOLS_H

#include "pastel/sys/view/view.h"

namespace Pastel
{

	template <int N, typename Type, typename Image_View>
	void clear(
		const NoDeduction<Type>& color,
		const View<N, Type, Image_View>& image);

	template <
		int N,
		typename Input_Element,
		typename Input_ConstView,
		typename Output_Element,
		typename Output_View>
		void copy(
		const ConstView<N, Input_Element, Input_ConstView>& input,
		const View<N, Output_Element, Output_View>& output);

	template <
		int N,
		typename Input_Element,
		typename Input_ConstView,
		typename Output_Element,
		typename Output_View,
		typename TransformFunctor>
		void transform(
		const ConstView<N, Input_Element, Input_ConstView>& input,
		const View<N, Output_Element, Output_View>& output,
		const TransformFunctor& transform);

	template <
		int N,
		typename Output_Element,
		typename Output_View,
		typename TransformFunctor>
		void transform(
		const View<N, Output_Element, Output_View>& output,
		const TransformFunctor& transform);

}

#include "pastel/sys/view/view_tools.hpp"

#endif
