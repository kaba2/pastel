// Description: Testing for functional transitive closure
// DocumentationOf: transitive_closure.h

#include "pastelsystest.h"

#include "pastel/sys/transitive_closure.h"
#include "pastel/sys/incidence_graph.h"
#include "pastel/sys/counting_iterator.h"
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
			typedef Pastel::Incidence_Graph<GraphType::Directed, integer> Graph;
			typedef Graph::Vertex Vertex;
			typedef Graph::Edge Edge;
			typedef Graph::Vertex_Iterator Vertex_Iterator;
			typedef Graph::Edge_Iterator Edge_Iterator;
			typedef Graph::Incidence_Iterator Incidence_Iterator;

			Graph relation;

			Vertex_Iterator a = relation.addVertex(1);
			Vertex_Iterator b = relation.addVertex(5);
			Vertex_Iterator c = relation.addVertex(2);
			Vertex_Iterator d = relation.addVertex(7);
			Vertex_Iterator e = relation.addVertex(6);
			Vertex_Iterator f = relation.addVertex(4);
			Vertex_Iterator g = relation.addVertex(8);
		
			Edge_Iterator at = relation.addEdge(a, a);
			Edge_Iterator bt = relation.addEdge(b, b);
			Edge_Iterator ct = relation.addEdge(c, c);
			Edge_Iterator dt = relation.addEdge(d, d);
			Edge_Iterator et = relation.addEdge(e, e);
			Edge_Iterator ft = relation.addEdge(f, f);

			relation.addEdge(a, b);
			relation.addEdge(c, b);
			relation.addEdge(b, d);
			relation.addEdge(d, e);
			relation.addEdge(e, d);

			auto fn = [](const Vertex_Iterator& vertex) -> integer 
			{
				return *vertex;
			};

			auto op = [](integer left, integer right) -> integer 
			{
				return left + right;
			};

			auto forEachDomain = [&relation](
				const std::function<void(const Vertex_Iterator&)>& visit) 
			{
				std::for_each(
					countingIterator(relation.vertexBegin()),
					countingIterator(relation.vertexEnd()),
					visit);
			};

			auto forEachRelated = [](
				const Vertex_Iterator& vertex, 
				const std::function<void(const Vertex_Iterator&)>& visit)
			{
				std::for_each(
					countingIterator(vertex->outgoingBegin()),
					countingIterator(vertex->outgoingEnd()),
					[&](const Incidence_Iterator& incidence)
				{
					visit(incidence->vertex());
				});
			};

			std::unordered_map<Vertex_Iterator, integer,
				IteratorAddress_Hash> closureMap; 

			auto report = [&](
				const Vertex_Iterator& vertex, integer value)
			{
				closureMap.insert(
					std::make_pair(vertex, value));
			};

			transitiveClosure<Vertex_Iterator, integer>(
				0, fn, op, forEachRelated, forEachDomain, report,
				false, IteratorAddress_Hash());
			{
				TEST_ENSURE_OP(closureMap[a], ==, 19);
				TEST_ENSURE_OP(closureMap[b], ==, 18);
				TEST_ENSURE_OP(closureMap[c], ==, 20);
				TEST_ENSURE_OP(closureMap[d], ==, 13);
				TEST_ENSURE_OP(closureMap[e], ==, 13);
				TEST_ENSURE_OP(closureMap[f], ==, 4);
				TEST_ENSURE_OP(closureMap[g], ==, 0);
			}

			relation.removeEdge(at);
			relation.removeEdge(bt);
			relation.removeEdge(ct);
			relation.removeEdge(dt);
			relation.removeEdge(et);

			closureMap.clear();
			transitiveClosure<Vertex_Iterator, integer>(
				0, fn, op, forEachRelated, forEachDomain, report,
				true, IteratorAddress_Hash());
			{
				TEST_ENSURE_OP(closureMap[a], ==, 19);
				TEST_ENSURE_OP(closureMap[b], ==, 18);
				TEST_ENSURE_OP(closureMap[c], ==, 20);
				TEST_ENSURE_OP(closureMap[d], ==, 13);
				TEST_ENSURE_OP(closureMap[e], ==, 13);
				TEST_ENSURE_OP(closureMap[f], ==, 4);
				TEST_ENSURE_OP(closureMap[g], ==, 8);
			}

			closureMap.clear();
			transitiveClosure<Vertex_Iterator, integer>(
				0, fn, op, forEachRelated, forEachDomain, report,
				false, IteratorAddress_Hash());
			{
				TEST_ENSURE_OP(closureMap[a], ==, 18);
				TEST_ENSURE_OP(closureMap[b], ==, 13);
				TEST_ENSURE_OP(closureMap[c], ==, 18);
				TEST_ENSURE_OP(closureMap[d], ==, 13);
				TEST_ENSURE_OP(closureMap[e], ==, 13);
				TEST_ENSURE_OP(closureMap[f], ==, 4);
				TEST_ENSURE_OP(closureMap[g], ==, 0);
			}

		}

		void testTransitiveClosureUnion()
		{
			typedef std::unordered_set<integer> Set;

			typedef Pastel::Incidence_Graph<GraphType::Directed, Set> Graph;
			typedef Graph::Vertex Vertex;
			typedef Graph::Edge Edge;
			typedef Graph::Vertex_Iterator Vertex_Iterator;
			typedef Graph::Edge_Iterator Edge_Iterator;
			typedef Graph::Incidence_Iterator Incidence_Iterator;

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

			auto f = [](const Vertex_Iterator& vertex) -> const Set&
			{
				return *vertex;
			};

			auto op = [](Set&& left, const Set& right) -> Set
			{
				std::for_each(right.begin(), right.end(),
					[&](integer that) 
				{
					left.insert(that);
				});
		
				return left;
			};

			auto forEachDomain = [&relation](
				const std::function<void(const Vertex_Iterator&)>& visit) 
			{
				std::for_each(
					countingIterator(relation.vertexBegin()),
					countingIterator(relation.vertexEnd()),
					visit);
			};

			auto forEachRelated = [](
				const Vertex_Iterator& vertex, 
				const std::function<void(const Vertex_Iterator&)>& visit)
			{
				std::for_each(
					countingIterator(vertex->outgoingBegin()),
					countingIterator(vertex->outgoingEnd()),
					[&](const Incidence_Iterator& incidence)
				{
					visit(incidence->vertex());
				});
			};

			auto report = [](
				const Vertex_Iterator& vertex, Set&& set)
			{
				/*
				std::for_each(((const Set&)*vertex).begin(), ((const Set&)*vertex).end(),
					[](const integer& that) {std::cout << that << ", ";});
				std::cout << " before, ";

				std::for_each(set.begin(), set.end(),
					[](const integer& that) {std::cout << that << ", ";});
				std::cout << " after." << std::endl;
				*/
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
