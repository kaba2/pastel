// Description: Maximum bipartite matching

#ifndef PASTEL_MAXIMUM_BIPARTITE_MATCHING_H
#define PASTEL_MAXIMUM_BIPARTITE_MATCHING_H

#include "pastel/sys/as_pair_concept.h"
#include "pastel/sys/reporter_concept.h"

namespace Pastel
{

	//! Computes the maximum matching in a bipartite graph.
	/*!
	Time complexity:
	O(sqrt(V) E) worst case, or
	O(log(V) E) average on random graphs,
	where 
	V is the number of vertices, and
	E is the number of edges.

	The vertices of the graph are divided into two groups, 'left'
	and 'right' vertices. The left and right ones are the first and
	second elements, respectively, of the pairs provided as inputs.
	The pairs themselves provided the edges of the graph. 
	*/
	template <
		typename Vertex_Pair_ForwardRange, 
		typename Vertex_Pair_Reporter,
		typename As_Pair,
		typename LeftHash,
		typename RightHash>
	void maximumBipartiteMatching(
		Vertex_Pair_ForwardRange range,
		const Vertex_Pair_Reporter& reporter,
		const As_Pair& asPair,
		const LeftHash& leftHash,
		const RightHash& rightHash);

	//! Computes the maximum matching in a bipartite graph.
	/*!
	This is a convenience function which calls

	maximumBipartiteMatching(
		range, reporter, asPair,
		std::hash<Left>(), std::hash<Right>());

	See the documentation for that function.
	*/
	template <
		typename Vertex_Pair_ForwardRange, 
		typename Vertex_Pair_Reporter,
		typename As_Pair>
	void maximumBipartiteMatching(
		Vertex_Pair_ForwardRange range,
		const Vertex_Pair_Reporter& reporter,
		const As_Pair& asPair);

	//! Computes the maximum matching in a bipartite graph.
	/*!
	This is a convenience function which calls

	maximumBipartiteMatching(
		range, reporter, Tuple_As_Pair());

	See the documentation for that function.
	*/
	template <
		typename Tuple2_ForwardRange,
		typename Vertex_Pair_Reporter>
	void maximumBipartiteMatching(
		Tuple2_ForwardRange range,
		const Vertex_Pair_Reporter& reporter);

}

#include "pastel/sys/maximum_bipartite_matching.hpp"

#endif
