// Description: Concepts for the functional transitive closure

#ifndef PASTELSYS_TRANSITIVE_CLOSURE_CONCEPTS_H
#define PASTELSYS_TRANSITIVE_CLOSURE_CONCEPTS_H

#include "pastel/sys/graph/transitive_closure.h"

#include <functional>

namespace Pastel
{

	namespace TransitiveClosure_Concepts
	{

		//! The elements of the set X.
		class Domain {};

		//! The elements Y of the commutative monoid (Y, +).
		class Codomain {};

		//! The commutative monoid operator + : Y^2 --> Y.
		class CodomainOperator
		{
		public:
			//! Returns left + right.
			/*!
			Must be associative, commutative, and have
			the identity element I. Note that the 'left'
			argument is passed by rvalue-reference:
			this can be used to optimize the operation.
			*/
			Codomain operator()(
				Codomain&& left, 
				const Codomain& right) const;
		};

		//! The function f : X --> Y.
		class Function
		{
		public:
			//! Returns f(x).
			/*!
			Note: you might want to return by const reference
			instead for performance. However, this is not 
			required.
			*/
			Codomain operator()(const Domain& x) const;
		};

		//! Visits each x in X.
		class ForEachDomain
		{
		public:
			//! Visits each x in X.
			void operator()(
				const std::function<void(const Domain&)>& visitor) const;
		};

		//! Visits each x' in X such that x ~ x'.
		class ForEachRelated
		{
		public:
			//! Visits each x' in X such that x ~ x'.
			void operator()(
				const Domain& x,
				const std::function<void(const Domain&)>& visit) const;
		};

		class Closure_Output
		{
		public:
			//! Reports the value of the closure-function at x.
			void operator()(
				const Domain& x,
				Codomain&& closure) const;
		};

	}

}

#endif

