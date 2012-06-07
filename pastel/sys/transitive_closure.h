// Description: Transitive closure of a function

#ifndef PASTEL_TRANSITIVE_CLOSURE_H
#define PASTEL_TRANSITIVE_CLOSURE_H

#include "pastel/sys/transitive_closure_concepts.h"
#include "pastel/sys/reporter_concept.h"
#include "pastel/sys/hash.h"

namespace Pastel
{

	//! Transitive closure of a function
	/*!
	See 'transitive_closure_concepts.h' for the concepts.

	Let X be a set, (Y, +) be a commutative monoid, f : X --> Y, 
	and ~ subset X^2. Then the transitive closure f^+ : X --> Y 
	of f is given by

	    f^+(x) = +{f(x'): x ~^+ x'},

	where ~^+ is the transitive closure of ~ (i.e. the intersection
	of all transitive relations containing ~). We define +{} = I,
	where I is the identity of the monoid, and +{y} = y, for
	all y in Y.

	reflexiveClosure:
	Extends the relation ~ to be reflexive, i.e. takes
	a reflexive-transitive closure instead.
	*/
	template <
		typename Domain, 
		typename Codomain,
		typename Function,
		typename CodomainOperator, 
		typename ForEachRelated,
		typename ForEachDomain,
		typename Function_Reporter,
		typename Domain_Hash>
	void transitiveClosure(
		const PASTEL_NO_DEDUCTION(Codomain)& identity,
		const Function& function,
		const CodomainOperator& codomainOperator,
		const ForEachRelated& forEachRelated,
		const ForEachDomain& forEachDomain,
		const Function_Reporter& functionReporter,
		bool reflexiveClosure = false,
		const Domain_Hash& domainHash = Hash<Domain>());

}

#include "pastel/sys/transitive_closure.hpp"

#endif
