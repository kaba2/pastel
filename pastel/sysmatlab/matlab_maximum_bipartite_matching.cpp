// Description: maximum_bipartite_matching
// DocumentationOf: maximum_bipartite_matching.m

#include "pastel/matlab/pastelmatlab.h"

#include "pastel/sys/graph/maximum_bipartite_matching.h"
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

			std::vector<integer> leftMatchSet;
			std::vector<integer> rightMatchSet;

			maximumBipartiteMatching(
				graph.cRowRange(0),
				graph.cRowRange(1),
				[&](const std::pair<integer, integer>& that)
			{
				leftMatchSet.push_back(that.first);
				rightMatchSet.push_back(that.second);
			});				

			// Output the matching.

			if (outputs > 0)
			{
				Array<int32> result =
					matlabCreateArray<int32>(leftMatchSet.size(), 2, outputSet[MatchSet]);

				boost::copy(
					range(leftMatchSet.cbegin(), leftMatchSet.cend()),
					result.rowBegin(0));

				boost::copy(
					range(rightMatchSet.cbegin(), rightMatchSet.cend()),
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
