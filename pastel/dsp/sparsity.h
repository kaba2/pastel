// Description: Sparsity measures

#ifndef PASTEL_SPARSITY_H
#define PASTEL_SPARSITY_H

#include <boost/iterator/iterator_traits.hpp>

namespace Pastel
{

	// Computes the square of 2-sparsity.
	template <typename Real_Range>
	typename boost::iterator_value<Real_Iterator>::type
		sparsity2(const Real_Range& input);

}

#include "pastel/dsp/sparsity.hpp"

#endif
