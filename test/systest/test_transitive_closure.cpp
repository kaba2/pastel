// Description: Testing for functional transitive closure
// DocumentationOf: transitive_closure.h

#include "pastelsystest.h"

#include "pastel/sys/transitive_closure.h"
#include "pastel/sys/adjacency_graph.h"
#include "pastel/sys/countingiterator.h"
#include "pastel/sys/mytypes.h"
#include "pastel/sys/hash.h"

#include <unordered_set>

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
			testTransitiveClosureAddition();
			testTransitiveClosureUnion();
		}

		void testTransitiveClosureAddition()
		{
			typedef Pastel::Adjacency_Graph<integer> Graph;
			typedef Graph::Vertex Vertex;
			typedef Graph::Edge Edge;
			typedef Graph::Vertex_Iterator Vertex_Iterator;
			typedef Graph::Edge_Iterator Edge_Iterator;

			Graph relation;

			Vertex_Iterator a = relation.addVertex(1);
			Vertex_Iterator b = relation.addVertex(5);
			Vertex_Iterator c = relation.addVertex(2);
			Vertex_Iterator d = relation.addVertex(7);
			Vertex_Iterator e = relation.addVertex(6);
		
			relation.addEdge(a, a);
			relation.addEdge(b, b);
			relation.addEdge(c, c);
			relation.addEdge(d, d);
			relation.addEdge(e, e);

			relation.addEdge(a, b);
			relation.addEdge(c, b);
			relation.addEdge(b, d);
			relation.addEdge(d, e);
			relation.addEdge(e, d);

			std::cout << "The graph has " << relation.vertices() << " vertices, and "
				<< relation.edges() << " edges." << std::endl;

			auto f = [](const Vertex_Iterator& vertex) -> integer 
			{
				return (*vertex)();
			};

			auto op = [](integer left, integer right) -> integer 
			{
				return left + right;
			};

			auto forEachDomain = [&relation](
				const std::function<void(const Vertex_Iterator&)>& visit) 
			{
				auto vertexRange = relation.vertexRange();
				std::for_each(
					countingIterator(vertexRange.begin()),
					countingIterator(vertexRange.end()),
					visit);
			};

			auto forEachRelated = [](
				const Vertex_Iterator& vertex, 
				const std::function<void(const Vertex_Iterator&)>& visit)
			{
				auto edgeRange = vertex->exidentRange();
				std::for_each(
					countingIterator(edgeRange.begin()),
					countingIterator(edgeRange.end()),
					[&](const Edge_Iterator& edge){visit(edge->to());});
			};

			auto report = [](
				const Vertex_Iterator& vertex, integer value)
			{
				std::cout << (*vertex)() << " before, ";
				(*vertex)() = value;
				std::cout << value << " after." << std::endl;
			};

			transitiveClosure<Vertex_Iterator, integer>(
				0, f, op, forEachRelated, forEachDomain, report,
				false, IteratorAddress_Hash());
		}

		void testTransitiveClosureUnion()
		{
			typedef std::unordered_set<integer> Set;

			typedef Pastel::Adjacency_Graph<Set> Graph;
			typedef Graph::Vertex Vertex;
			typedef Graph::Edge Edge;
			typedef Graph::Vertex_Iterator Vertex_Iterator;
			typedef Graph::Edge_Iterator Edge_Iterator;

			Graph relation;
	
			Set aSet;
			aSet.insert(1);
			Vertex_Iterator a = relation.addVertex(aSet);
	
			Set bSet;
			bSet.insert(5);
			Vertex_Iterator b = relation.addVertex(bSet);

			Set cSet;
			cSet.insert(2);
			Vertex_Iterator c = relation.addVertex(cSet);

			Set dSet;
			dSet.insert(7);
			Vertex_Iterator d = relation.addVertex(dSet);

			Set eSet;
			eSet.insert(6);
			Vertex_Iterator e = relation.addVertex(eSet);
		
			relation.addEdge(a, a);
			relation.addEdge(b, b);
			relation.addEdge(c, c);
			relation.addEdge(d, d);
			relation.addEdge(e, e);

			relation.addEdge(a, b);
			relation.addEdge(c, b);
			relation.addEdge(b, d);
			relation.addEdge(d, e);
			relation.addEdge(e, d);

			std::cout << "The graph has " << relation.vertices() << " vertices, and "
				<< relation.edges() << " edges." << std::endl;

			auto f = [](const Vertex_Iterator& vertex) -> const Set&
			{
				return (*vertex)();
			};

			auto op = [](Set&& left, const Set& right) -> Set
			{
				std::for_each(right.begin(), right.end(),
					[&](integer that) {left.insert(that);});
		
				return left;
			};

			auto forEachDomain = [&relation](
				const std::function<void(const Vertex_Iterator&)>& visit) 
			{
				auto vertexRange = relation.vertexRange();
				std::for_each(
					countingIterator(vertexRange.begin()),
					countingIterator(vertexRange.end()),
					visit);
			};

			auto forEachRelated = [](
				const Vertex_Iterator& vertex, 
				const std::function<void(const Vertex_Iterator&)>& visit)
			{
				auto edgeRange = vertex->exidentRange();
				std::for_each(
					countingIterator(edgeRange.begin()),
					countingIterator(edgeRange.end()),
					[&](const Edge_Iterator& edge){visit(edge->to());});
			};

			auto report = [](
				const Vertex_Iterator& vertex, Set&& set)
			{
				std::for_each((*vertex)().begin(), (*vertex)().end(),
					[](const integer& that) {std::cout << that << ", ";});
				std::cout << " before, ";

				std::for_each(set.begin(), set.end(),
					[](const integer& that) {std::cout << that << ", ";});
				std::cout << " after." << std::endl;
			};

			Set emptySet;

			transitiveClosure<Vertex_Iterator, Set>(
				emptySet, f, op, forEachRelated, forEachDomain, report,
				false, IteratorAddress_Hash());
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("transitive_closure", test);
	}

	CallFunction run(addTest);

}
