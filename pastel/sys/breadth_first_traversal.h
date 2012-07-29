// Description: Breadth-first traversal of a graph

#ifndef PASTEL_BREADTH_FIRST_TRAVERSAL_H
#define PASTEL_BREADTH_FIRST_TRAVERSAL_H

#include "pastel/sys/graph_traversal_concepts.h"

namespace Pastel
{

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
		typename Mark,
		typename Marked>
	void traverseBreadthFirst(
		const ForEachSeedVertex& forEachSeedVertex,
		const ForEachAdjacent& forEachAdjacent,
		const Mark& mark,
		const Marked& marked);

	//! An algorithm-object for breadth-first traversal.
	class BreadthFirst_GraphTraversal
	{
	public:
		template <
			typename Vertex,
			typename ForEachSeedVertex,
			typename ForEachAdjacent,
			typename Mark,
			typename Marked>
		void operator()(
			const ForEachSeedVertex& forEachSeedVertex,
			const ForEachAdjacent& forEachAdjacent,
			const Mark& mark,
			const Marked& marked)
		{
			traverseBreadthFirst<Vertex>(
				forEachSeedVertex,
				forEachAdjacent,
				mark,
				marked);
		}
	};

}

#include "pastel/sys/breadth_first_traversal.hpp"

#endif
