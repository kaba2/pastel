#ifndef PASTELSYS_VIEW_TOOLS_HPP
#define PASTELSYS_VIEW_TOOLS_HPP

#include "pastel/sys/view/view_tools.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/view/view_visit.h"

namespace Pastel
{

	namespace Clear_
	{

		template <typename Type, typename Reference>
		class Visitor
		{
		public:
			explicit Visitor(
				const Type& value)
				: value_(value)
			{
			}

			void operator()(Reference that) const
			{
				that = value_;
			}

		private:
			Type value_;
		};

	}

	template <int N, typename Type, typename Image_View>
	void clear(
		const NoDeduction<Type>& color,
		const View<N, Type, Image_View>& image)
	{
		Clear_::Visitor<Type, typename Image_View::Reference> visitor(color);
		visit(image, visitor);
	}

	namespace Copy_
	{

		template <typename Input_ConstReference, typename Output_Reference>
		class Visitor
		{
		public:
			void operator()(
				Input_ConstReference input,
				Output_Reference output) const
			{
				output = input;
			}
		};

	}

	template <
		int N,
		typename Input_Element,
		typename Input_ConstView,
		typename Output_Element,
		typename Output_View>
		void copy(
		const ConstView<N, Input_Element, Input_ConstView>& input,
		const View<N, Output_Element, Output_View>& output)
	{
		Copy_::Visitor<
			typename Input_ConstView::ConstReference,
			typename Output_View::Reference> visitor;
		visit(input, output, visitor);
	}

	namespace Transform_
	{

		template <typename Input_ConstReference, typename Output_Reference,
		typename TransformFunctor>
		class Visitor
		{
		public:
			explicit Visitor(
				const TransformFunctor& transform)
				: transform_(transform)
			{
			}

			void operator()(
				Input_ConstReference input,
				Output_Reference output) const
			{
				output = transform_(input);
			}

		private:
			const TransformFunctor& transform_;
		};

	}

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
		const TransformFunctor& transform)
	{
		Transform_::Visitor<
			typename Input_ConstView::ConstReference,
			typename Output_View::Reference, TransformFunctor>
			visitor(transform);
		visit(input, output, visitor);
	}

	template <
		int N,
		typename Output_Element,
		typename Output_View,
		typename TransformFunctor>
		void transform(
		const View<N, Output_Element, Output_View>& output,
		const TransformFunctor& transform)
	{
		Pastel::transform(output, output, transform);
	}

}

#endif
