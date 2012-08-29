#ifndef PASTELSYS_BREADTH_FIRST_TRAVERSAL_HPP
#define PASTELSYS_BREADTH_FIRST_TRAVERSAL_HPP

#include "pastel/sys/breadth_first_traversal.h"

#include <list>
#include <functional>

namespace Pastel
{

	class BreadthFirst_GraphTraversal
	{
	public:
		template <
			typename Vertex,
			typename ForEachSeedVertex,
			typename ForEachAdjacent,
			typename Mark,
			typename Marked>
		void work(
			const ForEachSeedVertex& forEachSeedVertex,
			const ForEachAdjacent& forEachAdjacent,
			const Mark& mark,
			const Marked& marked) const
		{
			Work<Vertex, ForEachSeedVertex, 
				ForEachAdjacent, Mark, Marked> work(
				forEachSeedVertex, forEachAdjacent,
				mark, marked);
		}

	private:
		template <
			typename Vertex,
			typename ForEachSeedVertex,
			typename ForEachAdjacent,
			typename Mark,
			typename Marked>
		class Work
		{
		public:
			Work(
				const ForEachSeedVertex& forEachSeedVertex_,
				const ForEachAdjacent& forEachAdjacent_,
				const Mark& mark_,
				const Marked& marked_)
				: forEachSeedVertex(forEachSeedVertex_)
				, forEachAdjacent(forEachAdjacent_)
				, mark(mark_)
				, marked(marked_)
				, workSet()
			{
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

					// Traverse the edges.
					forEachAdjacent(vertex,
						[&](const Vertex& that)
					{
						// Push new work to back (queue).
						workSet.push_back(that);
					});
				}
			}

		private:
			const ForEachSeedVertex& forEachSeedVertex;
			const ForEachAdjacent& forEachAdjacent;
			const Mark& mark;
			const Marked& marked;

			std::list<Vertex> workSet;
		};
	};

}

#endif
