#ifndef PASTEL_DEPTH_FIRST_TRAVERSAL_HPP
#define PASTEL_DEPTH_FIRST_TRAVERSAL_HPP

#include "pastel/sys/depth_first_traversal.h"
#include "pastel/sys/hash.h"

#include <unordered_set>
#include <functional>

namespace Pastel
{

	template <
		typename Vertex, 
		typename VertexHash = std::hash<Vertex>>
	class TraverseDepthFirst
	{
	public:
		template <
			typename ForEachSeedVertex,
			typename ForEachAdjacent,
			typename Vertex_Reporter>
		TraverseDepthFirst(
			const ForEachSeedVertex& forEachSeedVertex,
			const ForEachAdjacent& forEachAdjacent,
			const Vertex_Reporter& report)
			: visitedSet()
		{
			using namespace std::placeholders;

			// Traverse each seed-vertex depth-first.
			forEachSeedVertex(
				std::bind(&TraverseDepthFirst::visit<ForEachSeedVertex, 
				ForEachAdjacent, Vertex_Reporter>, 
				*this, _1, 
				std::cref(forEachSeedVertex), 
				std::cref(forEachAdjacent),
				std::cref(report)));
		}

		template <
			typename ForEachSeedVertex,
			typename ForEachAdjacent,
			typename Vertex_Reporter>
		void visit(const Vertex& vertex,
			const ForEachSeedVertex& forEachSeedVertex,
			const ForEachAdjacent& forEachAdjacent,
			const Vertex_Reporter& report)
		{
			using namespace std::placeholders;

			// Avoid visiting the same vertex twice.
			if (!visitedSet.count(vertex))
			{
				// Mark as visited.
				visitedSet.emplace(vertex);

				// Report this vertex.
				report(vertex);

				// Traverse the edges.
				forEachAdjacent(vertex,
					std::bind(&TraverseDepthFirst::visit<ForEachSeedVertex, 
					ForEachAdjacent, Vertex_Reporter>, 
					*this, _1, 
					std::cref(forEachSeedVertex), 
					std::cref(forEachAdjacent),
					std::cref(report)));
			}
		}

		typedef std::unordered_set<Vertex, VertexHash>
			VisitedSet;
		typedef typename VisitedSet::iterator
			Visiter_Iterator;
		typedef typename VisitedSet::const_iterator
			Visiter_ConstIterator;

		VisitedSet visitedSet;
	};

	template <
		typename Vertex,
		typename ForEachSeedVertex,
		typename ForEachAdjacent,
		typename Vertex_Reporter>
	void traverseDepthFirst(
		const ForEachSeedVertex& forEachSeedVertex,
		const ForEachAdjacent& forEachAdjacent,
		const Vertex_Reporter& report)
	{
		Pastel::traverseDepthFirst<Vertex>(
			forEachSeedVertex, forEachAdjacent,
			report, std::hash<Vertex>());
	}

	template <
		typename Vertex,
		typename ForEachSeedVertex,
		typename ForEachAdjacent,
		typename Vertex_Reporter,
		typename Vertex_Hash>
	void traverseDepthFirst(
		const ForEachSeedVertex& forEachSeedVertex,
		const ForEachAdjacent& forEachAdjacent,
		const Vertex_Reporter& report,
		const Vertex_Hash& vertexHash)
	{
		TraverseDepthFirst<Vertex, Vertex_Hash> work(
			forEachSeedVertex, forEachAdjacent, report);
	}

}

#endif
