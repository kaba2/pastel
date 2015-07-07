// Description: Testing for incidence graph
// DocumentationOf: incidence_graph.h

#include "test/test_init.h"

#include "pastel/sys/incidence_graph.h"

namespace
{

	class Test
	{
	public:
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

			REQUIRE(graph.vertices() == 0);
			REQUIRE(graph.edges() == 0);

			Vertex a = graph.insertVertex(1);
			REQUIRE(graph.vertices() == 1);
			REQUIRE(graph.edges() == 0);
			REQUIRE(*a == 1);
			REQUIRE(a->outgoingEdges() == 0);

			Vertex b = graph.insertVertex(2);
			REQUIRE(graph.vertices() == 2);
			REQUIRE(*b == 2);
			REQUIRE(b->outgoingEdges() == 0);

			Vertex c = graph.insertVertex(3);
			REQUIRE(graph.vertices() == 3);
			REQUIRE(*c == 3);
			REQUIRE(c->outgoingEdges() == 0);

			*c = 4;
			REQUIRE(*c == 4);

			Edge e1 = graph.insertEdge(a, b, 1);
			REQUIRE(e1->from() == a);
			REQUIRE(e1->to() == b);
			REQUIRE(graph.edges() == 1);
			REQUIRE(*e1 == 1);
			REQUIRE(a->outgoingEdges() == 1);
			REQUIRE(b->outgoingEdges() == 0);
			REQUIRE(c->outgoingEdges() == 0);

			Edge e2 = graph.insertEdge(b, c, 2);
			REQUIRE(e2->from() == b);
			REQUIRE(e2->to() == c);
			REQUIRE(graph.edges() == 2);
			REQUIRE(*e2 == 2);
			REQUIRE(a->outgoingEdges() == 1);
			REQUIRE(b->outgoingEdges() == 1);
			REQUIRE(c->outgoingEdges() == 0);

			Edge e3 = graph.insertEdge(c, a, 3);
			REQUIRE(e3->from() == c);
			REQUIRE(e3->to() == a);
			REQUIRE(graph.edges() == 3);
			REQUIRE(*e3 == 3);
			REQUIRE(a->outgoingEdges() == 1);
			REQUIRE(b->outgoingEdges() == 1);
			REQUIRE(c->outgoingEdges() == 1);

			Edge e4 = graph.insertEdge(b, a, 4);
			REQUIRE(e4->from() == b);
			REQUIRE(e4->to() == a);
			REQUIRE(graph.edges() == 4);
			REQUIRE(*e4 == 4);
			REQUIRE(a->outgoingEdges() == 1);
			REQUIRE(b->outgoingEdges() == 2);
			REQUIRE(c->outgoingEdges() == 1);

			Edge e5 = graph.insertEdge(c, b, 5);
			REQUIRE(e5->from() == c);
			REQUIRE(e5->to() == b);
			REQUIRE(graph.edges() == 5);
			REQUIRE(*e5 == 5);
			REQUIRE(a->outgoingEdges() == 1);
			REQUIRE(b->outgoingEdges() == 2);
			REQUIRE(c->outgoingEdges() == 2);

			Edge e6 = graph.insertEdge(a, c, 6);
			REQUIRE(e6->from() == a);
			REQUIRE(e6->to() == c);
			REQUIRE(graph.edges() == 6);
			REQUIRE(*e6 == 6);
			REQUIRE(a->outgoingEdges() == 2);
			REQUIRE(b->outgoingEdges() == 2);
			REQUIRE(c->outgoingEdges() == 2);

			Graph copy(graph);
			REQUIRE(copy.vertices() == graph.vertices());
			REQUIRE(copy.edges() == graph.edges());

			graph.removeEdge(e1);
			REQUIRE(graph.edges() == 5);
			REQUIRE(graph.vertices() == 3);
			REQUIRE(a->outgoingEdges() == 1);
			REQUIRE(b->outgoingEdges() == 2);
			REQUIRE(c->outgoingEdges() == 2);

			graph.removeEdge(e2);
			REQUIRE(graph.edges() == 4);
			REQUIRE(graph.vertices() == 3);
			REQUIRE(a->outgoingEdges() == 1);
			REQUIRE(b->outgoingEdges() == 1);
			REQUIRE(c->outgoingEdges() == 2);

			graph.removeEdge(e3);
			REQUIRE(graph.edges() == 3);
			REQUIRE(graph.vertices() == 3);
			REQUIRE(a->outgoingEdges() == 1);
			REQUIRE(b->outgoingEdges() == 1);
			REQUIRE(c->outgoingEdges() == 1);

			graph.removeVertex(a);
			REQUIRE(graph.vertices() == 2);
			REQUIRE(graph.edges() == 1);

			graph.removeVertex(b);
			REQUIRE(graph.vertices() == 1);
			REQUIRE(graph.edges() == 0);

			graph.removeVertex(c);
			REQUIRE(graph.vertices() == 0);
			REQUIRE(graph.edges() == 0);
		}

		void testSelfLoops()
		{
			Graph graph;

			Vertex a = graph.insertVertex();
			REQUIRE(graph.vertices() == 1);
			REQUIRE(graph.edges() == 0);

			Edge e1 = graph.insertEdge(a, a);
			REQUIRE(e1->from() == a);
			REQUIRE(e1->to() == a);
			REQUIRE(graph.vertices() == 1);
			REQUIRE(graph.edges() == 1);
			REQUIRE(a->outgoingEdges() == 1);

			Edge e2 = graph.insertEdge(a, a);
			REQUIRE(e2->from() == a);
			REQUIRE(e2->to() == a);
			REQUIRE(graph.vertices() == 1);
			REQUIRE(graph.edges() == 2);
			REQUIRE(a->outgoingEdges() == 2);

			graph.removeEdge(e1);
			REQUIRE(graph.vertices() == 1);
			REQUIRE(graph.edges() == 1);
			REQUIRE(a->outgoingEdges() == 1);

			graph.removeEdge(e2);
			REQUIRE(graph.vertices() == 1);
			REQUIRE(graph.edges() == 0);
			REQUIRE(a->outgoingEdges() == 0);
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
			unused(c);

			Edge e1 = graph.insertEdge(a, b);
			REQUIRE(!e1->directed());
			REQUIRE(e1->from() == a);
			REQUIRE(e1->to() == b);
			REQUIRE(a->incomingEdges() == 0);
			REQUIRE(a->outgoingEdges() == 0);
			REQUIRE(a->undirectedEdges() == 1);
			REQUIRE(b->incomingEdges() == 0);
			REQUIRE(b->outgoingEdges() == 0);
			REQUIRE(b->undirectedEdges() == 1);

			graph.reverse(e1);
			REQUIRE(!e1->directed());
			REQUIRE(e1->from() == b);
			REQUIRE(e1->to() == a);
			REQUIRE(a->incomingEdges() == 0);
			REQUIRE(a->outgoingEdges() == 0);
			REQUIRE(a->undirectedEdges() == 1);
			REQUIRE(b->incomingEdges() == 0);
			REQUIRE(b->outgoingEdges() == 0);
			REQUIRE(b->undirectedEdges() == 1);

			graph.setDirected(e1, true);
			REQUIRE(e1->directed());
			REQUIRE(e1->from() == b);
			REQUIRE(e1->to() == a);
			REQUIRE(a->incomingEdges() == 1);
			REQUIRE(a->outgoingEdges() == 0);
			REQUIRE(a->undirectedEdges() == 0);
			REQUIRE(b->incomingEdges() == 0);
			REQUIRE(b->outgoingEdges() == 1);
			REQUIRE(b->undirectedEdges() == 0);

			graph.reverse(e1);
			REQUIRE(e1->directed());
			REQUIRE(e1->from() == a);
			REQUIRE(e1->to() == b);
			REQUIRE(a->incomingEdges() == 0);
			REQUIRE(a->outgoingEdges() == 1);
			REQUIRE(a->undirectedEdges() == 0);
			REQUIRE(b->incomingEdges() == 1);
			REQUIRE(b->outgoingEdges() == 0);
			REQUIRE(b->undirectedEdges() == 0);
		}

		void testPure()
		{
			using Graph = Directed_Graph<>;
			using Vertex = Graph::Vertex_Iterator;
			using Edge = Graph::Edge_Iterator;

			Graph graph;
			Vertex a = graph.insertVertex();
			Vertex b = graph.insertVertex();

			REQUIRE(a->cIncomingBegin() == a->cIncomingEnd());
			REQUIRE(b->cOutgoingBegin() == b->cOutgoingEnd());

			Edge A = graph.insertEdge(a, b);

			REQUIRE(a->cIncomingBegin() == a->cIncomingEnd());
			REQUIRE(a->incomingEdges() == 0);
			REQUIRE(a->outgoingEdges() == 1);
			REQUIRE(a->cOutgoingBegin()->vertex() == b);

			REQUIRE(b->cOutgoingBegin() == b->cOutgoingEnd());
			REQUIRE(b->incomingEdges() == 1);
			REQUIRE(b->outgoingEdges() == 0);
			REQUIRE(b->cIncomingBegin()->vertex() == a);

			graph.removeEdge(A);

			REQUIRE(a->cIncomingBegin() == a->cIncomingEnd());
			REQUIRE(a->incomingEdges() == 0);
			REQUIRE(a->outgoingEdges() == 0);

			REQUIRE(b->cOutgoingBegin() == b->cOutgoingEnd());
			REQUIRE(b->incomingEdges() == 0);
			REQUIRE(b->outgoingEdges() == 0);
		}

		void testMerge()
		{
			Graph aGraph;

			Vertex a = aGraph.insertVertex();
			Vertex b = aGraph.insertVertex();
			Vertex c = aGraph.insertVertex();
			{
				REQUIRE(aGraph.vertices() == 3);
			}

			aGraph.insertEdge(a, b);
			aGraph.insertEdge(b, c);
			aGraph.insertEdge(c, a);
			{
				REQUIRE(aGraph.edges() == 3);
			}

			Graph bGraph;
			Vertex d = bGraph.insertVertex();
			Vertex e = bGraph.insertVertex();
			Vertex f = bGraph.insertVertex();
			{
				REQUIRE(bGraph.vertices() == 3);
			}

			bGraph.insertEdge(d, e);
			bGraph.insertEdge(e, f);
			bGraph.insertEdge(f, d);
			{
				REQUIRE(bGraph.edges() == 3);
			}

			aGraph.merge(bGraph);
			{
				REQUIRE(aGraph.vertices() == 6);
				REQUIRE(aGraph.edges() == 6);
				REQUIRE(bGraph.vertices() == 0);
				REQUIRE(bGraph.edges() == 0);
			}
		}

	};

	void test()
	{
		Test test;
		test.run();		
	}

}