// Description: Testing for maximum bipartite matching
// DocumentationOf: maximum_bipartite_matching.h

#include "test_pastelsys.h"

#include "pastel/sys/graph/maximum_bipartite_matching.h"
#include "pastel/sys/tuple/tuple_tools.h"
#include "pastel/sys/output.h"
#include "pastel/sys/sequence/random_subset.h"
#include "pastel/sys/random.h"

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

		using EdgeSet = std::vector<std::vector<integer>>;

		void testRandom()
		{
			integer trials = 10;
			for (integer trial = 0;trial < trials;++trial)
			{
				integer nA = randomInteger(100);
				integer nB = randomInteger(100);
				
				std::vector<integer> allEdgeSet;
				allEdgeSet.reserve(nB);
				for (integer b = 0;b < nB;++b)
				{
					allEdgeSet.push_back(b);
				}

				integer maxEdges = std::min(nB, (integer)10);

				EdgeSet edgeSet;
				edgeSet.resize(nA);

				for (integer a = 0;a < nA;++a)
				{
					integer edges = randomInteger(maxEdges);
					randomSubset(
						allEdgeSet.begin(), allEdgeSet.end(), edges);

					edgeSet[a].resize(edges);
					std::copy(
						allEdgeSet.begin(), allEdgeSet.begin() + edges, 
						edgeSet[a].begin());
				}

				auto forEachAdjacent = [&](
					integer a, auto&& visit)
				{
					for (integer b : edgeSet[a])
					{
						if (!visit(b))
						{
							break;
						}
					}
				};
				
				testCase(nB, std::move(edgeSet), 0);
			}
		}

		void testNonTrivial()
		{
			{
				EdgeSet edgeSet =
				{
					{0},
					{1},
					{0, 2},
					{1}
				};

				TEST_ENSURE(testCase(10, std::move(edgeSet), 3));
			}
		}

		void testSimple()
		{
			{
				EdgeSet edgeSet =
				{
					{0}
				};

				TEST_ENSURE(testCase(10, std::move(edgeSet), 1));
			}
			{
				EdgeSet edgeSet =
				{
					{1}
				};

				TEST_ENSURE(testCase(10, std::move(edgeSet), 1));
			}
			{
				EdgeSet edgeSet =
				{
					{},
					{0}
				};

				TEST_ENSURE(testCase(10, std::move(edgeSet), 1));
			}
			{
				EdgeSet edgeSet =
				{
					{0},
					{1}
				};

				TEST_ENSURE(testCase(10, std::move(edgeSet), 2));
			}
			{
				EdgeSet edgeSet =
				{
					{0},
					{0}
				};

				TEST_ENSURE(testCase(10, std::move(edgeSet), 1));
			}
			{
				EdgeSet edgeSet =
				{
					{0, 1}
				};

				TEST_ENSURE(testCase(10, std::move(edgeSet), 1));
			}
			{
				EdgeSet edgeSet =
				{
					{0},
					{0, 1}
				};

				TEST_ENSURE(testCase(10, std::move(edgeSet), 2));
			}
			{
				EdgeSet edgeSet =
				{
					{0, 1},
					{0, 1}
				};

				TEST_ENSURE(testCase(10, std::move(edgeSet), 2));
			}
		}

		bool testCase(
			integer nB,
			EdgeSet&& edgeSet,
			integer maximumMatchSize)
		{
			integer n = edgeSet.size();

			using Pair = std::pair<integer, integer>;
			std::vector<Pair> matchSet;

			auto forEachAdjacent = [&](
				integer a, auto&& visit)
			{
				for (integer b : edgeSet[a])
				{
					if (!visit(b))
					{
						break;
					}
				}
			};

			integer matchSize = maximumBipartiteMatching(
				n,
				nB,
				forEachAdjacent);

			if (maximumMatchSize > 0 && matchSize != maximumMatchSize)
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

