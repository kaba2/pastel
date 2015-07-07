// Description: More testing for HalfMesh
// DocumentationOf: halfmesh.h

#include "test/test_init.h"

#include "pastel/geometry/halfmesh/halfmesh.h"
#include "pastel/sys/random.h"

namespace
{

	class Test
	{
	public:
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
			for (int i = 0;i < 10000;++i)
			{
				halfMesh.insertVertex();
				REQUIRE(testInvariants(halfMesh));
			}

			{
				HalfEdge<> otherMesh(halfMesh);
				REQUIRE(testInvariants(otherMesh));
			}
		}

		void testSimpleEdge()
		{
			{
				HalfEdge<> halfMesh;
				HalfEdge<>::Vertex_Iterator a = halfMesh.insertVertex();
				REQUIRE(testInvariants(halfMesh));
				HalfEdge<>::Vertex_Iterator b = halfMesh.insertVertex();
				REQUIRE(testInvariants(halfMesh));

				HalfEdge<>::Edge_Iterator e = halfMesh.insertEdge(a, b);
				unused(e);
				REQUIRE(testInvariants(halfMesh));
			}
			{
				HalfEdge<> halfMesh;
				HalfEdge<>::Vertex_Iterator a = halfMesh.insertVertex();
				REQUIRE(testInvariants(halfMesh));
				HalfEdge<>::Vertex_Iterator b = halfMesh.insertVertex();
				REQUIRE(testInvariants(halfMesh));
				HalfEdge<>::Vertex_Iterator c = halfMesh.insertVertex();
				REQUIRE(testInvariants(halfMesh));

				halfMesh.insertEdge(a, b);
				REQUIRE(testInvariants(halfMesh));
				halfMesh.insertEdge(b, c);
				REQUIRE(testInvariants(halfMesh));
				halfMesh.insertEdge(a, c);
				REQUIRE(testInvariants(halfMesh));
			}
			{
				HalfEdge<> halfMesh;
				HalfEdge<>::Vertex_Iterator a = halfMesh.insertVertex();
				REQUIRE(testInvariants(halfMesh));
				halfMesh.insertEdge(a, a);
				REQUIRE(testInvariants(halfMesh));
			}
		}

		void testMultiHalfMesh()
		{
			{
				HalfEdge<> halfMesh;

				HalfEdge<>::Vertex_Iterator a = halfMesh.insertVertex();
				REQUIRE(testInvariants(halfMesh));
				HalfEdge<>::Vertex_Iterator b = halfMesh.insertVertex();
				REQUIRE(testInvariants(halfMesh));

				halfMesh.insertEdge(a, b);
				REQUIRE(testInvariants(halfMesh));
				halfMesh.insertEdge(b, a);
				REQUIRE(testInvariants(halfMesh));
				halfMesh.insertEdge(b, a);
				REQUIRE(testInvariants(halfMesh));
			}
			{
				HalfEdge<> halfMesh;

				HalfEdge<>::Vertex_Iterator a = halfMesh.insertVertex();
				REQUIRE(testInvariants(halfMesh));
				HalfEdge<>::Vertex_Iterator b = halfMesh.insertVertex();
				REQUIRE(testInvariants(halfMesh));

				halfMesh.insertEdge(a, b);
				REQUIRE(testInvariants(halfMesh));
				halfMesh.insertEdge(b, a);
				REQUIRE(testInvariants(halfMesh));
				halfMesh.insertEdge(b, a);
				REQUIRE(testInvariants(halfMesh));

				halfMesh.insertEdge(a, a);
				REQUIRE(testInvariants(halfMesh));
				halfMesh.insertEdge(b, b);
				REQUIRE(testInvariants(halfMesh));
				halfMesh.insertEdge(a, a);
				REQUIRE(testInvariants(halfMesh));
				halfMesh.insertEdge(b, b);
				REQUIRE(testInvariants(halfMesh));
			}
		}

		void testSimpleRemoval()
		{
			{
				HalfEdge<> halfMesh;
				HalfEdge<>::Vertex_Iterator a = halfMesh.insertVertex();
				REQUIRE(testInvariants(halfMesh));
				HalfEdge<>::Vertex_Iterator b = halfMesh.insertVertex();
				REQUIRE(testInvariants(halfMesh));
				HalfEdge<>::Edge_Iterator edge = halfMesh.insertEdge(a, b);
				REQUIRE(testInvariants(halfMesh));

				halfMesh.removeEdge(edge);
				REQUIRE(testInvariants(halfMesh));
			}
			{
				static constexpr int Vertices = 100;

				HalfEdge<> halfMesh;
				HalfEdge<>::Vertex_Iterator vertices[Vertices];
				HalfEdge<>::Edge_Iterator edges[Vertices - 1];
				for (int i = 0;i < Vertices;++i)
				{
					vertices[i] = halfMesh.insertVertex();
					REQUIRE(testInvariants(halfMesh));
				}

				for (int i = 1;i < Vertices;++i)
				{
					edges[i - 1] = halfMesh.insertEdge(vertices[0], vertices[i]);
					REQUIRE(testInvariants(halfMesh));
				}

				for (int i = 0;i < Vertices - 1;++i)
				{
					halfMesh.removeEdge(edges[i]);
					REQUIRE(testInvariants(halfMesh));
				}
			}
			/*
			{
			HalfEdge<> halfMesh;
			HalfEdge<>::Vertex_Iterator a = halfMesh.insertVertex();
			REQUIRE(testInvariants(halfMesh));
			HalfEdge<>::Edge_Iterator edge = halfMesh.insertEdge(a, a);
			REQUIRE(testInvariants(halfMesh));

			halfMesh.removeEdge(edge);
			REQUIRE(testInvariants(halfMesh));
			}
			*/

			/*
			{
			HalfEdge<> halfMesh;

			HalfEdge<>::Vertex_Iterator a = halfMesh.insertVertex();
			REQUIRE(testInvariants(halfMesh));
			HalfEdge<>::Vertex_Iterator b = halfMesh.insertVertex();
			REQUIRE(testInvariants(halfMesh));

			HalfEdge<>::Edge_Iterator A = halfMesh.insertEdge(a, b);
			REQUIRE(testInvariants(halfMesh));
			HalfEdge<>::Edge_Iterator B = halfMesh.insertEdge(b, a);
			REQUIRE(testInvariants(halfMesh));
			HalfEdge<>::Edge_Iterator C = halfMesh.insertEdge(b, a);
			REQUIRE(testInvariants(halfMesh));
			HalfEdge<>::Edge_Iterator D = halfMesh.insertEdge(a, a);
			REQUIRE(testInvariants(halfMesh));
			HalfEdge<>::Edge_Iterator E = halfMesh.insertEdge(b, b);
			REQUIRE(testInvariants(halfMesh));
			HalfEdge<>::Edge_Iterator F = halfMesh.insertEdge(a, a);
			REQUIRE(testInvariants(halfMesh));
			HalfEdge<>::Edge_Iterator G = halfMesh.insertEdge(b, b);
			REQUIRE(testInvariants(halfMesh));

			halfMesh.removeEdge(D);
			REQUIRE(testInvariants(halfMesh));
			halfMesh.removeEdge(E);
			REQUIRE(testInvariants(halfMesh));
			halfMesh.removeEdge(A);
			REQUIRE(testInvariants(halfMesh));
			halfMesh.removeEdge(B);
			REQUIRE(testInvariants(halfMesh));
			halfMesh.removeEdge(F);
			REQUIRE(testInvariants(halfMesh));
			halfMesh.removeEdge(C);
			REQUIRE(testInvariants(halfMesh));
			halfMesh.removeEdge(G);
			REQUIRE(testInvariants(halfMesh));
			}
			*/
		}

		void testComplexHalfMesh()
		{
			static constexpr int Vertices = 1000;
			static constexpr int Edges = 1000;

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
			REQUIRE(testInvariants(halfMesh));
			REQUIRE(vertices.size() == Vertices);

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
					REQUIRE(testInvariants(halfMesh));

					edges.push_back(edge);
				}
			}
			REQUIRE(testInvariants(halfMesh));
			REQUIRE(edges.size() == Edges);

			{
				for (integer i = 0;i < Edges;++i)
				{
					HalfEdge<>::Edge_Iterator edge = edges[i];
					REQUIRE(testInvariants(halfMesh));
					halfMesh.removeEdge(edge);
				}
			}
			REQUIRE(testInvariants(halfMesh));
		}
	};

	TEST_CASE("MoreHalfMesh", "[MoreHalfMesh]")
	{
	}

}
