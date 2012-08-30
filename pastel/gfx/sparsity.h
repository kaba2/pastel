// Description: Sparsity measures

#ifndef PASTELGFX_SPARSITY_H
#define PASTELGFX_SPARSITY_H

#include <boost/iterator/iterator_traits.hpp>

namespace Pastel
{

	// Computes the square of 2-sparsity.
	template <typename Real_Range>
	typename boost::range_value<Real_Range>::type
		sparsity2(const Real_Range& input);

}

#include "pastel/gfx/sparsity.hpp"

#endif
