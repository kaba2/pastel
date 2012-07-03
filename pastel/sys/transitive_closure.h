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
	Let X be a set, (Y, +) be a commutative monoid, f : X --> Y, 
	and ~ subset X^2. Then the transitive closure f^+ : X --> Y 
	of f is given by

	    f^+(x) = +{f(x'): x ~^+ x'},

	where ~^+ is the transitive closure of ~ (i.e. the intersection
	of all transitive relations containing ~). We define +{} = I,
	where I is the identity of the monoid, and +{y} = y, for
	all y in Y.

	See 'transitive_closure_concepts.h' for the concepts.

	identity:
	The identity element I of the monoid Y.

	function:
	The function f : X --> Y.

	codomainOperator:
	The monoid operator + : Y^2 --> Y.
	
	forEachRelated:
	For a given domain element x, calls the given function
	on all x' such that x ~ x'. Specifies implicitly the 
	relation ~ in X^2.

	forEachDomain:
	For each domain element, calls the given function.
	Specifies implicitly the domain-set X. Will be called
	twice: once to compute the transitive closure, and 
	once to report the results.

	closureReporter:
	Reports the transitive closure function f^+ : X --> Y to the user 
	one (x, y)-pair at a time by closureReporter(x, std::move(y)). Only 
	the elements in the domain will be reported, even if some elements 
	might be related to elements outside the domain.

	reflexiveClosure:
	Extends the relation ~ to be reflexive, i.e. takes
	a reflexive-transitive closure instead.

	domainHash:
	Specifies the hash function to use for the domain
	elements.
	*/
	template <
		typename Domain, 
		typename Codomain,
		typename Function,
		typename CodomainOperator, 
		typename ForEachRelated,
		typename ForEachDomain,
		typename Closure_Reporter,
		typename Domain_Hash>
	void transitiveClosure(
		const PASTEL_NO_DEDUCTION(Codomain)& identity,
		const Function& function,
		const CodomainOperator& codomainOperator,
		const ForEachRelated& forEachRelated,
		const ForEachDomain& forEachDomain,
		const Closure_Reporter& closureReporter,
		bool reflexiveClosure = false,
		const Domain_Hash& domainHash = Hash<Domain>());

}

#include "pastel/sys/transitive_closure.hpp"

#endif
