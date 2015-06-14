// Description: ExtendedConstView class
// Documentation: extendedconstviews.txt

#ifndef PASTELSYS_EXTENDEDCONSTVIEW_H
#define PASTELSYS_EXTENDEDCONSTVIEW_H

#include "pastel/sys/view/view.h"

namespace Pastel
{

	namespace ExtendedConstView_
	{

		template <integer N, typename Contained_Element, typename Contained_ExtendedConstView>
		class ExtendedConstViewBase
		{
		public:
			static constexpr int Dimension = N;

			using Element = Contained_Element;

			ExtendedConstViewBase()
				: view_()
			{
			}

			explicit ExtendedConstViewBase(
				const Contained_ExtendedConstView& view)
				: view_(view)
			{
			}

			bool empty() const
			{
				return anyEqual(view_.extent(), 0);
			}

			const Vector<integer, N>& extent() const
			{
				return view_.extent();
			}

			const Element& operator()(
				const Vector<integer, N>& position) const
			{
				return view_(position);
			}

			// FIX: Should be private
			Contained_ExtendedConstView view_;
		};

	}

	template <integer N, typename Contained_Element, typename Contained_ExtendedConstView>
	class ExtendedConstView
		: public ExtendedConstView_::ExtendedConstViewBase<N, Contained_Element, Contained_ExtendedConstView>
	{
	private:
		typedef ExtendedConstView_::
			ExtendedConstViewBase<N, Contained_Element, Contained_ExtendedConstView>
			Base;

	public:
		static constexpr int Dimension = N;

		using Element = typename Base::Element;
		using Base::extent;
		using Base::operator();

		ExtendedConstView()
			: Base()
		{
		}

		explicit ExtendedConstView(
			const Contained_ExtendedConstView& view)
			: Base(view)
		{
		}
	};

	template <typename Contained_Element, typename Contained_ExtendedConstView>
	class ExtendedConstView<1, Contained_Element, Contained_ExtendedConstView>
		: public ExtendedConstView_::ExtendedConstViewBase<1, Contained_Element, Contained_ExtendedConstView>
	{
	private:
		static constexpr integer N = 1;

		typedef ExtendedConstView_::
			ExtendedConstViewBase<N, Contained_Element, Contained_ExtendedConstView>
			Base;

	public:
		static constexpr int Dimension = N;

		using Element = typename Base::Element;
		using Base::extent;
		using Base::operator();

		ExtendedConstView()
			: Base()
		{
		}

		explicit ExtendedConstView(
			const Contained_ExtendedConstView& view)
			: Base(view)
		{
		}

		integer width() const
		{
			return Base::extent()[0];
		}

		const Element& operator()(integer x) const
		{
			return Base::operator()(Vector<integer, N>(x));
		}
	};

	template <typename Contained_Element, typename Contained_ExtendedConstView>
	class ExtendedConstView<2, Contained_Element, Contained_ExtendedConstView>
		: public ExtendedConstView_::ExtendedConstViewBase<2, Contained_Element, Contained_ExtendedConstView>
	{
	private:
		static constexpr integer N = 2;

		typedef ExtendedConstView_::
			ExtendedConstViewBase<N, Contained_Element, Contained_ExtendedConstView>
			Base;

	public:
		static constexpr int Dimension = N;

		using Element = typename Base::Element;
		using Base::extent;
		using Base::operator();

		ExtendedConstView()
			: Base()
		{
		}

		explicit ExtendedConstView(
			const Contained_ExtendedConstView& view)
			: Base(view)
		{
		}

		integer width() const
		{
			return Base::extent()[0];
		}

		integer height() const
		{
			return Base::extent()[1];
		}

		const Element& operator()(integer x, integer y) const
		{
			return Base::operator()(Vector<integer, N>(x, y));
		}
	};

	template <typename Contained_Element, typename Contained_ExtendedConstView>
	class ExtendedConstView<3, Contained_Element, Contained_ExtendedConstView>
		: public ExtendedConstView_::ExtendedConstViewBase<3, Contained_Element, Contained_ExtendedConstView>
	{
	private:
		static constexpr integer N = 3;

		typedef ExtendedConstView_::
			ExtendedConstViewBase<N, Contained_Element, Contained_ExtendedConstView>
			Base;

	public:
		static constexpr int Dimension = N;

		using Element = typename Base::Element;
		using Base::extent;
		using Base::operator();

		ExtendedConstView()
			: Base()
		{
		}

		explicit ExtendedConstView(
			const Contained_ExtendedConstView& view)
			: Base(view)
		{
		}

		integer width() const
		{
			return Base::extent()[0];
		}

		integer height() const
		{
			return Base::extent()[1];
		}

		integer depth() const
		{
			return Base::extent()[2];
		}

		const Element& operator()(integer x, integer y, integer z) const
		{
			return Base::operator()(Vector<integer, N>(x, y, z));
		}
	};


	template <integer N, typename Input_ExtendedConstView>
	ExtendedConstView<N, typename Input_ExtendedConstView::Element, Input_ExtendedConstView>
		extendedConstView(const Input_ExtendedConstView& view)
	{
		return ExtendedConstView<N, typename Input_ExtendedConstView::Element,
			Input_ExtendedConstView>(view);
	}

}

#endif
