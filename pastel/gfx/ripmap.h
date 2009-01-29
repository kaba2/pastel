#ifndef PASTELGFX_RIPMAP_H
#define PASTELGFX_RIPMAP_H

#include "pastel/gfx/filter.h"
#include "pastel/gfx/gaussianfilter.h"

#include "pastel/sys/view.h"

#include <vector>

namespace Pastel
{

	//! Computes ripmaps for a given image.
	/*!
	A ripmap is simply a collection of downsampled
	versions of an image. As an overload of terminology,
	the individual downsampled images are also called ripmaps.

	The result is an array of ripmaps, such that
	ripMapList(Vector<N, integer>(0)) contains the original image upsampled
	to the next power-of-two extents. Subsequently, if p is an
	array coordinate vector, then ripMapList(p + unitAxis<I, integer>())
	contains a half-extent version of ripMapList(p) along the I:th standard axis.

	You can choose the reconstruction filter to use
	in the resamplings. Note that if your filter has
	negative lobes it is possible,
	as always when resampling, to have
	negative values in the ripmaps. If the image
	contains color, then you will want to
	call 'transform(arrayView(ripMap(p)), fitColor)'
	for each ripmap afterwards.

	The memory footprint of the ripmaps can be computed as follows.
	Assume the dimensions is n and the original image is a
	cube with side length 2^m. The memory footprint is
	*/

	template <int N, typename Type>
	class RipMap
	{
	public:
		// Using default copy constructor.
		// Using default destructor.

		RipMap();

		template <typename Image_ConstView>
		explicit RipMap(
			const ConstView<N, Type, Image_ConstView>& image,
			const FilterRef& filter = gaussianFilter(2));

		void swap(RipMap& that);
		void clear();

		RipMap<N, Type>& operator=(const RipMap& that);

		template <typename Image_ConstView>
		void setImage(
			const ConstView<N, Type, Image_ConstView>& image,
			const FilterRef& filter = gaussianFilter(2));

		LinearArray<N, Type>& operator()(
			const Point<N, integer>& level);

		const LinearArray<N, Type>& operator()(
			const Point<N, integer>& level) const;

		const Vector<N, integer>& levels() const;

		bool empty() const;

		LinearArray<N, Type>& mostDetailed();
		const LinearArray<N, Type>& mostDetailed() const;

		LinearArray<N, Type>& coarsest();
		const LinearArray<N, Type>& coarsest() const;

		View<N, LinearArray<N, Type>,
			ArrayView<N, LinearArray<N, LinearArray<N, Type> > > >
			RipMap<N, Type>::view();

		ConstView<N, LinearArray<N, Type>,
			ConstArrayView<N, LinearArray<N, LinearArray<N, Type> > > >
			RipMap<N, Type>::constView() const;

	private:
		LinearArray<N, LinearArray<N, Type> > ripMapArray_;
	};

}

#include "pastel/gfx/ripmap.hpp"

#endif
