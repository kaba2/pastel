// Description: Concepts for the half-edge structure

#ifndef PASTELGEOMETRY_HALFMESH_CONCEPTS_H
#define PASTELGEOMETRY_HALFMESH_CONCEPTS_H

#include "pastel/geometry/halfmesh_fwd.h"

namespace Pastel
{

	namespace HalfMesh_
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
			PASTEL_CONSTEXPR bool MultipleEdges = UserDefinedBoolean;

			//! Whether to allow equal end-vertices for an edge.
			PASTEL_CONSTEXPR bool Loops = UserDefinedBoolean;
		};

		//! Half-edge structure customization
		template <typename Settings>
		class Customization
		{
		public:
			// You can expand the interface
			// of HalfMesh by defining functions
			// here.

		public:
			// Since the HalfMesh inherits from its customization,
			// you can extend the public interface of the HalfMesh
			// by specifying public functions.

		protected:
			// The customization functions should be protected
			// so that they can only be called by the HalfMesh
			// implementation.

			using Fwd = HalfMesh_Fwd<Settings>;

			//! Constructs an empty customization.
			/*!
			Exception safety: basic
			*/
			Customization() {}

			//! Called at the end of a constructor.
			/*!
			Exception safefy: basic
			*/
			void onConstruction() {};

			//! Swaps two customizations.
			/*!
			Time complexity: O(1)
			Exception safefy: nothrow
			*/
			void swap(Customization& that) {}

			//! Called at the start of clear().
			void onClear();

			//! Called at the end of insertVertex().
			//void onInsert(const Vertex& vertex);

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
