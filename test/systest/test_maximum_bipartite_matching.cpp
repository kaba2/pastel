// Description: Testing for maximum bipartite matching
// DocumentationOf: maximum_bipartite_matching.h

#include "test_pastelsys.h"

#include "pastel/sys/maximum_bipartite_matching.h"
#include "pastel/sys/tuple_tools.h"
#include "pastel/sys/outputs.h"
#include "pastel/sys/random_subset.h"
#include "pastel/sys/random.h"

#include <boost/range/adaptor/transformed.hpp>

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

		virtual void run()
		{
			testSimple();
			testNonTrivial();
			testRandom();
		}

		void testRandom()
		{
			const integer n = 100;
			const integer vertices = 100;
			const integer maxEdges = square(vertices);

			// Generate all edges.
			std::vector<Integer2> edgeSet;
			edgeSet.reserve(maxEdges);
			for (integer i = 0;i < vertices;++i)
			{
				for (integer j = 0;j < vertices;++j)
				{
					edgeSet.push_back(Integer2(i, j));
				}
			}

			for (integer i = 0;i < n;++i)
			{
				const integer edges =
					randomInteger(maxEdges) + 1;
				
				randomSubset(
					edgeSet.begin(), edgeSet.end(), edges);
				
				testCase(range(edgeSet.begin(), edgeSet.begin() + edges), 0);
			}
		}

		void testNonTrivial()
		{
			{
				Integer2 edgeSet[] =
				{
					Integer2(0, 0),
					Integer2(1, 1),
					Integer2(2, 2),
					Integer2(2, 0),
					Integer2(3, 1),
				};

				TEST_ENSURE(testCase(edgeSet, 3));
			}
		}

		void testSimple()
		{
			{
				Integer2 edgeSet[] =
				{
					Integer2(0, 0)
				};

				TEST_ENSURE(testCase(edgeSet, 1));
			}
			{
				Integer2 edgeSet[] =
				{
					Integer2(0, 1)
				};

				TEST_ENSURE(testCase(edgeSet, 1));
			}
			{
				Integer2 edgeSet[] =
				{
					Integer2(1, 0)
				};

				TEST_ENSURE(testCase(edgeSet, 1));
			}
			{
				Integer2 edgeSet[] =
				{
					Integer2(0, 0),
					Integer2(1, 1)
				};

				TEST_ENSURE(testCase(edgeSet, 2));
			}
			{
				Integer2 edgeSet[] =
				{
					Integer2(0, 0),
					Integer2(1, 0)
				};

				TEST_ENSURE(testCase(edgeSet, 1));
			}
			{
				Integer2 edgeSet[] =
				{
					Integer2(0, 0),
					Integer2(0, 1)
				};

				TEST_ENSURE(testCase(edgeSet, 1));
			}
			{
				Integer2 edgeSet[] =
				{
					Integer2(0, 0),
					Integer2(1, 0),
					Integer2(1, 1)
				};

				TEST_ENSURE(testCase(edgeSet, 2));
			}
			{
				Integer2 edgeSet[] =
				{
					Integer2(0, 0),
					Integer2(0, 1),
					Integer2(1, 0),
					Integer2(1, 1)
				};

				TEST_ENSURE(testCase(edgeSet, 2));
			}
		}

		template <int N>
		bool testCase(
			Integer2 (&edgeSet)[N],
			integer maximumMatchSize)
		{
			return testCase(range(edgeSet), maximumMatchSize);
		}

		template <typename Range>
		bool testCase(
			Range edgeSet,
			integer maximumMatchSize)
		{
			const integer n = edgeSet.size();

			using Pair = std::pair<integer, integer>;
			std::vector<Pair> matchSet;

			using namespace boost::adaptors;

			std::function<integer(const Integer2&)> firstElement = 
				[](const Integer2& pair) {return pair[0];};

			std::function<integer(const Integer2&)> secondElement = 
				[](const Integer2& pair) {return pair[1];};

			maximumBipartiteMatching(
				edgeSet | transformed(firstElement),
				edgeSet | transformed(secondElement),
				pushBackOutput(matchSet));

			if (maximumMatchSize > 0 && matchSet.size() != maximumMatchSize)
			{
				// The matching set must be of the maximum size.
				return false;
			}

			// The left vertices must not be shared between 
			// matching edges.
			{
				std::set<integer> vertexSet;
				for (integer i = 0;i < matchSet.size();++i)
				{
					vertexSet.insert(matchSet[i].first);					
				}

				if (vertexSet.size() != matchSet.size())
				{
					return false;
				}
			}

			// The right vertices must not be shared between 
			// matching edges.
			{
				std::set<integer> vertexSet;
				for (integer i = 0;i < matchSet.size();++i)
				{
					vertexSet.insert(matchSet[i].second);
				}

				if (vertexSet.size() != matchSet.size())
				{
					return false;
				}
			}

			std::vector<bool> foundSet(n, false);
			for (integer i = 0;i < matchSet.size();++i)
			{
				bool foundInOriginal = false;
				for (integer j = 0;j < n;++j)
				{
					if (edgeSet[j][0] == matchSet[i].first &&
						edgeSet[j][1] == matchSet[i].second)
					{
						if (foundSet[j])
						{
							// The matching set must not 
							// contain duplicates.
							return false;
						}

						foundInOriginal = true;
						foundSet[j] = true;
						break;
					}
				}

				if (!foundInOriginal)
				{
					// The matching set must be a subset 
					// of the input set.
					return false;
				}
			}

			return true;
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("maximumBipartiteMatching", test);
	}

	CallFunction run(addTest);

}
