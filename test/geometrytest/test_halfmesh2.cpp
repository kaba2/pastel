// Description: More testing for HalfMesh
// DocumentationOf: halfmesh.h

#include "test_pastelgeometry.h"

#include "pastel/geometry/halfmesh_tools.h"
#include "pastel/geometry/simplehalfmesh.h"
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
			halfMesh.addVertex();
			for (int i = 0;i < 10000;i++)
			{
				halfMesh.addVertex();
				TEST_ENSURE(checkInvariants(halfMesh));
			}

			{
				PureHalfMesh otherMesh(halfMesh);
				TEST_ENSURE(checkInvariants(otherMesh));
			}
		}

		void testSimpleEdge()
		{
			{
				PureHalfMesh halfMesh;
				PureHalfMesh::Vertex a = halfMesh.addVertex();
				TEST_ENSURE(checkInvariants(halfMesh));
				PureHalfMesh::Vertex b = halfMesh.addVertex();
				TEST_ENSURE(checkInvariants(halfMesh));

				PureHalfMesh::Edge e = halfMesh.addEdge(a, b);
				TEST_ENSURE(checkInvariants(halfMesh));
			}
			{
				PureHalfMesh halfMesh;
				PureHalfMesh::Vertex a = halfMesh.addVertex();
				TEST_ENSURE(checkInvariants(halfMesh));
				PureHalfMesh::Vertex b = halfMesh.addVertex();
				TEST_ENSURE(checkInvariants(halfMesh));
				PureHalfMesh::Vertex c = halfMesh.addVertex();
				TEST_ENSURE(checkInvariants(halfMesh));

				halfMesh.addEdge(a, b);
				TEST_ENSURE(checkInvariants(halfMesh));
				halfMesh.addEdge(b, c);
				TEST_ENSURE(checkInvariants(halfMesh));
				halfMesh.addEdge(a, c);
				TEST_ENSURE(checkInvariants(halfMesh));
			}
			{
				PureHalfMesh halfMesh;
				PureHalfMesh::Vertex a = halfMesh.addVertex();
				TEST_ENSURE(checkInvariants(halfMesh));
				halfMesh.addEdge(a, a);
				TEST_ENSURE(checkInvariants(halfMesh));
			}
		}

		void testMultiHalfMesh()
		{
			{
				PureHalfMesh halfMesh;

				PureHalfMesh::Vertex a = halfMesh.addVertex();
				TEST_ENSURE(checkInvariants(halfMesh));
				PureHalfMesh::Vertex b = halfMesh.addVertex();
				TEST_ENSURE(checkInvariants(halfMesh));

				halfMesh.addEdge(a, b);
				TEST_ENSURE(checkInvariants(halfMesh));
				halfMesh.addEdge(b, a);
				TEST_ENSURE(checkInvariants(halfMesh));
				halfMesh.addEdge(b, a);
				TEST_ENSURE(checkInvariants(halfMesh));
			}
			{
				PureHalfMesh halfMesh;

				PureHalfMesh::Vertex a = halfMesh.addVertex();
				TEST_ENSURE(checkInvariants(halfMesh));
				PureHalfMesh::Vertex b = halfMesh.addVertex();
				TEST_ENSURE(checkInvariants(halfMesh));

				halfMesh.addEdge(a, b);
				TEST_ENSURE(checkInvariants(halfMesh));
				halfMesh.addEdge(b, a);
				TEST_ENSURE(checkInvariants(halfMesh));
				halfMesh.addEdge(b, a);
				TEST_ENSURE(checkInvariants(halfMesh));

				halfMesh.addEdge(a, a);
				TEST_ENSURE(checkInvariants(halfMesh));
				halfMesh.addEdge(b, b);
				TEST_ENSURE(checkInvariants(halfMesh));
				halfMesh.addEdge(a, a);
				TEST_ENSURE(checkInvariants(halfMesh));
				halfMesh.addEdge(b, b);
				TEST_ENSURE(checkInvariants(halfMesh));
			}
		}

		void testSimpleRemoval()
		{
			{
				PureHalfMesh halfMesh;
				PureHalfMesh::Vertex a = halfMesh.addVertex();
				TEST_ENSURE(checkInvariants(halfMesh));
				PureHalfMesh::Vertex b = halfMesh.addVertex();
				TEST_ENSURE(checkInvariants(halfMesh));
				PureHalfMesh::Edge edge = halfMesh.addEdge(a, b);
				TEST_ENSURE(checkInvariants(halfMesh));

				halfMesh.removeEdge(edge);
				TEST_ENSURE(checkInvariants(halfMesh));
			}
			{
				enum
				{
					Vertices = 100
				};

				PureHalfMesh halfMesh;
				PureHalfMesh::Vertex vertices[Vertices];
				PureHalfMesh::Edge edges[Vertices - 1];
				for (int i = 0;i < Vertices;++i)
				{
					vertices[i] = halfMesh.addVertex();
					TEST_ENSURE(checkInvariants(halfMesh));
				}

				for (int i = 1;i < Vertices;++i)
				{
					edges[i - 1] = halfMesh.addEdge(vertices[0], vertices[i]);
					TEST_ENSURE(checkInvariants(halfMesh));
				}

				for (int i = 0;i < Vertices - 1;++i)
				{
					halfMesh.removeEdge(edges[i]);
					TEST_ENSURE(checkInvariants(halfMesh));
				}
			}
			/*
			{
			PureHalfMesh halfMesh;
			PureHalfMesh::Vertex a = halfMesh.addVertex();
			TEST_ENSURE(checkInvariants(halfMesh));
			PureHalfMesh::Edge edge = halfMesh.addEdge(a, a);
			TEST_ENSURE(checkInvariants(halfMesh));

			halfMesh.removeEdge(edge);
			TEST_ENSURE(checkInvariants(halfMesh));
			}
			*/

			/*
			{
			PureHalfMesh halfMesh;

			PureHalfMesh::Vertex a = halfMesh.addVertex();
			TEST_ENSURE(checkInvariants(halfMesh));
			PureHalfMesh::Vertex b = halfMesh.addVertex();
			TEST_ENSURE(checkInvariants(halfMesh));

			PureHalfMesh::Edge A = halfMesh.addEdge(a, b);
			TEST_ENSURE(checkInvariants(halfMesh));
			PureHalfMesh::Edge B = halfMesh.addEdge(b, a);
			TEST_ENSURE(checkInvariants(halfMesh));
			PureHalfMesh::Edge C = halfMesh.addEdge(b, a);
			TEST_ENSURE(checkInvariants(halfMesh));
			PureHalfMesh::Edge D = halfMesh.addEdge(a, a);
			TEST_ENSURE(checkInvariants(halfMesh));
			PureHalfMesh::Edge E = halfMesh.addEdge(b, b);
			TEST_ENSURE(checkInvariants(halfMesh));
			PureHalfMesh::Edge F = halfMesh.addEdge(a, a);
			TEST_ENSURE(checkInvariants(halfMesh));
			PureHalfMesh::Edge G = halfMesh.addEdge(b, b);
			TEST_ENSURE(checkInvariants(halfMesh));

			halfMesh.removeEdge(D);
			TEST_ENSURE(checkInvariants(halfMesh));
			halfMesh.removeEdge(E);
			TEST_ENSURE(checkInvariants(halfMesh));
			halfMesh.removeEdge(A);
			TEST_ENSURE(checkInvariants(halfMesh));
			halfMesh.removeEdge(B);
			TEST_ENSURE(checkInvariants(halfMesh));
			halfMesh.removeEdge(F);
			TEST_ENSURE(checkInvariants(halfMesh));
			halfMesh.removeEdge(C);
			TEST_ENSURE(checkInvariants(halfMesh));
			halfMesh.removeEdge(G);
			TEST_ENSURE(checkInvariants(halfMesh));
			}
			*/
		}

		void testComplexHalfMesh()
		{
			enum
			{
				Vertices = 1000,
				Edges = 1000
			};

			std::vector<PureHalfMesh::Vertex> vertices;
			std::vector<PureHalfMesh::Edge> edges;
			PureHalfMesh halfMesh;

			{
				vertices.reserve(Vertices);
				for (integer i = 0;i < Vertices;++i)
				{
					vertices.push_back(halfMesh.addVertex());
				}
			}
			TEST_ENSURE(checkInvariants(halfMesh));
			TEST_ENSURE_OP(vertices.size(), ==, Vertices);

			{
				edges.reserve(Edges);
				for (integer i = 0;i < Edges;++i)
				{
					PureHalfMesh::Edge edge;
					while (edge.empty())
					{
						integer aIndex = randomInteger() % Vertices;
						integer bIndex = randomInteger() % Vertices;

						PureHalfMesh::Vertex a = vertices[aIndex];
						PureHalfMesh::Vertex b = vertices[bIndex];
						edge = halfMesh.addEdge(a, b);
					}
					TEST_ENSURE(checkInvariants(halfMesh));

					edges.push_back(edge);
				}
			}
			TEST_ENSURE(checkInvariants(halfMesh));
			TEST_ENSURE_OP(edges.size(), ==, Edges);

			{
				for (integer i = 0;i < Edges;++i)
				{
					PureHalfMesh::Edge edge = edges[i];
					TEST_ENSURE(checkInvariants(halfMesh));
					halfMesh.removeEdge(edge);
				}
			}
			TEST_ENSURE(checkInvariants(halfMesh));
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
