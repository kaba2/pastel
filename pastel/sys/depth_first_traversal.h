// Description: Depth-first traversal of a graph

#ifndef PASTEL_DEPTH_FIRST_TRAVERSAL_H
#define PASTEL_DEPTH_FIRST_TRAVERSAL_H

#include "pastel/sys/depth_first_traversal_concepts.h"

namespace Pastel
{

	//! Traverses a graph in a depth-first manner.
	/*!
	This is a convenience function which calls

	traverseDepthFirst(
		forEachSeedVertex, forEachRelated,
		report, std::hash<Vertex>())
	*/
	template <
		typename Vertex,
		typename ForEachSeedVertex,
		typename ForEachRelated,
		typename Vertex_Reporter>
	void traverseDepthFirst(
		const ForEachSeedVertex& forEachSeedVertex,
		const ForEachRelated& forEachRelated,
		const Vertex_Reporter& report);

	//! Traverses a graph in a depth-first manner.
	/*!
	report:
	Reports a vertex to the user.

	returns:
	Whether all of the reachable vertices were traversed
	(early exit was not committed).

	For a vertex-set V, the set of seed vertices is a set
	S subset V. The seed vertices will be traversed sequentially
	in a depth-first manner. The traversal from an earlier seed 
	vertex may visit a later seed vertex, in which case the
	traversal of the later seed vertex reports nothing. Every
	vertex reachable from the seed vertices will be visited 
	exactly once.
	*/
	template <
		typename Vertex,
		typename ForEachSeedVertex,
		typename ForEachRelated,
		typename Vertex_Reporter,
		typename Vertex_Hash>
	void traverseDepthFirst(
		const ForEachSeedVertex& forEachSeedVertex,
		const ForEachRelated& forEachRelated,
		const Vertex_Reporter& report,
		const Vertex_Hash& vertexHash);

}

#include "pastel/sys/depth_first_traversal.hpp"

#endif
