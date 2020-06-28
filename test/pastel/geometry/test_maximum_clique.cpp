// Description: Testing for aligned box maximum clique
// DocumentationOf: maximum_clique_alignedbox.h

#include "test/test_init.h"

#include "pastel/sys/rational.h"

#include "pastel/geometry/maximum_clique_alignedbox.h"
#include "pastel/geometry/shape/alignedbox.h"

#include "pastel/sys/vector/vector_tools.h"
#include "pastel/sys/output/push_back_output.h"

namespace
{

	using Real = Rational<integer>;
	using Box = AlignedBox<Real, 2>;

	template <int N, int M>
	void testCase(
		const Box (&boxSet)[N],
		const Box& correct,
		integer sweepDirection,
		const integer (&correctSet)[M])
	{
		std::vector<const Box*> resultSet;

		Box clique = maximumClique(
			range(boxSet), 
			sweepDirection,
			pushBackOutput(resultSet));
		REQUIRE(correct.min() == clique.min());
		REQUIRE(correct.max() == clique.max());

		std::sort(resultSet.begin(), resultSet.end());

		integer n = resultSet.size();

		std::vector<integer> resultIndexSet;
		resultIndexSet.reserve(n);
		for (integer i = 0;i < n;++i)
		{
			resultIndexSet.push_back(
				resultSet[i] - boxSet);
		}

		REQUIRE((correctSet[0] == -1 ||
			std::equal(correctSet, correctSet + M, 
			resultIndexSet.begin())));

		/*
		for (integer i = 0;i < n;++i)
		{
			std::cout << resultIndexSet[i] << ", ";
		}
		std::cout << "end." << std::endl;

		std::cout << correct.min() << correct.max() << std::endl;
		std::cout << clique.min() << clique.max() << std::endl;
		std::cout << std::endl;
		*/
	}

	template <int N>
	void testCase(
		const Box (&boxSet)[N],
		const Box& correct,
		integer sweepDirection = 1)
	{
		integer correctSet[] = {-1};
		testCase(boxSet, correct, 
			sweepDirection, correctSet);
	}

}
		
TEST_CASE("maximumClique (maximumClique)")
{
	// Identical boxes.
	{
		Box boxSet[] = 
		{
			Box(-1, -1, 1, 1),
			Box(-1, -1, 1, 1)
		};
		boxSet[0].maxTopology().set(Topology::Closed);
		boxSet[1].maxTopology().set(Topology::Closed);

		Box correct(-1, -1, 1, 1);
		testCase(boxSet, correct);
	}
	// First box contained in the second.
	{
		Box boxSet[] = 
		{
			Box(-1, -1, 1, 1),
			Box(-2, -2, 2, 2)
		};
		boxSet[0].maxTopology().set(Topology::Closed);
		boxSet[1].maxTopology().set(Topology::Closed);

		Box correct(-1, -1, 1, 1);
		testCase(boxSet, correct);
	}
	// Overlap on both axes.
	{
		Box boxSet[] = 
		{
			Box(0, -1, 2, 1),
			Box(-1, 0, 1, 2)
		};
		boxSet[0].maxTopology().set(Topology::Closed);
		boxSet[1].maxTopology().set(Topology::Closed);

		Box correct(0, 0, 1, 1);
		testCase(boxSet, correct);
	}
	// Singular vertical overlap.
	{
		Box boxSet[] = 
		{
			Box(-2, -1, 0, 1),
			Box(0, -1, 2, 1)
		};
		boxSet[0].maxTopology().set(Topology::Closed);
		boxSet[1].maxTopology().set(Topology::Closed);

		Box correct(0, -1, 0, 1);
		testCase(boxSet, correct);
	}
	// Singular vertical overlap with open boxes.
	{
		Box boxSet[] = 
		{
			Box(-2, -1, 0, 1),
			Box(0, -1, 2, 1)
		};

		Box correct;
		testCase(boxSet, correct);
	}
	// Singular horizontal overlap.
	{
		Box boxSet[] = 
		{
			Box(-1, -2, 1, 0),
			Box(-1, 0, 1, 2)
		};
		boxSet[0].maxTopology().set(Topology::Closed);
		boxSet[1].maxTopology().set(Topology::Closed);

		Box correct(-1, 0, 1, 0);
		testCase(boxSet, correct);
	}
	// Singular horizontal overlap with open boxes.
	{
		Box boxSet[] = 
		{
			Box(-1, -2, 1, 0),
			Box(-1, 0, 1, 2)
		};

		Box correct;
		testCase(boxSet, correct);
	}
	// Singular point overlap.
	{
		Box boxSet[] = 
		{
			Box(0, 0, 2, 2),
			Box(-2, -2, 0, 0)
		};
		boxSet[0].maxTopology().set(Topology::Closed);
		boxSet[1].maxTopology().set(Topology::Closed);

		Box correct(0, 0, 0, 0);
		testCase(boxSet, correct);
	}
	// Singular point overlap with open boxes.
	{
		Box boxSet[] = 
		{
			Box(0, 0, 2, 2),
			Box(-2, -2, 0, 0)
		};

		Box correct;
		testCase(boxSet, correct);
	}
	// Disjoint boxes.
	{
		Box boxSet[] = 
		{
			Box(1, 1, 2, 2),
			Box(-2, -2, -1, -1)
		};
		Box correct;
		testCase(boxSet, correct);
	}
	// Multiple boxes, general position.
	{
		Box boxSet[] = 
		{
			Box(-1, -1, 1, 1),
			Box(0, 0, 2, 2),
			Box(0, -1, 2, 1)
		};
		boxSet[0].maxTopology().set(Topology::Closed);
		boxSet[1].maxTopology().set(Topology::Closed);
		boxSet[2].maxTopology().set(Topology::Closed);

		Box correct(0, 0, 1, 1);
		testCase(boxSet, correct);
	}
	// Multiple boxes.
	{
		Box boxSet[] = 
		{
			Box(-1, -1, 1, 1),
			Box(0, 0, 2, 2),
			Box(0, -1, 2, 1),
			Box(Real(-1, 2), Real(-1, 2), 
			Real(1, 2), Real(1, 2))
		};
		boxSet[0].maxTopology().set(Topology::Closed);
		boxSet[1].maxTopology().set(Topology::Closed);
		boxSet[2].maxTopology().set(Topology::Closed);
		boxSet[3].maxTopology().set(Topology::Closed);

		integer correctSet[] =
		{
			0, 1, 2, 3
		};
		Box correct(0, 0, Real(1, 2), Real(1, 2));
		testCase(boxSet, correct, 1, correctSet);
	}
	{
		Box boxSet[] = 
		{
			Box(0, 0, 2, 2),
			Box(3, 0, 5, 2),
			Box(4, 0, 6, 2),
			Box(7, 0, 9, 2)
		};
		boxSet[0].maxTopology().set(Topology::Closed);
		boxSet[1].maxTopology().set(Topology::Closed);
		boxSet[2].maxTopology().set(Topology::Closed);
		boxSet[3].maxTopology().set(Topology::Closed);

		integer correctSet[] =
		{
			1, 2
		};
		Box correct(4, 0, 5, 2);
		testCase(boxSet, correct, 1, correctSet);
	}
}
