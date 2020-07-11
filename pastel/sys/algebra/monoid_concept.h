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
	template <typename T>
	concept Additive_Monoid_Concept_ =
		Additive_SemiGroup_Concept<T> && 
		requires(T t) {
			// Returns whether t == 0.
			{zero(t)} -> std::convertible_to<bool>;
	};

	template <typename T>
	concept Additive_Monoid_Concept =
		Additive_Monoid_Concept_<RemoveCvRef<T>>;

	//! A multiplicative monoid.
	/*!
	A monoid (X, *, 1), where 1 in X, is a semi-group (X, *), 
	such that there exist 1 in X such that 
		
		    x * 1 = x = 1 * x, for all x in X.
	*/
	template <typename T>
	concept Multiplicative_Monoid_Concept_ =
		Multiplicative_SemiGroup_Concept<T> && 
		requires(T t) {
			// Returns whether t == 1.
			{one(t)} -> std::convertible_to<bool>;
			//! Returns the power t^p, for p in NN^{>= 0}.
			{pow(t, (integer)0)} -> std::convertible_to<T>;
	};

	template <typename T>
	concept Multiplicative_Monoid_Concept =
		Multiplicative_Monoid_Concept_<RemoveCvRef<T>>;

}

namespace Pastel
{

	//! Computes x^p, for p in NN^{>= 0}.
	/*!
	The notation x^p means to multiply x with itself p times.
	*/
	template <Multiplicative_Monoid_Concept T>
	T monoidPower(T x, integer p)
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
