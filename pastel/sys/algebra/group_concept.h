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
	template <typename T>
	concept Additive_Group_Concept__ = 
		Additive_Monoid_Concept_<T> && 
		requires(T t) {
		//! Adds -that to the element.
		{t -= t} -> std::convertible_to<T>;
		//! Returns left * (-right).
		{t - t} -> std::convertible_to<T>;
		//! Returns -x for an element x.
		{-t} -> std::convertible_to<T>;
	};

	template <typename T>
	concept Additive_Group_Concept_ = 
		Additive_Group_Concept__<RemoveCvRef<T>>;

	//! A multiplicative group.
	/*!
	A group (X, *, 1) is a monoid (X, *, 1)
	such that for every x in X there exists y in X
	such that x * y = 1. Since the y is unique,
	it is denoted by inverse(x).
	*/
	template <typename T>
	concept Multiplicative_Group_Concept__ = 
		Multiplicative_Monoid_Concept_<T> && 
		requires(T t) {
		//! Multiplies with inverse(that) from the right.
		{t /= t} -> std::convertible_to<T>;
		//! Returns left * inverse(right).
		{t / t} -> std::convertible_to<T>;
		//! Returns inverse(x) for an element x.
		{inverse(t)} -> std::convertible_to<T>;
	};

	template <typename T>
	concept Multiplicative_Group_Concept_ = 
		Multiplicative_Group_Concept__<RemoveCvRef<T>>;

}

namespace Pastel
{

	//! Computes x^p, for p an integer.
	template <Multiplicative_Group_Concept_ T>
	T groupPower(T x, integer p)
	{
		T result = monoidPower(std::move(x), std::abs(p));

		if (p < 0)
		{
			result = inverse(result);
		}

		return result;
	}

}

#endif
