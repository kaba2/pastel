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

	typedef HalfEdge<int, int, int, int> Mesh;
	typedef Mesh::Vertex_Iterator Vertex_Iterator;
	typedef Mesh::Half_Iterator Half_Iterator;
	typedef Mesh::Edge_Iterator Edge_Iterator;
	typedef Mesh::Polygon_Iterator Polygon_Iterator;
	typedef Mesh::Vertex_ConstIterator Vertex_ConstIterator;
	typedef Mesh::Half_ConstIterator Half_ConstIterator;
	typedef Mesh::Edge_ConstIterator Edge_ConstIterator;
	typedef Mesh::Polygon_ConstIterator Polygon_ConstIterator;

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

			integer n = 100;
			
			std::vector<Vertex_Iterator> vertex;
			for (integer i = 0;i < n;++i)
			{
				vertex[i] = mesh.insertVertex();
			}

			testInvariants(mesh);

			Mesh otherMesh(mesh);
			testInvariants(otherMesh);
		}

		void testEdge()
		{
			Mesh mesh;

			const integer VertexCount = 100;
			std::vector<Vertex_Iterator> vertex(VertexCount);

			for (integer i = 0;i < VertexCount;++i)
			{
				vertex[i] = mesh.insertVertex();
			}

			{
				Mesh otherMesh(mesh);
				testInvariants(otherMesh);
			}

			const integer BucketSize = 100;
			const integer BucketCount = 10;
			const integer EdgeCount = BucketSize * BucketCount;
			std::vector<Edge_Iterator> edgeList(EdgeCount);

			for (integer i = 0;i < BucketCount;++i)
			{
				for (integer j = 0;j < BucketSize;++j)
				{
					const integer aVertex =
						randomInteger(VertexCount);
					const integer bVertex =
						randomInteger(VertexCount);
					bool alreadyExisted = false;
					Edge_Iterator edge = 
						mesh.insertEdge(vertex[aVertex], vertex[bVertex]);
					edgeList[i * BucketSize + j] = edge;
					testInvariants(mesh);
				}

			}

			testInvariants(mesh);

			{
				Mesh otherMesh(mesh);
				testInvariants(otherMesh);
			}

			for (integer i = 0;i < EdgeCount;++i)
			{
				const integer index = randomInteger(EdgeCount);
				if (!edgeList[index].empty())
				{
					mesh.removeEdge(edgeList[index]);
					edgeList[index].clear();
				}

				testInvariants(mesh);
			}

			{
				Mesh otherMesh(mesh);
				testInvariants(otherMesh);
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
					TEST_ENSURE(!vertex[i][j].empty());
					*(vertex[i][j]) = i * 10 + j;
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

			testInvariants(mesh);

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

			testInvariants(mesh);

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

			testInvariants(mesh);

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

			testInvariants(mesh);

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

			testInvariants(mesh);

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

			testInvariants(mesh);

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

			testInvariants(mesh);

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

			testInvariants(mesh);

			// o---o---o---o
			// |0\1|###|#6#|
			// o---o#2#o---o
			// |#######|5\4|
			// o---o---o---o
			// |###7###|#3#|
			// o---o---o---o

			mesh.removePolygon(polygon[5]);

			testInvariants(mesh);

			mesh.removePolygon(polygon[0]);

			testInvariants(mesh);

			mesh.removePolygon(polygon[7]);

			testInvariants(mesh);

			mesh.removePolygon(polygon[6]);

			testInvariants(mesh);

			mesh.removePolygon(polygon[4]);

			testInvariants(mesh);

			mesh.removePolygon(polygon[1]);

			testInvariants(mesh);

			mesh.removePolygon(polygon[2]);

			testInvariants(mesh);

			mesh.removePolygon(polygon[3]);

			testInvariants(mesh);
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
					TEST_ENSURE(!vertex[i][j].empty());
					*(vertex[i][j]) = i * 10 + j;
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
			testInvariants(mesh);

			points.clear();
			points.push_back(vertex[2][1]);
			points.push_back(vertex[2][2]);
			points.push_back(vertex[3][1]);

			mesh.insertPolygon(points);
			testInvariants(mesh);

			points.clear();
			points.push_back(vertex[2][2]);
			points.push_back(vertex[2][3]);
			points.push_back(vertex[3][3]);

			mesh.insertPolygon(points);
			testInvariants(mesh);

			points.clear();
			points.push_back(vertex[1][2]);
			points.push_back(vertex[1][3]);
			points.push_back(vertex[2][2]);

			mesh.insertPolygon(points);
			testInvariants(mesh);

			points.clear();
			points.push_back(vertex[0][1]);
			points.push_back(vertex[0][2]);
			points.push_back(vertex[1][2]);

			mesh.insertPolygon(points);
			testInvariants(mesh);

			points.clear();
			points.push_back(vertex[0][1]);
			points.push_back(vertex[1][2]);
			points.push_back(vertex[1][1]);

			mesh.insertPolygon(points);
			testInvariants(mesh);

			points.clear();
			points.push_back(vertex[1][3]);
			points.push_back(vertex[2][3]);
			points.push_back(vertex[2][2]);

			mesh.insertPolygon(points);
			testInvariants(mesh);

			points.clear();
			points.push_back(vertex[3][2]);
			points.push_back(vertex[4][2]);
			points.push_back(vertex[4][1]);

			mesh.insertPolygon(points);
			testInvariants(mesh);

			points.clear();
			points.push_back(vertex[3][2]);
			points.push_back(vertex[3][3]);
			points.push_back(vertex[4][2]);

			mesh.insertPolygon(points);
			testInvariants(mesh);

			points.clear();
			points.push_back(vertex[2][2]);
			points.push_back(vertex[3][3]);
			points.push_back(vertex[3][2]);

			mesh.insertPolygon(points);
			testInvariants(mesh);

			points.clear();
			points.push_back(vertex[0][2]);
			points.push_back(vertex[1][3]);
			points.push_back(vertex[1][2]);

			mesh.insertPolygon(points);
			testInvariants(mesh);

			points.clear();
			points.push_back(vertex[3][1]);
			points.push_back(vertex[3][2]);
			points.push_back(vertex[4][1]);

			mesh.insertPolygon(points);
			testInvariants(mesh);

			points.clear();
			points.push_back(vertex[2][2]);
			points.push_back(vertex[3][2]);
			points.push_back(vertex[3][1]);

			mesh.insertPolygon(points);
			testInvariants(mesh);

			points.clear();
			points.push_back(vertex[1][1]);
			points.push_back(vertex[2][1]);
			points.push_back(vertex[2][0]);

			mesh.insertPolygon(points);
			testInvariants(mesh);

			points.clear();
			points.push_back(vertex[1][1]);
			points.push_back(vertex[2][2]);
			points.push_back(vertex[2][1]);

			mesh.insertPolygon(points);
			testInvariants(mesh);

			points.clear();
			points.push_back(vertex[0][0]);
			points.push_back(vertex[1][1]);
			points.push_back(vertex[1][0]);

			mesh.insertPolygon(points);
			testInvariants(mesh);

			points.clear();
			points.push_back(vertex[1][1]);
			points.push_back(vertex[1][2]);
			points.push_back(vertex[2][2]);

			mesh.insertPolygon(points);
			testInvariants(mesh);
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
