// Description: Testing for HalfMesh
// DocumentationOf: halfmesh.h

#include "test/test_init.h"

#include "pastel/geometry/halfmesh/halfmesh.h"
#include "pastel/sys/random.h"

#include <algorithm>
#include <iostream>

namespace
{

	using Mesh = HalfEdge<int, int, int, int, false, true>;
	using Vertex_Iterator = Mesh::Vertex_Iterator;
	using Half_Iterator = Mesh::Half_Iterator;
	using Edge_Iterator = Mesh::Edge_Iterator;
	using Polygon_Iterator = Mesh::Polygon_Iterator;
	using Vertex_ConstIterator = Mesh::Vertex_ConstIterator;
	using Half_ConstIterator = Mesh::Half_ConstIterator;
	using Edge_ConstIterator = Mesh::Edge_ConstIterator;
	using Polygon_ConstIterator = Mesh::Polygon_ConstIterator;

	template <typename InputIterator,
		typename OtherInputIterator>
	bool rotationEqual(
	InputIterator from, InputIterator to,
	OtherInputIterator otherFrom)
	{
		if (from == to)
		{
			return true;
		}

		InputIterator pivot(std::find(from, to, *otherFrom));
		int delta = std::distance(from, pivot);
		OtherInputIterator otherPivot(otherFrom);
		std::advance(otherPivot, delta);

		return std::equal(pivot, to, otherFrom) ||
			std::equal(from, pivot, otherPivot);
	}

}

TEST_CASE("Basic (HalfMesh)")
{
	Mesh mesh;
	REQUIRE(testInvariants(mesh));

	mesh.clear();
	REQUIRE(testInvariants(mesh));

	Mesh otherMesh(mesh);
	REQUIRE(testInvariants(mesh));
	REQUIRE(testInvariants(otherMesh));

	otherMesh.swap(mesh);
	REQUIRE(testInvariants(mesh));
	REQUIRE(testInvariants(otherMesh));

	otherMesh.clear();
	REQUIRE(testInvariants(otherMesh));

	mesh.clear();
	REQUIRE(testInvariants(mesh));
}

TEST_CASE("VertexAdd (HalfMesh)")
{
	Mesh mesh;

	integer n = 100;
	for (integer i = 0;i < n;++i)
	{
		mesh.insertVertex();
		REQUIRE(testInvariants(mesh));
	}

	Mesh otherMesh(mesh);
	REQUIRE(testInvariants(mesh));
	REQUIRE(testInvariants(otherMesh));
}

TEST_CASE("Edge (HalfMesh)")
{
	Mesh mesh;

	integer VertexCount = 100;
	std::vector<Vertex_Iterator> vertex(VertexCount);

	for (integer i = 0;i < VertexCount;++i)
	{
		vertex[i] = mesh.insertVertex();
		REQUIRE(testInvariants(mesh));
	}

	{
		Mesh otherMesh(mesh);
		REQUIRE(testInvariants(mesh));
		REQUIRE(testInvariants(otherMesh));
	}

	integer BucketSize = 100;
	integer BucketCount = 10;

	integer EdgeCount = BucketSize * BucketCount;
	std::vector<Edge_Iterator> edgeList(EdgeCount);

	for (integer i = 0;i < BucketCount;++i)
	{
		for (integer j = 0;j < BucketSize;++j)
		{
			integer aVertex = randomInteger(VertexCount);
			integer bVertex = randomInteger(VertexCount);

			auto edgePair = 
				mesh.insertEdge(vertex[aVertex], vertex[bVertex]);
			REQUIRE(testInvariants(mesh));

			auto edge = edgePair.first;
			ENSURE(edge.isNormal());

			if (edgePair.second)
			{
				edgeList[i * BucketSize + j] = edge;
			}
		}

	}

	REQUIRE(testInvariants(mesh));

	{
		Mesh otherMesh(mesh);
		REQUIRE(testInvariants(mesh));
		REQUIRE(testInvariants(otherMesh));
	}

	for (integer i = 0;i < EdgeCount;++i)
	{
		integer index = randomInteger(EdgeCount);
		if (edgeList[index].isNormal())
		{
			mesh.removeEdge(edgeList[index]);
			edgeList[index].clear();
		}

		REQUIRE(testInvariants(mesh));
	}
}

TEST_CASE("Polygon (HalfMesh)")
{
	Mesh mesh;

	Vertex_Iterator vertex[4][4];

	for (integer i = 0;i < 4;++i)
	{
		for (integer j = 0;j < 4;++j)
		{
			vertex[i][j] = mesh.insertVertex();
			REQUIRE(testInvariants(mesh));
			REQUIRE(!vertex[i][j].empty());
			(vertex[i][j])->data() = i * 10 + j;
		}
	}

	// o   o   o   o
	//
	// o   o   o   o
	//
	// o   o   o   o
	//
	// o   o   o   o

	Polygon_Iterator polygon[16];

	std::vector<Vertex_Iterator> points;

	points.clear();
	points.push_back(vertex[0][0]);
	points.push_back(vertex[1][0]);
	points.push_back(vertex[1][1]);

	polygon[0] = mesh.insertPolygon(points);
	REQUIRE(testInvariants(mesh));

	// o   o   o   o
	// |0\.
	// o---o   o   o
	//
	// o   o   o   o
	//
	// o   o   o   o

	points.clear();
	points.push_back(vertex[0][0]);
	points.push_back(vertex[1][1]);
	points.push_back(vertex[0][1]);

	polygon[1] = mesh.insertPolygon(points);
	REQUIRE(testInvariants(mesh));

	// o---o   o   o
	// |0\1|
	// o---o   o   o
	//
	// o   o   o   o
	//
	// o   o   o   o

	points.clear();
	points.push_back(vertex[0][1]);
	points.push_back(vertex[1][1]);
	points.push_back(vertex[1][0]);
	points.push_back(vertex[2][0]);
	points.push_back(vertex[2][1]);
	points.push_back(vertex[2][2]);
	points.push_back(vertex[1][2]);
	points.push_back(vertex[0][2]);

	polygon[2] = mesh.insertPolygon(points);
	REQUIRE(testInvariants(mesh));

	// o---o---o   o
	// |0\1|###|
	// o---o#2#o   o
	// |#######|
	// o---o---o   o
	//
	// o   o   o   o

	points.clear();
	points.push_back(vertex[2][2]);
	points.push_back(vertex[3][2]);
	points.push_back(vertex[3][3]);
	points.push_back(vertex[2][3]);

	polygon[3] = mesh.insertPolygon(points);
	REQUIRE(testInvariants(mesh));

	// o---o---o   o
	// |0\1|###|
	// o---o#2#o   o
	// |#######|
	// o---o---o---o
	//         |#3#|
	// o   o   o---o

	points.clear();
	points.push_back(vertex[1][2]);
	points.push_back(vertex[2][3]);
	points.push_back(vertex[1][3]);

	polygon[4] = mesh.insertPolygon(points);
	REQUIRE(testInvariants(mesh));

	// o---o---o   o
	// |0\1|###|
	// o---o#2#o---o
	// |#######| \4|
	// o---o---o---o
	//         |#3#|
	// o   o   o---o

	points.clear();
	points.push_back(vertex[1][2]);
	points.push_back(vertex[2][2]);
	points.push_back(vertex[2][3]);

	polygon[5] = mesh.insertPolygon(points);
	REQUIRE(testInvariants(mesh));

	// o---o---o   o
	// |0\1|###|
	// o---o#2#o---o
	// |#######|5\4|
	// o---o---o---o
	//         |#3#|
	// o   o   o---o

	points.clear();
	points.push_back(vertex[0][2]);
	points.push_back(vertex[1][2]);
	points.push_back(vertex[1][3]);
	points.push_back(vertex[0][3]);

	polygon[6] = mesh.insertPolygon(points);
	REQUIRE(testInvariants(mesh));

	// o---o---o---o
	// |0\1|###|#6#|
	// o---o#2#o---o
	// |#######|5\4|
	// o---o---o---o
	//         |#3#|
	// o   o   o---o

	points.clear();
	points.push_back(vertex[2][0]);
	points.push_back(vertex[3][0]);
	points.push_back(vertex[3][1]);
	points.push_back(vertex[3][2]);
	points.push_back(vertex[2][2]);
	points.push_back(vertex[2][1]);

	polygon[7] = mesh.insertPolygon(points);
	REQUIRE(testInvariants(mesh));

	// o---o---o---o
	// |0\1|###|#6#|
	// o---o#2#o---o
	// |#######|5\4|
	// o---o---o---o
	// |###7###|#3#|
	// o---o---o---o

	mesh.removePolygon(polygon[5]);
	REQUIRE(testInvariants(mesh));

	mesh.removePolygon(polygon[0]);
	REQUIRE(testInvariants(mesh));

	mesh.removePolygon(polygon[7]);
	REQUIRE(testInvariants(mesh));

	mesh.removePolygon(polygon[6]);
	REQUIRE(testInvariants(mesh));

	mesh.removePolygon(polygon[4]);
	REQUIRE(testInvariants(mesh));

	mesh.removePolygon(polygon[1]);
	REQUIRE(testInvariants(mesh));

	mesh.removePolygon(polygon[2]);
	REQUIRE(testInvariants(mesh));

	mesh.removePolygon(polygon[3]);
	REQUIRE(testInvariants(mesh));
}

TEST_CASE("Polygon2 (HalfMesh)")
{
	Mesh mesh;

	Vertex_Iterator vertex[5][4];

	for (integer i = 0;i < 5;++i)
	{
		for (integer j = 0;j < 4;++j)
		{
			vertex[i][j] = mesh.insertVertex();
			REQUIRE(testInvariants(mesh));

			REQUIRE(!vertex[i][j].empty());
			(vertex[i][j])->data() = i * 10 + j;
		}
	}

	// o   o   o   o
	//
	// o   o   o   o
	//
	// o   o   o   o
	//
	// o   o   o   o

	std::vector<Vertex_Iterator> points;

	points.clear();
	points.push_back(vertex[3][1]);
	points.push_back(vertex[4][1]);
	points.push_back(vertex[4][0]);

	mesh.insertPolygon(points);
	REQUIRE(testInvariants(mesh));

	points.clear();
	points.push_back(vertex[2][1]);
	points.push_back(vertex[2][2]);
	points.push_back(vertex[3][1]);

	mesh.insertPolygon(points);
	REQUIRE(testInvariants(mesh));

	points.clear();
	points.push_back(vertex[2][2]);
	points.push_back(vertex[2][3]);
	points.push_back(vertex[3][3]);

	mesh.insertPolygon(points);
	REQUIRE(testInvariants(mesh));

	points.clear();
	points.push_back(vertex[1][2]);
	points.push_back(vertex[1][3]);
	points.push_back(vertex[2][2]);

	mesh.insertPolygon(points);
	REQUIRE(testInvariants(mesh));

	points.clear();
	points.push_back(vertex[0][1]);
	points.push_back(vertex[0][2]);
	points.push_back(vertex[1][2]);

	mesh.insertPolygon(points);
	REQUIRE(testInvariants(mesh));

	points.clear();
	points.push_back(vertex[0][1]);
	points.push_back(vertex[1][2]);
	points.push_back(vertex[1][1]);

	mesh.insertPolygon(points);
	REQUIRE(testInvariants(mesh));

	points.clear();
	points.push_back(vertex[1][3]);
	points.push_back(vertex[2][3]);
	points.push_back(vertex[2][2]);

	mesh.insertPolygon(points);
	REQUIRE(testInvariants(mesh));

	points.clear();
	points.push_back(vertex[3][2]);
	points.push_back(vertex[4][2]);
	points.push_back(vertex[4][1]);

	mesh.insertPolygon(points);
	REQUIRE(testInvariants(mesh));

	points.clear();
	points.push_back(vertex[3][2]);
	points.push_back(vertex[3][3]);
	points.push_back(vertex[4][2]);

	mesh.insertPolygon(points);
	REQUIRE(testInvariants(mesh));

	points.clear();
	points.push_back(vertex[2][2]);
	points.push_back(vertex[3][3]);
	points.push_back(vertex[3][2]);

	mesh.insertPolygon(points);
	REQUIRE(testInvariants(mesh));

	points.clear();
	points.push_back(vertex[0][2]);
	points.push_back(vertex[1][3]);
	points.push_back(vertex[1][2]);

	mesh.insertPolygon(points);
	REQUIRE(testInvariants(mesh));

	points.clear();
	points.push_back(vertex[3][1]);
	points.push_back(vertex[3][2]);
	points.push_back(vertex[4][1]);

	mesh.insertPolygon(points);
	REQUIRE(testInvariants(mesh));

	points.clear();
	points.push_back(vertex[2][2]);
	points.push_back(vertex[3][2]);
	points.push_back(vertex[3][1]);

	mesh.insertPolygon(points);
	REQUIRE(testInvariants(mesh));

	points.clear();
	points.push_back(vertex[1][1]);
	points.push_back(vertex[2][1]);
	points.push_back(vertex[2][0]);

	mesh.insertPolygon(points);
	REQUIRE(testInvariants(mesh));

	points.clear();
	points.push_back(vertex[1][1]);
	points.push_back(vertex[2][2]);
	points.push_back(vertex[2][1]);

	mesh.insertPolygon(points);
	REQUIRE(testInvariants(mesh));

	points.clear();
	points.push_back(vertex[0][0]);
	points.push_back(vertex[1][1]);
	points.push_back(vertex[1][0]);

	mesh.insertPolygon(points);
	REQUIRE(testInvariants(mesh));

	points.clear();
	points.push_back(vertex[1][1]);
	points.push_back(vertex[1][2]);
	points.push_back(vertex[2][2]);

	mesh.insertPolygon(points);
	REQUIRE(testInvariants(mesh));
}

TEST_CASE("Merge (HalfMesh)")
{
	Mesh mesh;

	Vertex_Iterator vertex[4][4];

	for (integer i = 0; i < 4; ++i)
	{
		for (integer j = 0; j < 4; ++j)
		{
			vertex[i][j] = mesh.insertVertex();
			REQUIRE(testInvariants(mesh));
			REQUIRE(!vertex[i][j].empty());
			(vertex[i][j])->data() = i * 10 + j;
		}
	}

	// o   o   o   o
	//
	// o   o   o   o
	// 
	// o---o---o---o
	// |   |       |
	// o---o---o---o

	{
		Polygon_Iterator aPolygon = 
			mesh.insertPolygon(
			range({
				vertex[0][0],
				vertex[1][0],
				vertex[1][1],
				vertex[0][1]
			}));
		unused(aPolygon);

		REQUIRE(testInvariants(mesh));
		REQUIRE(mesh.vertices() == 16);
		REQUIRE(mesh.edges() == 4);
		REQUIRE(mesh.polygons() == 1);

		Polygon_Iterator bPolygon =
			mesh.insertPolygon(
			range({
				vertex[1][0],
				vertex[2][0],
				vertex[3][0],
				vertex[3][1],
				vertex[2][1],
				vertex[1][1]
			}));
		unused(bPolygon);

		REQUIRE(testInvariants(mesh));
		REQUIRE(mesh.vertices() == 16);
		REQUIRE(mesh.edges() == 9);
		REQUIRE(mesh.polygons() == 2);

		mesh.merge(mesh.findHalf(vertex[1][0], vertex[1][1]));

		REQUIRE(testInvariants(mesh));
		REQUIRE(mesh.vertices() == 16);
		REQUIRE(mesh.edges() == 8);
		REQUIRE(mesh.polygons() == 1);
	}
}

