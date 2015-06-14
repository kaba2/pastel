// Description: EwaImage_Texture class
// Detail: Image-based texture with radial reconstruction and EWA filtering.

#ifndef PASTELGFX_EWAIMAGE_TEXTURE_H
#define PASTELGFX_EWAIMAGE_TEXTURE_H

#include "pastel/gfx/texture/texture.h"

#include "pastel/sys/syscommon.h"
#include "pastel/sys/vector/vector_tools.h"
#include "pastel/sys/extender/clampextender.h"

#include "pastel/gfx/filter/triangle_filter.h"
#include "pastel/gfx/filter/lanczos_filter.h"
#include "pastel/gfx/mipmap/mipmap.h"

#include "pastel/math/matrix/matrix.h"

#include "pastel/geometry/shape/alignedbox.h"

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

	- Prefer Triangle_Filter to Gaussian_Filter for minification.
	It has a smaller radius, no visible clamping step,
	and performs equivalently to the eye.
	- The combination of a Triangle_Filter for minification
	and a Lanczos_Filter(2) for magnification seems to deliver
	the best results of all combinations. These are
	the defaults.
	*/

	template <typename Type, integer N = 2>
	class EwaImage_Texture
		: public Texture<Type, N>
	{
	public:
		using ArrayExtender_ = ArrayExtender<N, Type>;
		
		explicit EwaImage_Texture(
			const MipMap<Type, N>& mipMap,
			const ArrayExtender_& extender = ArrayExtender_(),
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
			PASTEL_STATIC_ASSERT(N == 2);
		}

		void setFilter(
			const FilterPtr& maxFilter,
			const FilterPtr& minFilter = triangleFilter());

		virtual Type operator()(
			const Vector<real, N>& p,
			const Matrix<real>& m) const;

		void setMipMap(const MipMap<Type, N>& mipMap)
		{
			mipMap_ = &mipMap;
		}

		const MipMap<Type, N>& mipMap() const
		{
			PENSURE(mipMap_);
			return *mipMap_;
		}

		void setExtender(const ArrayExtender_& extender)
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
			const Matrix<real>& quadraticForm,
			const AlignedBoxD& bound,
			real scaling,
			real tTransition,
			const Array<Type, N>& image) const;

		const MipMap<Type, N>* mipMap_;
		ArrayExtender_ extender_;
		std::vector<real> minFilterTable_;
		std::vector<real> maxFilterTable_;
		integer filterTableSize_;
		real filterRadius_;
	};

}

#include "pastel/gfx/texture/ewaimage_texture.hpp"

#endif
