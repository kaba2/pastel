// Description: Maximum bipartite matching

#ifndef PASTELSYS_MAXIMUM_BIPARTITE_MATCHING_H
#define PASTELSYS_MAXIMUM_BIPARTITE_MATCHING_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/output/output_concept.h"

namespace Pastel
{

	//! Maximum bipartite matching
	/*!
	Preconditions:
	nA >= 0
	nB >= 0

	Time complexity:
	O(sqrt(V) E) worst case, or
	O(log(V) E) average on random graphs,
	where 
	V is the number of vertices, and
	E is the number of edges.

	nA, nB:
	The number of vertices in the A and B set,
	respectively.

	returns:
	The number of edges in the maximum matching.

	Optional arguments
	------------------

	forEachAdjacentToA (A x (B -> bool)) -> C):
	A function g : A x (B -> bool) -> C, which for
	g(a, f) calls f(b) for all (a, b) in E. The f
	returns whether to continue iterating over adjacent
	vertices, to which g must respond.

	report (A x B -> C):
	The edges in the maximum matching will be reported
	in the form report(a, b), where a in A and b in B.
	*/
	template <
		typename ForEachAdjacentToA,
		typename... ArgumentSet>
	integer maximumBipartiteMatching(
		integer nA,
		integer nB,
		const ForEachAdjacentToA& forEachAdjacentToA,
		ArgumentSet&&... argumentSet);

}

#include "pastel/sys/graph/matching/maximum_bipartite_matching.hpp"

#endif
