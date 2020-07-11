// Description: random_orthogonal
// DocumentationOf: random_orthogonal.m

#include "pastel/matlab/pastelmatlab.h"

#include "pastel/math/sampling/random_orthogonal.h"

#include <algorithm>

void force_linking_random_orthogonal() {};

using namespace Pastel;

namespace
{

	void matlabRandomOrthogonal(
		int outputs, mxArray *outputSet[],
		int inputs, const mxArray *inputSet[])
	{
		enum Input
		{
			N,
			Orientation,
			Inputs
		};

		enum Output
		{
			Qi,
			Outputs
		};

		ENSURE_OP(inputs, ==, Inputs);
		ENSURE_OP(outputs, ==, Outputs);

		integer n = matlabAsScalar<integer>(inputSet[N]);
		integer orientation = matlabAsScalar<integer>(inputSet[Orientation]);

		Matrix<dreal> Q_ = randomOrthogonal<dreal>(n, 
			PASTEL_TAG(orientation), orientation);

		MatrixView<dreal> Q = matlabCreateMatrix<dreal>(view(Q_), outputSet[Qi]);
	}

	void addFunction()
	{
		matlabAddFunction(
			"random_orthogonal",
			matlabRandomOrthogonal);
	}

	CallFunction run(addFunction);

}

