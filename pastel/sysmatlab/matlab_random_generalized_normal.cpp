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
		real shape = matlabAsScalar<real>(inputSet[Shape]);
		real scale = matlabAsScalar<real>(inputSet[Scale]);

		Array<real> result =
			matlabCreateArray<real>(Vector2i(m, n),
			outputSet[Result]);

		for (real& element : result)
		{
			element = randomGeneralizedGaussian<real>(shape, scale);
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

