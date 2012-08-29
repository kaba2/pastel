#ifndef PASTELSYS_GRAPH_TRAVERSAL_CONCEPTS_H
#define PASTELSYS_GRAPH_TRAVERSAL_CONCEPTS_H

#include "pastel/sys/algorithm_concept.h"

#include "pastel/sys/mytypes.h"

#include <functional>

namespace Pastel
{

	namespace GraphTraversal_Concepts
	{

		//! The vertex-set V of the graph.
		class Vertex {};

		//! Visits each v in S.
		/*!
		The S subset V is the set of seed-vertices.
		*/
		class ForEachSeedVertex
		{
		public:
			void operator()(
				const std::function<void(const Vertex&)>& visit) const;
		};

		//! Visits each v' in V such that v has an edge to v'.
		class ForEachAdjacent
		{
		public:
			void operator()(
				const Vertex& vertex,
				const std::function<void(const Vertex&)>& visit) const;
		};

		//! Marks a vertex as visited.
		class Mark
		{
		public:
			void operator()(const Vertex& vertex) const;
		};

		//! Returns whether a vertex has been marked.
		class Marked
		{
		public:
			bool operator()(const Vertex& vertex) const;
		};

		//! A concept for graph traversal.
		class GraphTraversal_Algorithm
			: public Algorithm_Concept::Algorithm
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
				const Marked& marked);
		};

	}

}

#endif
