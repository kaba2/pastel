#ifndef PASTELSYS_VIEW_TOOLS_HPP
#define PASTELSYS_VIEW_TOOLS_HPP

#include "pastel/sys/view_tools.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/view_visit.h"

namespace Pastel
{

	namespace Detail_Clear
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
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<N, Type, Image_View>& image)
	{
		Detail_Clear::Visitor<Type, typename Image_View::Reference> visitor(color);
		visit(image, visitor);
	}

	namespace Detail_Copy
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
		Detail_Copy::Visitor<
			typename Input_ConstView::ConstReference,
			typename Output_View::Reference> visitor;
		visit(input, output, visitor);
	}

	namespace Detail_Transform
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
				Output_Reference& output) const
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
		Detail_Transform::Visitor<Input_Element, Output_Element, TransformFunctor>
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
