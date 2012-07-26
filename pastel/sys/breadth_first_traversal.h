// Description: Breadth-first traversal of a graph

#ifndef PASTEL_BREADTH_FIRST_TRAVERSAL_H
#define PASTEL_BREADTH_FIRST_TRAVERSAL_H

#include "pastel/sys/breadth_first_traversal_concepts.h"

namespace Pastel
{

	//! Traverses a graph in a breadth-first manner.
	/*!
	This is a convenience function which calls

	traverseBreadthFirst(
		forEachSeedVertex, forEachAdjacent,
		report, std::hash<Vertex>())
	*/
	template <
		typename Vertex,
		typename ForEachSeedVertex,
		typename ForEachAdjacent,
		typename Vertex_Reporter>
	void traverseBreadthFirst(
		const ForEachSeedVertex& forEachSeedVertex,
		const ForEachAdjacent& forEachAdjacent,
		const Vertex_Reporter& report);

	//! Traverses a graph in a breadth-first manner.
	/*!
	Preconditions:
	Vertex is copy-constructible.

	For a vertex-set V, the set of seed vertices is a set
	S subset V. The seed vertices will be traversed sequentially
	in a breadth-first manner. Every vertex reachable from 
	the seed vertices will be visited exactly once.
	*/
	template <
		typename Vertex,
		typename ForEachSeedVertex,
		typename ForEachAdjacent,
		typename Vertex_Reporter,
		typename Vertex_Hash>
	void traverseBreadthFirst(
		const ForEachSeedVertex& forEachSeedVertex,
		const ForEachAdjacent& forEachAdjacent,
		const Vertex_Reporter& report,
		const Vertex_Hash& vertexHash);

}

#include "pastel/sys/breadth_first_traversal.hpp"

#endif
