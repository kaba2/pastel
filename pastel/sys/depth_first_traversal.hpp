#ifndef PASTEL_DEPTH_FIRST_TRAVERSAL_HPP
#define PASTEL_DEPTH_FIRST_TRAVERSAL_HPP

#include "pastel/sys/depth_first_traversal.h"

#include <functional>

namespace Pastel
{

	namespace TraverseDepthFirst_
	{

		template <
			typename Vertex,
			typename ForEachSeedVertex,
			typename ForEachAdjacent,
			typename Vertex_Reporter,
			typename Mark,
			typename Marked>
		class Work
		{
		public:
			Work(
				const ForEachSeedVertex& forEachSeedVertex_,
				const ForEachAdjacent& forEachAdjacent_,
				const Vertex_Reporter& report_,
				const Mark& mark_,
				const Marked& marked_)
				: forEachSeedVertex(forEachSeedVertex_)
				, forEachAdjacent(forEachAdjacent_)
				, report(report_)
				, mark(mark_)
				, marked(marked_)
			{
				using namespace std::placeholders;

				// Traverse each seed-vertex depth-first.
				forEachSeedVertex(
					std::bind(&Work::visit, 
					std::ref(*this), _1));
			}

			void visit(const Vertex& vertex)
			{
				using namespace std::placeholders;

				// Avoid visiting the same vertex twice.
				if (!marked(vertex))
				{
					// Mark as visited.
					mark(vertex);

					// Report this vertex.
					report(vertex);

					// Traverse the edges.
					forEachAdjacent(
						vertex,
						std::bind(&Work::visit, 
						std::ref(*this), _1));
				}
			}

		private:
			const ForEachSeedVertex& forEachSeedVertex;
			const ForEachAdjacent& forEachAdjacent;
			const Vertex_Reporter& report;
			const Mark& mark;
			const Marked& marked;
		};

	}

	template <
		typename Vertex,
		typename ForEachSeedVertex,
		typename ForEachAdjacent,
		typename Vertex_Reporter,
		typename Mark,
		typename Marked>
	void traverseDepthFirst(
		const ForEachSeedVertex& forEachSeedVertex,
		const ForEachAdjacent& forEachAdjacent,
		const Vertex_Reporter& report,
		const Mark& mark,
		const Marked& marked)
	{
		TraverseDepthFirst_::Work<Vertex, 
		ForEachSeedVertex, ForEachAdjacent, Vertex_Reporter,
		Mark, Marked> work(
			forEachSeedVertex, forEachAdjacent, report,
			mark, marked);
	}

}

#endif
