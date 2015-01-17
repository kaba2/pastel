// Description: Maximum bipartite matching

#ifndef PASTELSYS_MAXIMUM_BIPARTITE_MATCHING_H
#define PASTELSYS_MAXIMUM_BIPARTITE_MATCHING_H

#include "pastel/sys/output/output_concept.h"

namespace Pastel
{

	//! Computes a maximum matching in a bipartite graph.
	/*!
	Preconditions:
	boost::size(leftRange) == boost::size(rightRange)

	Time complexity:
	O(sqrt(V) E) worst case, or
	O(log(V) E) average on random graphs,
	where 
	V is the number of vertices, and
	E is the number of edges.

	leftRange, rightRange:
	Sequences of vertices L(i) and R(i), 0 < i < n, denoting
	the class of left vertices and right vertices, respectively. 
	The edges of the graph are formed from (L(i), R(i)), 
	for 0 < i < n. The type of the left vertices may differ
	from the type of the right vertices. The left vertices are
	always treated as disjoint from the right vertices, even
	if the objects themselves would be equal.

	reporter:
	A reporter to report the edges in the maximum bipartite
	matching.

	leftHash, rightHash:
	Hash functions for hashing elements of the left vertex-set
	and the right vertex-set, respectively.
	*/
	template <
		typename Left_Vertex_Range, 
		typename Right_Vertex_Range, 
		typename Vertex_Pair_Output,
		typename Left_Hash,
		typename Right_Hash>
	void maximumBipartiteMatching(
		Left_Vertex_Range leftRange,
		Right_Vertex_Range rightRange,
		const Vertex_Pair_Output& reporter,
		const Left_Hash& leftHash,
		const Right_Hash& rightHash);

	//! Computes the maximum matching in a bipartite graph.
	/*!
	This is a convenience function which calls

	maximumBipartiteMatching(
		leftRange, rightRange, reporter,
		std::hash<Left>(), std::hash<Right>());

	See the documentation for that function.
	*/
	template <
		typename Left_Vertex_Range, 
		typename Right_Vertex_Range, 
		typename Vertex_Pair_Output>
	void maximumBipartiteMatching(
		Left_Vertex_Range leftRange,
		Right_Vertex_Range rightRange,
		const Vertex_Pair_Output& reporter);

}

#include "pastel/sys/maximum_bipartite_matching.hpp"

#endif
