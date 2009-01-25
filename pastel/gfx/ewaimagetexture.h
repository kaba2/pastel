#ifndef PASTELGFX_EWAIMAGETEXTURE_H
#define PASTELGFX_EWAIMAGETEXTURE_H

#include "pastel/sys/syscommon.h"
#include "pastel/sys/vector_tools.h"
#include "pastel/sys/clampextender.h"

#include "pastel/gfx/trianglefilter.h"
#include "pastel/gfx/lanczosfilter.h"
#include "pastel/gfx/mipmap.h"

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

	template <typename Type>
	class EwaImageTexture
		: public Texture<Type>
	{
	public:
		explicit EwaImageTexture(
			const MipMap<2, Type>& mipMap,
			const ArrayExtender<2, Type>& extender = ArrayExtender<2, Type>(),
			const FilterRef& maxFilter = lanczosFilter(2),
			const FilterRef& minFilter = triangleFilter())
			: mipMap_(&mipMap)
			, extender_(extender)
			, minFilterTable_()
			, maxFilterTable_()
			, filterRadius_(0)
		{
			setFilter(maxFilter, minFilter);
		}

		virtual ~EwaImageTexture()
		{
		}

		void setFilter(
			const FilterRef& maxFilter,
			const FilterRef& minFilter = triangleFilter())
		{
			ENSURE(!maxFilter.empty() && !minFilter.empty());

			const real filterRadius = 
				std::max(minFilter->radius(), maxFilter->radius());

			const integer tableSize = filterRadius * 1024;
			const real scaling = square(filterRadius) / tableSize;

			std::vector<real> minFilterTable(tableSize);
			std::vector<real> maxFilterTable(tableSize);

			for (integer i = 0;i < tableSize;++i)
			{
				const real t = std::sqrt(i * scaling);

				minFilterTable[i] = (*minFilter)(t);
				maxFilterTable[i] = (*maxFilter)(t);
			}

			minFilterTable_.swap(minFilterTable);
			maxFilterTable_.swap(maxFilterTable);
			filterTableSize_ = tableSize;
			filterRadius_ = filterRadius;
		}

		virtual Type operator()(
			const Point2& p, 
			const Vector2& dpDx, 
			const Vector2& dpDy) const;

		void setMipMap(const MipMap<2, Type>& mipMap)
		{
			mipMap_ = &mipMap;
		}

		void setExtender(const ArrayExtender<2, Type>& extender)
		{
			extender_ = extender;
		}

		virtual std::string name() const
		{
			return std::string("EwaImage");
		}

	private:
		Type sampleEwa(
			const Point2& uv,
			const Matrix2& quadraticForm,
			const AlignedBox2& bound,
			real scaling,
			real tTransition,
			const LinearArray<2, Type>& image) const;

		const MipMap<2, Type>* mipMap_;
		ArrayExtender<2, Type> extender_;
		std::vector<real> minFilterTable_;
		std::vector<real> maxFilterTable_;
		integer filterTableSize_;
		real filterRadius_;
	};

}

#include "pastel/gfx/ewaimagetexture.hpp"

#endif
