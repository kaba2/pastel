// Description: maximum_bipartite_matching
// DocumentationOf: maximum_bipartite_matching.m

#include "pastel/matlab/pastelmatlab.h"

#include "pastel/sys/graph/matching.h"
#include "pastel/sys/range.h"

#include <boost/range/algorithm/copy.hpp>

#include <algorithm>

void force_linking_maximum_bipartite_matching() {}

namespace Pastel
{

	namespace
	{

		void matlabMaximumBipartiteMatching(
			int outputs, mxArray *outputSet[],
			int inputs, const mxArray *inputSet[])
		{
			enum
			{
				Graph,
				Mode,
				Inputs
			};

			enum
			{
				MatchSet,
				Outputs
			};

			ENSURE_OP(inputs, ==, Inputs);
			ENSURE_OP(outputs, <=, Outputs);

			Array<integer> graph = matlabAsArray<integer>(inputSet[Graph]);
			ENSURE_OP(graph.height(), ==, 2);

			std::string mode = matlabAsString(inputSet[Mode]);
			ENSURE(mode == "maximum" || mode == "maximal");

			integer nA = 0;
			integer nB = 0;

			std::vector<std::vector<integer>> edgeSet;
			edgeSet.resize(graph.width());
			for (integer i = 0;i < graph.width();++i)
			{
				integer from = graph(i, 0);
				if (nA < from + 1)
				{
					nA = from + 1;
				}

				integer to = graph(i, 1);
				if (nB < to + 1)
				{
					nB = to + 1;
				}

				bool nonNegativeVertex = (from >= 0) && (to >= 0);
				ENSURE2(nonNegativeVertex, from, to);

				edgeSet[from].push_back(to);
			}

			auto forEachAdjacent = [&](integer a, auto&& visit)
			{
				for (auto&& b : edgeSet[a])
				{
					if (!visit(b))
					{
						break;						
					}
				}
			};

			std::vector<integer> leftMatchSet;
			std::vector<integer> rightMatchSet;

			if (mode == "maximum")
			{
				maximumBipartiteMatching(
					nA, nB,
					forEachAdjacent,
					PASTEL_TAG(report),
					[&](integer a, integer b)
				{
					leftMatchSet.push_back(a);
					rightMatchSet.push_back(b);
				});				
			}
			else
			{
				maximalBipartiteMatching(
					nA, nB,
					forEachAdjacent,
					PASTEL_TAG(report),
					[&](integer a, integer b)
				{
					leftMatchSet.push_back(a);
					rightMatchSet.push_back(b);
				});				
			}

			// Output the matching.

			if (outputs > 0)
			{
				Array<int32> result =
					matlabCreateArray<int32>(leftMatchSet.size(), 2, outputSet[MatchSet]);

				ranges::copy(
					leftMatchSet,
					result.rowBegin(0));

				ranges::copy(
					rightMatchSet,
					result.rowBegin(1));
			}
		}

		void addFunction()
		{
			matlabAddFunction(
				"maximum_bipartite_matching",
				matlabMaximumBipartiteMatching);
		}

		CallFunction call(addFunction);
	}

}
