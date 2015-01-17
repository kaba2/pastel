#ifndef PASTELSYS_DEPTH_FIRST_TRAVERSAL_HPP
#define PASTELSYS_DEPTH_FIRST_TRAVERSAL_HPP

#include "pastel/sys/graph/depth_first/depth_first.h"

#include <boost/bind.hpp>

#include <functional>

namespace Pastel
{

	class DepthFirst_GraphTraversal
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
			{
				// Traverse each seed-vertex depth-first.
				forEachSeedVertex(
					boost::bind(&Work::visit, 
					boost::ref(*this), _1));
			}

			void visit(const Vertex& vertex)
			{
				// Avoid visiting the same vertex twice.
				if (!marked(vertex))
				{
					// Mark as visited.
					mark(vertex);

					// Traverse the edges.
					forEachAdjacent(
						vertex,
						boost::bind(&Work::visit, 
						boost::ref(*this), _1));
				}
			}

		private:
			const ForEachSeedVertex& forEachSeedVertex;
			const ForEachAdjacent& forEachAdjacent;
			const Mark& mark;
			const Marked& marked;
		};
	};

}

#endif
