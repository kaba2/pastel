// Description: A function for executing graph-traversal algorithm-objects.

#ifndef PASTELSYS_GENERIC_TRAVERSAL_H
#define PASTELSYS_GENERIC_TRAVERSAL_H

#include "pastel/sys/graph_traversal_concepts.h"

namespace Pastel
{

	template <
		typename Vertex,
		typename ForEachSeedVertex,
		typename ForEachAdjacent,
		typename Mark,
		typename Marked,
		typename GraphTraversal_Algorithm>
	void traverseGraph(
		const ForEachSeedVertex& forEachSeedVertex,
		const ForEachAdjacent& forEachAdjacent,
		const Mark& mark,
		const Marked& marked,
		const GraphTraversal_Algorithm& algorithm)
	{
		algorithm.work<Vertex>(
			forEachSeedVertex,
			forEachAdjacent,
			mark,
			marked);
	}

}

#endif
