// Description: Maximal bipartite matching

#ifndef PASTELSYS_MAXIMAL_BIPARTITE_MATCHING_H
#define PASTELSYS_MAXIMAL_BIPARTITE_MATCHING_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/output/output_concept.h"

namespace Pastel
{

	//! Maximal bipartite matching
	/*!
	Maximal matching works as a fast
	approximation to the maximum matching.
	A maximal matching is not necessary
	a maximum matching; see 
	maximum_bipartite_matching.h 
	for that. 

	Preconditions:
	nA >= 0
	nB >= 0

	Time complexity:
	O(|A| + |E|) worst case
	where 
	V = A union B is the set of vertices, and
	E subset A x B is the set of edges.

	nA, nB:
	The number of vertices in the A and B set,
	respectively.

	returns:
	The number of edges in the maximal matching.

	Optional arguments
	------------------

	forEachAdjacentToA (A x (B -> bool)) -> C):
	A function g : A x (B -> bool) -> C, which for
	g(a, f) calls f(b) for all (a, b) in E. The f
	returns whether to continue iterating over adjacent
	vertices, to which g must respond.

	report (A x B -> C):
	The edges in the maximal matching will be reported
	in the form report(a, b), where a in A and b in B.
	*/
	template <
		typename ForEachAdjacentToA,
		typename... ArgumentSet>
	integer maximalBipartiteMatching(
		integer nA,
		integer nB,
		const ForEachAdjacentToA& forEachAdjacentToA,
		ArgumentSet&&... argumentSet);

}

#include "pastel/sys/graph/matching/maximal_bipartite_matching.hpp"

#endif
