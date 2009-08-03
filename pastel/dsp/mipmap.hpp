#ifndef PASTEL_MIPMAP_HPP
#define PASTEL_MIPMAP_HPP

#include "pastel/dsp/mipmap.h"
#include "pastel/dsp/filter.h"
#include "pastel/dsp/resample.h"

#include "pastel/sys/array.h"
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
		const FilterPtr& filter)
		: mipMapArray_()
		, extender_(extender)
	{
		const Vector<integer, N> originalExtent = image.extent();

		const integer maxExtent = max(originalExtent);
		const integer roundedExtent = roundUpToPowerOf2(maxExtent);
		Vector<integer, N> topExtent(roundedExtent);
		const integer images = integerLog2(roundedExtent) + 1;

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
		const FilterPtr& filter)
	{
		MipMap<N, Type> copy(image, extender, filter);
		swap(copy);
	}

	template <int N, typename Type>
	const Array<Type, N>&
		MipMap<N, Type>::operator()(
		integer level) const
	{
		PENSURE2(level >= 0 && level < levels(), level, levels());

		return mipMapArray_[level];
	}

	template <int N, typename Type>
	const Array<Type, N>&
		MipMap<N, Type>::mostDetailed() const
	{
		return (*this)(0);
	}

	template <int N, typename Type>
	const Array<Type, N>&
		MipMap<N, Type>::coarsest() const
	{
		return (*this)(levels() - 1);
	}

	template <int N, typename Type>
	typename MipMap<N, Type>::MipView 
		MipMap<N, Type>::view(
		integer level)
	{
		PENSURE2(level >= 0 && level < levels(), level, levels());

		return arrayView(mipMapArray_[level]);
	}

	template <int N, typename Type>
	typename MipMap<N, Type>::ConstMipView 
		MipMap<N, Type>::view(
		integer level) const
	{
		PENSURE2(level >= 0 && level < levels(), level, levels());

		return constArrayView(mipMapArray_[level]);
	}

	template <int N, typename Type>
	typename MipMap<N, Type>::MipView 
		MipMap<N, Type>::mostDetailedView()
	{
		return view(0);
	}

	template <int N, typename Type>
	typename MipMap<N, Type>::ConstMipView 
		MipMap<N, Type>::mostDetailedView() const
	{
		return view(0);
	}

	template <int N, typename Type>
	typename MipMap<N, Type>::MipView 
		MipMap<N, Type>::coarsestView()
	{
		return view(levels() - 1);
	}

	template <int N, typename Type>
	typename MipMap<N, Type>::ConstMipView 
		MipMap<N, Type>::coarsestView() const
	{
		return view(levels() - 1);
	}

	template <int N, typename Type>
	Vector<integer, N> MipMap<N, Type>::extent() const
	{
		if (empty())
		{
			return Vector<integer, N>(0);
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
