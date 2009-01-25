#include "pastel/geometry/trianglemesh.h"

#include "pastel/sys/constants.h"
#include "pastel/sys/memoryview.h"
#include "pastel/math/affinetransformation.h"

namespace Pastel
{

	PASTELGEOMETRY PropertyId<integer, Point3>
		TriangleMesh::VertexPositionId(0);
	PASTELGEOMETRY PropertyId<integer, Vector3>
		TriangleMesh::VertexNormalId(1);
	PASTELGEOMETRY PropertyId<integer, Point2>
		TriangleMesh::VertexUvId(2);
	PASTELGEOMETRY PropertyId<integer, RgbaColor>
		TriangleMesh::VertexColorId(3);

	PASTELGEOMETRY PropertyId<integer, Integer3>
		TriangleMesh::TriangleIndexId(0);
	PASTELGEOMETRY PropertyId<integer, Vector3>
		TriangleMesh::TriangleNormalId(1);

	TriangleMesh::TriangleMesh()
		: vertexData_()
		, triangleData_()
	{
		initialize();
	}

	TriangleMesh::TriangleMesh(const TriangleMesh& that)
		: vertexData_(that.vertexData_)
		, triangleData_(that.triangleData_)
	{
		initialize();
	}

	TriangleMesh::~TriangleMesh()
	{
	}

	TriangleMesh& TriangleMesh::operator=(const TriangleMesh& that)
	{
		TriangleMesh copy(that);
		swap(copy);
		return *this;
	}

	void TriangleMesh::swap(TriangleMesh& that)
	{
		vertexData_.swap(that.vertexData_);
		triangleData_.swap(that.triangleData_);
	}

	void TriangleMesh::freeMem()
	{
		clear();
		vertexData_.freeMem();
		triangleData_.freeMem();
	}

	void TriangleMesh::clear()
	{
		vertexData_.clear();
		triangleData_.clear();
	}

	void TriangleMesh::addVertexNormals()
	{
		vertexData_.add(
			VertexNormalId);
	}

	void TriangleMesh::removeVertexNormals()
	{
		vertexData_.remove(
			VertexNormalId);
	}

	void TriangleMesh::addVertexUvs()
	{
		vertexData_.add<Point2>(
			VertexUvId);
	}

	void TriangleMesh::removeVertexUvs()
	{
		vertexData_.remove(
			VertexUvId);
	}

	void TriangleMesh::addVertexColors()
	{
		vertexData_.add<RgbaColor>(
			VertexColorId);
	}

	void TriangleMesh::removeVertexColors()
	{
		vertexData_.remove(
			VertexColorId);
	}

	MemoryView<Point3> TriangleMesh::vertexPositions()
	{
		MemoryView<Point3> result;
		vertexProperty<Point3>(VertexPositionId, result);
		return result;
	}

	ConstMemoryView<Point3> TriangleMesh::vertexPositions() const
	{
		ConstMemoryView<Point3> result;
		vertexProperty<Point3>(VertexPositionId, result);
		return result;
	}

	MemoryView<Vector3> TriangleMesh::vertexNormals()
	{
		MemoryView<Vector3> result;
		vertexProperty<Vector3>(VertexNormalId, result);
		return result;
	}

	ConstMemoryView<Vector3> TriangleMesh::vertexNormals() const
	{
		ConstMemoryView<Vector3> result;
		vertexProperty<Vector3>(VertexNormalId, result);
		return result;
	}

	MemoryView<Point2> TriangleMesh::vertexUvs()
	{
		MemoryView<Point2> result;
		vertexProperty<Point2>(VertexUvId, result);
		return result;
	}

	ConstMemoryView<Point2> TriangleMesh::vertexUvs() const
	{
		ConstMemoryView<Point2> result;
		vertexProperty<Point2>(VertexUvId, result);
		return result;
	}

	MemoryView<RgbaColor> TriangleMesh::vertexColors()
	{
		MemoryView<RgbaColor> result;
		vertexProperty<RgbaColor>(VertexColorId, result);
		return result;
	}

	ConstMemoryView<RgbaColor> TriangleMesh::vertexColors() const
	{
		ConstMemoryView<RgbaColor> result;
		vertexProperty<RgbaColor>(VertexColorId, result);
		return result;
	}

	ConstMemoryView<Integer3> TriangleMesh::triangleIndices() const
	{
		ConstMemoryView<Integer3> result;
		triangleProperty<Integer3>(TriangleIndexId, result);
		return result;
	}

	integer TriangleMesh::createProperty()
	{
		static integer propertyId__ = 65536;
		integer result = propertyId__;
		++propertyId__;
		return result;
	}

	void TriangleMesh::reserveVertices(integer vertexCount)
	{
		if (vertexData_.size() < vertexCount)
		{
			vertexData_.resize(vertexCount);
		}
	}

	integer TriangleMesh::addTriangle(integer aIndex,
		integer bIndex, integer cIndex)
	{
		ENSURE1(aIndex >= 0 &&
			aIndex < vertexData_.size(), aIndex);
		ENSURE1(bIndex >= 0 &&
			bIndex < vertexData_.size(), bIndex);
		ENSURE1(cIndex >= 0 &&
			cIndex < vertexData_.size(), cIndex);

		integer triangleIndex = triangleData_.size();
		integer newSize = triangleIndex + 1;

		triangleData_.resize(newSize);

		MemoryView<Integer3> indexArray;
		bool result = triangleProperty<Integer3>(
			TriangleIndexId, indexArray);
		ASSERT(result);

		Integer3& triangle = indexArray[triangleIndex];
		triangle[0] = aIndex;
		triangle[1] = bIndex;
		triangle[2] = cIndex;

		return triangleIndex;
	}

	integer TriangleMesh::vertexCount() const
	{
		return vertexData_.size();
	}

	integer TriangleMesh::triangleCount() const
	{
		return triangleData_.size();
	}

	// Private

	void TriangleMesh::initialize()
	{
		vertexData_.add(
			VertexPositionId);
		triangleData_.add(
			TriangleIndexId);
	}

}
