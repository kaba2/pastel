// Description: MipMap class
// Detail: A set of precomputed successive downsamplings of an image

#ifndef PASTEL_MIPMAP_H
#define PASTEL_MIPMAP_H

#include "pastel/dsp/filter.h"
#include "pastel/dsp/gaussian_filter.h"

#include "pastel/sys/clampextender.h"
#include "pastel/sys/arrayextender.h"
#include "pastel/sys/view.h"
#include "pastel/sys/arrayview.h"

#include <vector>

namespace Pastel
{

	//! Computes mipmaps for a given image.
	/*!
	A mipmap is simply a collection of downsampled
	versions of an image. As an overload of terminology,
	the individual downsampled images are also called mipmaps.

	The result is an array of mipmaps, such that
	mipMap[0] contains the original image upsampled
	to a square to the next power-of-two extents.
	Subsequently, for i > 0 mipMap[i] contains a half-extent
	(downsampled to half in all dimensions) version of mipMap[i - 1].

	You can choose the reconstruction filter to use
	in the resamplings. Note that if your filter has
	negative lobes, it is possible to have
	negative values in the mipmaps. If the image
	contains color, then you will want to
	call 'transform(mipMap, fitColor)'.
	*/

	template <typename Type, int N = 2>
	class MipMap
	{
	public:
		typedef View<N, Type, ArrayView<N, Array<Type, N> > >
			MipView;
		typedef ConstView<N, Type, ConstArrayView<N, Array<Type, N> > >
			ConstMipView;		

		// Using default copy constructor.
		// Using default destructor.

		MipMap();

		template <typename Image_ConstView>
		explicit MipMap(
			const ConstView<N, Type, Image_ConstView>& image,
			const ArrayExtender<N, Type>& extender = ArrayExtender<N, Type>(clampExtender()),
			const FilterPtr& filter = gaussianFilter(2));

		template <typename Image_ConstView>
		void setImage(
			const ConstView<N, Type, Image_ConstView>& image,
			const ArrayExtender<N, Type>& extender = ArrayExtender<N, Type>(clampExtender()),
			const FilterPtr& filter = gaussianFilter(2));

		void swap(MipMap& that);
		void clear();

		MipMap<Type, N>& operator=(const MipMap& that);

		const Array<Type, N>& operator()(integer level) const;
		const Array<Type, N>& mostDetailed() const;
		const Array<Type, N>& coarsest() const;

		MipView view(integer level);
		ConstMipView view(integer level) const;

		MipView mostDetailedView();
		ConstMipView mostDetailedView() const;

		MipView coarsestView();
		ConstMipView coarsestView() const;

		Vector<integer, N> extent() const;

		integer levels() const;
		bool empty() const;

	private:
		std::vector<Array<Type, N> > mipMapArray_;
		ArrayExtender<N, Type> extender_;
	};

}

#include "pastel/dsp/mipmap.hpp"

#endif
