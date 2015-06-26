#ifndef PASTELSYS_MAXIMAL_BIPARTITE_MATCHING_HPP
#define PASTELSYS_MAXIMAL_BIPARTITE_MATCHING_HPP

#include "pastel/sys/graph/matching/maximal_bipartite_matching.h"

#include <unordered_set>

namespace Pastel
{

	template <
		typename ForEachAdjacentToA,
		typename... ArgumentSet>
	integer maximalBipartiteMatching(
		integer nA,
		integer nB,
		const ForEachAdjacentToA& forEachAdjacentToA,
		ArgumentSet&&... argumentSet)
	{
		// This is a simple greedy algorithm to compute a 
		// maximal matching.

		ENSURE_OP(nA, >=, 0);
		ENSURE_OP(nB, >=, 0);

		// Let G = (V, E) be a simple directed graph, such that
		// 
		//     A = {0} x [0, nA[,
		//     B = {1} x [0, nB[,
		//     V = A union B,
		//     E subset A x B.
		//
		// The G is a bipartite graph. A _matching_ in G is M subset E,
		// such that 
		//
		//     (a_1, b_1), (a_2, b_2) in M ==> a_1 != a_2 and b_1 != b_2,
		//
		// for all a_1, a_2 in A, b_1, b_2 in B. We wish to find a 
		// matching with maximal cardinality |M|.

		if (nA == 0 || nB == 0)
		{
			// Since either A or B is empty, there are no matchings 
			// in the graph.
			return 0;
		}

		auto&& report = PASTEL_ARG_S(report, nullOutput());

		// The set of paired vertices in the B-set.
		std::unordered_set<integer> bReservedSet;

		integer nMatch = 0;
		for (integer a = 0;a < nA;++a)
		{
			forEachAdjacentToA(a, [&](integer b)
			{
				if (bReservedSet.count(b) == 0)
				{
					// The 'b' is unpaired; pair it to 'a'.
					bReservedSet.insert(b);

					// The edges in the matching never
					// change; the algorithm is greedy.
					// Report the edge.
					report(a, b);
					++nMatch;

					// Stop going through adjacent vertices.
					return false;
				}

				// Next adjacent vertex.
				return true;
			});
		}

		// Return the number of edges in the matching.
		return nMatch;
	};

}

#endif
