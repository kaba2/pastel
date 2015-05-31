// Description: Group concept
// Documentation: algebra.txt

#ifndef PASTELSYS_GROUP_CONCEPT_H
#define PASTELSYS_GROUP_CONCEPT_H

#include "pastel/sys/algebra/monoid_concept.h"

#include "pastel/sys/algebra/native_group.h"

namespace Pastel
{

	//! An additive group.
	/*!
	A group (X, +, 0) is a monoid (X, +, 0)
	such that for every x in X there exists y in X
	such that x + y = 0. Since the y is unique,
	it is denoted by -x.
	*/
	struct Additive_Group_Concept
	: Refines<Additive_Monoid_Concept>
	{
		template <typename Type>
		auto requires(Type&& t) -> decltype
		(
			conceptCheck(
				//! Adds -that to the element.
				Concept::hasType<Type&>(t -= t),
				//! Returns left * (-right).
				Concept::convertsTo<Type>(t - t),
				//! Returns -x for an element x.
				Concept::convertsTo<Type>(-t)
			)
		);
	};

	//! A multiplicative group.
	/*!
	A group (X, *, 1) is a monoid (X, *, 1)
	such that for every x in X there exists y in X
	such that x * y = 1. Since the y is unique,
	it is denoted by inverse(x).
	*/
	struct Multiplicative_Group_Concept
	: Refines<Multiplicative_Monoid_Concept>
	{
		template <typename Type>
		auto requires(Type&& t) -> decltype
		(
			conceptCheck(
				//! Multiplies with inverse(that) from the right.
				Concept::hasType<Type&>(t /= t),
				//! Returns left * inverse(right).
				Concept::convertsTo<Type>(t / t),
				//! Returns inverse(x) for an element x.
				Concept::convertsTo<Type>(inverse(t))
			)
		);
	};

}

namespace Pastel
{

	//! Computes x^p, for p an integer.
	template <typename Multiplicative_Group>
	Multiplicative_Group groupPower(
		Multiplicative_Group x,
		integer p)
	{
		Multiplicative_Group result = 
			monoidPower(std::move(x), std::abs(p));

		if (p < 0)
		{
			result = inverse(result);
		}

		return result;
	}

}

#endif
