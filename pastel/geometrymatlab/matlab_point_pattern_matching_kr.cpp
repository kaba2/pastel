// Description: point_pattern_matching_kr
// DocumentationOf: point_pattern_matching_kr.m

#include "pastel/matlab/pastelmatlab.h"

#include "pastel/geometry/point_pattern_matching_kr.h"
#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/slidingmidpoint_splitrule.h"

#include "pastel/sys/array_pointpolicy.h"

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
				MatchingMode,
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

			MatchingMode::Enum matchingModeSet[] =
			{
				MatchingMode::FirstMatch,
				MatchingMode::MaximumMatch
			};
			const integer matchingModes = 
				sizeof(matchingModeSet) / sizeof(MatchingMode::Enum);

			const real* modelData = mxGetPr(inputSet[ModelSet]);
			const integer modelPoints = mxGetN(inputSet[ModelSet]);
			const real* sceneData = mxGetPr(inputSet[SceneSet]);
			const integer scenePoints = mxGetN(inputSet[SceneSet]);
			const integer kNearest = asScalar<integer>(inputSet[KNearest]);
			const real minMatchRatio = asScalar<real>(inputSet[MinMatchRatio]);
			const real matchingDistance = 
				asScalar<real>(inputSet[MatchingDistance]);
			const real maxBias =
				asScalar<real>(inputSet[MaxBias]);
			const integer matchingModeId = asScalar<integer>(inputSet[MatchingMode]);

			ENSURE_OP(matchingModeId, >=, 0);
			ENSURE_OP(matchingModeId, <, matchingModes);
			
			const MatchingMode::Enum matchingMode = 
				matchingModeSet[matchingModeId];

			const integer modelDimension = mxGetM(inputSet[ModelSet]);
			const integer sceneDimension = mxGetM(inputSet[SceneSet]);
			
			ENSURE_OP(modelDimension, ==, sceneDimension);

			const integer n = modelDimension;

			typedef PointKdTree<real, Dynamic, Array_PointPolicy<real> > SceneTree;
			typedef SceneTree::Point_ConstIterator SceneIterator;

			typedef PointKdTree<real, Dynamic, Array_PointPolicy<real> > ModelTree;
			typedef ModelTree::Point_ConstIterator ModelIterator;

			Array_PointPolicy<real> pointPolicy(n);

			SceneTree sceneTree(pointPolicy);
			sceneTree.insertRange(
				constSparseRange(
				countingIterator(sceneData),
				countingIterator(sceneData + scenePoints * n),
				n));

			ModelTree modelTree(pointPolicy);
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
				pushBackReporter(pairSet));

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
