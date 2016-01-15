// Description: Lambda-function vector-expression
// Documentation: vectorexpression.txt

#ifndef PASTELSYS_LAMBDA_VECTOREXPRESSION_H
#define PASTELSYS_LAMBDA_VECTOREXPRESSION_H

#include "pastel/sys/vector/vectorexpression.h"

namespace Pastel
{

	template <
		typename Real,
		integer N,
		typename Function>
	class VectorLambda
		: public VectorExpression<Real, N, VectorLambda<Real, N, Function>>
	{
	public:
		using StorageType = const VectorLambda;

		VectorLambda(
			const Function& function,
			integer size)
			: function_(function)
			, size_(size)
		{
		}

		Real operator[](integer index) const
		{
			unused(index);
			return function_(index);
		}

		integer size() const
		{
			return size_;
		}

		bool involves(
			const void* memoryBegin, const void* memoryEnd) const
		{
			return false;
		}

		bool evaluateBeforeAssignment(
			const void* memoryBegin, const void* memoryEnd) const
		{
			return false;
		}

	private:
		Function function_;
		integer size_;
	};

	template <
		typename Real,
		integer N = Dynamic,
		typename Function>
	decltype(auto) lambdaVector(
		const Function& function,
		integer size)
	{
		return VectorLambda<Real, N, Function>(function, size);
	}

}

#endif
