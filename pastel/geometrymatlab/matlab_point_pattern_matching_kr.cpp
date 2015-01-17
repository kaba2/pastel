// Description: point_pattern_matching_kr
// DocumentationOf: point_pattern_matching_kr.m

#include "pastel/matlab/pastelmatlab.h"

#include "pastel/geometry/point_pattern_matching_kr.h"
#include "pastel/geometry/pointkdtree/pointkdtree.h"
#include "pastel/geometry/splitrules/slidingmidpoint_splitrule.h"

#include "pastel/sys/locator/pointer_locator.h"

void force_linking_point_pattern_matching_kr() {}

namespace Pastel
{

	namespace
	{

		void matlabPointPatternMatchKr(
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
			integer kNearest = asScalar<integer>(inputSet[KNearest]);
			real minMatchRatio = asScalar<real>(inputSet[MinMatchRatio]);
			real matchingDistance = 
				asScalar<real>(inputSet[MatchingDistance]);
			real maxBias =
				asScalar<real>(inputSet[MaxBias]);
			integer matchingModeId = asScalar<integer>(inputSet[MatchingModeId]);

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
			Vector<real> translation(ofDimension(n));
			real bias = 0;

			// Compute the point pattern match.

			Euclidean_NormBijection<real> normBijection;
			auto match = Pastel::pointPatternMatchKr(
				modelTree, sceneTree, kNearest,
				minMatchRatio, matchingDistance, maxBias,
				matchingMode, normBijection, 
				pushBackOutput(pairSet));

			// Output the pairing.

			Array<int32> outPairSet = createArray<int32>(
				Vector2i(pairSet.size(), 2), outputSet[PairSet]);

			for (integer i = 0;i < pairSet.size();++i)
			{
				// The +1 is because Matlab has 1-based indexing.
				outPairSet(i, 0) = (pairSet[i].second->point() - modelData) / n + 1;
				outPairSet(i, 1) = (pairSet[i].first->point() - sceneData) / n + 1;
			}

			// Output the translation.

			Array<real> outTranslation =
				createArray<real>(Vector2i(1, n), outputSet[Translation]);
			std::copy(match.translation.begin(), match.translation.end(),
				outTranslation.begin());

			// Output the bias.

			real* outBias = createScalar<real>(outputSet[Bias]);
			*outBias = match.bias;

			// Output the success flag.

			int32* outSuccess = createScalar<int32>(outputSet[Success]);
			*outSuccess = match.success ? 1 : 0;
		}

		void addFunction()
		{
			matlabAddFunction(
				"point_pattern_matching_kr",
				matlabPointPatternMatchKr);
		}

		CallFunction run(addFunction);

	}

}
