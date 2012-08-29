// Description: RipMap class
// Detail: A set of precomputed successive downsamplings of an image

#ifndef PASTELDSP_RIPMAP_H
#define PASTELDSP_RIPMAP_H

#include "pastel/dsp/filter.h"
#include "pastel/dsp/gaussian_filter.h"

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
	ripMap(Vector<integer, N>(0)) contains the original image upsampled
	to the next power-of-two extents. Subsequently, if p is an
	array coordinate vector, then ripMap(p + unitAxis<I>())
	contains a half-extent version of ripMap(p) along the I:th standard axis.

	You can choose the reconstruction filter to use
	in the resamplings. Note that if your filter has
	negative lobes it is possible,
	as always when resampling, to have
	negative values in the ripmaps. If the image
	contains color, then you will want to
	call 'transform(arrayView(ripMap(p)), fitColor)'
	for each ripmap afterwards.
	*/

	template <typename Type, int N = 2>
	class RipMap
	{
	public:
		// Using default copy constructor.
		// Using default destructor.

		RipMap();

		template <typename Image_ConstView>
		explicit RipMap(
			const ConstView<N, Type, Image_ConstView>& image,
			const FilterPtr& filter = gaussianFilter(2));

		void swap(RipMap& that);
		void clear();

		RipMap<Type, N>& operator=(const RipMap& that);

		template <typename Image_ConstView>
		void setImage(
			const ConstView<N, Type, Image_ConstView>& image,
			const FilterPtr& filter = gaussianFilter(2));

		Array<Type, N>& operator()(
			const Vector<integer, N>& level);

		const Array<Type, N>& operator()(
			const Vector<integer, N>& level) const;

		const Vector<integer, N>& levels() const;

		bool empty() const;

		const Array<Type, N>& mostDetailed() const;

		const Array<Type, N>& coarsest() const;

		View<N, Array<Type, N>,
			ArrayView<N, Array<Array<Type, N>, N> > >
			view();

		ConstView<N, Array<Type, N>,
			ConstArrayView<N, Array<Array<Type, N>, N> > >
			constView() const;

	private:
		Array<Array<Type, N>, N> ripMapArray_;
	};

}

#include "pastel/dsp/ripmap.hpp"

#include "pastel/dsp/ripmap_tools.h"

#endif
