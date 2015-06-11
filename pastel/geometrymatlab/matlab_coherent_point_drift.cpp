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

		arma::Col<real> t0 = 
			matlabAsMatrix<real>(inputSet[T0i]);

		integer minIterations =
			matlabAsScalar<integer>(inputSet[MinIterations]);

		integer maxIterations =
			matlabAsScalar<integer>(inputSet[MaxIterations]);

		real minError = 
			matlabAsScalar<real>(inputSet[MinError]);

		bool qSpecified = !Q0.is_empty();
		bool sSpecified = !S0.is_empty();
		bool tSpecified = !t0.is_empty();

		const real* q0Pointer = Q0.memptr();
		const real* s0Pointer = S0.memptr();
		const real* t0Pointer = t0.memptr();

		auto match = coherentPointDrift(
			std::move(fromSet), 
			std::move(toSet),
			matrix,
			scaling,
			translation,
			PASTEL_TAG(orientation), orientation,
			PASTEL_TAG(Q0), std::move(Q0),
			PASTEL_TAG(S0), std::move(S0),
			PASTEL_TAG(t0), std::move(t0),
			PASTEL_TAG(minIterations), minIterations,
			PASTEL_TAG(maxIterations), maxIterations,
			PASTEL_TAG(minError), minError);

		if (qSpecified)
		{
			ENSURE(match.Q.memptr() == q0Pointer);
			outputSet[Qi] = (mxArray*)inputSet[Q0i];
		}
		else
		{
			matlabCreateArray<real>(match.Q, outputSet[Qi]);
		}

		if (sSpecified)
		{
			ENSURE(match.S.memptr() == s0Pointer);
			outputSet[Si] = (mxArray*)inputSet[S0i];
		}
		else
		{
			matlabCreateArray<real>(match.S, outputSet[Si]);
		}

		if (tSpecified)
		{
			ENSURE(match.t.memptr() == t0Pointer);
			outputSet[Ti] = (mxArray*)inputSet[T0i];
		}
		else
		{
			matlabCreateArray<real>(match.t, outputSet[Ti]);
		}

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

