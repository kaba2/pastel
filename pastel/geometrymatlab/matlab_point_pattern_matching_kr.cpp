// Description: match_points_kr
// DocumentationOf: match_points_kr.m

#include "pastel/matlab/pastelmatlab.h"

#include "pastel/geometry/pattern_matching/match_points_kr.h"
#include "pastel/geometry/pointkdtree/pointkdtree.h"
#include "pastel/geometry/splitrule/slidingmidpoint_splitrule.h"

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
				MatchingDistance,
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

			MatchingMode matchingModeSet[] =
			{
				MatchingMode::FirstMatch,
				MatchingMode::MaximumMatch
			};
			integer matchingModes = 
				sizeof(matchingModeSet) / sizeof(MatchingMode);


			const real* modelData = mxGetPr(inputSet[ModelSet]);
			integer modelPoints = mxGetN(inputSet[ModelSet]);

			const real* sceneData = mxGetPr(inputSet[SceneSet]);
			integer scenePoints = mxGetN(inputSet[SceneSet]);
			integer kNearest = matlabAsScalar<integer>(inputSet[KNearest]);
			real minMatchRatio = matlabAsScalar<real>(inputSet[MinMatchRatio]);
			real matchingDistance = 
				matlabAsScalar<real>(inputSet[MatchingDistance]);
			real maxBias =
				matlabAsScalar<real>(inputSet[MaxBias]);
			integer matchingModeId = matlabAsScalar<integer>(inputSet[MatchingModeId]);

			ENSURE_OP(matchingModeId, >=, 0);
			ENSURE_OP(matchingModeId, <, matchingModes);
			
			MatchingMode matchingMode = 
				matchingModeSet[matchingModeId];

			integer modelDimension = mxGetM(inputSet[ModelSet]);
			integer sceneDimension = mxGetM(inputSet[SceneSet]);
			
			ENSURE_OP(modelDimension, ==, sceneDimension);

			integer n = modelDimension;

			using Locator = Pointer_Locator<real>;

			using Settings = PointKdTree_Settings<Locator>;

			using SceneTree = PointKdTree<Settings>;
			using SceneIterator = SceneTree::Point_ConstIterator;

			using ModelTree = PointKdTree<Settings>;
			using ModelIterator = ModelTree::Point_ConstIterator;

			Locator locator(n);

			SceneTree sceneTree(locator);
			sceneTree.insertRange(
				constSparseRange(
				countingIterator(sceneData),

				countingIterator(sceneData + scenePoints * n),
				n));

			ModelTree modelTree(locator);
			modelTree.insertRange(
				constSparseRange(
				countingIterator(modelData),
				countingIterator(modelData + modelPoints * n),
				n));

			sceneTree.refine(SlidingMidpoint_SplitRule());
			modelTree.refine(SlidingMidpoint_SplitRule());

			// Here's where we store the results.

			std::vector<std::pair<SceneIterator, ModelIterator> > pairSet;

			// Compute the point pattern match.

			Euclidean_NormBijection<real> normBijection;
			auto match = Pastel::matchPointsKr(
				modelTree,
				sceneTree, 
				normBijection, 
				pushBackOutput(pairSet),
				[&](auto& p)
			{
				p.kNearest = kNearest;
				p.minMatchRatio = minMatchRatio;
				p.matchingDistance = matchingDistance;
				p.maxBias = maxBias;
				p.matchingMode = matchingMode;
			});

			// Output the pairing.

			Array<int32> outPairSet = matlabCreateArray<int32>(
				Vector2i(pairSet.size(), 2), outputSet[PairSet]);

			for (integer i = 0;i < pairSet.size();++i)
			{
				// The +1 is because Matlab has 1-based indexing.
				outPairSet(i, 0) = (pairSet[i].second->point() - modelData) / n + 1;
				outPairSet(i, 1) = (pairSet[i].first->point() - sceneData) / n + 1;
			}

			// Output the translation.

			Array<real> outTranslation =
				matlabCreateArray<real>(Vector2i(1, n), outputSet[Translation]);
			std::copy(match.translation.begin(), match.translation.end(),
				outTranslation.begin());

			// Output the bias.

			real* outBias = matlabCreateScalar<real>(outputSet[Bias]);
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
