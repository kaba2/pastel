// Description: An extended view using a clamped border
// Documentation: extendedconstviews.txt

#ifndef PASTELSYS_CLAMPEDCONSTVIEW_H
#define PASTELSYS_CLAMPEDCONSTVIEW_H

#include "pastel/sys/view/extendedconstview.h"
#include "pastel/sys/view/view.h"
#include "pastel/sys/vector/vector_tools.h"

namespace Pastel
{

	template <int N, typename Contained_ConstView>
	class ClampedConstView
	{
	public:
		static PASTEL_CONSTEXPR int Dimension = N;

		using Element = typename Contained_ConstView::Element;

		ClampedConstView()
			: view_()
		{
		}

		explicit ClampedConstView(
			const Contained_ConstView& view)
			: view_(view)
		{
		}

		const Vector<integer, N>& extent() const
		{
			return view_.extent();
		}

		const Element& operator()(
			const Vector<integer, N>& position) const
		{
			return view_(Vector<integer, N>(clamp(position,
				Vector<integer, N>(0), evaluate(view_.extent() - 1))));
		}

	private:
		Contained_ConstView view_;
	};

	template <int N, typename Input_Element, typename Input_ConstView>
	ExtendedConstView<N, Input_Element, ClampedConstView<N, ConstView<N, Input_Element, Input_ConstView> > >

		clampedConstView(const ConstView<N, Input_Element, Input_ConstView>& view)
	{
		return extendedConstView<N>(ClampedConstView<N,
			ConstView<N, Input_Element, Input_ConstView> >(view));
	}

}

#endif
