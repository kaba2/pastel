// Description: random_generalized_normal
// DocumentationOf: random_generalized_normal.m

#include "pastel/matlab/pastelmatlab.h"

#include "pastel/sys/random/random_generalizedgaussian.h"

void force_linking_random_generalized_normal() {};

using namespace Pastel;

namespace
{

	void matlabRandomGeneralizedNormal(
		int outputs, mxArray *outputSet[],
		int inputs, const mxArray *inputSet[])
	{
		enum Input
		{
			M,
			N,
			Shape,
			Scale,
			Inputs
		};

		enum Output
		{
			Result,
			Outputs
		};

		ENSURE_OP(inputs, ==, Inputs);
		ENSURE_OP(outputs, ==, Outputs);

		integer m = matlabAsScalar<integer>(inputSet[M]);
		integer n = matlabAsScalar<integer>(inputSet[N]);
		dreal shape = matlabAsScalar<dreal>(inputSet[Shape]);
		dreal scale = matlabAsScalar<dreal>(inputSet[Scale]);

		MatrixView<dreal> result = matlabCreateMatrix<dreal>(m, n, outputSet[Result]);

		for (integer i = 0; i < result.size(); ++i)
		{
			result(i) = randomGeneralizedGaussian<dreal>(shape, scale);
		}
	}

	void addFunction()
	{
		matlabAddFunction(
			"random_generalized_normal",
			matlabRandomGeneralizedNormal);
	}

	CallFunction run(addFunction);

}

