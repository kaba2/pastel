#ifndef PASTEL_IMAGEPDF_HPP
#define PASTEL_IMAGEPDF_HPP

#include "pastel/gfx/imagepdf.h"

#include "pastel/sys/arrayview.h"

#include <algorithm>

namespace Pastel
{

	template <int N, typename Real>
	template <typename Image_ConstView>
	ImagePdf<N, Real>::ImagePdf(
		const ConstView<N, Real, Image_ConstView>& image)
		: cdf_()
	{
		computeCdf(image);
	}

	template <int N, typename Real>
	template <typename Image_ConstView>
	void ImagePdf<N, Real>::computeCdf(
		const ConstView<N, Real, Image_ConstView>& image)
	{
		Real imageSum = 0;

		std::vector<CdfElement> cdfTmp;

		CdfFunctor cdfFunctor(imageSum, cdfTmp);
		visitPosition(image, cdfFunctor);

		cdfTmp.swap(cdf_);
	}

	template <int N, typename Real>
	Point<N, integer> ImagePdf<N, Real>::operator()() const
	{
		if (cdf_.empty())
		{
			return Point<N, integer>(0);
		}

		const Real value = random<Real>() * cdf_.back().cdf();

		return std::lower_bound(cdf_.begin(), cdf_.end(),
			CdfElement(value, Point<N, integer>(0)))->position();
	}

}

#endif
