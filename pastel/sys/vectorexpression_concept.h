// Description: Vector expression concept

#ifndef PASTELSYS_VECTOREXPRESSION_CONCEPT_H
#define PASTELSYS_VECTOREXPRESSION_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	namespace VectorExpression_Concept
	{

		class VectorExpression
		{
		public:
			//! Compile-time dimension.
			/*!
			Can be either Dynamic or a positive integer.
			If N != Dynamic, then N == dimension().
			*/
			enum {N = UserDefinedInteger};

			//! The type of the elements.
			typedef UserDefinedType Real;

			//! A const-iterator to elements of type Real.
			typedef UserDefinedType ConstIterator;

			//! Returns a const-iterator to the beginning of the elements.
			ConstIterator begin() const;

			//! Returns a const-iterator to the end of the elements.
			ConstIterator end() const;

			//! Returns the dimension of the vector.
			integer size() const;

			//! Compute the i:th element of the vector expression.
			/*!
			Preconditions:
			index >= 0
			index < dimension()

			The requested element is computed lazily on the fly.
			Because of this, the elements can't be modified.
			*/
			Real operator[](integer index) const;

			//! Returns the dimension of the vector.
			/*!
			Implemented automatically by the VectorExpression 
			CRTP base class. Calls size().
			*/
			integer dimension() const;

			//! Returns whether the expression uses the given memory region.
			/*!
			A vector expression 'expr' _uses_ a memory region if there is an
			index 'i' such that 'expr[i]' accesses the memory region to
			compute its result.
			*/
			bool involves(
				const void* memoryBegin, 
				const void* memoryEnd) const;

			//! Returns whether to evaluate the expression before assignment.
			/*!
			A vector expression 'expr' needs to be evaluated into a temporary 
			before assignment 'v = expr' if and only if there is an index 
			'i' such that 'expr[i]' uses some memory location other than 'v[i]' 
			from the given memory region. Particularly, no evaluation to a
			temporary needs to be done when doing only elementwise 
			combinations.
			*/
			bool evaluateBeforeAssignment(
				const void* memoryBegin, 
				const void* memoryEnd) const;

			// Note: The following are implemented automatically when 
			// deriving from the VectorExpression CRTP base class.

			//! Element-wise sum with 'that'.
			VectorExpression operator+(
				const VectorExpression& that) const;

			//! Element-wise difference with 'that'.
			VectorExpression operator-(
				const VectorExpression& that) const;

			//! Element-wise product with 'that'.
			VectorExpression operator*(
				const VectorExpression& that) const;

			//! Element-wise division with 'that'.
			VectorExpression operator/(
				const VectorExpression& that) const;

			//! Element-wise negation.
			VectorExpression operator-() const;

			//! Element-wise sum with a constant vector 'that'.
			VectorExpression operator+(
				const Real& that) const;

			//! Element-wise sum with a constant vector from left.
			friend VectorExpression operator+(
				const Real& left, 
				const VectorExpression& right);

			//! Element-wise difference with a constant vector 'that'.
			VectorExpression operator-(
				const Real& that) const;

			//! Element-wise difference with a constant vector from left.
			friend VectorExpression operator-(
				const Real& left, 
				const VectorExpression& right);

			//! Element-wise product with a constant vector 'that'.
			VectorExpression operator*(
				const Real& that) const;

			//! Element-wise product with a constant vector from left.
			friend VectorExpression operator*(
				const Real& left, 
				const VectorExpression& right);

			//! Element-wise division with a constant vector 'that'.
			VectorExpression operator/(
				const Real& that) const;

			//! Element-wise division with a constant vector from left.
			friend VectorExpression operator/(
				const Real& left, 
				const VectorExpression& right);

			//! Returns whether two vector expression are equal.
			/*!
			Vector expression are equal when they compare equal
			elementwise.
			*/
			bool operator==(const VectorExpression& that) const;

			//! Returns whether two vector expression are not equal.
			/*!
			Returns '!(*this == that)'.
			*/
			bool operator!=(const VectorExpression& that) const;
		};

	}

}

#endif
