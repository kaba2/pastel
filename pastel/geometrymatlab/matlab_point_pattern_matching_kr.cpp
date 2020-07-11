// Description: match_points_kr
// Documentation: match_points_kr.txt

#include "pastel/matlab/pastelmatlab.h"

#include "pastel/geometry/pattern_matching/match_points_kr.h"
#include "pastel/geometry/pointkdtree/pointkdtree.h"
#include "pastel/geometry/splitrule/slidingmidpoint_splitrule.h"
#include "pastel/geometry/nearestset/kdtree_nearestset.h"

#include "pastel/sys/set/sparse_set.h"
#include "pastel/sys/set/interval_set.h"
#include "pastel/sys/locator/pointer_locator.h"

void force_linking_match_points_kr() {}

namespace Pastel
{

	namespace
	{

		void matlabMatchPointsKr(
			int outputs, mxArray *outputSet[],
			int inputs, const mxArray *inputSet[])
		{
			enum
			{
				ModelSet,
				SceneSet,
				KNearest,
				MinMatchRatio,
				MatchingDistance2,
				MaxBias,
				MatchingModeId,
				Inputs
			};

			enum
			{
				PairSet,
				Translation,
				Bias,
				Success,
				Outputs
			};

			ENSURE_OP(inputs, ==, Inputs);

			auto norm = Euclidean_Norm<dreal>();

			MatchPointsKr_MatchingMode matchingModeSet[] =
			{
				MatchPointsKr_MatchingMode::First,
				MatchPointsKr_MatchingMode::Maximum
			};
			integer matchingModes = 
				sizeof(matchingModeSet) / sizeof(MatchPointsKr_MatchingMode);
			
			const dreal* modelData = mxGetPr(inputSet[ModelSet]);
			integer modelPoints = mxGetN(inputSet[ModelSet]);

			const dreal* sceneData = mxGetPr(inputSet[SceneSet]);
			integer scenePoints = mxGetN(inputSet[SceneSet]);
			integer kNearest = matlabAsScalar<integer>(inputSet[KNearest]);
			dreal minMatchRatio = matlabAsScalar<dreal>(inputSet[MinMatchRatio]);
			auto matchingDistance2 = 
				norm[matlabAsScalar<dreal>(inputSet[MatchingDistance2])];
			dreal maxBias =
				matlabAsScalar<dreal>(inputSet[MaxBias]);
			integer matchingModeId = matlabAsScalar<integer>(inputSet[MatchingModeId]);

			ENSURE_OP(matchingModeId, >=, 0);
			ENSURE_OP(matchingModeId, <, matchingModes);
			
			MatchPointsKr_MatchingMode matchingMode = 
				matchingModeSet[matchingModeId];

			integer modelDimension = mxGetM(inputSet[ModelSet]);
			integer sceneDimension = mxGetM(inputSet[SceneSet]);
			
			ENSURE_OP(modelDimension, ==, sceneDimension);

			integer n = modelDimension;

			using Locator = Pointer_Locator<dreal>;

			using Settings = PointKdTree_Settings<Locator>;

			using SceneTree = PointKdTree<Settings>;
			using SceneIterator = SceneTree::Point_ConstIterator;

			using ModelTree = PointKdTree<Settings>;
			using ModelIterator = ModelTree::Point_ConstIterator;

			Locator locator(n);

			SceneTree sceneTree(locator);
			sceneTree.insertSet(
				sparseSet(
					intervalSet(
						sceneData, 
						sceneData + scenePoints * n
					),
				n)
			);

			ModelTree modelTree(locator);
			modelTree.insertSet(
				sparseSet(
					intervalSet(
						modelData,
						modelData + modelPoints * n
					),
				n)
			);

			sceneTree.refine(SlidingMidpoint_SplitRule());
			modelTree.refine(SlidingMidpoint_SplitRule());

			// Here's where we store the results.

			std::vector<std::pair<SceneIterator, ModelIterator> > pairSet;

			// Compute the point pattern match.

			auto match = Pastel::matchPointsKr(
				kdTreeNearestSet(modelTree),
				kdTreeNearestSet(sceneTree), 
				PASTEL_TAG(norm), norm, 
				PASTEL_TAG(report), pushBackOutput(pairSet),
				PASTEL_TAG(kNearest), kNearest,
				PASTEL_TAG(minMatchRatio), minMatchRatio,
				PASTEL_TAG(matchingDistance2), matchingDistance2,
				PASTEL_TAG(maxBias), maxBias,
				PASTEL_TAG(matchingMode), matchingMode);

			// Output the pairing.

			MatrixView<int32> outPairSet = matlabCreateMatrix<int32>(
				2, pairSet.size(), outputSet[PairSet]);

			for (integer i = 0;i < pairSet.size();++i)
			{
				// The +1 is because Matlab has 1-based indexing.
				outPairSet(0, i) = (pairSet[i].second->point() - modelData) / n + 1;
				outPairSet(1, i) = (pairSet[i].first->point() - sceneData) / n + 1;
			}

			// Output the translation.

			MatrixView<dreal> outTranslation =
				matlabCreateMatrix<dreal>(n, 1, outputSet[Translation]);
			std::copy(match.translation.begin(), match.translation.end(),
				outTranslation.data());

			// Output the bias.

			dreal* outBias = matlabCreateScalar<dreal>(outputSet[Bias]);
			*outBias = match.bias;

			// Output the success flag.

			int32* outSuccess = matlabCreateScalar<int32>(outputSet[Success]);
			*outSuccess = match.success ? 1 : 0;
		}

		void addFunction()
		{
			matlabAddFunction(
				"match_points_kr",
				matlabMatchPointsKr);
		}

		CallFunction run(addFunction);

	}

}
