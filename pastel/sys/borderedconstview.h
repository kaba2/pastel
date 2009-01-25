#ifndef PASTELSYS_BORDEREDCONSTVIEW_H
#define PASTELSYS_BORDEREDCONSTVIEW_H

#include "pastel/sys/extendedconstview.h"
#include "pastel/sys/view.h"

namespace Pastel
{

	template <int N, typename Contained_ConstView>
	class BorderedConstView
	{
	public:
		enum
		{
			Dimension = N
		};

		typedef typename Contained_ConstView::Element Element;

		BorderedConstView()
			: view_()
			, border_()
		{
		}

		explicit BorderedConstView(
			const Contained_ConstView& view,
			const Element& border)
			: view_(view)
			, border_(border)
		{
		}

		const Vector<N, integer>& extent() const
		{
			return view_.extent();
		}

		const Element& operator()(
			const Point<N, integer>& position) const
		{
			if (anyLess(position, 0) ||
				anyGreaterEqual(asVector(position), view_.extent()))
			{
				return border_;
			}

			return view_(position);
		}

	private:
		const Contained_ConstView view_;
		const Element border_;
	};

	template <int N, typename Input_Element, typename Input_ConstView>
	ExtendedConstView<N, Input_Element, 
		BorderedConstView<N, ConstView<N, Input_Element, Input_ConstView> > > 
		borderedConstView(
		const ConstView<N, Input_Element, Input_ConstView>& view,
		const PASTEL_NO_DEDUCTION(Input_Element)& border)
	{
		return extendedConstView<N>(BorderedConstView<
			N, ConstView<N, Input_Element, Input_ConstView> >(view, border));
	}

}

#endif
