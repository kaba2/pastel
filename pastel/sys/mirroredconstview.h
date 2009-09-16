#ifndef PASTEL_MIRROREDCONSTVIEW_H
#define PASTEL_MIRROREDCONSTVIEW_H

#include "pastel/sys/extendedconstview.h"
#include "pastel/sys/view.h"
#include "pastel/sys/syscommon.h"

namespace Pastel
{

	template <int N, typename Contained_ConstView>
	class MirroredConstView
	{
	public:
		enum
		{
			Dimension = N
		};

		typedef typename Contained_ConstView::Element Element;

		MirroredConstView()
			: view_()
		{
		}

		explicit MirroredConstView(
			const Contained_ConstView& view)
			: view_(view)
			, extent_(view.extent())
			, doubleExtent_(extent_ * 2)
		{
		}

		const Vector<integer, N>& extent() const
		{
			return extent_;
		}

		const Element& operator()(
			const Vector<integer, N>& position) const
		{
			Vector<integer, N> newPosition;

			for (integer i = 0;i < N;++i)
			{
				newPosition[i] = mod(position[i], doubleExtent_[i] - 1);

				if (newPosition[i] >= extent_[i])
				{
					newPosition[i] = doubleExtent_[i] - newPosition[i];
				}
			}

			return view_(newPosition);
		}

	private:
		const Contained_ConstView view_;
		const Vector<integer, N> extent_;
		const Vector<integer, N> doubleExtent_;
	};

	template <int N, typename Input_Element, typename Input_ConstView>
	ExtendedConstView<N, Input_Element, MirroredConstView<N, Input_ConstView> >
		mirroredConstView(const ConstView<N, Input_Element, Input_ConstView>& view)
	{
		return extendedConstView<N>(MirroredConstView<N,
			Input_ConstView>(view.contained()));
	}

}

#endif
