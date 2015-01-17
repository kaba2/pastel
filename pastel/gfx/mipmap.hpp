#ifndef PASTELGFX_MIPMAP_HPP
#define PASTELGFX_MIPMAP_HPP

#include "pastel/gfx/mipmap.h"
#include "pastel/gfx/filter.h"
#include "pastel/gfx/resampling.h"

#include "pastel/sys/array.h"
#include "pastel/sys/view/arrayview.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	template <typename Type, int N>
	MipMap<Type, N>::MipMap()
		: mipMapArray_()
		, extender_()
	{
	}

	template <typename Type, int N>
	template <typename Image_ConstView>
	MipMap<Type, N>::MipMap(
		const ConstView<N, Type, Image_ConstView>& image,
		const ArrayExtender_& extender,
		const FilterPtr& filter)
		: mipMapArray_()
		, extender_(extender)
	{
		Vector<integer, N> originalExtent = image.extent();

		integer maxExtent = max(originalExtent);
		integer roundedExtent = roundUpToPowerOfTwo(maxExtent);
		Vector<integer, N> topExtent(roundedExtent);
		integer images = integerLog2(roundedExtent) + 1;

		std::vector<Array<Type, N> > mipMapArray(images);

		mipMapArray.front().setExtent(topExtent);

		// Upsample to power-of-two size.

		if (topExtent == originalExtent)
		{
			copy(image, arrayView(mipMapArray.front()));
		}
		else
		{
			resample<Type>(
				image,
				extender,
				filter,
				arrayView(mipMapArray.front()));
		}

		// Sequentially downsample to ever smaller images.

		Vector<integer, N> resampleExtent = topExtent;

		for (integer i = 1;i < images;++i)
		{
			for (integer k = 0;k < N;++k)
			{
				resampleExtent[k] >>= 1;
			}

			mipMapArray[i].setExtent(resampleExtent);

			resample<Type>(
				constArrayView(mipMapArray[i - 1]),
				extender,
				filter,
				arrayView(mipMapArray[i]));
		}

		ASSERT(allEqual(mipMapArray.back().extent(), 1));

		mipMapArray_.swap(mipMapArray);
	}

	template <typename Type, int N>

	void MipMap<Type, N>::swap(MipMap& that)
	{
		mipMapArray_.swap(that.mipMapArray_);
	}

	template <typename Type, int N>
	void MipMap<Type, N>::clear()
	{
		mipMapArray_.clear();
	}

	template <typename Type, int N>
	MipMap<Type, N>& MipMap<Type, N>::operator=(const MipMap& that)
	{
		MipMap copy(that);
		swap(copy);
		return *this;
	}

	template <typename Type, int N>
	template <typename Image_ConstView>
	void MipMap<Type, N>::setImage(
		const ConstView<N, Type, Image_ConstView>& image,
		const ArrayExtender_& extender,
		const FilterPtr& filter)
	{
		MipMap<Type, N> copy(image, extender, filter);
		swap(copy);
	}

	template <typename Type, int N>
	const Array<Type, N>&
		MipMap<Type, N>::operator()(
		integer level) const
	{
		PENSURE2(level >= 0 && level < levels(), level, levels());

		return mipMapArray_[level];
	}

	template <typename Type, int N>
	const Array<Type, N>&
		MipMap<Type, N>::mostDetailed() const
	{
		return (*this)(0);
	}

	template <typename Type, int N>
	const Array<Type, N>&
		MipMap<Type, N>::coarsest() const
	{
		return (*this)(levels() - 1);
	}

	template <typename Type, int N>
	typename MipMap<Type, N>::MipView 
		MipMap<Type, N>::view(
		integer level)
	{
		PENSURE2(level >= 0 && level < levels(), level, levels());

		return arrayView(mipMapArray_[level]);
	}

	template <typename Type, int N>
	typename MipMap<Type, N>::ConstMipView 
		MipMap<Type, N>::view(
		integer level) const
	{
		PENSURE2(level >= 0 && level < levels(), level, levels());

		return constArrayView(mipMapArray_[level]);
	}

	template <typename Type, int N>
	typename MipMap<Type, N>::MipView 
		MipMap<Type, N>::mostDetailedView()
	{
		return view(0);
	}

	template <typename Type, int N>
	typename MipMap<Type, N>::ConstMipView 
		MipMap<Type, N>::mostDetailedView() const
	{
		return view(0);
	}

	template <typename Type, int N>
	typename MipMap<Type, N>::MipView 
		MipMap<Type, N>::coarsestView()
	{
		return view(levels() - 1);
	}

	template <typename Type, int N>
	typename MipMap<Type, N>::ConstMipView 
		MipMap<Type, N>::coarsestView() const
	{
		return view(levels() - 1);
	}

	template <typename Type, int N>
	Vector<integer, N> MipMap<Type, N>::extent() const
	{
		if (empty())
		{
			return Vector<integer, N>(0);
		}

		return mipMapArray_.front().extent();
	}

	template <typename Type, int N>
	integer MipMap<Type, N>::levels() const
	{
		return mipMapArray_.size();
	}

	template <typename Type, int N>
	bool MipMap<Type, N>::empty() const
	{
		return mipMapArray_.empty();
	}

}

#endif
