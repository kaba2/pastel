// Description: Testing for Incidence_Graph
// DocumentationOf: incidence_graph.h

#include "pastelsystest.h"

#include "pastel/sys/incidence_graph.h"

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

		typedef Incidence_Graph<GraphType::Directed, integer, integer> Graph;
		typedef Graph::Vertex_Iterator Vertex;
		typedef Graph::Edge_Iterator Edge;

		virtual void run()
		{
			testAddRemove();
			testSelfLoops();
			testMixed();
			testPure();
			testMerge();
		}

		void testAddRemove()
		{
			Graph graph;

			TEST_ENSURE_OP(graph.vertices(), ==, 0);
			TEST_ENSURE_OP(graph.edges(), ==, 0);

			Vertex a = graph.addVertex(1);
			TEST_ENSURE_OP(graph.vertices(), ==, 1);
			TEST_ENSURE_OP(graph.edges(), ==, 0);
			TEST_ENSURE_OP(*a, ==, 1);
			TEST_ENSURE_OP(a->outgoingEdges(), ==, 0);

			Vertex b = graph.addVertex(2);
			TEST_ENSURE_OP(graph.vertices(), ==, 2);
			TEST_ENSURE_OP(*b, ==, 2);
			TEST_ENSURE_OP(b->outgoingEdges(), ==, 0);

			Vertex c = graph.addVertex(3);
			TEST_ENSURE_OP(graph.vertices(), ==, 3);
			TEST_ENSURE_OP(*c, ==, 3);
			TEST_ENSURE_OP(c->outgoingEdges(), ==, 0);

			*c = 4;
			TEST_ENSURE_OP(*c, ==, 4);
		
			Edge e1 = graph.addEdge(a, b, 1);
			TEST_ENSURE(e1->from() == a);
			TEST_ENSURE(e1->to() == b);
			TEST_ENSURE_OP(graph.edges(), ==, 1);
			TEST_ENSURE_OP(*e1, ==, 1);
			TEST_ENSURE_OP(a->outgoingEdges(), ==, 1);
			TEST_ENSURE_OP(b->outgoingEdges(), ==, 0);
			TEST_ENSURE_OP(c->outgoingEdges(), ==, 0);

			Edge e2 = graph.addEdge(b, c, 2);
			TEST_ENSURE(e2->from() == b);
			TEST_ENSURE(e2->to() == c);
			TEST_ENSURE_OP(graph.edges(), ==, 2);
			TEST_ENSURE_OP(*e2, ==, 2);
			TEST_ENSURE_OP(a->outgoingEdges(), ==, 1);
			TEST_ENSURE_OP(b->outgoingEdges(), ==, 1);
			TEST_ENSURE_OP(c->outgoingEdges(), ==, 0);

			Edge e3 = graph.addEdge(c, a, 3);
			TEST_ENSURE(e3->from() == c);
			TEST_ENSURE(e3->to() == a);
			TEST_ENSURE_OP(graph.edges(), ==, 3);
			TEST_ENSURE_OP(*e3, ==, 3);
			TEST_ENSURE_OP(a->outgoingEdges(), ==, 1);
			TEST_ENSURE_OP(b->outgoingEdges(), ==, 1);
			TEST_ENSURE_OP(c->outgoingEdges(), ==, 1);

			Edge e4 = graph.addEdge(b, a, 4);
			TEST_ENSURE(e4->from() == b);
			TEST_ENSURE(e4->to() == a);
			TEST_ENSURE_OP(graph.edges(), ==, 4);
			TEST_ENSURE_OP(*e4, ==, 4);
			TEST_ENSURE_OP(a->outgoingEdges(), ==, 1);
			TEST_ENSURE_OP(b->outgoingEdges(), ==, 2);
			TEST_ENSURE_OP(c->outgoingEdges(), ==, 1);

			Edge e5 = graph.addEdge(c, b, 5);
			TEST_ENSURE(e5->from() == c);
			TEST_ENSURE(e5->to() == b);
			TEST_ENSURE_OP(graph.edges(), ==, 5);
			TEST_ENSURE_OP(*e5, ==, 5);
			TEST_ENSURE_OP(a->outgoingEdges(), ==, 1);
			TEST_ENSURE_OP(b->outgoingEdges(), ==, 2);
			TEST_ENSURE_OP(c->outgoingEdges(), ==, 2);

			Edge e6 = graph.addEdge(a, c, 6);
			TEST_ENSURE(e6->from() == a);
			TEST_ENSURE(e6->to() == c);
			TEST_ENSURE_OP(graph.edges(), ==, 6);
			TEST_ENSURE_OP(*e6, ==, 6);
			TEST_ENSURE_OP(a->outgoingEdges(), ==, 2);
			TEST_ENSURE_OP(b->outgoingEdges(), ==, 2);
			TEST_ENSURE_OP(c->outgoingEdges(), ==, 2);

			Graph copy(graph);
			TEST_ENSURE_OP(copy.vertices(), ==, graph.vertices());
			TEST_ENSURE_OP(copy.edges(), ==, graph.edges());

			graph.removeEdge(e1);
			TEST_ENSURE_OP(graph.edges(), ==, 5);
			TEST_ENSURE_OP(graph.vertices(), ==, 3);
			TEST_ENSURE_OP(a->outgoingEdges(), ==, 1);
			TEST_ENSURE_OP(b->outgoingEdges(), ==, 2);
			TEST_ENSURE_OP(c->outgoingEdges(), ==, 2);

			graph.removeEdge(e2);
			TEST_ENSURE_OP(graph.edges(), ==, 4);
			TEST_ENSURE_OP(graph.vertices(), ==, 3);
			TEST_ENSURE_OP(a->outgoingEdges(), ==, 1);
			TEST_ENSURE_OP(b->outgoingEdges(), ==, 1);
			TEST_ENSURE_OP(c->outgoingEdges(), ==, 2);

			graph.removeEdge(e3);
			TEST_ENSURE_OP(graph.edges(), ==, 3);
			TEST_ENSURE_OP(graph.vertices(), ==, 3);
			TEST_ENSURE_OP(a->outgoingEdges(), ==, 1);
			TEST_ENSURE_OP(b->outgoingEdges(), ==, 1);
			TEST_ENSURE_OP(c->outgoingEdges(), ==, 1);

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
			TEST_ENSURE(e1->from() == a);
			TEST_ENSURE(e1->to() == a);
			TEST_ENSURE_OP(graph.vertices(), ==, 1);
			TEST_ENSURE_OP(graph.edges(), ==, 1);
			TEST_ENSURE_OP(a->outgoingEdges(), ==, 1);

			Edge e2 = graph.addEdge(a, a);
			TEST_ENSURE(e2->from() == a);
			TEST_ENSURE(e2->to() == a);
			TEST_ENSURE_OP(graph.vertices(), ==, 1);
			TEST_ENSURE_OP(graph.edges(), ==, 2);
			TEST_ENSURE_OP(a->outgoingEdges(), ==, 2);

			graph.removeEdge(e1);
			TEST_ENSURE_OP(graph.vertices(), ==, 1);
			TEST_ENSURE_OP(graph.edges(), ==, 1);
			TEST_ENSURE_OP(a->outgoingEdges(), ==, 1);

			graph.removeEdge(e2);
			TEST_ENSURE_OP(graph.vertices(), ==, 1);
			TEST_ENSURE_OP(graph.edges(), ==, 0);
			TEST_ENSURE_OP(a->outgoingEdges(), ==, 0);
		}

		void testMixed()
		{
			typedef Incidence_Graph<GraphType::Mixed, integer, integer> Graph;
			typedef Graph::Vertex_Iterator Vertex;
			typedef Graph::Edge_Iterator Edge;

			Graph graph;

			Vertex a = graph.addVertex();
			Vertex b = graph.addVertex();
			Vertex c = graph.addVertex();

			Edge e1 = graph.addEdge(a, b);
			TEST_ENSURE(!e1->directed());
			TEST_ENSURE(e1->from() == a);
			TEST_ENSURE(e1->to() == b);
			TEST_ENSURE_OP(a->incomingEdges(), ==, 0);
			TEST_ENSURE_OP(a->outgoingEdges(), ==, 0);
			TEST_ENSURE_OP(a->undirectedEdges(), ==, 1);
			TEST_ENSURE_OP(b->incomingEdges(), ==, 0);
			TEST_ENSURE_OP(b->outgoingEdges(), ==, 0);
			TEST_ENSURE_OP(b->undirectedEdges(), ==, 1);
			
			graph.reverse(e1);
			TEST_ENSURE(!e1->directed());
			TEST_ENSURE(e1->from() == b);
			TEST_ENSURE(e1->to() == a);
			TEST_ENSURE_OP(a->incomingEdges(), ==, 0);
			TEST_ENSURE_OP(a->outgoingEdges(), ==, 0);
			TEST_ENSURE_OP(a->undirectedEdges(), ==, 1);
			TEST_ENSURE_OP(b->incomingEdges(), ==, 0);
			TEST_ENSURE_OP(b->outgoingEdges(), ==, 0);
			TEST_ENSURE_OP(b->undirectedEdges(), ==, 1);

			graph.setDirected(e1, true);
			TEST_ENSURE(e1->directed());
			TEST_ENSURE(e1->from() == b);
			TEST_ENSURE(e1->to() == a);
			TEST_ENSURE_OP(a->incomingEdges(), ==, 1);
			TEST_ENSURE_OP(a->outgoingEdges(), ==, 0);
			TEST_ENSURE_OP(a->undirectedEdges(), ==, 0);
			TEST_ENSURE_OP(b->incomingEdges(), ==, 0);
			TEST_ENSURE_OP(b->outgoingEdges(), ==, 1);
			TEST_ENSURE_OP(b->undirectedEdges(), ==, 0);

			graph.reverse(e1);
			TEST_ENSURE(e1->directed());
			TEST_ENSURE(e1->from() == a);
			TEST_ENSURE(e1->to() == b);
			TEST_ENSURE_OP(a->incomingEdges(), ==, 0);
			TEST_ENSURE_OP(a->outgoingEdges(), ==, 1);
			TEST_ENSURE_OP(a->undirectedEdges(), ==, 0);
			TEST_ENSURE_OP(b->incomingEdges(), ==, 1);
			TEST_ENSURE_OP(b->outgoingEdges(), ==, 0);
			TEST_ENSURE_OP(b->undirectedEdges(), ==, 0);
		}

		void testPure()
		{
			typedef Incidence_Graph<GraphType::Directed> Graph;
			typedef Graph::Vertex_Iterator Vertex;
			typedef Graph::Edge_Iterator Edge;

			Graph graph;
			Vertex a = graph.addVertex();
			Vertex b = graph.addVertex();

			TEST_ENSURE(a->cIncomingBegin() == a->cIncomingEnd());
			TEST_ENSURE(b->cOutgoingBegin() == b->cOutgoingEnd());

			Edge A = graph.addEdge(a, b);
			
			TEST_ENSURE(a->cIncomingBegin() == a->cIncomingEnd());
			TEST_ENSURE_OP(a->incomingEdges(), ==, 0);
			TEST_ENSURE_OP(a->outgoingEdges(), ==, 1);
			TEST_ENSURE(a->cOutgoingBegin()->vertex() == b);

			TEST_ENSURE(b->cOutgoingBegin() == b->cOutgoingEnd());
			TEST_ENSURE_OP(b->incomingEdges(), ==, 1);
			TEST_ENSURE_OP(b->outgoingEdges(), ==, 0);
			TEST_ENSURE(b->cIncomingBegin()->vertex() == a);

			graph.removeEdge(A);

			TEST_ENSURE(a->cIncomingBegin() == a->cIncomingEnd());
			TEST_ENSURE_OP(a->incomingEdges(), ==, 0);
			TEST_ENSURE_OP(a->outgoingEdges(), ==, 0);

			TEST_ENSURE(b->cOutgoingBegin() == b->cOutgoingEnd());
			TEST_ENSURE_OP(b->incomingEdges(), ==, 0);
			TEST_ENSURE_OP(b->outgoingEdges(), ==, 0);
		}

		void testMerge()
		{
			Graph aGraph;

			Vertex a = aGraph.addVertex();
			Vertex b = aGraph.addVertex();
			Vertex c = aGraph.addVertex();
			{
				TEST_ENSURE_OP(aGraph.vertices(), ==, 3);
			}

			aGraph.addEdge(a, b);
			aGraph.addEdge(b, c);
			aGraph.addEdge(c, a);
			{
				TEST_ENSURE_OP(aGraph.edges(), ==, 3);
			}

			Graph bGraph;
			Vertex d = bGraph.addVertex();
			Vertex e = bGraph.addVertex();
			Vertex f = bGraph.addVertex();
			{
				TEST_ENSURE_OP(bGraph.vertices(), ==, 3);
			}

			bGraph.addEdge(d, e);
			bGraph.addEdge(e, f);
			bGraph.addEdge(f, d);
			{
				TEST_ENSURE_OP(bGraph.edges(), ==, 3);
			}
			
			aGraph.merge(bGraph);
			{
				TEST_ENSURE_OP(aGraph.vertices(), ==, 6);
				TEST_ENSURE_OP(aGraph.edges(), ==, 6);
				TEST_ENSURE_OP(bGraph.vertices(), ==, 0);
				TEST_ENSURE_OP(bGraph.edges(), ==, 0);
			}
		}

	};

	void test()
	{
		Test test;
		test.run();		
	}

	void addTests()
	{
		testRunner().add("Incidence_Graph", test);
	}

	CallFunction run(addTests);

}
