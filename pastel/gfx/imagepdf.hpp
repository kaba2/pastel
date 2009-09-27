#ifndef PASTEL_IMAGEPDF_HPP
#define PASTEL_IMAGEPDF_HPP

#include "pastel/gfx/imagepdf.h"

#include "pastel/sys/arrayview.h"

#include <algorithm>

namespace Pastel
{

	template <typename Real, int N>
	template <typename Image_ConstView>
	ImagePdf<Real, N>::ImagePdf(
		const ConstView<N, Real, Image_ConstView>& image)
		: cdf_()
	{
		computeCdf(image);
	}

	template <typename Real, int N>
	template <typename Image_ConstView>
	void ImagePdf<Real, N>::computeCdf(
		const ConstView<N, Real, Image_ConstView>& image)
	{
		Real imageSum = 0;

		std::vector<CdfElement> cdfTmp;

		CdfFunctor cdfFunctor(imageSum, cdfTmp);
		visitPosition(image, cdfFunctor);

		cdfTmp.swap(cdf_);
	}

	template <typename Real, int N>
	Vector<integer, N> ImagePdf<Real, N>::operator()() const
	{
		if (cdf_.empty())
		{
			return Vector<integer, N>(0);
		}

		const Real value = random<Real>() * cdf_.back().cdf();

		return std::lower_bound(cdf_.begin(), cdf_.end(),
			CdfElement(value, Vector<integer, N>(0)))->position();
	}

}

#endif
