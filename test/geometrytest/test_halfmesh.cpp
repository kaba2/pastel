// Description: Testing for HalfMesh
// DocumentationOf: halfmesh.h

#include "test_pastelgeometry.h"

#include "pastel/geometry/halfmesh_tools.h"
#include "pastel/geometry/simplehalfmesh.h"
#include "pastel/sys/random.h"

#include <algorithm>
#include <iostream>

using namespace Pastel;

namespace
{

	typedef SimpleHalfMesh<int, int, int, int> Mesh;
	typedef Mesh::Vertex Vertex;
	typedef Mesh::Half Half;
	typedef Mesh::Edge Edge;
	typedef Mesh::Polygon Polygon;
	typedef Mesh::ConstVertexIterator ConstVertexIterator;
	typedef Mesh::ConstHalfIterator ConstHalfIterator;
	typedef Mesh::ConstEdgeIterator ConstEdgeIterator;
	typedef Mesh::ConstPolygonIterator ConstPolygonIterator;

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

	class Test
		: public TestSuite
	{
	public:
		Test()
			: TestSuite(&testReport())
		{
		}

		virtual void run()
		{
			testBasic();
			testVertexAdd();
			testPolygon();
			testPolygon2();
			testEdge();
		}

		void testBasic()
		{
			Mesh mesh;
			mesh.clear();

			Mesh otherMesh(mesh);
			otherMesh.swap(mesh);

			otherMesh.clear();
			mesh.clear();
		}

		void testVertexAdd()
		{
			Mesh mesh;

			const integer VertexCount = 100;
			Vertex vertex[VertexCount];

			for (integer i = 0;i < VertexCount;++i)
			{
				vertex[i] = mesh.addVertex();
				TEST_ENSURE(!vertex[i].empty());
			}

			checkInvariants(mesh);

			Mesh otherMesh(mesh);
			checkInvariants(otherMesh);
		}

		void testEdge()
		{
			Mesh mesh;

			const integer VertexCount = 100;
			std::vector<Vertex> vertex(VertexCount);

			for (integer i = 0;i < VertexCount;++i)
			{
				vertex[i] = mesh.addVertex();
				TEST_ENSURE(!vertex[i].empty());
			}

			{
				Mesh otherMesh(mesh);
				checkInvariants(otherMesh);
			}

			const integer BucketSize = 100;
			const integer BucketCount = 10;
			const integer EdgeCount = BucketSize * BucketCount;
			std::vector<Edge> edgeList(EdgeCount);

			for (integer i = 0;i < BucketCount;++i)
			{
				for (integer j = 0;j < BucketSize;++j)
				{
					const integer aVertex =
						randomInteger() % VertexCount;
					const integer bVertex =
						randomInteger() % VertexCount;
					bool alreadyExisted = false;
					Edge edge(
						mesh.addEdge(vertex[aVertex], vertex[bVertex],
						&alreadyExisted));
					if (alreadyExisted)
					{
						edge.clear();
					}
					edgeList[i * BucketSize + j] = edge;
					checkInvariants(mesh);
				}

			}

			checkInvariants(mesh);

			{
				Mesh otherMesh(mesh);
				checkInvariants(otherMesh);
			}

			for (integer i = 0;i < EdgeCount;++i)
			{
				const integer index = randomInteger() % EdgeCount;
				if (!edgeList[index].empty())
				{
					mesh.removeEdge(edgeList[index]);
					edgeList[index].clear();
				}

				checkInvariants(mesh);
			}

			{
				Mesh otherMesh(mesh);
				checkInvariants(otherMesh);
			}
		}

		void testPolygon()
		{
			Mesh mesh;

			Vertex vertex[4][4];

			for (integer i = 0;i < 4;++i)
			{
				for (integer j = 0;j < 4;++j)
				{
					vertex[i][j] = mesh.addVertex();
					TEST_ENSURE(!vertex[i][j].empty());
					vertex[i][j]() = i * 10 + j;
				}
			}

			// o   o   o   o
			//
			// o   o   o   o
			//
			// o   o   o   o
			//
			// o   o   o   o

			Polygon polygon[16];

			std::vector<Vertex> points;

			points.clear();
			points.push_back(vertex[0][0]);
			points.push_back(vertex[1][0]);
			points.push_back(vertex[1][1]);

			polygon[0] = mesh.addPolygon(points);

			checkInvariants(mesh);

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

			polygon[1] = mesh.addPolygon(points);

			checkInvariants(mesh);

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

			polygon[2] = mesh.addPolygon(points);

			checkInvariants(mesh);

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

			polygon[3] = mesh.addPolygon(points);

			checkInvariants(mesh);

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

			polygon[4] = mesh.addPolygon(points);

			checkInvariants(mesh);

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

			polygon[5] = mesh.addPolygon(points);

			checkInvariants(mesh);

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

			polygon[6] = mesh.addPolygon(points);

			checkInvariants(mesh);

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

			polygon[7] = mesh.addPolygon(points);

			checkInvariants(mesh);

			// o---o---o---o
			// |0\1|###|#6#|
			// o---o#2#o---o
			// |#######|5\4|
			// o---o---o---o
			// |###7###|#3#|
			// o---o---o---o

			mesh.removePolygon(polygon[5]);

			checkInvariants(mesh);

			mesh.removePolygon(polygon[0]);

			checkInvariants(mesh);

			mesh.removePolygon(polygon[7]);

			checkInvariants(mesh);

			mesh.removePolygon(polygon[6]);

			checkInvariants(mesh);

			mesh.removePolygon(polygon[4]);

			checkInvariants(mesh);

			mesh.removePolygon(polygon[1]);

			checkInvariants(mesh);

			mesh.removePolygon(polygon[2]);

			checkInvariants(mesh);

			mesh.removePolygon(polygon[3]);

			checkInvariants(mesh);
		}

		void testPolygon2()
		{
			Mesh mesh;

			Vertex vertex[5][4];

			for (integer i = 0;i < 5;++i)
			{
				for (integer j = 0;j < 4;++j)
				{
					vertex[i][j] = mesh.addVertex();
					TEST_ENSURE(!vertex[i][j].empty());
					vertex[i][j]() = i * 10 + j;
				}
			}

			// o   o   o   o
			//
			// o   o   o   o
			//
			// o   o   o   o
			//
			// o   o   o   o

			std::vector<Vertex> points;

			points.clear();
			points.push_back(vertex[3][1]);
			points.push_back(vertex[4][1]);
			points.push_back(vertex[4][0]);

			mesh.addPolygon(points);
			checkInvariants(mesh);

			points.clear();
			points.push_back(vertex[2][1]);
			points.push_back(vertex[2][2]);
			points.push_back(vertex[3][1]);

			mesh.addPolygon(points);
			checkInvariants(mesh);

			points.clear();
			points.push_back(vertex[2][2]);
			points.push_back(vertex[2][3]);
			points.push_back(vertex[3][3]);

			mesh.addPolygon(points);
			checkInvariants(mesh);

			points.clear();
			points.push_back(vertex[1][2]);
			points.push_back(vertex[1][3]);
			points.push_back(vertex[2][2]);

			mesh.addPolygon(points);
			checkInvariants(mesh);

			points.clear();
			points.push_back(vertex[0][1]);
			points.push_back(vertex[0][2]);
			points.push_back(vertex[1][2]);

			mesh.addPolygon(points);
			checkInvariants(mesh);

			points.clear();
			points.push_back(vertex[0][1]);
			points.push_back(vertex[1][2]);
			points.push_back(vertex[1][1]);

			mesh.addPolygon(points);
			checkInvariants(mesh);

			points.clear();
			points.push_back(vertex[1][3]);
			points.push_back(vertex[2][3]);
			points.push_back(vertex[2][2]);

			mesh.addPolygon(points);
			checkInvariants(mesh);

			points.clear();
			points.push_back(vertex[3][2]);
			points.push_back(vertex[4][2]);
			points.push_back(vertex[4][1]);

			mesh.addPolygon(points);
			checkInvariants(mesh);

			points.clear();
			points.push_back(vertex[3][2]);
			points.push_back(vertex[3][3]);
			points.push_back(vertex[4][2]);

			mesh.addPolygon(points);
			checkInvariants(mesh);

			points.clear();
			points.push_back(vertex[2][2]);
			points.push_back(vertex[3][3]);
			points.push_back(vertex[3][2]);

			mesh.addPolygon(points);
			checkInvariants(mesh);

			points.clear();
			points.push_back(vertex[0][2]);
			points.push_back(vertex[1][3]);
			points.push_back(vertex[1][2]);

			mesh.addPolygon(points);
			checkInvariants(mesh);

			points.clear();
			points.push_back(vertex[3][1]);
			points.push_back(vertex[3][2]);
			points.push_back(vertex[4][1]);

			mesh.addPolygon(points);
			checkInvariants(mesh);

			points.clear();
			points.push_back(vertex[2][2]);
			points.push_back(vertex[3][2]);
			points.push_back(vertex[3][1]);

			mesh.addPolygon(points);
			checkInvariants(mesh);

			points.clear();
			points.push_back(vertex[1][1]);
			points.push_back(vertex[2][1]);
			points.push_back(vertex[2][0]);

			mesh.addPolygon(points);
			checkInvariants(mesh);

			points.clear();
			points.push_back(vertex[1][1]);
			points.push_back(vertex[2][2]);
			points.push_back(vertex[2][1]);

			mesh.addPolygon(points);
			checkInvariants(mesh);

			points.clear();
			points.push_back(vertex[0][0]);
			points.push_back(vertex[1][1]);
			points.push_back(vertex[1][0]);

			mesh.addPolygon(points);
			checkInvariants(mesh);

			points.clear();
			points.push_back(vertex[1][1]);
			points.push_back(vertex[1][2]);
			points.push_back(vertex[2][2]);

			mesh.addPolygon(points);
			checkInvariants(mesh);
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("HalfMesh", test);
	}

	CallFunction run(addTest);

}
