#ifndef PASTEL_REAL_CONCEPT_HPP
#define PASTEL_REAL_CONCEPT_HPP

#include "pastel/sys/real_concept.h"

namespace Pastel
{

	template <typename Real>
	class Infinity 
	{
	public:
		Real operator()() const;
	};

	template <typename Real>
	class Nan 
	{
	public:
		Real operator()() const;
	};

	template <typename Real>
	Real infinity()
	{
		Infinity<Real> f;
		return f();
	}

	template <typename Real>
	Real nan()
	{
		Nan<Real> f;
		return f();
	}

}

#endif
