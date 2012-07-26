#ifndef PASTEL_BREADTH_FIRST_TRAVERSAL_HPP
#define PASTEL_BREADTH_FIRST_TRAVERSAL_HPP

#include "pastel/sys/breadth_first_traversal.h"
#include "pastel/sys/hash.h"

#include <unordered_set>
#include <list>
#include <functional>

namespace Pastel
{

	template <
		typename Vertex,
		typename ForEachSeedVertex,
		typename ForEachAdjacent,
		typename Vertex_Reporter,
		typename Vertex_Hash>
	class TraverseBreadthFirst
	{
	public:
	
		TraverseBreadthFirst(
			const ForEachSeedVertex& forEachSeedVertex_,
			const ForEachAdjacent& forEachAdjacent_,
			const Vertex_Reporter& report_,
			const Vertex_Hash& vertexHash_)
			: forEachSeedVertex(forEachSeedVertex_)
			, forEachAdjacent(forEachAdjacent_)
			, report(report_)
			, vertexHash(vertexHash_)
			, visitedSet()
			, workSet()
		{
		}

		void work()
		{
			using namespace std::placeholders;

			// Traverse each seed-vertex breadth-first.
			forEachSeedVertex(
				[&](const Vertex& that)
			{
				workSet.push_back(that);
			});

			// Do the breadth-first traversal.
			while(!workSet.empty())
			{
				// Retrieve new work from the front (queue).
				visit(workSet.front());
				workSet.pop_front();
			}
		}

		void visit(const Vertex& vertex)
		{
			// Avoid visiting the same vertex twice.
			if (!visitedSet.count(vertex))
			{
				// Mark as visited.
				visitedSet.emplace(vertex);

				// Report this vertex.
				report(vertex);

				// Traverse the edges.
				forEachAdjacent(vertex,
					[&](const Vertex& that)
				{
					// Push new work to back (queue).
					workSet.push_back(that);
				});
			}
		}

		const ForEachSeedVertex& forEachSeedVertex;
		const ForEachAdjacent& forEachAdjacent;
		const Vertex_Reporter& report;
		const Vertex_Hash& vertexHash;

		std::unordered_set<Vertex> visitedSet;
		std::list<Vertex> workSet;
	};

	template <
		typename Vertex,
		typename ForEachSeedVertex,
		typename ForEachAdjacent,
		typename Vertex_Reporter>
	void traverseBreadthFirst(
		const ForEachSeedVertex& forEachSeedVertex,
		const ForEachAdjacent& forEachAdjacent,
		const Vertex_Reporter& report)
	{
		Pastel::traverseBreadthFirst<Vertex>(
			forEachSeedVertex, forEachAdjacent,
			report, std::hash<Vertex>());
	}

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
		const Vertex_Hash& vertexHash)
	{
		TraverseBreadthFirst<Vertex, ForEachSeedVertex, 
			ForEachAdjacent, Vertex_Reporter, Vertex_Hash> work(
			forEachSeedVertex, forEachAdjacent,
			report, vertexHash);

		work.work();
	}

}

#endif
