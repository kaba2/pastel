#include "pastelgeometrytest.h"

#include "pastel/geometry/halfmesh_tools.h"
#include "pastel/geometry/simplehalfmesh.h"
#include "pastel/sys/random.h"

using namespace Pastel;

namespace
{

	void printHalfMesh(const PureHalfMesh& halfMesh)
	{
	}

	void testTrivial()
	{
		log() << "- trivial" << logNewLine;

		PureHalfMesh halfMesh;
		halfMesh.addVertex();
		for (int i = 0;i < 10000;i++)
		{
			halfMesh.addVertex();
			checkInvariants(halfMesh);
		}

		{
			PureHalfMesh otherMesh(halfMesh);
			checkInvariants(otherMesh);
		}
	}

	void testSimpleEdge()
	{
		log() << "- simple edge" << logNewLine;

		{
			PureHalfMesh halfMesh;
			PureHalfMesh::Vertex a = halfMesh.addVertex();
			checkInvariants(halfMesh);
			PureHalfMesh::Vertex b = halfMesh.addVertex();
			checkInvariants(halfMesh);

			PureHalfMesh::Edge e = halfMesh.addEdge(a, b);
			checkInvariants(halfMesh);
		}
		{
			PureHalfMesh halfMesh;
			PureHalfMesh::Vertex a = halfMesh.addVertex();
			checkInvariants(halfMesh);
			PureHalfMesh::Vertex b = halfMesh.addVertex();
			checkInvariants(halfMesh);
			PureHalfMesh::Vertex c = halfMesh.addVertex();
			checkInvariants(halfMesh);

			halfMesh.addEdge(a, b);
			checkInvariants(halfMesh);
			halfMesh.addEdge(b, c);
			checkInvariants(halfMesh);
			halfMesh.addEdge(a, c);
			checkInvariants(halfMesh);
		}
		{
			PureHalfMesh halfMesh;
			PureHalfMesh::Vertex a = halfMesh.addVertex();
			checkInvariants(halfMesh);
			halfMesh.addEdge(a, a);
			checkInvariants(halfMesh);
		}
	}

	void testMultiHalfMesh()
	{
		log() << "- multi halfMesh" << logNewLine;

		{
			PureHalfMesh halfMesh;

			PureHalfMesh::Vertex a = halfMesh.addVertex();
			checkInvariants(halfMesh);
			PureHalfMesh::Vertex b = halfMesh.addVertex();
			checkInvariants(halfMesh);

			halfMesh.addEdge(a, b);
			checkInvariants(halfMesh);
			halfMesh.addEdge(b, a);
			checkInvariants(halfMesh);
			halfMesh.addEdge(b, a);
			checkInvariants(halfMesh);
		}
		{
			PureHalfMesh halfMesh;

			PureHalfMesh::Vertex a = halfMesh.addVertex();
			checkInvariants(halfMesh);
			PureHalfMesh::Vertex b = halfMesh.addVertex();
			checkInvariants(halfMesh);

			halfMesh.addEdge(a, b);
			checkInvariants(halfMesh);
			halfMesh.addEdge(b, a);
			checkInvariants(halfMesh);
			halfMesh.addEdge(b, a);
			checkInvariants(halfMesh);

			halfMesh.addEdge(a, a);
			checkInvariants(halfMesh);
			halfMesh.addEdge(b, b);
			checkInvariants(halfMesh);
			halfMesh.addEdge(a, a);
			checkInvariants(halfMesh);
			halfMesh.addEdge(b, b);
			checkInvariants(halfMesh);
		}
	}

	void testSimpleRemoval()
	{
		log() << "- simple removal" << logNewLine;

		{
			PureHalfMesh halfMesh;
			PureHalfMesh::Vertex a = halfMesh.addVertex();
			checkInvariants(halfMesh);
			PureHalfMesh::Vertex b = halfMesh.addVertex();
			checkInvariants(halfMesh);
			PureHalfMesh::Edge edge = halfMesh.addEdge(a, b);
			checkInvariants(halfMesh);

			halfMesh.removeEdge(edge);
			checkInvariants(halfMesh);
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
				checkInvariants(halfMesh);
			}

			for (int i = 1;i < Vertices;++i)
			{
				edges[i - 1] = halfMesh.addEdge(vertices[0], vertices[i]);
				checkInvariants(halfMesh);
			}

			for (int i = 0;i < Vertices - 1;++i)
			{
				halfMesh.removeEdge(edges[i]);
				checkInvariants(halfMesh);
			}
		}
		/*
		{
		PureHalfMesh halfMesh;
		PureHalfMesh::Vertex a = halfMesh.addVertex();
		checkInvariants(halfMesh);
		PureHalfMesh::Edge edge = halfMesh.addEdge(a, a);
		checkInvariants(halfMesh);

		halfMesh.removeEdge(edge);
		checkInvariants(halfMesh);
		}
		*/

		/*
		{
		PureHalfMesh halfMesh;

		PureHalfMesh::Vertex a = halfMesh.addVertex();
		checkInvariants(halfMesh);
		PureHalfMesh::Vertex b = halfMesh.addVertex();
		checkInvariants(halfMesh);

		PureHalfMesh::Edge A = halfMesh.addEdge(a, b);
		checkInvariants(halfMesh);
		PureHalfMesh::Edge B = halfMesh.addEdge(b, a);
		checkInvariants(halfMesh);
		PureHalfMesh::Edge C = halfMesh.addEdge(b, a);
		checkInvariants(halfMesh);
		PureHalfMesh::Edge D = halfMesh.addEdge(a, a);
		checkInvariants(halfMesh);
		PureHalfMesh::Edge E = halfMesh.addEdge(b, b);
		checkInvariants(halfMesh);
		PureHalfMesh::Edge F = halfMesh.addEdge(a, a);
		checkInvariants(halfMesh);
		PureHalfMesh::Edge G = halfMesh.addEdge(b, b);
		checkInvariants(halfMesh);

		halfMesh.removeEdge(D);
		checkInvariants(halfMesh);
		halfMesh.removeEdge(E);
		checkInvariants(halfMesh);
		halfMesh.removeEdge(A);
		checkInvariants(halfMesh);
		halfMesh.removeEdge(B);
		checkInvariants(halfMesh);
		halfMesh.removeEdge(F);
		checkInvariants(halfMesh);
		halfMesh.removeEdge(C);
		checkInvariants(halfMesh);
		halfMesh.removeEdge(G);
		checkInvariants(halfMesh);
		}
		*/
	}

	void testComplexHalfMesh()
	{
		log() << "- complex halfMesh" << logNewLine;

		enum
		{
			Vertices = 100000,
			Edges = 100000
		};

		std::vector<PureHalfMesh::Vertex> vertices;
		std::vector<PureHalfMesh::Edge> edges;
		PureHalfMesh halfMesh;

		{
			int previous = 0;
			for (int i = 0;i < Vertices;++i)
			{
				vertices.push_back(halfMesh.addVertex());
				if (i / 1000 != previous)
				{
					log() << i << ", ";
					previous = i / 1000;
				}
			}
		}
		checkInvariants(halfMesh);
		log() << "vertices added" << logNewLine;

		{
			PureHalfMesh otherMesh(halfMesh);
			checkInvariants(otherMesh);
		}

		{
			for (int i = 0;i < Edges;++i)
			{
				PureHalfMesh::Edge edge;
				while (edge.empty())
				{
					int aIndex = randomInteger() % Vertices;
					int bIndex = randomInteger() % Vertices;

					PureHalfMesh::Vertex a = vertices[aIndex];
					PureHalfMesh::Vertex b = vertices[bIndex];

					edge = halfMesh.addEdge(a, b);
				}

				edges.push_back(edge);
			}
		}
		checkInvariants(halfMesh);
		log() << "edges added" << logNewLine;

		{
			PureHalfMesh otherMesh(halfMesh);
			checkInvariants(otherMesh);
		}

		{
			for (int i = 0;i < Edges;++i)
			{
				int index = randomInteger() % Edges;
				PureHalfMesh::Edge edge = edges[index];
				edges[index] = PureHalfMesh::Edge();

				if (!edge.empty())
				{
					halfMesh.removeEdge(edge);
				}
			}
		}
		checkInvariants(halfMesh);

		{
			PureHalfMesh otherMesh(halfMesh);
			checkInvariants(otherMesh);
		}
	}

	void testBegin()
	{
		testTrivial();
		testSimpleEdge();
		testMultiHalfMesh();
		testSimpleRemoval();
		testComplexHalfMesh();
	}

	void testAdd()
	{
		geometryTestList().add("HalfMesh2", testBegin);
	}

	CallFunction run(testAdd);

}
