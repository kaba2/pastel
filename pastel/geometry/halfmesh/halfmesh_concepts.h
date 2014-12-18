// Description: Concepts for the half-edge structure

#ifndef PASTELGEOMETRY_HALFMESH_CONCEPTS_H
#define PASTELGEOMETRY_HALFMESH_CONCEPTS_H

#include "pastel/geometry/halfmesh/halfmesh_fwd.h"

namespace Pastel
{

	namespace HalfMesh_Concepts
	{

		//! Half-edge structure settings
		class Settings
		{
		public:
			// Define a data type as void to avoid 
			// allocating any memory for that data.

			//! The user-data stored at the vertices.
			using VertexData = UserDefinedType;
			
			//! The user-data stored at the half-edges.
			using HalfData = UserDefinedType;

			//! The user-data stored at the edges.
			using EdgeData = UserDefinedType;

			//! The user-data stored at the polygons.
			using PolygonData = UserDefinedType;

			//! Whether to allow multiple edges between two vertices.
			static PASTEL_CONSTEXPR bool MultipleEdges = UserDefinedBoolean;

			//! Whether to allow equal end-vertices for an edge.
			static PASTEL_CONSTEXPR bool Loops = UserDefinedBoolean;
		};

		//! Half-edge structure customization
		template <typename Settings>
		class Customization
		{
		public:
			// Since the HalfMesh inherits from its customization,
			// you can extend the public interface of the HalfMesh
			// by specifying public functions.

		protected:
			// The customization functions should be protected
			// so that they can only be called by the HalfMesh
			// implementation.

			using Fwd = HalfMesh_Fwd<Settings>;
			PASTEL_FWD(Vertex_Iterator);
			PASTEL_FWD(Half_Iterator);
			PASTEL_FWD(Edge_Iterator);
			PASTEL_FWD(Polygon_Iterator);

			//! Constructs an empty customization.
			/*!
			Exception safety: basic
			*/
			Customization() {}

			//! Swaps two customizations.
			/*!
			Time complexity: O(1)
			Exception safefy: nothrow
			*/
			void swap(Customization& that) {}

			//! Called at the start of clear().
			void onClear() {}

			//! Called at the end of insertVertex().
			void onInsertVertex(const Vertex_Iterator& vertex) {}

			//! Called at the start of removeVertex().
			void onRemoveVertex(const Vertex_Iterator& vertex) {}

			//! Called at the end of insertEdge().
			void onInsertEdge(const Edge_Iterator& edge) {}

			//! Called at the start of removeEdge().
			void onRemoveEdge(const Edge_Iterator& edge) {}

			//! Called at the end of insertPolygon().
			void onInsertPolygon(const Polygon_Iterator& polygon) {}

			//! Called at the start of removePolygon().
			void onRemovePolygon(const Polygon_Iterator& polygon) {}

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
