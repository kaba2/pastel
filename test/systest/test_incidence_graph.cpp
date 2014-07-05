// Description: Testing for incidence graph
// DocumentationOf: incidence_graph.h

#include "test_pastelsys.h"

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

		using Graph = Directed_Graph<integer, integer>;
		using Vertex = Graph::Vertex_Iterator;
		using Edge = Graph::Edge_Iterator;

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

			Vertex a = graph.insertVertex(1);
			TEST_ENSURE_OP(graph.vertices(), ==, 1);
			TEST_ENSURE_OP(graph.edges(), ==, 0);
			TEST_ENSURE_OP(*a, ==, 1);
			TEST_ENSURE_OP(a->outgoingEdges(), ==, 0);

			Vertex b = graph.insertVertex(2);
			TEST_ENSURE_OP(graph.vertices(), ==, 2);
			TEST_ENSURE_OP(*b, ==, 2);
			TEST_ENSURE_OP(b->outgoingEdges(), ==, 0);

			Vertex c = graph.insertVertex(3);
			TEST_ENSURE_OP(graph.vertices(), ==, 3);
			TEST_ENSURE_OP(*c, ==, 3);
			TEST_ENSURE_OP(c->outgoingEdges(), ==, 0);

			*c = 4;
			TEST_ENSURE_OP(*c, ==, 4);
		
			Edge e1 = graph.insertEdge(a, b, 1);
			TEST_ENSURE(e1->from() == a);
			TEST_ENSURE(e1->to() == b);
			TEST_ENSURE_OP(graph.edges(), ==, 1);
			TEST_ENSURE_OP(*e1, ==, 1);
			TEST_ENSURE_OP(a->outgoingEdges(), ==, 1);
			TEST_ENSURE_OP(b->outgoingEdges(), ==, 0);
			TEST_ENSURE_OP(c->outgoingEdges(), ==, 0);

			Edge e2 = graph.insertEdge(b, c, 2);
			TEST_ENSURE(e2->from() == b);
			TEST_ENSURE(e2->to() == c);
			TEST_ENSURE_OP(graph.edges(), ==, 2);
			TEST_ENSURE_OP(*e2, ==, 2);
			TEST_ENSURE_OP(a->outgoingEdges(), ==, 1);
			TEST_ENSURE_OP(b->outgoingEdges(), ==, 1);
			TEST_ENSURE_OP(c->outgoingEdges(), ==, 0);

			Edge e3 = graph.insertEdge(c, a, 3);
			TEST_ENSURE(e3->from() == c);
			TEST_ENSURE(e3->to() == a);
			TEST_ENSURE_OP(graph.edges(), ==, 3);
			TEST_ENSURE_OP(*e3, ==, 3);
			TEST_ENSURE_OP(a->outgoingEdges(), ==, 1);
			TEST_ENSURE_OP(b->outgoingEdges(), ==, 1);
			TEST_ENSURE_OP(c->outgoingEdges(), ==, 1);

			Edge e4 = graph.insertEdge(b, a, 4);
			TEST_ENSURE(e4->from() == b);
			TEST_ENSURE(e4->to() == a);
			TEST_ENSURE_OP(graph.edges(), ==, 4);
			TEST_ENSURE_OP(*e4, ==, 4);
			TEST_ENSURE_OP(a->outgoingEdges(), ==, 1);
			TEST_ENSURE_OP(b->outgoingEdges(), ==, 2);
			TEST_ENSURE_OP(c->outgoingEdges(), ==, 1);

			Edge e5 = graph.insertEdge(c, b, 5);
			TEST_ENSURE(e5->from() == c);
			TEST_ENSURE(e5->to() == b);
			TEST_ENSURE_OP(graph.edges(), ==, 5);
			TEST_ENSURE_OP(*e5, ==, 5);
			TEST_ENSURE_OP(a->outgoingEdges(), ==, 1);
			TEST_ENSURE_OP(b->outgoingEdges(), ==, 2);
			TEST_ENSURE_OP(c->outgoingEdges(), ==, 2);

			Edge e6 = graph.insertEdge(a, c, 6);
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

			Vertex a = graph.insertVertex();
			TEST_ENSURE_OP(graph.vertices(), ==, 1);
			TEST_ENSURE_OP(graph.edges(), ==, 0);

			Edge e1 = graph.insertEdge(a, a);
			TEST_ENSURE(e1->from() == a);
			TEST_ENSURE(e1->to() == a);
			TEST_ENSURE_OP(graph.vertices(), ==, 1);
			TEST_ENSURE_OP(graph.edges(), ==, 1);
			TEST_ENSURE_OP(a->outgoingEdges(), ==, 1);

			Edge e2 = graph.insertEdge(a, a);
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
			using Graph = Mixed_Graph<integer, integer>;
			using Vertex = Graph::Vertex_Iterator;
			using Edge = Graph::Edge_Iterator;

			Graph graph;

			Vertex a = graph.insertVertex();
			Vertex b = graph.insertVertex();
			Vertex c = graph.insertVertex();

			Edge e1 = graph.insertEdge(a, b);
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
			using Graph = Directed_Graph<>;
			using Vertex = Graph::Vertex_Iterator;
			using Edge = Graph::Edge_Iterator;

			Graph graph;
			Vertex a = graph.insertVertex();
			Vertex b = graph.insertVertex();

			TEST_ENSURE(a->cIncomingBegin() == a->cIncomingEnd());
			TEST_ENSURE(b->cOutgoingBegin() == b->cOutgoingEnd());

			Edge A = graph.insertEdge(a, b);
			
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

			Vertex a = aGraph.insertVertex();
			Vertex b = aGraph.insertVertex();
			Vertex c = aGraph.insertVertex();
			{
				TEST_ENSURE_OP(aGraph.vertices(), ==, 3);
			}

			aGraph.insertEdge(a, b);
			aGraph.insertEdge(b, c);
			aGraph.insertEdge(c, a);
			{
				TEST_ENSURE_OP(aGraph.edges(), ==, 3);
			}

			Graph bGraph;
			Vertex d = bGraph.insertVertex();
			Vertex e = bGraph.insertVertex();
			Vertex f = bGraph.insertVertex();
			{
				TEST_ENSURE_OP(bGraph.vertices(), ==, 3);
			}

			bGraph.insertEdge(d, e);
			bGraph.insertEdge(e, f);
			bGraph.insertEdge(f, d);
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
		testRunner().add("IncidenceGraph", test);
	}

	CallFunction run(addTests);

}
