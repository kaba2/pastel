// Description: point_pattern_match
// Documentation: point_pattern_match.txt

#include "pastel/matlab/matlab.h"

#include "pastel/geometry/point_pattern_match_gmo.h"
#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/slidingmidpoint_splitrule_pointkdtree.h"

#include "pastel/sys/array_pointpolicy.h"

void force_linking_point_pattern_match_gmo() {}

namespace Pastel
{

	namespace
	{

		void matlabPointPatternMatchGmo(
			int outputs, mxArray *outputSet[],
			int inputs, const mxArray *inputSet[])
		{
			enum
			{
				modelIndex,
				sceneIndex,
				minMatchRatioIndex,
				matchingDistanceIndex,
				confidenceIndex
			};

			const real* modelData = mxGetPr(inputSet[modelIndex]);
			const integer modelPoints = mxGetN(inputSet[modelIndex]);
			const real* sceneData = mxGetPr(inputSet[sceneIndex]);
			const integer scenePoints = mxGetN(inputSet[sceneIndex]);
			const real minMatchRatio = asScalar<real>(inputSet[minMatchRatioIndex]);
			const real matchingDistance = 
				asScalar<real>(inputSet[matchingDistanceIndex]);
			const real confidence =
				asScalar<real>(inputSet[confidenceIndex]);

			const integer n = mxGetM(inputSet[modelIndex]);

			typedef PointKdTree<real, Dynamic, Array_PointPolicy<real> > SceneTree;
			typedef SceneTree::Point_ConstIterator SceneIterator;

			typedef PointKdTree<real, Dynamic, Array_PointPolicy<real> > ModelTree;
			typedef ModelTree::Point_ConstIterator ModelIterator;

			Array_PointPolicy<real> pointPolicy(n);

			SceneTree sceneTree(pointPolicy);
			sceneTree.insert(
				constSparseRange(
				countingIterator(sceneData),
				countingIterator(sceneData + scenePoints * n),
				n));

			//constSparseIterator(countingIterator(sceneData), n), 
			//constSparseIterator(countingIterator(sceneData), n) + scenePoints);

			ModelTree modelTree(pointPolicy);
			modelTree.insert(
				constSparseRange(
				countingIterator(modelData),
				countingIterator(modelData + modelPoints * n),
				n));

			//constSparseIterator(countingIterator(modelData), n),
			//constSparseIterator(countingIterator(modelData), n) + modelPoints);

			sceneTree.refine(SlidingMidpoint_SplitRule_PointKdTree());
			modelTree.refine(SlidingMidpoint_SplitRule_PointKdTree());

			// Here's where we store the results.

			std::vector<std::pair<SceneIterator, ModelIterator> > pairSet;
			Vector<real> translation(ofDimension(n));

			// Compute the point pattern match.

			const bool success = Pastel::pointPatternMatchGmo(
				modelTree, sceneTree, 
				minMatchRatio, matchingDistance,
				confidence, translation, std::back_inserter(pairSet));

			// Output the pairing.

			outputSet[0] = mxCreateDoubleMatrix(2, pairSet.size(), mxREAL);
			real* rawPair = mxGetPr(outputSet[0]);

			for (integer i = 0;i < pairSet.size();++i)
			{
				//rawPair[i * 2] = pairSet[i].first == sceneTree.end() ? 1 : 0;
				//rawPair[i * 2 + 1] = pairSet[i].second == modelTree.end() ? -1 : +1;

				rawPair[i * 2] = (pairSet[i].first->point() - sceneData) / n;
				rawPair[i * 2 + 1] = (pairSet[i].second->point() - modelData) / n;
			}

			// Output the translation.

			outputSet[1] = mxCreateDoubleMatrix(n, 1, mxREAL);
			real* rawTranslation = mxGetPr(outputSet[1]);

			std::copy(translation.begin(), translation.end(),
				rawTranslation);

			// Output the success flag.

			outputSet[2] = mxCreateDoubleMatrix(1, 1, mxREAL);
			real* rawSuccess = mxGetPr(outputSet[2]);
			*rawSuccess = success ? 1 : 0;
		}

		void addFunction()
		{
			matlabAddFunction(
				"point_pattern_match_gmo",
				matlabPointPatternMatchGmo);
		}

		CallFunction run(addFunction);

	}

}
