// Description: Testing for Adjacency_Graph
// DocumentationOf: adjacency_graph.h

#include "pastelsystest.h"

#include "pastel/sys/adjacency_graph.h"

using namespace Pastel;

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

		typedef Adjacency_Graph<GraphType::Directed, integer, integer> Graph;
		typedef Graph::Vertex_Iterator Vertex;
		typedef Graph::Edge_Iterator Edge;

		virtual void run()
		{
			testAddRemove();
			testSelfLoops();
		}

		void testAddRemove()
		{
			Graph graph;

			TEST_ENSURE_OP(graph.vertices(), ==, 0);
			TEST_ENSURE_OP(graph.edges(), ==, 0);

			Vertex a = graph.addVertex(1);
			TEST_ENSURE_OP(graph.vertices(), ==, 1);
			TEST_ENSURE_OP(graph.edges(), ==, 0);
			TEST_ENSURE_OP(a->data(), ==, 1);
			TEST_ENSURE_OP(a->outgoing(), ==, 0);

			Vertex b = graph.addVertex(2);
			TEST_ENSURE_OP(graph.vertices(), ==, 2);
			TEST_ENSURE_OP(b->data(), ==, 2);
			TEST_ENSURE_OP(b->outgoing(), ==, 0);

			Vertex c = graph.addVertex(3);
			TEST_ENSURE_OP(graph.vertices(), ==, 3);
			TEST_ENSURE_OP(c->data(), ==, 3);
			TEST_ENSURE_OP(c->outgoing(), ==, 0);

			c->data() = 4;
			TEST_ENSURE_OP(c->data(), ==, 4);
		
			Edge e1 = graph.addEdge(a, b, 1);
			TEST_ENSURE_OP(graph.edges(), ==, 1);
			TEST_ENSURE_OP(e1->data(), ==, 1);
			TEST_ENSURE_OP(a->outgoing(), ==, 1);
			TEST_ENSURE_OP(b->outgoing(), ==, 0);
			TEST_ENSURE_OP(c->outgoing(), ==, 0);

			Edge e2 = graph.addEdge(b, c, 2);
			TEST_ENSURE_OP(graph.edges(), ==, 2);
			TEST_ENSURE_OP(e2->data(), ==, 2);
			TEST_ENSURE_OP(a->outgoing(), ==, 1);
			TEST_ENSURE_OP(b->outgoing(), ==, 1);
			TEST_ENSURE_OP(c->outgoing(), ==, 0);

			Edge e3 = graph.addEdge(c, a, 3);
			TEST_ENSURE_OP(graph.edges(), ==, 3);
			TEST_ENSURE_OP(e3->data(), ==, 3);
			TEST_ENSURE_OP(a->outgoing(), ==, 1);
			TEST_ENSURE_OP(b->outgoing(), ==, 1);
			TEST_ENSURE_OP(c->outgoing(), ==, 1);

			Edge e4 = graph.addEdge(b, a, 4);
			TEST_ENSURE_OP(graph.edges(), ==, 4);
			TEST_ENSURE_OP(e4->data(), ==, 4);
			TEST_ENSURE_OP(a->outgoing(), ==, 1);
			TEST_ENSURE_OP(b->outgoing(), ==, 2);
			TEST_ENSURE_OP(c->outgoing(), ==, 1);

			Edge e5 = graph.addEdge(c, b, 5);
			TEST_ENSURE_OP(graph.edges(), ==, 5);
			TEST_ENSURE_OP(e5->data(), ==, 5);
			TEST_ENSURE_OP(a->outgoing(), ==, 1);
			TEST_ENSURE_OP(b->outgoing(), ==, 2);
			TEST_ENSURE_OP(c->outgoing(), ==, 2);

			Edge e6 = graph.addEdge(a, c, 6);
			TEST_ENSURE_OP(graph.edges(), ==, 6);
			TEST_ENSURE_OP(e6->data(), ==, 6);
			TEST_ENSURE_OP(a->outgoing(), ==, 2);
			TEST_ENSURE_OP(b->outgoing(), ==, 2);
			TEST_ENSURE_OP(c->outgoing(), ==, 2);

			graph.removeEdge(e1);
			TEST_ENSURE_OP(graph.edges(), ==, 5);
			TEST_ENSURE_OP(graph.vertices(), ==, 3);
			TEST_ENSURE_OP(a->outgoing(), ==, 1);
			TEST_ENSURE_OP(b->outgoing(), ==, 2);
			TEST_ENSURE_OP(c->outgoing(), ==, 2);

			graph.removeEdge(e2);
			TEST_ENSURE_OP(graph.edges(), ==, 4);
			TEST_ENSURE_OP(graph.vertices(), ==, 3);
			TEST_ENSURE_OP(a->outgoing(), ==, 1);
			TEST_ENSURE_OP(b->outgoing(), ==, 1);
			TEST_ENSURE_OP(c->outgoing(), ==, 2);

			graph.removeEdge(e3);
			TEST_ENSURE_OP(graph.edges(), ==, 3);
			TEST_ENSURE_OP(graph.vertices(), ==, 3);
			TEST_ENSURE_OP(a->outgoing(), ==, 1);
			TEST_ENSURE_OP(b->outgoing(), ==, 1);
			TEST_ENSURE_OP(c->outgoing(), ==, 1);

			graph.removeVertex(a);
			TEST_ENSURE_OP(graph.vertices(), ==, 2);
			TEST_ENSURE_OP(graph.edges(), ==, 1);

			graph.removeVertex(b);
			TEST_ENSURE_OP(graph.vertices(), ==, 1);
			TEST_ENSURE_OP(graph.edges(), ==, 0);

			graph.removeVertex(c);
			TEST_ENSURE_OP(graph.vertices(), ==, 0);
			TEST_ENSURE_OP(graph.edges(), ==, 0);
		}

		void testSelfLoops()
		{
			Graph graph;

			Vertex a = graph.addVertex();
			TEST_ENSURE_OP(graph.vertices(), ==, 1);
			TEST_ENSURE_OP(graph.edges(), ==, 0);

			Edge e1 = graph.addEdge(a, a);
			TEST_ENSURE_OP(graph.vertices(), ==, 1);
			TEST_ENSURE_OP(graph.edges(), ==, 1);
			TEST_ENSURE_OP(a->outgoing(), ==, 1);

			Edge e2 = graph.addEdge(a, a);
			TEST_ENSURE_OP(graph.vertices(), ==, 1);
			TEST_ENSURE_OP(graph.edges(), ==, 2);
			TEST_ENSURE_OP(a->outgoing(), ==, 2);

			graph.removeEdge(e1);
			TEST_ENSURE_OP(graph.vertices(), ==, 1);
			TEST_ENSURE_OP(graph.edges(), ==, 1);
			TEST_ENSURE_OP(a->outgoing(), ==, 1);

			graph.removeEdge(e2);
			TEST_ENSURE_OP(graph.vertices(), ==, 1);
			TEST_ENSURE_OP(graph.edges(), ==, 0);
			TEST_ENSURE_OP(a->outgoing(), ==, 0);
		}

	};

	void test()
	{
		Test test;
		test.run();		
	}

	void addTests()
	{
		testRunner().add("Adjacency_Graph", test);
	}

	CallFunction run(addTests);

}
