// Description: An extended view using repeated tiling
// Documentation: extendedconstviews.txt

#ifndef PASTELSYS_REPEATEDCONSTVIEW_H
#define PASTELSYS_REPEATEDCONSTVIEW_H

#include "pastel/sys/view/extendedconstview.h"
#include "pastel/sys/view/view.h"

namespace Pastel
{

	template <int N, typename Contained_ConstView>
	class RepeatedConstView
	{
	public:
		static constexpr int Dimension = N;

		using Element = typename Contained_ConstView::Element;

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
				newPosition[i] = mod(position[i], extent_[i]);
			}

			return view_(newPosition);
		}

	private:
		Contained_ConstView view_;
		Vector<integer, N> extent_;
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
