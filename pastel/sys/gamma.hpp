#ifndef PASTELSYS_GAMMA_HPP
#define PASTELSYS_GAMMA_HPP

#include "pastel/sys/gamma.h"
#include "pastel/sys/constants.h"

#include <cmath>

namespace Pastel
{

	template <typename Real>
	Real gamma(PASTEL_NO_DEDUCTION(Real) z)
	{
		// From Wikipedia, "Lanczos approximation".

		const integer g = 7;
		Real p[9] = {
			0.99999999999980993, 
			676.5203681218851, 
			-1259.1392167224028,
			 771.32342877765313, 
			 -176.61502916214059, 
			 12.507343278686905,
			 -0.13857109526572012, 
			 9.9843695780195716e-6, 
			 1.5056327351493116e-7};
		
		if (z < 0.5)
		{
			return constantPi<Real>() / 
				(std::sin(constantPi<Real>() * z) * gamma<Real>(1 - z));
		}

		z -= 1;
		Real x = p[0];
		for (integer i = 1;i < g + 2;++i)
		{
			x += p[i] / (z + i);
		}
		const Real t = z + g + 0.5;

		return std::sqrt(2 * constantPi<Real>()) * 
			std::pow(t, z + 0.5) * std::exp(-t) * x;
	}

	template <typename Real>
	Real lnGamma(PASTEL_NO_DEDUCTION(Real) z)
	{
		// From Wikipedia, "Lanczos approximation".

		const integer g = 7;
		Real p[9] = {
			0.99999999999980993, 
			676.5203681218851, 
			-1259.1392167224028,
			 771.32342877765313, 
			 -176.61502916214059, 
			 12.507343278686905,
			 -0.13857109526572012, 
			 9.9843695780195716e-6, 
			 1.5056327351493116e-7};
		
		if (z < 0.5)
		{
			return std::log(constantPi<Real>()) -
				(std::log(std::sin(constantPi<Real>() * z))
				+ lnGamma<Real>(1 - z));
		}

		z -= 1;
		Real x = p[0];
		for (integer i = 1;i < g + 2;++i)
		{
			x += p[i] / (z + i);
		}
		const Real t = z + g + 0.5;

		return 0.5 * std::log(2 * constantPi<Real>()) + 
			(z + 0.5) * std::log(t) - t + std::log(x);
	}

}

#endif
