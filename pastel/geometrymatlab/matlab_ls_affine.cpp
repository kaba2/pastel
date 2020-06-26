// Description: ls_affine
// DocumentationOf: ls_affine.m

#include "pastel/matlab/pastelmatlab.h"

#include "pastel/geometry/pattern_matching/ls_affine.h"

#include <algorithm>

void force_linking_ls_affine() {};

using namespace Pastel;

namespace
{

	void matlabLsAffine(
		int outputs, mxArray *outputSet[],
		int inputs, const mxArray *inputSet[])
	{
		enum Input
		{
			FromSet,
			ToSet,
			Wi,
			Matrix,
			Scaling,
			Translation,
			Orientation,
			Inputs
		};

		enum Output
		{
			Qi,
			Si,
			Ti,
			Outputs
		};

		ENSURE_OP(inputs, ==, Inputs);
		ENSURE_OP(outputs, ==, Outputs);

		arma::Mat<dreal> fromSet = 
			matlabAsMatrix<dreal>(inputSet[FromSet]);

		arma::Mat<dreal> toSet = 
			matlabAsMatrix<dreal>(inputSet[ToSet]);

		arma::Mat<dreal> W = 
			matlabAsMatrix<dreal>(inputSet[Wi]);

		auto matrix = matlabStringAsEnum<LsAffine_Matrix>(
			inputSet[Matrix],
			"free", LsAffine_Matrix::Free,
			"identity", LsAffine_Matrix::Identity);

		auto scaling = matlabStringAsEnum<LsAffine_Scaling>(
			inputSet[Scaling],
			"free", LsAffine_Scaling::Free,
			"diagonal", LsAffine_Scaling::Diagonal,
			"conformal", LsAffine_Scaling::Conformal,
			"rigid", LsAffine_Scaling::Rigid);

		auto translation = matlabStringAsEnum<LsAffine_Translation>(
			inputSet[Translation],
			"free", LsAffine_Translation::Free,
			"identity", LsAffine_Translation::Identity);

		integer orientation = 
			matlabAsScalar<integer>(inputSet[Orientation]);

		auto match = lsAffine(
			fromSet, toSet,
			PASTEL_TAG(W), W,
			PASTEL_TAG(matrix), matrix,
			PASTEL_TAG(scaling), scaling,
			PASTEL_TAG(translation), translation,
			PASTEL_TAG(orientation), orientation);

		matlabCreateArray<dreal>(match.Q, outputSet[Qi]);
		matlabCreateArray<dreal>(match.S, outputSet[Si]);
		matlabCreateArray<dreal>(match.t, outputSet[Ti]);
	}

	void addFunction()
	{
		matlabAddFunction(
			"ls_affine",
			matlabLsAffine);
	}

	CallFunction run(addFunction);

}

