// Description: Monoid concept
// Documentation: algebra.txt

#ifndef PASTELSYS_MONOID_CONCEPT_H
#define PASTELSYS_MONOID_CONCEPT_H

#include "pastel/sys/algebra/semigroup_concept.h"

#include "pastel/sys/algebra/native_monoid.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	//! An additive monoid.
	/*!
	A monoid (X, +, 0), where 0 in X, is a semi-group (X, +), 
	such that there exist 0 in X such that 
		
		    x + 0 = x = 0 + x, for all x in X.
	*/
	struct Additive_Monoid_Concept
	: Refines<Additive_SemiGroup_Concept>
	{
		template <typename Type>
		auto requires_(Type&& t) -> decltype
		(
			conceptCheck(
				// Returns whether t == 0.
				Concept::convertsTo<bool>(zero(t))
			)
		);
	};

	//! A multiplicative monoid.
	/*!
	A monoid (X, *, 1), where 1 in X, is a semi-group (X, *), 
	such that there exist 1 in X such that 
		
		    x * 1 = x = 1 * x, for all x in X.
	*/
	struct Multiplicative_Monoid_Concept
	: Refines<Multiplicative_SemiGroup_Concept>
	{
		template <typename Type>
		auto requires_(Type&& t) -> decltype
		(
			conceptCheck(
				// Returns whether t == 1.
				Concept::convertsTo<bool>(one(t)),
				//! Returns the power t^p, for p in NN^{>= 0}.
				Concept::convertsTo<Type>(pow(t, (integer)0))
			)
		);
	};

}

namespace Pastel
{

	//! Computes x^p, for p in NN^{>= 0}.
	/*!
	The notation x^p means to multiply x with itself p times.
	*/
	template <typename Multiplicative_Monoid>
	Multiplicative_Monoid monoidPower(
		Multiplicative_Monoid x,
		integer p)
	{
		ENSURE_OP(p, >=, 0);

		if (p == 0)
		{
			return 1;
		}

		return semiGroupPower(std::move(x), p);
	}

}

#endif
