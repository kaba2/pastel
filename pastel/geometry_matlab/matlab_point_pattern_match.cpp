// Description: point_pattern_match

#include "pastel/geometry_matlab/pastelgeometry_matlab.h"

#include "pastel/geometry/point_pattern_matching.h"
#include "pastel/geometry/array_pointpolicy.h"

using namespace Pastel;

namespace
{

	void matlabPointPatternMatch(
		int outputs, mxArray *outputSet[],
		int inputs, const mxArray *inputSet[])
	{
		enum
		{
			modelIndex,
			sceneIndex,
			minMatchRatioIndex,
			relativeMatchingDistanceIndex,
			confidenceIndex
		};

		const real* modelData = mxGetPr(inputSet[modelIndex]);
		const integer modelPoints = mxGetN(inputSet[modelIndex]);
		const real* sceneData = mxGetPr(inputSet[sceneIndex]);
		const integer scenePoints = mxGetN(inputSet[modelIndex]);
		const real minMatchRatio = getReal(inputSet[minMatchRatioIndex]);
		const real relativeMatchingDistance = 
			getReal(inputSet[relativeMatchingDistanceIndex]);
		const real confidence =
			getReal(inputSet[confidenceIndex]);

		//const integer threads = getInteger(inputSet[threadsIndex]);
		//setNumberOfThreads(threads);

		ConformalAffine2 similarity;

		const bool success = pointPatternMatch(
			randomAccessRange(
			constSparseIterator(countingIterator(sceneData), 2), 
			scenePoints),
			randomAccessRange(
			constSparseIterator(countingIterator(modelData), 2), 
			modelPoints),
			minMatchRatio,
			relativeMatchingDistance,
			confidence,
			similarity,
			Array_PointPolicy<real>(),
			Array_PointPolicy<real>());

		// Output the similarity.
			
		outputSet[0] = mxCreateDoubleMatrix(1, 4, mxREAL);
		real* rawResult = mxGetPr(outputSet[0]);

		rawResult[0] = similarity.scaling();
		rawResult[1] = similarity.rotation();
		rawResult[2] = similarity.translation().x();
		rawResult[3] = similarity.translation().y();

		// Output the success flag.
		
		outputSet[1] = mxCreateDoubleMatrix(1, 1, mxREAL);
		real* rawSuccess = mxGetPr(outputSet[1]);
		*rawSuccess = success ? 1 : 0;
	}

	void addFunction()
	{
		geometryMatlabAddFunction(
			"point_pattern_match",
			matlabPointPatternMatch);
	}

	CallFunction run(addFunction);

}
