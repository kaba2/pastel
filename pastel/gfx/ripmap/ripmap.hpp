#ifndef PASTELGFX_RIPMAP_HPP
#define PASTELGFX_RIPMAP_HPP

#include "pastel/gfx/ripmap/ripmap.h"
#include "pastel/gfx/filter.h"
#include "pastel/gfx/resampling.h"

#include "pastel/sys/extender/clampextender.h"
#include "pastel/sys/array.h"
#include "pastel/sys/view/arrayview.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	namespace ComputeRipMaps_
	{

		template <typename Type, integer N>
		class Visitor
		{
		public:
			explicit Visitor(
				Array<Array<Type, N>, N>& imageArray,
				const FilterPtr& filter)
				: ripMapArray_(imageArray)
				, filter_(filter)
			{
			}

			void operator()(
				const Vector<integer, N>& position,
				Array<Type, N>& image) const
			{
				if (allEqual(position, 0))
				{
					return;
				}

				Vector<integer, N> previousPosition(position);
				for (integer i = 0;i < N;++i)
				{
					if (previousPosition[i] > 0)
					{
						--previousPosition[i];
						break;
					}
				}

				const Array<Type, N>& previousImage =
					ripMapArray_(previousPosition);

				Vector<integer, N> resampleExtent =
					ripMapArray_(Vector<integer, N>(0)).extent();
				for (integer i = 0;i < N;++i)
				{
					resampleExtent[i] >>= position[i];
				}

				image.setExtent(resampleExtent);

				resample<Type>(
					constArrayView(previousImage),
					clampExtender(),
					filter_,
					arrayView(image));
			}

		private:
			Array<Array<Type, N>, N>& ripMapArray_;
			const FilterPtr& filter_;
		};

	}

	template <typename Type, integer N>
	template <typename Image_ConstView>
	RipMap<Type, N>::RipMap(
		const ConstView<N, Type, Image_ConstView>& image,
		const FilterPtr& filter)
		: ripMapArray_()
	{
		Vector<integer, N> originalExtent = image.extent();
		Vector<integer, N> topExtent;
		Vector<integer, N> levels;

		for (integer i = 0;i < N;++i)
		{
			topExtent[i] = roundUpToPowerOfTwo(originalExtent[i]);
			levels[i] = integerLog2(topExtent[i]) + 1;
		}

		Array<Array<Type, N>, N> imageArray(levels);

		if (imageArray.empty())
		{
			ripMapArray_.swap(imageArray);
			return;
		}

		imageArray(Vector<integer, N>(0)).setExtent(topExtent);

		// Upsample to power-of-two size.

		if (topExtent == originalExtent)
		{
			copy(image, arrayView(imageArray(Vector<integer, N>(0))));
		}
		else
		{
			resample<Type>(
				image,
				clampExtender(),
				filter,
				arrayView(imageArray(Vector<integer, N>(0))));
		}

		ComputeRipMaps_::Visitor<Type, N> visitor(imageArray, filter);
		visitPosition(arrayView(imageArray), visitor);

		ripMapArray_.swap(imageArray);
	}

	template <typename Type, integer N>

	void RipMap<Type, N>::swap(RipMap& that)
	{
		ripMapArray_.swap(that.ripMapArray_);
	}

	template <typename Type, integer N>
	void RipMap<Type, N>::clear()
	{
		ripMapArray_.clear();
	}

	template <typename Type, integer N>
	RipMap<Type, N>& RipMap<Type, N>::operator=(const RipMap& that)
	{
		RipMap copy(that);
		swap(copy);
		return *this;
	}

	template <typename Type, integer N>
	template <typename Image_ConstView>
	void RipMap<Type, N>::setImage(
		const ConstView<N, Type, Image_ConstView>& image,
		const FilterPtr& filter)
	{
		RipMap<Type, N> copy(image, filter);
		swap(copy);
	}

	template <typename Type, integer N>
	Array<Type, N>& RipMap<Type, N>::operator()(
		const Vector<integer, N>& level)
	{
		return ripMapArray_(level);
	}

	template <typename Type, integer N>
	const Array<Type, N>& RipMap<Type, N>::operator()(
		const Vector<integer, N>& level) const
	{
		return ripMapArray_(level);
	}

	template <typename Type, integer N>
	const Array<Type, N>& RipMap<Type, N>::mostDetailed() const
	{
		return ripMapArray_(Vector<integer, N>(0));
	}

	template <typename Type, integer N>
	const Array<Type, N>& RipMap<Type, N>::coarsest() const
	{
		return ripMapArray_(ripMapArray_.extent() - 1);
	}

	template <typename Type, integer N>
	const Vector<integer, N>& RipMap<Type, N>::levels() const
	{
		return ripMapArray_.extent();
	}

	template <typename Type, integer N>
	bool RipMap<Type, N>::empty() const
	{
		return ripMapArray_.empty();
	}

	template <typename Type, integer N>
	View<N, Array<Type, N>,
		ArrayView<N, Array<Array<Type, N>, N> > >
		RipMap<Type, N>::view()
	{
		return arrayView(ripMapArray_);
	}

	template <typename Type, integer N>
	ConstView<N, Array<Type, N>,
		ConstArrayView<N, Array<Array<Type, N>, N> > >
		RipMap<Type, N>::constView() const
	{
		return constArrayView(ripMapArray_);
	}

}

#endif
