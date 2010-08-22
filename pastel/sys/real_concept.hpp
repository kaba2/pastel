#ifndef PASTEL_REAL_CONCEPT_HPP
#define PASTEL_REAL_CONCEPT_HPP

#include "pastel/sys/real_concept.h"

namespace Pastel
{

	template <typename Real>
	Real infinity()
	{
		return Pastel::infinity((Real*)0);
	}

	template <typename Real>
	Real nan()
	{
		return Pastel::nan((Real*)0);
	}

}

#endif
