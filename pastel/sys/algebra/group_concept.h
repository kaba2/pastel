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
		auto requires_(Type&& t) -> decltype
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

	//! An additive group.
	/*!
	A group (X, +, 0) is a monoid (X, +, 0)
	such that for every x in X there exists y in X
	such that x + y = 0. Since the y is unique,
	it is denoted by -x.
	*/
	template <typename T>
	concept Additive_Group_Concept_ = 
		Additive_Monoid_Concept_<T> && 
		requires(T t) {
		//! Adds -that to the element.
		{t -= t} -> std::convertible_to<RemoveCvRef<T>>/*HERE*/;
		//! Returns left * (-right).
		{t - t} -> std::convertible_to<RemoveCvRef<T>>;
		//! Returns -x for an element x.
		{-t} -> std::convertible_to<RemoveCvRef<T>>;
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
		auto requires_(Type&& t) -> decltype
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

	//! A multiplicative group.
	/*!
	A group (X, *, 1) is a monoid (X, *, 1)
	such that for every x in X there exists y in X
	such that x * y = 1. Since the y is unique,
	it is denoted by inverse(x).
	*/
	template <typename T>
	concept Multiplicative_Group_Concept_ = 
		Multiplicative_Monoid_Concept_<T> && 
		requires(T t) {
		//! Multiplies with inverse(that) from the right.
		{t /= t} -> std::convertible_to<RemoveCvRef<T>>/*HERE*/;
		//! Returns left * inverse(right).
		{t / t} -> std::convertible_to<RemoveCvRef<T>>;
		//! Returns inverse(x) for an element x.
		{inverse(t)} -> std::convertible_to<RemoveCvRef<T>>;
	};

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
