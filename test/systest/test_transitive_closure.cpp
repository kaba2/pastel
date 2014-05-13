// Description: Testing for functional transitive closure
// DocumentationOf: transitive_closure.h

#include "test_pastelsys.h"

#include "pastel/sys/transitive_closure.h"
#include "pastel/sys/incidence_graph.h"
#include "pastel/sys/counting_iterator.h"
#include "pastel/sys/mytypes.h"
#include "pastel/sys/iteratoraddress_hash.h"

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
			typedef Pastel::Directed_Graph<integer> Graph;
			typedef Graph::Vertex Vertex;
			typedef Graph::Edge Edge;
			typedef Graph::Vertex_Iterator Vertex_Iterator;
			typedef Graph::Edge_Iterator Edge_Iterator;
			typedef Graph::Incidence_Iterator Incidence_Iterator;

			Graph relation;

			Vertex_Iterator a = relation.insertVertex(1);
			Vertex_Iterator b = relation.insertVertex(5);
			Vertex_Iterator c = relation.insertVertex(2);
			Vertex_Iterator d = relation.insertVertex(7);
			Vertex_Iterator e = relation.insertVertex(6);
			Vertex_Iterator f = relation.insertVertex(4);
			Vertex_Iterator g = relation.insertVertex(8);
		
			Edge_Iterator at = relation.insertEdge(a, a);
			Edge_Iterator bt = relation.insertEdge(b, b);
			Edge_Iterator ct = relation.insertEdge(c, c);
			Edge_Iterator dt = relation.insertEdge(d, d);
			Edge_Iterator et = relation.insertEdge(e, e);
			Edge_Iterator ft = relation.insertEdge(f, f);

			relation.insertEdge(a, b);
			relation.insertEdge(c, b);
			relation.insertEdge(b, d);
			relation.insertEdge(d, e);
			relation.insertEdge(e, d);

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
				0, forEachDomain, forEachRelated, fn, op, report,
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
				0, forEachDomain, forEachRelated, fn, op, report,
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
				0, forEachDomain, forEachRelated, fn, op, report,
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

			typedef Pastel::Directed_Graph<Set> Graph;
			typedef Graph::Vertex Vertex;
			typedef Graph::Edge Edge;
			typedef Graph::Vertex_Iterator Vertex_Iterator;
			typedef Graph::Edge_Iterator Edge_Iterator;
			typedef Graph::Incidence_Iterator Incidence_Iterator;

			Graph relation;
	
			Set aSet;
			aSet.insert(1);
			Vertex_Iterator a = relation.insertVertex(aSet);
	
			Set bSet;
			bSet.insert(5);
			Vertex_Iterator b = relation.insertVertex(bSet);

			Set cSet;
			cSet.insert(2);
			Vertex_Iterator c = relation.insertVertex(cSet);

			Set dSet;
			dSet.insert(7);
			Vertex_Iterator d = relation.insertVertex(dSet);

			Set eSet;
			eSet.insert(6);
			Vertex_Iterator e = relation.insertVertex(eSet);
		
			relation.insertEdge(a, a);
			relation.insertEdge(b, b);
			relation.insertEdge(c, c);
			relation.insertEdge(d, d);
			relation.insertEdge(e, e);

			relation.insertEdge(a, b);
			relation.insertEdge(c, b);
			relation.insertEdge(b, d);
			relation.insertEdge(d, e);
			relation.insertEdge(e, d);

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
				emptySet, forEachDomain, forEachRelated, f, op, report,
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
