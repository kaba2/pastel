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
			Pi,
			Ri,
			Wi,
			Matrix_,
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

		auto P = matlabAsMatrix<dreal>(inputSet[Pi]);
		auto R = matlabAsMatrix<dreal>(inputSet[Ri]);
		auto W = matlabAsMatrix<dreal>(inputSet[Wi]);

		auto matrix = matlabStringAsEnum<LsAffine_Matrix>(
			inputSet[Matrix_],
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

		integer d = P.rows();

		auto Q = matlabCreateMatrix<dreal>(d, d, outputSet[Qi]);
		auto S = matlabCreateMatrix<dreal>(d, d, outputSet[Si]);
		auto t = matlabCreateColMatrix<dreal>(d, outputSet[Ti]);

		lsAffine(
			P.view(), R.view(), 
			Q, S, t,
			PASTEL_TAG(W), W.view(),
			PASTEL_TAG(matrix), matrix,
			PASTEL_TAG(scaling), scaling,
			PASTEL_TAG(translation), translation,
			PASTEL_TAG(orientation), orientation);
	}

	void addFunction()
	{
		matlabAddFunction(
			"ls_affine",
			matlabLsAffine);
	}

	CallFunction run(addFunction);

}

