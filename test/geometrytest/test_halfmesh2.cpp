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
			HalfEdge<> halfMesh;
			halfMesh.insertVertex();
			for (int i = 0;i < 10000;i++)
			{
				halfMesh.insertVertex();
				TEST_ENSURE(testInvariants(halfMesh));
			}

			{
				HalfEdge<> otherMesh(halfMesh);
				TEST_ENSURE(testInvariants(otherMesh));
			}
		}

		void testSimpleEdge()
		{
			{
				HalfEdge<> halfMesh;
				HalfEdge<>::Vertex_Iterator a = halfMesh.insertVertex();
				TEST_ENSURE(testInvariants(halfMesh));
				HalfEdge<>::Vertex_Iterator b = halfMesh.insertVertex();
				TEST_ENSURE(testInvariants(halfMesh));

				HalfEdge<>::Edge_Iterator e = halfMesh.insertEdge(a, b);
				TEST_ENSURE(testInvariants(halfMesh));
			}
			{
				HalfEdge<> halfMesh;
				HalfEdge<>::Vertex_Iterator a = halfMesh.insertVertex();
				TEST_ENSURE(testInvariants(halfMesh));
				HalfEdge<>::Vertex_Iterator b = halfMesh.insertVertex();
				TEST_ENSURE(testInvariants(halfMesh));
				HalfEdge<>::Vertex_Iterator c = halfMesh.insertVertex();
				TEST_ENSURE(testInvariants(halfMesh));

				halfMesh.insertEdge(a, b);
				TEST_ENSURE(testInvariants(halfMesh));
				halfMesh.insertEdge(b, c);
				TEST_ENSURE(testInvariants(halfMesh));
				halfMesh.insertEdge(a, c);
				TEST_ENSURE(testInvariants(halfMesh));
			}
			{
				HalfEdge<> halfMesh;
				HalfEdge<>::Vertex_Iterator a = halfMesh.insertVertex();
				TEST_ENSURE(testInvariants(halfMesh));
				halfMesh.insertEdge(a, a);
				TEST_ENSURE(testInvariants(halfMesh));
			}
		}

		void testMultiHalfMesh()
		{
			{
				HalfEdge<> halfMesh;

				HalfEdge<>::Vertex_Iterator a = halfMesh.insertVertex();
				TEST_ENSURE(testInvariants(halfMesh));
				HalfEdge<>::Vertex_Iterator b = halfMesh.insertVertex();
				TEST_ENSURE(testInvariants(halfMesh));

				halfMesh.insertEdge(a, b);
				TEST_ENSURE(testInvariants(halfMesh));
				halfMesh.insertEdge(b, a);
				TEST_ENSURE(testInvariants(halfMesh));
				halfMesh.insertEdge(b, a);
				TEST_ENSURE(testInvariants(halfMesh));
			}
			{
				HalfEdge<> halfMesh;

				HalfEdge<>::Vertex_Iterator a = halfMesh.insertVertex();
				TEST_ENSURE(testInvariants(halfMesh));
				HalfEdge<>::Vertex_Iterator b = halfMesh.insertVertex();
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
				HalfEdge<> halfMesh;
				HalfEdge<>::Vertex_Iterator a = halfMesh.insertVertex();
				TEST_ENSURE(testInvariants(halfMesh));
				HalfEdge<>::Vertex_Iterator b = halfMesh.insertVertex();
				TEST_ENSURE(testInvariants(halfMesh));
				HalfEdge<>::Edge_Iterator edge = halfMesh.insertEdge(a, b);
				TEST_ENSURE(testInvariants(halfMesh));

				halfMesh.removeEdge(edge);
				TEST_ENSURE(testInvariants(halfMesh));
			}
			{
				PASTEL_CONSTEXPR int Vertices = 100;

				HalfEdge<> halfMesh;
				HalfEdge<>::Vertex_Iterator vertices[Vertices];
				HalfEdge<>::Edge_Iterator edges[Vertices - 1];
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
			HalfEdge<> halfMesh;
			HalfEdge<>::Vertex_Iterator a = halfMesh.insertVertex();
			TEST_ENSURE(testInvariants(halfMesh));
			HalfEdge<>::Edge_Iterator edge = halfMesh.insertEdge(a, a);
			TEST_ENSURE(testInvariants(halfMesh));

			halfMesh.removeEdge(edge);
			TEST_ENSURE(testInvariants(halfMesh));
			}
			*/

			/*
			{
			HalfEdge<> halfMesh;

			HalfEdge<>::Vertex_Iterator a = halfMesh.insertVertex();
			TEST_ENSURE(testInvariants(halfMesh));
			HalfEdge<>::Vertex_Iterator b = halfMesh.insertVertex();
			TEST_ENSURE(testInvariants(halfMesh));

			HalfEdge<>::Edge_Iterator A = halfMesh.insertEdge(a, b);
			TEST_ENSURE(testInvariants(halfMesh));
			HalfEdge<>::Edge_Iterator B = halfMesh.insertEdge(b, a);
			TEST_ENSURE(testInvariants(halfMesh));
			HalfEdge<>::Edge_Iterator C = halfMesh.insertEdge(b, a);
			TEST_ENSURE(testInvariants(halfMesh));
			HalfEdge<>::Edge_Iterator D = halfMesh.insertEdge(a, a);
			TEST_ENSURE(testInvariants(halfMesh));
			HalfEdge<>::Edge_Iterator E = halfMesh.insertEdge(b, b);
			TEST_ENSURE(testInvariants(halfMesh));
			HalfEdge<>::Edge_Iterator F = halfMesh.insertEdge(a, a);
			TEST_ENSURE(testInvariants(halfMesh));
			HalfEdge<>::Edge_Iterator G = halfMesh.insertEdge(b, b);
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

			std::vector<HalfEdge<>::Vertex_Iterator> vertices;
			std::vector<HalfEdge<>::Edge_Iterator> edges;
			HalfEdge<> halfMesh;

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
					HalfEdge<>::Edge_Iterator edge;
					while (edge.empty())
					{
						integer aIndex = randomInteger(Vertices);
						integer bIndex = randomInteger(Vertices);

						HalfEdge<>::Vertex_Iterator a = vertices[aIndex];
						HalfEdge<>::Vertex_Iterator b = vertices[bIndex];
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
					HalfEdge<>::Edge_Iterator edge = edges[i];
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
