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
			FromSet,
			ToSet,
			NoiseRatio,
			Matrix,
			Scaling,
			Translation,
			Orientation,
			Q0i,
			S0i,
			T0i,
			MinIterations,
			MaxIterations,
			MinError,
			Inputs
		};

		enum Output
		{
			Qi,
			Si,
			Ti,
			Sigma2,
			Outputs
		};

		ENSURE_OP(inputs, ==, Inputs);
		ENSURE_OP(outputs, ==, Outputs);

		arma::Mat<real> fromSet = 
			matlabAsMatrix<real>(inputSet[FromSet]);

		arma::Mat<real> toSet = 
			matlabAsMatrix<real>(inputSet[ToSet]);

		auto matrix = matlabStringAsEnum<Cpd_Matrix>(
			inputSet[Matrix],
			"free", Cpd_Matrix::Free,
			"identity", Cpd_Matrix::Identity);

		auto scaling = matlabStringAsEnum<Cpd_Scaling>(
			inputSet[Scaling],
			"free", Cpd_Scaling::Free,
			"conformal", Cpd_Scaling::Conformal,
			"rigid", Cpd_Scaling::Rigid);

		auto translation = matlabStringAsEnum<Cpd_Translation>(
			inputSet[Translation],
			"free", Cpd_Translation::Free,
			"identity", Cpd_Translation::Identity);

		integer orientation = 
			matlabAsScalar<integer>(inputSet[Orientation]);

		arma::Mat<real> Q0 = 
			matlabAsMatrix<real>(inputSet[Q0i]);

		arma::Mat<real> S0 = 
			matlabAsMatrix<real>(inputSet[S0i]);

		arma::Mat<real> t0 = 
			matlabAsMatrix<real>(inputSet[T0i]);

		integer minIterations =
			matlabAsScalar<integer>(inputSet[MinIterations]);

		integer maxIterations =
			matlabAsScalar<integer>(inputSet[MaxIterations]);

		real minError = 
			matlabAsScalar<real>(inputSet[MinError]);

		auto* q0Pointer = Q0.memptr();

		auto match = coherentPointDrift(
			std::move(fromSet), 
			std::move(toSet),
			PASTEL_TAG(matrix), matrix,
			PASTEL_TAG(scaling), scaling,
			PASTEL_TAG(translation), translation,
			PASTEL_TAG(orientation), orientation,
			PASTEL_TAG(Q0), std::move(Q0),
			PASTEL_TAG(S0), std::move(S0),
			PASTEL_TAG(t0), std::move(t0),
			PASTEL_TAG(minIterations), minIterations,
			PASTEL_TAG(maxIterations), maxIterations,
			PASTEL_TAG(minError), minError);

		// Make sure memory was not reallocated.
		ENSURE(match.Q.memptr() == q0Pointer);

		outputSet[Qi] = (mxArray*)inputSet[Q0i];
		outputSet[Si] = (mxArray*)inputSet[S0i];
		outputSet[Ti] = (mxArray*)inputSet[T0i];
		*matlabCreateScalar<real>(outputSet[Sigma2]) = match.sigma2;
	}

	void addFunction()
	{
		matlabAddFunction(
			"coherent_point_drift",
			matlabCoherentPointDrift);
	}

	CallFunction run(addFunction);

}

