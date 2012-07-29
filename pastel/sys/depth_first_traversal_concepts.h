// Description: Concepts for the depth-first traversal

#ifndef PASTEL_DEPTH_FIRST_TRAVERSAL_CONCEPTS_H
#define PASTEL_DEPTH_FIRST_TRAVERSAL_CONCEPTS_H

#include "pastel/sys/depth_first_traversal.h"

#include <functional>

namespace Pastel
{

	namespace DepthFirstTraversal_Concepts
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
			//! Visits each v in S subset V.
			void operator()(
				const std::function<void(const Vertex&)>& visit) const;
		};

		//! Visits each v' in V such that v has an edge to v'.
		class ForEachAdjacent
		{
		public:
			//! Visits each v' in V such that v has an edge to v'.
			void operator()(
				const Vertex& vertex,
				const std::function<void(const Vertex&)>& visit) const;
		};

		//! Reports each traversed vertex to the user.
		class Vertex_Reporter
		{
		public:
			//! Reports a traversed vertex.
			/*!
			returns:
			Whether to continue the traversal.
			*/
			bool operator()(const Vertex& vertex) const;
		};

		//! Marks a vertex as visited.
		class Mark
		{
		public:
			//! Marks a vertex as visited.
			void operator()(const Vertex& vertex) const;
		};

		//! Returns whether a vertex has been marked.
		class Marked
		{
		public:
			//! Returns whether a vertex has been marked.
			bool operator()(const Vertex& vertex) const;
		};

	}

}

#endif

