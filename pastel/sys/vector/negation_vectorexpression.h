// Description: Negation vector-expression
// Documentation: vectorexpression.txt

#ifndef PASTELSYS_NEGATION_VECTOREXPRESSION_H
#define PASTELSYS_NEGATION_VECTOREXPRESSION_H

#include "pastel/sys/negation_vectorexpression.h"

namespace Pastel
{

	template <
		typename Real,
		int N,
		typename Expression>
	class VectorNegation
		: public VectorExpression<Real, N, VectorNegation<Real, N, Expression> >
	{
	public:
		using StorageType = const VectorNegation&;

		explicit VectorNegation(
			const Expression& data)
			: data_(data)
		{
		}

		Real operator[](integer index) const
		{
			return -data_[index];
		}

		integer size() const
		{
			return data_.size();
		}

		bool involves(
			const void* memoryBegin, const void* memoryEnd) const
		{
			return data_.involves(memoryBegin, memoryEnd);
		}

		bool evaluateBeforeAssignment(
			const void* memoryBegin, const void* memoryEnd) const
		{
			return data_.evaluateBeforeAssignment(memoryBegin, memoryEnd);
		}

	private:
		typename Expression::StorageType data_;
	};

}

#endif
