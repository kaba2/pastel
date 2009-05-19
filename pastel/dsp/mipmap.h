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
	mipMapList[0] contains the original image upsampled
	to a square to the next power-of-two extents.
	Subsequently, for i > 0 mipMapList[i] contains a half-extent
	(downsampled to half in all dimensions) version of mipMapList[i - 1].

	You can choose the reconstruction filter to use
	in the resamplings. Note that if your filter has
	negative lobes it is possible, to have
	negative values in the mipmaps. If the image
	contains color, then you will want to
	call 'transform(mipMap, fitColor)'.

	The memory footprint of the mipmaps can be computed as follows.
	Assume the dimensions is n and the original image is a
	cube with side length 2^m. The memory footprint is

	M(n) = sum[i = 0..m] (2^n)^i / 2^mn
	= [((2^n)^(m + 1) - 1) / (2^n - 1)] / 2^mn
	= (2^n - 1 / 2^mn) / (2^n - 1)
	<= 2^n / (2^n - 1) = M'(n)

	n	M'(n)
	1	2
	2	4 / 3
	3	8 / 7
	4	16 / 15
	etc.

	As n -> oo, M'(n) -> 1 and thus the additional required
	memory is negligible. In any dimension n, M'(n) <= 2.
	*/

	template <int N, typename Type>
	class MipMap
	{
	public:
		typedef View<N, Type, ArrayView<N, Array<N, Type> > >
			MipView;
		typedef ConstView<N, Type, ConstArrayView<N, Array<N, Type> > >
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

		MipMap<N, Type>& operator=(const MipMap& that);

		const Array<N, Type>& operator()(integer level) const;
		const Array<N, Type>& mostDetailed() const;
		const Array<N, Type>& coarsest() const;

		MipView view(integer level);
		ConstMipView view(integer level) const;

		MipView mostDetailedView();
		ConstMipView mostDetailedView() const;

		MipView coarsestView();
		ConstMipView coarsestView() const;

		Vector<N, integer> extent() const;

		integer levels() const;
		bool empty() const;

	private:
		std::vector<Array<N, Type> > mipMapArray_;
		ArrayExtender<N, Type> extender_;
	};

}

#include "pastel/dsp/mipmap.hpp"

#endif
