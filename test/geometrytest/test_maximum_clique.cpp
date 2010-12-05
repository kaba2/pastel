#include "pastelgeometrytest.h"

#include "pastel/geometry/maximum_clique_alignedbox.h"
#include "pastel/geometry/alignedbox.h"

#include "pastel/sys/rational_tools.h"
#include "pastel/sys/vector_tools.h"

using namespace Pastel;

namespace
{

	class MaximumClique_Test
		: public TestSuite
	{
	public:
		typedef Rational<integer> Real;

		MaximumClique_Test()
			: TestSuite(&geometryTestReport())
		{
		}

		virtual void run()
		{
			test();
		}

		typedef AlignedBox<Real, 2> Box;

		template <int N>
		void testCase(
			const Box (&boxSet)[N],
			const Box& correct,
			MaximumClique_BoxType::Enum boxType = MaximumClique_BoxType::Closed,
			integer sweepDirection = 1)
		{
			const integer correctSet[] = {-1};
			testCase(boxSet, correct, boxType, 
				sweepDirection, correctSet);
		}

		template <int N, int M>
		void testCase(
			const Box (&boxSet)[N],
			const Box& correct,
			MaximumClique_BoxType::Enum boxType,
			integer sweepDirection,
			const integer (&correctSet)[M])
		{
			std::vector<const Box*> resultSet;

			const Box clique = 
				maximumClique(range(boxSet), boxType,
				sweepDirection,
				std::back_inserter(resultSet));
			TEST_ENSURE(correct.min() == clique.min());
			TEST_ENSURE(correct.max() == clique.max());

			std::sort(resultSet.begin(), resultSet.end());

			const integer n = resultSet.size();

			std::vector<integer> resultIndexSet;
			resultIndexSet.reserve(n);
			for (integer i = 0;i < n;++i)
			{
				resultIndexSet.push_back(
					resultSet[i] - boxSet);
			}

			TEST_ENSURE(correctSet[0] == -1 ||
				std::equal(correctSet, correctSet + M, 
				resultIndexSet.begin()));

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

		void test()
		{
			{
				Box boxSet[] = 
				{
					Box(-1, -1, 1, 1),
					Box(-1, -1, 1, 1)
				};
				const Box correct(-1, -1, 1, 1);
				testCase(boxSet, correct);
			}
			{
				Box boxSet[] = 
				{
					Box(-1, -1, 1, 1),
					Box(-2, -2, 2, 2)
				};
				const Box correct(-1, -1, 1, 1);
				testCase(boxSet, correct);
			}
			{
				Box boxSet[] = 
				{
					Box(0, -1, 2, 1),
					Box(-1, 0, 1, 2)
				};
				const Box correct(0, 0, 1, 1);
				testCase(boxSet, correct);
			}
			{
				Box boxSet[] = 
				{
					Box(-2, -1, 0, 1),
					Box(0, -1, 2, 1)
				};
				const Box correct(0, -1, 0, 1);
				testCase(boxSet, correct);
			}
			{
				Box boxSet[] = 
				{
					Box(-2, -1, 0, 1),
					Box(0, -1, 2, 1)
				};
				const Box correct;
				testCase(boxSet, correct, 
					MaximumClique_BoxType::Open);
			}
			{
				Box boxSet[] = 
				{
					Box(-1, -2, 1, 0),
					Box(-1, 0, 1, 2)
				};
				const Box correct(-1, 0, 1, 0);
				testCase(boxSet, correct);
			}
			{
				Box boxSet[] = 
				{
					Box(-1, -2, 1, 0),
					Box(-1, 0, 1, 2)
				};
				const Box correct;
				testCase(boxSet, correct, 
					MaximumClique_BoxType::Open);
			}
			{
				Box boxSet[] = 
				{
					Box(0, 0, 2, 2),
					Box(-2, -2, 0, 0)
				};
				const Box correct(0, 0, 0, 0);
				testCase(boxSet, correct);
			}
			{
				Box boxSet[] = 
				{
					Box(0, 0, 2, 2),
					Box(-2, -2, 0, 0)
				};
				const Box correct;
				testCase(boxSet, correct, 
					MaximumClique_BoxType::Open);
			}
			{
				Box boxSet[] = 
				{
					Box(1, 1, 2, 2),
					Box(-2, -2, -1, -1)
				};
				const Box correct;
				testCase(boxSet, correct);
			}
			{
				Box boxSet[] = 
				{
					Box(-1, -1, 1, 1),
					Box(0, 0, 2, 2),
					Box(0, -1, 2, 1)
				};
				const Box correct(0, 0, 1, 1);
				testCase(boxSet, correct);
			}
			{
				Box boxSet[] = 
				{
					Box(-1, -1, 1, 1),
					Box(0, 0, 2, 2),
					Box(0, -1, 2, 1),
					Box(Real(-1, 2), Real(-1, 2), 
					Real(1, 2), Real(1, 2))
				};
				integer correctSet[] =
				{
					0, 1, 2, 3
				};
				const Box correct(0, 0, Real(1, 2), Real(1, 2));
				testCase(boxSet, correct, 
					MaximumClique_BoxType::Closed, 1, correctSet);
			}
			{
				Box boxSet[] = 
				{
					Box(0, 0, 2, 2),
					Box(3, 0, 5, 2),
					Box(4, 0, 6, 2),
					Box(7, 0, 9, 2),
				};
				integer correctSet[] =
				{
					1, 2
				};
				const Box correct(4, 0, 5, 2);
				testCase(boxSet, correct, 
					MaximumClique_BoxType::Closed, 1, correctSet);
			}
		}
	};

	void testMaximumClique()
	{
		MaximumClique_Test test;
		test.run();
	}

	void addTest()
	{
		geometryTestList().add("MaximumClique", testMaximumClique);
	}

	CallFunction run(addTest);

}
