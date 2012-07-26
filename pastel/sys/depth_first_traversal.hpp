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
		typename ForEachSeedVertex,
		typename ForEachRelated,
		typename Vertex_Reporter,
		typename Vertex_Hash>
	class TraverseDepthFirst
	{
	public:
	
		TraverseDepthFirst(
			const ForEachSeedVertex& forEachSeedVertex_,
			const ForEachRelated& forEachRelated_,
			const Vertex_Reporter& report_,
			const Vertex_Hash& vertexHash_)
			: forEachSeedVertex(forEachSeedVertex_)
			, forEachRelated(forEachRelated_)
			, report(report_)
			, vertexHash(vertexHash_)
			, visitedSet()
		{
		}

		void work()
		{
			using namespace std::placeholders;

			// Traverse each seed-vertex depth-first.
			forEachSeedVertex(
				std::bind(&TraverseDepthFirst::visit, *this, _1));
		}

		void visit(const Vertex& vertex)
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
				forEachRelated(vertex,
					std::bind(&TraverseDepthFirst::visit, *this, _1));
			}
		}

		const ForEachSeedVertex& forEachSeedVertex;
		const ForEachRelated& forEachRelated;
		const Vertex_Reporter& report;
		const Vertex_Hash& vertexHash;

		std::unordered_set<Vertex> visitedSet;
	};

	template <
		typename Vertex,
		typename ForEachSeedVertex,
		typename ForEachRelated,
		typename Vertex_Reporter>
	void traverseDepthFirst(
		const ForEachSeedVertex& forEachSeedVertex,
		const ForEachRelated& forEachRelated,
		const Vertex_Reporter& report)
	{
		Pastel::traverseDepthFirst<Vertex>(
			forEachSeedVertex, forEachRelated,
			report, std::hash<Vertex>());
	}

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
		const Vertex_Hash& vertexHash)
	{
		TraverseDepthFirst<Vertex, ForEachSeedVertex, 
			ForEachRelated, Vertex_Reporter, Vertex_Hash> work(
			forEachSeedVertex, forEachRelated,
			report, vertexHash);

		work.work();
	}

}

#endif
