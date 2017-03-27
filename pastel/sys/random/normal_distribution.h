// Description: Normal distribution

#ifndef PASTELSYS_NORMAL_DISTRIBUTION_H
#define PASTELSYS_NORMAL_DISTRIBUTION_H

#include "pastel/sys/real/real_concept.h"

#include <armadillo>

namespace Pastel
{

	template <typename Real>
	class Normal_Distribution
	{
	public:
		using Point = arma::Col<Real>;

		explicit Normal_Distribution(integer n = 1)
		: mean_(n, arma::fill::zeros)
		, scale_(n, arma::fill::ones)
		, rotation_(n, n, arma::fill::eye)
		, detCovariance_(1)
		{
		}

		Normal_Distribution(const Normal_Distribution&) = default;
		Normal_Distribution(Normal_Distribution&&) = default;
		Normal_Distribution& operator=(const Normal_Distribution&) = default;
		Normal_Distribution& operator=(Normal_Distribution&&) = default;

		Normal_Distribution(
			arma::Col<Real> mean,
			arma::Col<Real> scale,
			arma::Mat<Real> rotation)
		: mean_(std::move(mean))
		, scale_(std::move(scale))
		, rotation_(std::move(rotation))
		, detCovariance_(0)
		{
			ENSURE_OP(mean.n_elem, ==, rotation.n_rows);
			ENSURE_OP(mean.n_elem, ==, rotation.n_cols);
			detCovariance_ = arma::prod(scale_);
		}

		integer n() const
		{
			return mean_.n_elem;
		}

		PASTEL_GETTER(mean);
		PASTEL_GETTER(scale);
		PASTEL_GETTER(rotation);
		PASTEL_GETTER(detCovariance);

	private:
		arma::Col<Real> mean_;
		arma::Col<Real> scale_;
		arma::Mat<Real> rotation_;
		Real detCovariance_;
	};

}

#endif
