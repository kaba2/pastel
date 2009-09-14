// Documentation: halfmesh.txt

#ifndef PASTEL_PROPERTYHALFMESH_H
#define PASTEL_PROPERTYHALFMESH_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/parallelarray.h"
#include "pastel/sys/indexallocator.h"
#include "pastel/sys/tuple.h"

#include "pastel/geometry/geometrylibrary.h"
#include "pastel/geometry/halfmesh.h"

namespace Pastel
{

	class PASTELGEOMETRY PropertyHalfMesh_Policy
	{
	public:
		typedef integer VertexUserData;
		typedef integer HalfUserData;
		typedef integer EdgeUserData;
		typedef integer PolygonUserData;

	private:
		typedef integer EntityData;

	public:
		// Using default destructor.

		PropertyHalfMesh_Policy();
		PropertyHalfMesh_Policy(const PropertyHalfMesh_Policy& that);
		~PropertyHalfMesh_Policy();

		PropertyHalfMesh_Policy& operator=(const PropertyHalfMesh_Policy& that);

		//! Swaps two meshes.
		void swap(PropertyHalfMesh_Policy& that);

		//! Remove all data of the mesh.
		void clear();

		// Vertices

		template <typename Type>
		bool vertexPropertyAdd(
			const PropertyId<integer, Type>& key,
			const PASTEL_NO_DEDUCTION(Type)& defaultValue = Type());

		template <typename Type>
		bool vertexPropertyRemove(
			const PropertyId<integer, Type>& key);

		template <typename Type>
		bool vertexProperty(
			const PropertyId<integer, Type>& key,
			MemoryView<Type>& view);

		template <typename Type>
		bool vertexProperty(
			const PropertyId<integer, Type>& key,
			ConstMemoryView<Type>& view);

		template <typename Type>
		bool vertexProperty(
			const PropertyId<integer, Type>& key,
			ConstMemoryView<Type>& view) const;

		// Half-edges

		template <typename Type>
		bool halfPropertyAdd(
			const PropertyId<integer, Type>& key,
			const PASTEL_NO_DEDUCTION(Type)& defaultValue = Type());

		template <typename Type>
		bool halfPropertyRemove(
			const PropertyId<integer, Type>& key);

		template <typename Type>
		bool halfProperty(
			const PropertyId<integer, Type>& key,
			MemoryView<Type>& view);

		template <typename Type>
		bool halfProperty(
			const PropertyId<integer, Type>& key,
			ConstMemoryView<Type>& view);

		template <typename Type>
		bool halfProperty(
			const PropertyId<integer, Type>& key,
			ConstMemoryView<Type>& view) const;

		// Edges

		template <typename Type>
		bool edgePropertyAdd(
			const PropertyId<integer, Type>& key,
			const PASTEL_NO_DEDUCTION(Type)& defaultValue = Type());

		template <typename Type>
		bool edgePropertyRemove(
			const PropertyId<integer, Type>& key);

		template <typename Type>
		bool edgeProperty(
			const PropertyId<integer, Type>& key,
			MemoryView<Type>& view);

		template <typename Type>
		bool edgeProperty(
			const PropertyId<integer, Type>& key,
			ConstMemoryView<Type>& view);

		template <typename Type>
		bool edgeProperty(
			const PropertyId<integer, Type>& key,
			ConstMemoryView<Type>& view) const;

		// Polygons

		template <typename Type>
		bool polygonPropertyAdd(
			const PropertyId<integer, Type>& key,
			const PASTEL_NO_DEDUCTION(Type)& defaultValue = Type());

		template <typename Type>
		bool polygonPropertyRemove(
			const PropertyId<integer, Type>& key);

		template <typename Type>
		bool polygonProperty(
			const PropertyId<integer, Type>& key,
			MemoryView<Type>& view);

		template <typename Type>
		bool polygonProperty(
			const PropertyId<integer, Type>& key,
			ConstMemoryView<Type>& view);

		template <typename Type>
		bool polygonProperty(
			const PropertyId<integer, Type>& key,
			ConstMemoryView<Type>& view) const;

		//! Pre-allocates the given number of vertex data.
		/*!
		References to vertex data are invalidated every time
		an internal data reallocation happens. By pre-allocating
		memory, you are able to avoid the reallocation.
		*/
		void reserveVertices(integer vertices);

		//! Pre-allocates the given number of half-edge data.
		/*!
		References to half-edge data are invalidated every time
		an internal data reallocation happens. By pre-allocating
		memory, you are able to avoid the reallocation.
		*/
		void reserveHalfs(integer halfs);

		//! Pre-allocates the given number of edge data.
		/*!
		References to edge data are invalidated every time
		an internal data reallocation happens. By pre-allocating
		memory, you are able to avoid the reallocation.
		*/
		void reserveEdges(integer edges);

		//! Pre-allocates the given number of polygon data.
		/*!
		References to polygon data are invalidated every time
		an internal data reallocation happens. By pre-allocating
		memory, you are able to avoid the reallocation.
		*/
		void reservePolygons(integer polygons);

	protected:
		void constructVertex(EntityData* data);
		void destructVertex(EntityData* data);
		void constructHalf(EntityData* data);
		void destructHalf(EntityData* data);
		void constructEdge(EntityData* data);
		void destructEdge(EntityData* data);
		void constructPolygon(EntityData* data);
		void destructPolygon(EntityData* data);

	private:
		IndexAllocator vertexIndices_;
		IndexAllocator halfIndices_;
		IndexAllocator edgeIndices_;
		IndexAllocator polygonIndices_;

		ParallelArray<integer> vertexData_;
		ParallelArray<integer> halfData_;
		ParallelArray<integer> edgeData_;
		ParallelArray<integer> polygonData_;
	};

	typedef HalfMesh<PropertyHalfMesh_Policy> PropertyHalfMesh;

}

#include "pastel/geometry/propertyhalfmesh.hpp"

#endif
