#ifndef PASTELGFX_RIPMAP_HPP
#define PASTELGFX_RIPMAP_HPP

#include "pastel/gfx/ripmap.h"
#include "pastel/gfx/filter.h"
#include "pastel/gfx/resample.h"

#include "pastel/sys/clampextender.h"
#include "pastel/sys/lineararray.h"
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
				LinearArray<N, LinearArray<N, Type> >& imageArray,
				const FilterRef& filter)
				: ripMapArray_(imageArray)
				, filter_(filter)
			{
			}

			void operator()(
				const Point<N, integer>& position,
				LinearArray<N, Type>& image) const
			{
				if (allEqual(position, 0))
				{
					return;
				}

				Point<N, integer> previousPosition(position);
				for (integer i = 0;i < N;++i)
				{
					if (previousPosition[i] > 0)
					{
						--previousPosition[i];
						break;
					}
				}

				const LinearArray<N, Type>& previousImage =
					ripMapArray_(previousPosition);

				Vector<N, integer> resampleExtent = 
					ripMapArray_(Point<N, integer>(0)).extent();
				for (integer i = 0;i < N;++i)
				{
					resampleExtent[i] >>= position[i];
				}

				image.setExtent(resampleExtent);

				resample(
					constArrayView(previousImage), 
					clampExtender(),
					filter_,
					arrayView(image));
			}

		private:
			LinearArray<N, LinearArray<N, Type> >& ripMapArray_;
			const FilterRef& filter_;
		};

	}

	template <int N, typename Type>
	template <typename Image_ConstView>
	RipMap<N, Type>::RipMap(
		const ConstView<N, Type, Image_ConstView>& image,
		const FilterRef& filter)
		: ripMapArray_()
	{
		const Vector<N, integer> originalExtent = image.extent();
		Vector<N, integer> topExtent;
		Vector<N, integer> levels;
		
		for (integer i = 0;i < N;++i)
		{
			topExtent[i] = roundUpToPowerOf2(originalExtent[i]);
			levels[i] = integerLog2(topExtent[i]) + 1;
		}

		LinearArray<N, LinearArray<N, Type> > imageArray(levels);

		if (imageArray.empty())
		{
			ripMapArray_.swap(imageArray);
			return;
		}

		imageArray(Point<N, integer>(0)).setExtent(topExtent);
		
		// Upsample to power-of-two size.

		if (topExtent == originalExtent)
		{
			copy(image, arrayView(imageArray(Point<N, integer>(0))));
		}
		else
		{
			resample(
				image, 
				clampExtender(),
				filter,
				arrayView(imageArray(Point<N, integer>(0))));
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
		const FilterRef& filter)
	{
		RipMap<N, Type> copy(image, filter);
		swap(copy);
	}

	template <int N, typename Type>
	LinearArray<N, Type>& RipMap<N, Type>::operator()(
		const Point<N, integer>& level)
	{
		return ripMapArray_(level);
	}

	template <int N, typename Type>
	const LinearArray<N, Type>& RipMap<N, Type>::operator()(
		const Point<N, integer>& level) const
	{
		return ripMapArray_(level);
	}

	template <int N, typename Type>
	LinearArray<N, Type>& RipMap<N, Type>::mostDetailed()
	{
		return ripMapArray_(Point<N, integer>(0));
	}

	template <int N, typename Type>
	const LinearArray<N, Type>& RipMap<N, Type>::mostDetailed() const
	{
		return ripMapArray_(Point<N, integer>(0));
	}

	template <int N, typename Type>
	LinearArray<N, Type>& RipMap<N, Type>::coarsest()
	{
		return ripMapArray_(asPoint(ripMapArray_.extent() - 1));
	}

	template <int N, typename Type>
	const LinearArray<N, Type>& RipMap<N, Type>::coarsest() const
	{
		return ripMapArray_(asPoint(ripMapArray_.extent() - 1));
	}

	template <int N, typename Type>
	const Vector<N, integer>& RipMap<N, Type>::levels() const
	{
		return ripMapArray_.extent();
	}

	template <int N, typename Type>
	bool RipMap<N, Type>::empty() const
	{
		return ripMapArray_.empty();
	}

	template <int N, typename Type>
	View<N, LinearArray<N, Type>, 
		ArrayView<N, LinearArray<N, LinearArray<N, Type> > > >
		RipMap<N, Type>::view()
	{
		return arrayView(ripMapArray_);
	}

	template <int N, typename Type>
	ConstView<N, LinearArray<N, Type>, 
		ConstArrayView<N, LinearArray<N, LinearArray<N, Type> > > >
		RipMap<N, Type>::constView() const
	{
		return constArrayView(ripMapArray_);
	}

}

#endif
