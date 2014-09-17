// Description: Testing for HalfMesh
// DocumentationOf: halfmesh.h

#include "test_pastelgeometry.h"

#include "pastel/geometry/halfmesh.h"
#include "pastel/sys/random.h"

#include <algorithm>
#include <iostream>

using namespace Pastel;

namespace
{

	using Mesh = HalfEdge<int, int, int, int>;
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
			TEST_ENSURE(testInvariants(mesh));

			mesh.clear();
			TEST_ENSURE(testInvariants(mesh));

			Mesh otherMesh(mesh);
			TEST_ENSURE(testInvariants(mesh));
			TEST_ENSURE(testInvariants(otherMesh));

			otherMesh.swap(mesh);
			TEST_ENSURE(testInvariants(mesh));
			TEST_ENSURE(testInvariants(otherMesh));

			otherMesh.clear();
			TEST_ENSURE(testInvariants(otherMesh));

			mesh.clear();
			TEST_ENSURE(testInvariants(mesh));
		}

		void testVertexAdd()
		{
			Mesh mesh;

			integer n = 100;
			for (integer i = 0;i < n;++i)
			{
				mesh.insertVertex();
				TEST_ENSURE(testInvariants(mesh));
			}

			Mesh otherMesh(mesh);
			TEST_ENSURE(testInvariants(mesh));
			TEST_ENSURE(testInvariants(otherMesh));
		}

		void testEdge()
		{
			Mesh mesh;

			integer VertexCount = 100;
			std::vector<Vertex_Iterator> vertex(VertexCount);

			for (integer i = 0;i < VertexCount;++i)
			{
				vertex[i] = mesh.insertVertex();
				TEST_ENSURE(testInvariants(mesh));
			}

			{
				Mesh otherMesh(mesh);
				TEST_ENSURE(testInvariants(mesh));
				TEST_ENSURE(testInvariants(otherMesh));
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
					
					Edge_Iterator edge = 
						mesh.insertEdge(vertex[aVertex], vertex[bVertex]);
					TEST_ENSURE(testInvariants(mesh));
					ENSURE(edge.isNormal());

					edgeList[i * BucketSize + j] = edge;

				}

			}

			TEST_ENSURE(testInvariants(mesh));

			{
				Mesh otherMesh(mesh);
				TEST_ENSURE(testInvariants(mesh));
				TEST_ENSURE(testInvariants(otherMesh));
			}

			for (integer i = 0;i < EdgeCount;++i)
			{
				integer index = randomInteger(EdgeCount);
				if (edgeList[index].isNormal())
				{
					mesh.removeEdge(edgeList[index]);
					edgeList[index].clear();
				}

				TEST_ENSURE(testInvariants(mesh));
			}
		}

		void testPolygon()
		{
			Mesh mesh;

			Vertex_Iterator vertex[4][4];

			for (integer i = 0;i < 4;++i)
			{
				for (integer j = 0;j < 4;++j)
				{
					vertex[i][j] = mesh.insertVertex();
					TEST_ENSURE(testInvariants(mesh));
					TEST_ENSURE(!vertex[i][j].empty());
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
			TEST_ENSURE(testInvariants(mesh));

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
			TEST_ENSURE(testInvariants(mesh));

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
			TEST_ENSURE(testInvariants(mesh));

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
			TEST_ENSURE(testInvariants(mesh));

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
			TEST_ENSURE(testInvariants(mesh));

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
			TEST_ENSURE(testInvariants(mesh));

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
			TEST_ENSURE(testInvariants(mesh));

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
			TEST_ENSURE(testInvariants(mesh));

			// o---o---o---o
			// |0\1|###|#6#|
			// o---o#2#o---o
			// |#######|5\4|
			// o---o---o---o
			// |###7###|#3#|
			// o---o---o---o

			mesh.removePolygon(polygon[5]);
			TEST_ENSURE(testInvariants(mesh));

			mesh.removePolygon(polygon[0]);
			TEST_ENSURE(testInvariants(mesh));

			mesh.removePolygon(polygon[7]);
			TEST_ENSURE(testInvariants(mesh));

			mesh.removePolygon(polygon[6]);
			TEST_ENSURE(testInvariants(mesh));

			mesh.removePolygon(polygon[4]);
			TEST_ENSURE(testInvariants(mesh));

			mesh.removePolygon(polygon[1]);
			TEST_ENSURE(testInvariants(mesh));

			mesh.removePolygon(polygon[2]);
			TEST_ENSURE(testInvariants(mesh));

			mesh.removePolygon(polygon[3]);
			TEST_ENSURE(testInvariants(mesh));
		}

		void testPolygon2()
		{
			Mesh mesh;

			Vertex_Iterator vertex[5][4];

			for (integer i = 0;i < 5;++i)
			{
				for (integer j = 0;j < 4;++j)
				{
					vertex[i][j] = mesh.insertVertex();
					TEST_ENSURE(testInvariants(mesh));

					TEST_ENSURE(!vertex[i][j].empty());
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
			TEST_ENSURE(testInvariants(mesh));

			points.clear();
			points.push_back(vertex[2][1]);
			points.push_back(vertex[2][2]);
			points.push_back(vertex[3][1]);

			mesh.insertPolygon(points);
			TEST_ENSURE(testInvariants(mesh));

			points.clear();
			points.push_back(vertex[2][2]);
			points.push_back(vertex[2][3]);
			points.push_back(vertex[3][3]);

			mesh.insertPolygon(points);
			TEST_ENSURE(testInvariants(mesh));

			points.clear();
			points.push_back(vertex[1][2]);
			points.push_back(vertex[1][3]);
			points.push_back(vertex[2][2]);

			mesh.insertPolygon(points);
			TEST_ENSURE(testInvariants(mesh));

			points.clear();
			points.push_back(vertex[0][1]);
			points.push_back(vertex[0][2]);
			points.push_back(vertex[1][2]);

			mesh.insertPolygon(points);
			TEST_ENSURE(testInvariants(mesh));

			points.clear();
			points.push_back(vertex[0][1]);
			points.push_back(vertex[1][2]);
			points.push_back(vertex[1][1]);

			mesh.insertPolygon(points);
			TEST_ENSURE(testInvariants(mesh));

			points.clear();
			points.push_back(vertex[1][3]);
			points.push_back(vertex[2][3]);
			points.push_back(vertex[2][2]);

			mesh.insertPolygon(points);
			TEST_ENSURE(testInvariants(mesh));

			points.clear();
			points.push_back(vertex[3][2]);
			points.push_back(vertex[4][2]);
			points.push_back(vertex[4][1]);

			mesh.insertPolygon(points);
			TEST_ENSURE(testInvariants(mesh));

			points.clear();
			points.push_back(vertex[3][2]);
			points.push_back(vertex[3][3]);
			points.push_back(vertex[4][2]);

			mesh.insertPolygon(points);
			TEST_ENSURE(testInvariants(mesh));

			points.clear();
			points.push_back(vertex[2][2]);
			points.push_back(vertex[3][3]);
			points.push_back(vertex[3][2]);

			mesh.insertPolygon(points);
			TEST_ENSURE(testInvariants(mesh));

			points.clear();
			points.push_back(vertex[0][2]);
			points.push_back(vertex[1][3]);
			points.push_back(vertex[1][2]);

			mesh.insertPolygon(points);
			TEST_ENSURE(testInvariants(mesh));

			points.clear();
			points.push_back(vertex[3][1]);
			points.push_back(vertex[3][2]);
			points.push_back(vertex[4][1]);

			mesh.insertPolygon(points);
			TEST_ENSURE(testInvariants(mesh));

			points.clear();
			points.push_back(vertex[2][2]);
			points.push_back(vertex[3][2]);
			points.push_back(vertex[3][1]);

			mesh.insertPolygon(points);
			TEST_ENSURE(testInvariants(mesh));

			points.clear();
			points.push_back(vertex[1][1]);
			points.push_back(vertex[2][1]);
			points.push_back(vertex[2][0]);

			mesh.insertPolygon(points);
			TEST_ENSURE(testInvariants(mesh));

			points.clear();
			points.push_back(vertex[1][1]);
			points.push_back(vertex[2][2]);
			points.push_back(vertex[2][1]);

			mesh.insertPolygon(points);
			TEST_ENSURE(testInvariants(mesh));

			points.clear();
			points.push_back(vertex[0][0]);
			points.push_back(vertex[1][1]);
			points.push_back(vertex[1][0]);

			mesh.insertPolygon(points);
			TEST_ENSURE(testInvariants(mesh));

			points.clear();
			points.push_back(vertex[1][1]);
			points.push_back(vertex[1][2]);
			points.push_back(vertex[2][2]);

			mesh.insertPolygon(points);
			TEST_ENSURE(testInvariants(mesh));
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
