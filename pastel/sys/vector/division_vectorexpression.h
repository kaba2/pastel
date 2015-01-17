// Description: Division vector-expression
// Documentation: vectorexpression.txt

#ifndef PASTELSYS_DIVISION_VECTOREXPRESSION_H
#define PASTELSYS_DIVISION_VECTOREXPRESSION_H

#include "pastel/sys/vector/vectorexpression.h"

namespace Pastel
{

	template <
		typename Real,
		int N,
		typename LeftExpression,
		typename RightExpression>
	class VectorDivision
		: public VectorExpression<Real, N, VectorDivision<Real, N, 
		LeftExpression, RightExpression> >
	{
	public:
		using StorageType = const VectorDivision&;

		VectorDivision(
			const LeftExpression& left,
			const RightExpression& right)
			: left_(left)
			, right_(right)
		{
			PENSURE_OP(left.size(), ==, right.size());
		}

		Real operator[](integer index) const
		{
			return left_[index] / right_[index];
		}

		integer size() const
		{
			return left_.size();
		}

		bool involves(
			const void* memoryBegin, const void* memoryEnd) const
		{
			return left_.involves(memoryBegin, memoryEnd) ||
				right_.involves(memoryBegin, memoryEnd);
		}

		bool evaluateBeforeAssignment(
			const void* memoryBegin, const void* memoryEnd) const
		{
			return left_.evaluateBeforeAssignment(memoryBegin, memoryEnd) ||
				right_.evaluateBeforeAssignment(memoryBegin, memoryEnd);
		}

	private:
		typename LeftExpression::StorageType left_;
		typename RightExpression::StorageType right_;
	};

}

#endif
