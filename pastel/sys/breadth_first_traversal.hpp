#ifndef PASTEL_BREADTH_FIRST_TRAVERSAL_HPP
#define PASTEL_BREADTH_FIRST_TRAVERSAL_HPP

#include "pastel/sys/breadth_first_traversal.h"

#include <list>
#include <functional>

namespace Pastel
{

	namespace TraverseBreadthFirst_
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
				if (!marked(vertex))
				{
					// Mark as visited.
					mark(vertex);

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
			const Mark& mark;
			const Marked& marked;

			std::list<Vertex> workSet;
		};

	}

	template <
		typename Vertex,
		typename ForEachSeedVertex,
		typename ForEachAdjacent,
		typename Vertex_Reporter,
		typename Mark,
		typename Marked>
	void traverseBreadthFirst(
		const ForEachSeedVertex& forEachSeedVertex,
		const ForEachAdjacent& forEachAdjacent,
		const Vertex_Reporter& report,
		const Mark& mark,
		const Marked& marked)
	{
		TraverseBreadthFirst_::Work<Vertex, ForEachSeedVertex, 
			ForEachAdjacent, Vertex_Reporter, Mark, Marked> work(
			forEachSeedVertex, forEachAdjacent,
			report, mark, marked);

		work.work();
	}

}

#endif
