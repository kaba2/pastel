#ifndef PASTELSYS_CLAMPEDCONSTVIEW_H
#define PASTELSYS_CLAMPEDCONSTVIEW_H

#include "pastel/sys/extendedconstview.h"
#include "pastel/sys/view.h"
#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <int N, typename Contained_ConstView>
	class ClampedConstView
	{
	public:
		enum
		{
			Dimension = N
		};

		typedef typename Contained_ConstView::Element Element;

		ClampedConstView()
			: view_()
		{
		}

		explicit ClampedConstView(
			const Contained_ConstView& view)
			: view_(view)
		{
		}

		const Vector<N, integer>& extent() const
		{
			return view_.extent();
		}

		const Element& operator()(
			const Point<N, integer>& position) const
		{
			return view_(Point<N, integer>(clamp(asVector(position),
				Vector<N, integer>(0), evaluate(view_.extent() - 1))));
		}

	private:
		const Contained_ConstView view_;
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
