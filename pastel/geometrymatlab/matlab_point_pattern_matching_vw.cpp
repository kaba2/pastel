// Description: match_points_vw
// Documentation: match_points_vw.txt

#include "pastel/matlab/pastelmatlab.h"

#include "pastel/geometry/pattern_matching/match_points_vw.h"
#include "pastel/sys/locator/pointer_locator.h"

void force_linking_match_points_vw() {}

namespace Pastel
{

	namespace
	{

		void matlabMatchPointsVw(
			int outputs, mxArray *outputSet[],
			int inputs, const mxArray *inputSet[])
		{
			enum
			{
				ModelSet,
				SceneSet,
				MinMatchRatio,
				RelativeMatchingDistance,
				Confidence,
				Inputs
			};

			enum
			{
				Similarity,
				Success,
				Outputs
			};

			ENSURE_OP(inputs, ==, Inputs);

			const real* modelData = 
				mxGetPr(inputSet[ModelSet]);
			integer modelPoints = 
				mxGetN(inputSet[ModelSet]);

			const real* sceneData = 
				mxGetPr(inputSet[SceneSet]);
			integer scenePoints = 
				mxGetN(inputSet[SceneSet]);
			real minMatchRatio = 
				matlabAsScalar<real>(inputSet[MinMatchRatio]);
			real relativeMatchingDistance = 
				matlabAsScalar<real>(inputSet[RelativeMatchingDistance]);
			real confidence =
				matlabAsScalar<real>(inputSet[Confidence]);

			ConformalAffine2D<real> similarity;

			bool success = pointPatternMatchVw(
				range(
				constSparseIterator(countingIterator(sceneData), 2), 
				scenePoints),
				range(
				constSparseIterator(countingIterator(modelData), 2), 
				modelPoints),
				minMatchRatio,
				relativeMatchingDistance,
				confidence,
				similarity,
				Pointer_Locator<real, 2>(),
				Pointer_Locator<real, 2>());

			// Output the similarity.

			if (outputs > 0)
			{
				Array<real> result =
					matlabCreateArray<real>(4, 1, outputSet[Similarity]);

				result(0) = similarity.scaling();
				result(1) = similarity.rotation();
				result(2) = similarity.translation()[0];
				result(3) = similarity.translation()[1];
			}

			// Output the success flag.
			if (outputs > 1)
			{

				integer* outSuccess = matlabCreateScalar<integer>(outputSet[Success]);
				*outSuccess = success ? 1 : 0;
			}
		}

		void addFunction()
		{
			matlabAddFunction(
				"match_points_vw",
				matlabMatchPointsVw);
		}

		CallFunction call(addFunction);
	}

}
