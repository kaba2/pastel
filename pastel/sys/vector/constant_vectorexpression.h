// Description: Constant vector-expression
// Documentation: vectorexpression.txt

#ifndef PASTELSYS_CONSTANT_VECTOREXPRESSION_H
#define PASTELSYS_CONSTANT_VECTOREXPRESSION_H

#include "pastel/sys/vector/vectorexpression.h"

namespace Pastel
{

	template <
		typename Real,
		integer N>
	class VectorConstant
		: public VectorExpression<Real, N, VectorConstant<Real, N> >
	{
	public:
		using StorageType = const VectorConstant;

		VectorConstant(
			const Real& that,
			integer size)
			: data_(that)
			, size_(size)
		{
		}

		Real operator[](integer index) const
		{
			unused(index);
			return data_;
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
		Real data_;
		integer size_;
	};

}

#endif
