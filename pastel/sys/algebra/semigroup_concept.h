// Description: Semi-group concept
// Documentation: algebra.txt

#ifndef PASTELSYS_SEMIGROUP_CONCEPT_H
#define PASTELSYS_SEMIGROUP_CONCEPT_H

#include "pastel/sys/algebra/element_concept.h"
#include "pastel/sys/algebra/native_semigroup.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	//! An additive semi-group.
	/*!
	A semi-group is a pair (X, +), where X is a set 
	and + : X^2 --> X is associative.
	*/
	struct Additive_SemiGroup_Concept
	: Refines<Element_Concept>
	{
		template <typename Type>
		auto requires_(Type&& t) -> decltype
		(
			conceptCheck(
				//! Adds 'that' to the element.
				Concept::hasType<Type&>(t += t),
				//! Returns left + right.
				Concept::convertsTo<Type>(t + t)
			)
		);
	};

	//! An additive semi-group.
	/*!
	A semi-group is a pair (X, +), where X is a set 
	and + : X^2 --> X is associative.
	*/
	template <typename T>
	concept Additive_SemiGroup_Concept__ = 
		Element_Concept_<T> && 
		requires(T t) {
		//! Adds 'that' to the element.
		t += t;
		//! Returns left + right.
		{t + t} -> std::convertible_to<T>;
	};

	template <typename T>
	concept Additive_SemiGroup_Concept_ = 
		Additive_SemiGroup_Concept__<RemoveCvRef<T>>;

	// For native types the operators += and + are inbuilt.

	//! A multiplicative semi-group.
	/*!
	A semi-group is a pair (X, *), where X is a set 
	and * : X^2 --> X is associative.
	*/
	struct Multiplicative_SemiGroup_Concept
	: Refines<Element_Concept>
	{
		template <typename Type>
		auto requires_(Type&& t) -> decltype
		(
			conceptCheck(
				//! Adds 'that' to the element.
				Concept::hasType<Type&>(t *= t),
				//! Returns left * right.
				Concept::convertsTo<Type>(t * t),
				//! Returns the power t^p, for p in NN^{> 0}.
				Concept::convertsTo<Type>(pow(t, (integer)1))
			)
		);
	};

	//! A multiplicative semi-group.
	/*!
	A semi-group is a pair (X, *), where X is a set 
	and * : X^2 --> X is associative.
	*/
	template <typename T>
	concept Multiplicative_SemiGroup_Concept__ = 
		Element_Concept_<T> && 
		requires(T t) {
		//! Multiplies 'that' to the element.
		{t *= t} -> std::convertible_to<T>;
		//! Returns left * right.
		{t * t} -> std::convertible_to<T>;
		//! Returns the power t^p, for p in NN^{> 0}.
		{pow(t, (integer)1)} -> std::convertible_to<T>;
	};

	template <typename T>
	concept Multiplicative_SemiGroup_Concept_ = 
		Multiplicative_SemiGroup_Concept__<RemoveCvRef<T>>;

	// For native types the operators *= and * are inbuilt.

}

namespace Pastel
{

	//! Computes x^p, for p in NN^{> 0}.
	/*!
	The notation x^p means to multiply x with itself p times.
	*/
	template <Multiplicative_SemiGroup_Concept_ T>
	T semiGroupPower(T x, integer p)
	{
		ENSURE_OP(p, >, 0);

		// The binary expansion of p in NN^{>= 0} is
		//
		//     p = sum_{i in [0, n]} b_i 2^i,
		//
		// where b_i in {0, 1}. It follows that
		//
		//     x^p = prod_{i in [0, n]} x^{b_i 2^i}.
		//
		// On the other hand, consider 
		//
		//     a_0 = x
		//     a_i = a_{i - 1}^2.
		//
		// Then
		//
		//     a_i = x^{2^i}.
		//
		// Therefore, we can compute the power as
		//
		//     x^p = prod_{i in [0, n]} a_i^{b_i}.
		//         = prod_{i in [0, n] : b_i = 1} a_i.

		T result = x;
		
		--p;
		if (p == 0)
		{
			return result;
		}

		while (true)
		{
			ASSERT_OP(p, >, 0);

			if ((p & 1) != 0)
			{
				result *= x;
			}

			p >>= 1;
			if (p == 0)
			{
				break;
			}

			x *= x;
		}

		return result;
	}

}

#endif
