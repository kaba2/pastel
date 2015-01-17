// Description: Concepts for the incidence graph

#ifndef PASTELSYS_INCIDENCE_GRAPH_CONCEPTS_H
#define PASTELSYS_INCIDENCE_GRAPH_CONCEPTS_H

#include "pastel/sys/incidence_graph/incidence_graph_fwd.h"

namespace Pastel
{

	namespace IncidenceGraph_Concepts
	{

		class Settings
		{
		public:
			//! The type of the graph.
			/*!
			The graph can be of three different types:
			* directed
			* undirected
			* mixed

			While it is always possible to choose the 
			mixed type, which is the most general, memory
			can be preserved if the type of the graph can
			be chosen either as directed or undirected.
			*/
			static PASTEL_CONSTEXPR GraphType Type = GraphType::Directed;

			//! Vertex data
			/*!
			Defining this void avoids allocating any
			memory for the vertex data.
			*/
			using VertexData = UserDefinedType;

			//! Edge data
			/*!
			Defining this void avoids allocating any
			memory for the edge data.
			*/
			using EdgeData = UserDefinedType;
		};

		template <typename Settings>
		class Customization
		{
		public:
			// Since the incidence graph inherits from its customization,
			// you can extend the public interface of the incidence graph 
			// by specifying public functions.

		protected:
			// The customization functions should be protected
			// so that they can only be called by the incidence graph
			// implementation.

			using Fwd = IncidenceGraph_Fwd<Settings>;

			PASTEL_FWD(Vertex_Iterator);
			PASTEL_FWD(Vertex_ConstIterator);
			PASTEL_FWD(Edge_Iterator);
			PASTEL_FWD(Edge_ConstIterator);

			//! Constructs an empty customization.
			/*!
			Exception safety: strong
			*/
			Customization() {}

			//! Swaps two customizations.
			/*!
			Time complexity: O(1).
			Exception safefy: nothrow
			*/
			void swap(Customization& that) {}

			//! Called at the start of clear().
			/*!
			Exception safety: nothrow
			*/
			void onClear() {}

			//! Called after adding a vertex.
			/*!
			Exception safety: strong

			vertex:
			The vertex that was added.
			*/
			void onAddVertex(const Vertex_Iterator& vertex) {}

			//! Called before removing a vertex.
			/*!
			Exception safety: nothrow

			vertex:
			The vertex that is to be removed.
			*/
			void onRemoveVertex(const Vertex_Iterator& that) {}

			//! Called after adding an edge.
			/*!
			Exception safety: strong

			edge:
			The edge that was added.
			*/
			void onAddEdge(const Edge_Iterator& that) {}

			//! Called before removing an edge.
			/*!
			Exception safety: nothrow

			edge:
			The edge that is to be removed.
			*/
			void onRemoveEdge(const Edge_Iterator& that) {}

		private:
			// These functions will not be used, and so should
			// be deleted to avoid accidental splicing.
			Customization(const Customization& that) = delete;
			Customization(Customization&& that) = delete;
			Customization& operator=(Customization) = delete;

			// You can introduce customization state here.
		};

	}

}

#endif
