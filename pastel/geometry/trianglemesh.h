#ifndef PASTELGEOMETRY_TRIANGLEMESH_H
#define PASTELGEOMETRY_TRIANGLEMESH_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/parallelarray.h"

#include "pastel/geometry/geometrylibrary.h"

#include "pastel/geometry/triangle.h"

#include "pastel/gfx/color.h"

namespace Pastel
{

	class PASTELGEOMETRY TriangleMesh
	{
	public:
		static PropertyId<integer, Point3> VertexPositionId;
		static PropertyId<integer, Vector3> VertexNormalId;
		static PropertyId<integer, Point2> VertexUvId;
		static PropertyId<integer, RgbaColor> VertexColorId;

		static PropertyId<integer, Integer3> TriangleIndexId;
		static PropertyId<integer, Vector3> TriangleNormalId;

		TriangleMesh();
		TriangleMesh(const TriangleMesh& that);
		~TriangleMesh();
		TriangleMesh& operator=(const TriangleMesh& that);

		void swap(TriangleMesh& that);
		void freeMem();

		void clear();

		void addVertexNormals();
		void removeVertexNormals();

		void addVertexUvs();
		void removeVertexUvs();

		void addVertexColors();
		void removeVertexColors();

		MemoryView<Point3> vertexPositions();
		ConstMemoryView<Point3> vertexPositions() const;

		MemoryView<Vector3> vertexNormals();
		ConstMemoryView<Vector3> vertexNormals() const;

		MemoryView<Point2> vertexUvs();
		ConstMemoryView<Point2> vertexUvs() const;

		MemoryView<RgbaColor> vertexColors();
		ConstMemoryView<RgbaColor> vertexColors() const;

		ConstMemoryView<Integer3> triangleIndices() const;

		static integer createProperty();

		void reserveVertices(integer vertexCount);

		integer addTriangle(integer aIndex,
			integer bIndex, integer cIndex);

		integer vertexCount() const;
		integer triangleCount() const;

		template <typename Type>
		bool vertexPropertyAdd(
			const PropertyId<integer, Type>& key);

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

		// Triangles

		template <typename Type>
		bool trianglePropertyAdd(
			const PropertyId<integer, Type>& key);

		template <typename Type>
		bool trianglePropertyRemove(
			const PropertyId<integer, Type>& key);

		template <typename Type>
		bool triangleProperty(
			const PropertyId<integer, Type>& key,
			MemoryView<Type>& view);

		template <typename Type>
		bool triangleProperty(
			const PropertyId<integer, Type>& key,
			ConstMemoryView<Type>& view);

		template <typename Type>
		bool triangleProperty(
			const PropertyId<integer, Type>& key,
			ConstMemoryView<Type>& view) const;

	private:
		void initialize();

		ParallelArray<integer> vertexData_;
		ParallelArray<integer> triangleData_;
	};

}

#include "pastel/geometry/trianglemesh.hpp"

#endif
