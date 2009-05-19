#ifndef PASTEL_REPEATEDCONSTVIEW_H
#define PASTEL_REPEATEDCONSTVIEW_H

#include "pastel/sys/extendedconstview.h"
#include "pastel/sys/view.h"
#include "pastel/sys/syscommon.h"

namespace Pastel
{

	template <int N, typename Contained_ConstView>
	class RepeatedConstView
	{
	public:
		enum
		{
			Dimension = N
		};

		typedef typename Contained_ConstView::Element Element;

		RepeatedConstView()
			: view_()
		{
		}

		explicit RepeatedConstView(
			const Contained_ConstView& view)
			: view_(view)
			, extent_(view.extent())
		{
		}

		const Vector<N, integer>& extent() const
		{
			return extent_;
		}

		const Element& operator()(
			const Point<N, integer>& position) const
		{
			Point<N, integer> newPosition;
			for (integer i = 0;i < N;++i)
			{
				newPosition[i] = mod(position[i], extent_[i]);
			}

			return view_(newPosition);
		}

	private:
		const Contained_ConstView view_;
		const Vector<N, integer> extent_;
	};

	template <int N, typename Input_Element, typename Input_ConstView>
	ExtendedConstView<N, Input_Element, RepeatedConstView<N, Input_ConstView> >
		repeatedConstView(const ConstView<N, Input_Element, Input_ConstView>& view)
	{
		return extendedConstView<N>(RepeatedConstView<N,
			Input_ConstView>(view.contained()));
	}

}

#endif
