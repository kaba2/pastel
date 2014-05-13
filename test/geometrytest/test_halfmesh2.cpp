// Description: More testing for HalfMesh
// DocumentationOf: halfmesh.h

#include "test_pastelgeometry.h"

#include "pastel/geometry/halfmesh.h"
#include "pastel/sys/random.h"

using namespace Pastel;
using namespace std;

namespace
{

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
			testTrivial();
			testSimpleEdge();
			testMultiHalfMesh();
			testSimpleRemoval();
			testComplexHalfMesh();
		}

		void testTrivial()
		{
			PureHalfMesh halfMesh;
			halfMesh.insertVertex();
			for (int i = 0;i < 10000;i++)
			{
				halfMesh.insertVertex();
				TEST_ENSURE(testInvariants(halfMesh));
			}

			{
				PureHalfMesh otherMesh(halfMesh);
				TEST_ENSURE(testInvariants(otherMesh));
			}
		}

		void testSimpleEdge()
		{
			{
				PureHalfMesh halfMesh;
				PureHalfMesh::Vertex a = halfMesh.insertVertex();
				TEST_ENSURE(testInvariants(halfMesh));
				PureHalfMesh::Vertex b = halfMesh.insertVertex();
				TEST_ENSURE(testInvariants(halfMesh));

				PureHalfMesh::Edge e = halfMesh.insertEdge(a, b);
				TEST_ENSURE(testInvariants(halfMesh));
			}
			{
				PureHalfMesh halfMesh;
				PureHalfMesh::Vertex a = halfMesh.insertVertex();
				TEST_ENSURE(testInvariants(halfMesh));
				PureHalfMesh::Vertex b = halfMesh.insertVertex();
				TEST_ENSURE(testInvariants(halfMesh));
				PureHalfMesh::Vertex c = halfMesh.insertVertex();
				TEST_ENSURE(testInvariants(halfMesh));

				halfMesh.insertEdge(a, b);
				TEST_ENSURE(testInvariants(halfMesh));
				halfMesh.insertEdge(b, c);
				TEST_ENSURE(testInvariants(halfMesh));
				halfMesh.insertEdge(a, c);
				TEST_ENSURE(testInvariants(halfMesh));
			}
			{
				PureHalfMesh halfMesh;
				PureHalfMesh::Vertex a = halfMesh.insertVertex();
				TEST_ENSURE(testInvariants(halfMesh));
				halfMesh.insertEdge(a, a);
				TEST_ENSURE(testInvariants(halfMesh));
			}
		}

		void testMultiHalfMesh()
		{
			{
				PureHalfMesh halfMesh;

				PureHalfMesh::Vertex a = halfMesh.insertVertex();
				TEST_ENSURE(testInvariants(halfMesh));
				PureHalfMesh::Vertex b = halfMesh.insertVertex();
				TEST_ENSURE(testInvariants(halfMesh));

				halfMesh.insertEdge(a, b);
				TEST_ENSURE(testInvariants(halfMesh));
				halfMesh.insertEdge(b, a);
				TEST_ENSURE(testInvariants(halfMesh));
				halfMesh.insertEdge(b, a);
				TEST_ENSURE(testInvariants(halfMesh));
			}
			{
				PureHalfMesh halfMesh;

				PureHalfMesh::Vertex a = halfMesh.insertVertex();
				TEST_ENSURE(testInvariants(halfMesh));
				PureHalfMesh::Vertex b = halfMesh.insertVertex();
				TEST_ENSURE(testInvariants(halfMesh));

				halfMesh.insertEdge(a, b);
				TEST_ENSURE(testInvariants(halfMesh));
				halfMesh.insertEdge(b, a);
				TEST_ENSURE(testInvariants(halfMesh));
				halfMesh.insertEdge(b, a);
				TEST_ENSURE(testInvariants(halfMesh));

				halfMesh.insertEdge(a, a);
				TEST_ENSURE(testInvariants(halfMesh));
				halfMesh.insertEdge(b, b);
				TEST_ENSURE(testInvariants(halfMesh));
				halfMesh.insertEdge(a, a);
				TEST_ENSURE(testInvariants(halfMesh));
				halfMesh.insertEdge(b, b);
				TEST_ENSURE(testInvariants(halfMesh));
			}
		}

		void testSimpleRemoval()
		{
			{
				PureHalfMesh halfMesh;
				PureHalfMesh::Vertex a = halfMesh.insertVertex();
				TEST_ENSURE(testInvariants(halfMesh));
				PureHalfMesh::Vertex b = halfMesh.insertVertex();
				TEST_ENSURE(testInvariants(halfMesh));
				PureHalfMesh::Edge edge = halfMesh.insertEdge(a, b);
				TEST_ENSURE(testInvariants(halfMesh));

				halfMesh.removeEdge(edge);
				TEST_ENSURE(testInvariants(halfMesh));
			}
			{
				PASTEL_CONSTEXPR int Vertices = 100;

				PureHalfMesh halfMesh;
				PureHalfMesh::Vertex vertices[Vertices];
				PureHalfMesh::Edge edges[Vertices - 1];
				for (int i = 0;i < Vertices;++i)
				{
					vertices[i] = halfMesh.insertVertex();
					TEST_ENSURE(testInvariants(halfMesh));
				}

				for (int i = 1;i < Vertices;++i)
				{
					edges[i - 1] = halfMesh.insertEdge(vertices[0], vertices[i]);
					TEST_ENSURE(testInvariants(halfMesh));
				}

				for (int i = 0;i < Vertices - 1;++i)
				{
					halfMesh.removeEdge(edges[i]);
					TEST_ENSURE(testInvariants(halfMesh));
				}
			}
			/*
			{
			PureHalfMesh halfMesh;
			PureHalfMesh::Vertex a = halfMesh.insertVertex();
			TEST_ENSURE(testInvariants(halfMesh));
			PureHalfMesh::Edge edge = halfMesh.insertEdge(a, a);
			TEST_ENSURE(testInvariants(halfMesh));

			halfMesh.removeEdge(edge);
			TEST_ENSURE(testInvariants(halfMesh));
			}
			*/

			/*
			{
			PureHalfMesh halfMesh;

			PureHalfMesh::Vertex a = halfMesh.insertVertex();
			TEST_ENSURE(testInvariants(halfMesh));
			PureHalfMesh::Vertex b = halfMesh.insertVertex();
			TEST_ENSURE(testInvariants(halfMesh));

			PureHalfMesh::Edge A = halfMesh.insertEdge(a, b);
			TEST_ENSURE(testInvariants(halfMesh));
			PureHalfMesh::Edge B = halfMesh.insertEdge(b, a);
			TEST_ENSURE(testInvariants(halfMesh));
			PureHalfMesh::Edge C = halfMesh.insertEdge(b, a);
			TEST_ENSURE(testInvariants(halfMesh));
			PureHalfMesh::Edge D = halfMesh.insertEdge(a, a);
			TEST_ENSURE(testInvariants(halfMesh));
			PureHalfMesh::Edge E = halfMesh.insertEdge(b, b);
			TEST_ENSURE(testInvariants(halfMesh));
			PureHalfMesh::Edge F = halfMesh.insertEdge(a, a);
			TEST_ENSURE(testInvariants(halfMesh));
			PureHalfMesh::Edge G = halfMesh.insertEdge(b, b);
			TEST_ENSURE(testInvariants(halfMesh));

			halfMesh.removeEdge(D);
			TEST_ENSURE(testInvariants(halfMesh));
			halfMesh.removeEdge(E);
			TEST_ENSURE(testInvariants(halfMesh));
			halfMesh.removeEdge(A);
			TEST_ENSURE(testInvariants(halfMesh));
			halfMesh.removeEdge(B);
			TEST_ENSURE(testInvariants(halfMesh));
			halfMesh.removeEdge(F);
			TEST_ENSURE(testInvariants(halfMesh));
			halfMesh.removeEdge(C);
			TEST_ENSURE(testInvariants(halfMesh));
			halfMesh.removeEdge(G);
			TEST_ENSURE(testInvariants(halfMesh));
			}
			*/
		}

		void testComplexHalfMesh()
		{
			PASTEL_CONSTEXPR int Vertices = 1000;
			PASTEL_CONSTEXPR int Edges = 1000;

			std::vector<PureHalfMesh::Vertex> vertices;
			std::vector<PureHalfMesh::Edge> edges;
			PureHalfMesh halfMesh;

			{
				vertices.reserve(Vertices);
				for (integer i = 0;i < Vertices;++i)
				{
					vertices.push_back(halfMesh.insertVertex());
				}
			}
			TEST_ENSURE(testInvariants(halfMesh));
			TEST_ENSURE_OP(vertices.size(), ==, Vertices);

			{
				edges.reserve(Edges);
				for (integer i = 0;i < Edges;++i)
				{
					PureHalfMesh::Edge edge;
					while (edge.empty())
					{
						integer aIndex = randomInteger(Vertices);
						integer bIndex = randomInteger(Vertices);

						PureHalfMesh::Vertex a = vertices[aIndex];
						PureHalfMesh::Vertex b = vertices[bIndex];
						edge = halfMesh.insertEdge(a, b);
					}
					TEST_ENSURE(testInvariants(halfMesh));

					edges.push_back(edge);
				}
			}
			TEST_ENSURE(testInvariants(halfMesh));
			TEST_ENSURE_OP(edges.size(), ==, Edges);

			{
				for (integer i = 0;i < Edges;++i)
				{
					PureHalfMesh::Edge edge = edges[i];
					TEST_ENSURE(testInvariants(halfMesh));
					halfMesh.removeEdge(edge);
				}
			}
			TEST_ENSURE(testInvariants(halfMesh));
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("MoreHalfMesh", test);
	}

	CallFunction run(addTest);

}
