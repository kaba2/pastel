// Description: EwaImage_Texture class
// Detail: Image-based texture with radial reconstruction and EWA filtering.
// Documentation: texture.txt

#ifndef PASTEL_EWAIMAGE_TEXTURE_H
#define PASTEL_EWAIMAGE_TEXTURE_H

#include "pastel/sys/syscommon.h"
#include "pastel/sys/vector_tools.h"
#include "pastel/sys/clampextender.h"

#include "pastel/dsp/triangle_filter.h"
#include "pastel/dsp/lanczos_filter.h"
#include "pastel/dsp/mipmap.h"

#include "pastel/math/matrix.h"

#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	//! Elliptically weighted average image texture.
	/*!
	When choosing the filter note the following:

	- The reconstruction is radial.
	- Performance scales quadratically in filter radius.
	- Performance is not affected by the complexity of the
	used filter since the filter is stored in a look-up table.

	Here's a table that summarizes the quality of
	each filter:

				Minification	Magnification
	Box         bad				very blocky
	Triangle	excellent		blocky
	Gaussian(1) excellent		blurry, clamping step visible
	Gaussian(2)	excellent 		blurry
	Mitchell	aliasing		very good
	Lanczos(2)	aliasing		excellent

	- Prefer TriangleFilter to GaussianFilter for minification.
	It has a smaller radius, no visible clamping step,
	and performs equivalently to the eye.
	- The combination of a TriangleFilter for minification
	and a LanczosFilter(2) for magnification seems to deliver
	the best results of all combinations. These are
	the defaults.
	*/

	template <typename Type, int N = 2>
	class EwaImage_Texture
		: public Texture<Type, N>
	{
	public:
		explicit EwaImage_Texture(
			const MipMap<N, Type>& mipMap,
			const ArrayExtender<N, Type>& extender = ArrayExtender<N, Type>(),
			const FilterPtr& maxFilter = lanczosFilter(2),
			const FilterPtr& minFilter = triangleFilter())
			: mipMap_(&mipMap)
			, extender_(extender)
			, minFilterTable_()
			, maxFilterTable_()
			, filterRadius_(0)
		{
			setFilter(maxFilter, minFilter);
		}

		virtual ~EwaImage_Texture()
		{
			BOOST_STATIC_ASSERT(N == 2);
		}

		void setFilter(
			const FilterPtr& maxFilter,
			const FilterPtr& minFilter = triangleFilter());

		virtual Type operator()(
			const Vector<real, N>& p,
			const Matrix<real, N, N>& m) const;

		void setMipMap(const MipMap<N, Type>& mipMap)
		{
			mipMap_ = &mipMap;
		}

		void setExtender(const ArrayExtender<N, Type>& extender)
		{
			extender_ = extender;
		}

		virtual std::string name() const
		{
			return std::string("EwaImage");
		}

	private:
		Type sampleEwa(
			const Vector<real, N>& uv,
			const Matrix<real, N, N>& quadraticForm,
			const AlignedBox<real, N>& bound,
			real scaling,
			real tTransition,
			const Array<Type, N>& image) const;

		const MipMap<N, Type>* mipMap_;
		ArrayExtender<N, Type> extender_;
		std::vector<real> minFilterTable_;
		std::vector<real> maxFilterTable_;
		integer filterTableSize_;
		real filterRadius_;
	};

}

#include "pastel/gfx/ewaimage_texture.hpp"

#endif
