#ifndef PASTELGFX_MIPMAP_HPP
#define PASTELGFX_MIPMAP_HPP

#include "pastel/gfx/mipmap.h"
#include "pastel/gfx/filter.h"
#include "pastel/gfx/resample.h"

#include "pastel/sys/lineararray.h"
#include "pastel/sys/syscommon.h"
#include "pastel/sys/arrayview.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	template <int N, typename Type>
	MipMap<N, Type>::MipMap()
		: mipMapArray_()
		, extender_()
	{
	}

	template <int N, typename Type>
	template <typename Image_ConstView>
	MipMap<N, Type>::MipMap(
		const ConstView<N, Type, Image_ConstView>& image,
		const ArrayExtender<N, Type>& extender,
		const FilterRef& filter)
		: mipMapArray_()
		, extender_(extender)
	{
		const Vector<N, integer> originalExtent = image.extent();

		const integer maxExtent = max(originalExtent);
		const integer roundedExtent = roundUpToPowerOf2(maxExtent);
		Vector<N, integer> topExtent(roundedExtent);
		const integer images = integerLog2(roundedExtent) + 1;

		std::vector<LinearArray<N, Type> > mipMapArray(images);

		mipMapArray.front().setExtent(topExtent);

		// Upsample to power-of-two size.

		if (topExtent == originalExtent)
		{
			copy(image, arrayView(mipMapArray.front()));
		}
		else
		{
			resample(
				image,
				extender,
				filter,
				arrayView(mipMapArray.front()));
		}

		// Sequentially downsample to ever smaller images.

		Vector<N, integer> resampleExtent = topExtent;

		for (integer i = 1;i < images;++i)
		{
			for (integer k = 0;k < N;++k)
			{
				resampleExtent[k] >>= 1;
			}

			mipMapArray[i].setExtent(resampleExtent);

			resample(
				constArrayView(mipMapArray[i - 1]),
				extender,
				filter,
				arrayView(mipMapArray[i]));
		}

		ASSERT(allEqual(mipMapArray.back().extent(), 1));

		mipMapArray_.swap(mipMapArray);
	}

	template <int N, typename Type>
	void MipMap<N, Type>::swap(MipMap& that)
	{
		mipMapArray_.swap(that.mipMapArray_);
	}

	template <int N, typename Type>
	void MipMap<N, Type>::clear()
	{
		mipMapArray_.clear();
	}

	template <int N, typename Type>
	MipMap<N, Type>& MipMap<N, Type>::operator=(const MipMap& that)
	{
		MipMap copy(that);
		swap(copy);
		return *this;
	}

	template <int N, typename Type>
	template <typename Image_ConstView>
	void MipMap<N, Type>::setImage(
		const ConstView<N, Type, Image_ConstView>& image,
		const ArrayExtender<N, Type>& extender,
		const FilterRef& filter)
	{
		MipMap<N, Type> copy(image, extender, filter);
		swap(copy);
	}

	template <int N, typename Type>
	LinearArray<N, Type>& MipMap<N, Type>::operator()(
		integer level)
	{
		PENSURE2(level >= 0 && level < levels(), level, levels());

		return mipMapArray_[level];
	}

	template <int N, typename Type>
	const LinearArray<N, Type>& MipMap<N, Type>::operator()(
		integer level) const
	{
		PENSURE2(level >= 0 && level < levels(), level, levels());

		return mipMapArray_[level];
	}

	template <int N, typename Type>
	LinearArray<N, Type>& MipMap<N, Type>::mostDetailed()
	{
		return (*this)(0);
	}

	template <int N, typename Type>
	const LinearArray<N, Type>& MipMap<N, Type>::mostDetailed() const
	{
		return (*this)(0);
	}

	template <int N, typename Type>
	LinearArray<N, Type>& MipMap<N, Type>::coarsest()
	{
		return (*this)(levels() - 1);
	}

	template <int N, typename Type>
	const LinearArray<N, Type>& MipMap<N, Type>::coarsest() const
	{
		return (*this)(levels() - 1);
	}

	template <int N, typename Type>
	Vector<N, integer> MipMap<N, Type>::extent() const
	{
		if (empty())
		{
			return Vector<N, integer>(0);
		}

		return mipMapArray_.front().extent();
	}

	template <int N, typename Type>
	integer MipMap<N, Type>::levels() const
	{
		return mipMapArray_.size();
	}

	template <int N, typename Type>
	bool MipMap<N, Type>::empty() const
	{
		return mipMapArray_.empty();
	}

}

#endif
