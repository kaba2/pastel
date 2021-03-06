// Description: MipMap class
// Detail: A set of precomputed successive downsamplings of an image

#ifndef PASTELGFX_MIPMAP_H
#define PASTELGFX_MIPMAP_H

#include "pastel/gfx/filter.h"
#include "pastel/gfx/filter/gaussian_filter.h"

#include "pastel/sys/extender/clampextender.h"
#include "pastel/sys/extender/arrayextender.h"
#include "pastel/sys/view/view.h"
#include "pastel/sys/view/arrayview.h"

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
		using ArrayExtender_ = ArrayExtender<N, Type>;

		// Using default copy constructor.
		// Using default destructor.

		MipMap();

		template <typename Image_ConstView>
		explicit MipMap(
			const ConstView<N, Type, Image_ConstView>& image,
			const ArrayExtender_& extender = ArrayExtender_(clampExtender()),
			const FilterPtr& filter = gaussianFilter(2));

		template <typename Image_ConstView>
		void setImage(
			const ConstView<N, Type, Image_ConstView>& image,
			const ArrayExtender_& extender = ArrayExtender_(clampExtender()),
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
		ArrayExtender_ extender_;
	};

}

#include "pastel/gfx/mipmap/mipmap.hpp"

#include "pastel/gfx/mipmap/mipmap_tools.h"

#endif
