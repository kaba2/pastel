// Description: Normal distribution

#ifndef PASTELSYS_NORMAL_DISTRIBUTION_H
#define PASTELSYS_NORMAL_DISTRIBUTION_H

#include "pastel/sys/real/real_concept.h"
#include "pastel/math/matrix.h"

namespace Pastel
{

	template <typename Real>
	class Normal_Distribution
	{
	public:
		using Point = ColMatrix<Real>;

		explicit Normal_Distribution(integer n = 1)
		: mean_(ColMatrix<Real>::Zero(n))
		, scale_(ColMatrix<Real>::One(n))
		, rotation_(Matrix<Real>::Identity(n, n))
		, detCovariance_(1)
		{
		}

		Normal_Distribution(const Normal_Distribution&) = default;
		Normal_Distribution(Normal_Distribution&&) = default;
		Normal_Distribution& operator=(const Normal_Distribution&) = default;
		Normal_Distribution& operator=(Normal_Distribution&&) = default;

		Normal_Distribution(
			ColMatrix<Real> mean,
			ColMatrix<Real> scale,
			Matrix<Real> rotation)
		: mean_(std::move(mean))
		, scale_(std::move(scale))
		, rotation_(std::move(rotation))
		, detCovariance_(0)
		{
			ENSURE_OP(mean.size(), ==, rotation.rows());
			ENSURE_OP(mean.size(), ==, rotation.cols());
			detCovariance_ = scale_.prod();
		}

		integer n() const
		{
			return mean_.size();
		}

		const ColMatrix<Real>& mean() const {
			return mean_;
		}

		const ColMatrix<Real>& scale() const {
			return scale_;
		}

		const Matrix<Real>& rotation() const {
			return rotation_;
		}

		Real detCovariance() const {
			return detCovariance_;
		}

	private:
		ColMatrix<Real> mean_;
		ColMatrix<Real> scale_;
		Matrix<Real> rotation_;
		Real detCovariance_;
	};

}

#endif
