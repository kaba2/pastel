// Description: coherent_point_drift
// DocumentationOf: coherent_point_drift.m

#include "pastel/matlab/pastelmatlab.h"

#include "pastel/geometry/pattern_matching/coherent_point_drift.h"

#include <algorithm>

void force_linking_coherent_point_drift() {};

using namespace Pastel;

namespace
{

	void matlabCoherentPointDrift(
		int outputs, mxArray *outputSet[],
		int inputs, const mxArray *inputSet[])
	{
		enum Input
		{
			Pi,
			Ri,
			NoiseRatio,
			Matrix_,
			Scaling,
			Translation,
			Orientation,
			Qi,
			Si,
			Ti,
			MinIterations,
			MaxIterations,
			MinError,
			Inputs
		};

		enum Output
		{
			Qo,
			So,
			To,
			Sigma2,
			Outputs
		};

		ENSURE_OP(inputs, ==, Inputs);
		ENSURE_OP(outputs, ==, Outputs);

		auto P = matlabAsMatrix<dreal>(inputSet[Pi]);
		auto R = matlabAsMatrix<dreal>(inputSet[Ri]);

		auto matrix = matlabStringAsEnum<Cpd_Matrix>(
			inputSet[Matrix_],
			"free", Cpd_Matrix::Free,
			"identity", Cpd_Matrix::Identity);

		auto scaling = matlabStringAsEnum<Cpd_Scaling>(
			inputSet[Scaling],
			"free", Cpd_Scaling::Free,
			"diagonal", Cpd_Scaling::Diagonal,
			"conformal", Cpd_Scaling::Conformal,
			"rigid", Cpd_Scaling::Rigid);

		auto translation = matlabStringAsEnum<Cpd_Translation>(
			inputSet[Translation],
			"free", Cpd_Translation::Free,
			"identity", Cpd_Translation::Identity);

		integer orientation = 
			matlabAsScalar<integer>(inputSet[Orientation]);

		auto Q = matlabAsMatrix<dreal>(inputSet[Qi]);
		auto S = matlabAsMatrix<dreal>(inputSet[Si]);
		auto t = matlabAsColMatrix<dreal>(inputSet[Ti]);

		integer minIterations =
			matlabAsScalar<integer>(inputSet[MinIterations]);

		integer maxIterations =
			matlabAsScalar<integer>(inputSet[MaxIterations]);

		dreal minError = 
			matlabAsScalar<dreal>(inputSet[MinError]);

		integer d = P.rows();
		integer n = P.cols();
		integer m = R.cols();

		bool qSpecified = !Q.isEmpty();
		if (!qSpecified) {
			Q.resize(d, d);
		}

		bool sSpecified = !S.isEmpty();
		if (!sSpecified) {
			S.resize(d, d);
		}

		bool tSpecified = !t.isEmpty();
		if (!tSpecified) {
			t.resize(d, 1);
		}

		const dreal* q0Pointer = Q.data();
		const dreal* s0Pointer = S.data();
		const dreal* tPointer = t.data();

		dreal sigma2 = coherentPointDrift(
			P.view(), 
			R.view(),
			Q.view(),
			S.view(),
			t.view(),
			PASTEL_TAG(initialize), true,
			PASTEL_TAG(matrix), matrix,
			PASTEL_TAG(scaling), scaling,
			PASTEL_TAG(translation), translation,
			PASTEL_TAG(orientation), orientation,
			PASTEL_TAG(minIterations), minIterations,
			PASTEL_TAG(maxIterations), maxIterations,
			PASTEL_TAG(minError), minError);

		if (qSpecified)
		{
			ENSURE(Q.data() == q0Pointer);
			outputSet[Qo] = (mxArray*)inputSet[Qi];
		}
		else
		{
			matlabCreateMatrix<dreal>(Q.view(), outputSet[Qo]);
		}

		if (sSpecified)
		{
			ENSURE(S.data() == s0Pointer);
			outputSet[So] = (mxArray*)inputSet[Si];
		}
		else
		{
			matlabCreateMatrix<dreal>(S.view(), outputSet[So]);
		}

		if (tSpecified)
		{
			ENSURE(t.data() == tPointer);
			outputSet[To] = (mxArray*)inputSet[Ti];
		}
		else
		{
			matlabCreateMatrix<dreal>(t.view(), outputSet[To]);
		}

		*matlabCreateScalar<dreal>(outputSet[Sigma2]) = sigma2;
	}

	void addFunction()
	{
		matlabAddFunction(
			"coherent_point_drift",
			matlabCoherentPointDrift);
	}

	CallFunction run(addFunction);

}

