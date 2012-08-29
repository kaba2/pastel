// Description: point_pattern_match
// Documentation: point_pattern_match.txt

#include "pastel/matlab/matlab.h"

#include "pastel/geometry/point_pattern_matching_vw.h"
#include "pastel/sys/array_pointpolicy.h"

void force_linking_point_pattern_match() {}

namespace Pastel
{

	namespace
	{

		void matlabPointPatternMatch(
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
			const integer modelPoints = 
				mxGetN(inputSet[ModelSet]);
			const real* sceneData = 
				mxGetPr(inputSet[SceneSet]);
			const integer scenePoints = 
				mxGetN(inputSet[SceneSet]);
			const real minMatchRatio = 
				asScalar<real>(inputSet[MinMatchRatio]);
			const real relativeMatchingDistance = 
				asScalar<real>(inputSet[RelativeMatchingDistance]);
			const real confidence =
				asScalar<real>(inputSet[Confidence]);

			ConformalAffine2 similarity;

			const bool success = pointPatternMatch(
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
				Array_PointPolicy2(),
				Array_PointPolicy2());

			// Output the similarity.

			if (outputs > 0)
			{
				RealArrayPtr result =
					createArray<real>(4, 1, outputSet[Similarity]);

				(*result)(0) = similarity.scaling();
				(*result)(1) = similarity.rotation();
				(*result)(2) = similarity.translation().x();
				(*result)(3) = similarity.translation().y();
			}

			// Output the success flag.
			if (outputs > 1)
			{
				integer* outSuccess = createScalar<integer>(outputSet[Success]);
				*outSuccess = success ? 1 : 0;
			}
		}

		void addFunction()
		{
			matlabAddFunction(
				"point_pattern_match",
				matlabPointPatternMatch);
		}

		CallFunction call(addFunction);
	}

}
