#ifndef PASTEL_RIPMAP_HPP
#define PASTEL_RIPMAP_HPP

#include "pastel/dsp/ripmap.h"
#include "pastel/dsp/filter.h"
#include "pastel/dsp/resample.h"

#include "pastel/sys/clampextender.h"
#include "pastel/sys/array.h"
#include "pastel/sys/syscommon.h"
#include "pastel/sys/arrayview.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	namespace Detail_ComputeRipMaps
	{

		template <int N, typename Type>
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

	template <int N, typename Type>
	template <typename Image_ConstView>
	RipMap<N, Type>::RipMap(
		const ConstView<N, Type, Image_ConstView>& image,
		const FilterPtr& filter)
		: ripMapArray_()
	{
		const Vector<integer, N> originalExtent = image.extent();
		Vector<integer, N> topExtent;
		Vector<integer, N> levels;

		for (integer i = 0;i < N;++i)
		{
			topExtent[i] = roundUpToPowerOf2(originalExtent[i]);
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

		Detail_ComputeRipMaps::Visitor<N, Type> visitor(imageArray, filter);
		visitPosition(arrayView(imageArray), visitor);

		ripMapArray_.swap(imageArray);
	}

	template <int N, typename Type>
	void RipMap<N, Type>::swap(RipMap& that)
	{
		ripMapArray_.swap(that.ripMapArray_);
	}

	template <int N, typename Type>
	void RipMap<N, Type>::clear()
	{
		ripMapArray_.clear();
	}

	template <int N, typename Type>
	RipMap<N, Type>& RipMap<N, Type>::operator=(const RipMap& that)
	{
		RipMap copy(that);
		swap(copy);
		return *this;
	}

	template <int N, typename Type>
	template <typename Image_ConstView>
	void RipMap<N, Type>::setImage(
		const ConstView<N, Type, Image_ConstView>& image,
		const FilterPtr& filter)
	{
		RipMap<N, Type> copy(image, filter);
		swap(copy);
	}

	template <int N, typename Type>
	Array<Type, N>& RipMap<N, Type>::operator()(
		const Vector<integer, N>& level)
	{
		return ripMapArray_(level);
	}

	template <int N, typename Type>
	const Array<Type, N>& RipMap<N, Type>::operator()(
		const Vector<integer, N>& level) const
	{
		return ripMapArray_(level);
	}

	template <int N, typename Type>
	const Array<Type, N>& RipMap<N, Type>::mostDetailed() const
	{
		return ripMapArray_(Vector<integer, N>(0));
	}

	template <int N, typename Type>
	const Array<Type, N>& RipMap<N, Type>::coarsest() const
	{
		return ripMapArray_(ripMapArray_.extent() - 1);
	}

	template <int N, typename Type>
	const Vector<integer, N>& RipMap<N, Type>::levels() const
	{
		return ripMapArray_.extent();
	}

	template <int N, typename Type>
	bool RipMap<N, Type>::empty() const
	{
		return ripMapArray_.empty();
	}

	template <int N, typename Type>
	View<N, Array<Type, N>,
		ArrayView<N, Array<Array<Type, N>, N> > >
		RipMap<N, Type>::view()
	{
		return arrayView(ripMapArray_);
	}

	template <int N, typename Type>
	ConstView<N, Array<Type, N>,
		ConstArrayView<N, Array<Array<Type, N>, N> > >
		RipMap<N, Type>::constView() const
	{
		return constArrayView(ripMapArray_);
	}

}

#endif
