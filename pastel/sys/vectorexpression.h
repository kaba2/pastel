// Description: VectorExpression CRTP base class

#ifndef PASTELSYS_VECTOREXPRESSION_H
#define PASTELSYS_VECTOREXPRESSION_H

#include "pastel/sys/vector.h"
#include "pastel/sys/memory_overlaps.h"

namespace Pastel
{

	template <int LeftN, int RightN>
	class ResultN
	{
	public:
		static PASTEL_CONSTEXPR int N = 
			(LeftN == Dynamic) ? RightN : LeftN;
	};

	template <typename Real, int N>
	class Vector;

	template <
		typename Real,
		int N>
	class VectorConstant;

	template <
		typename Real,
		int N,
		typename Expression>
	class VectorNegation;

	template <
		typename Real,
		int N,
		typename LeftExpression,
		typename RightExpression>
	class VectorAddition;

	template <
		typename Real,
		int N,
		typename LeftExpression,
		typename RightExpression>
	class VectorSubtraction;

	template <
		typename Real,
		int N,
		typename LeftExpression,
		typename RightExpression>
	class VectorMultiplication;

	template <
		typename Real,
		int N,
		typename LeftExpression,
		typename RightExpression>
	class VectorDivision;

	template <
		typename Real,
		int N,
		typename Expression>
	class VectorExpression
	{
	protected:
		VectorExpression()
		{
		}

		VectorExpression(const VectorExpression& that)
		{
		}

		VectorExpression& operator=(const VectorExpression& that)
		{
			return *this;
		}

	public:
		Real operator[](integer index) const
		{
			return ((const Expression&)*this)[index];
		}

		integer size() const
		{
			return ((const Expression&)*this).size();
		}

		integer n() const
		{
			return ((const Expression&)*this).size();
		}

		bool involves(
			const void* memoryBegin, const void* memoryEnd) const
		{
			return ((const Expression&)*this).involves(memoryBegin, memoryEnd);
		}

		bool evaluateBeforeAssignment(
			const void* memoryBegin, const void* memoryEnd) const
		{
			return ((const Expression&)*this).evaluateBeforeAssignment(memoryBegin, memoryEnd);
		}

		template <int RightN, typename RightExpression>
		bool operator==(const VectorExpression<Real, RightN, RightExpression>& right) const
		{
			const Expression& left = (const Expression&)*this;

			PENSURE2(left.size() == right.size(), left.size(), right.size());

			integer n = size();
			for (integer i = 0;i < n;++i)
			{
				if (!(left[i] == right[i]))
				{
					return false;
				}
			}
			
			return true;
		}

		template <int RightN, typename RightExpression>

		bool operator!=(const VectorExpression<Real, RightN, RightExpression>& right) const
		{
			return !(*this == right);
		}

		// Negation

		VectorNegation<Real, N, Expression> operator-() const
		{

			return VectorNegation<Real, N, Expression>((const Expression&)*this);
		}

		// Summation

		template <int RightN, typename RightExpression>
		VectorAddition<Real, ResultN<N, RightN>::N, Expression, 
			RightExpression>
			operator+(const VectorExpression

			<Real, RightN, RightExpression>& right) const
		{
			return VectorAddition
				<Real, ResultN<N, RightN>::N, Expression, 
				RightExpression >
				((const Expression&)*this,
				(const RightExpression&)right);
		}

		VectorAddition
			<Real, N, Expression, VectorConstant<Real, N> >

			operator+(const Real& right) const
		{
			return VectorAddition
				<Real, N, Expression, VectorConstant<Real, N> >
				((const Expression&)*this,
				VectorConstant<Real, N>(right, size()));
		}

		friend const VectorAddition
			<Real, N, VectorConstant<Real, N>, Expression>
			operator+(const Real& left, const VectorExpression& right)
		{
			return VectorAddition
				<Real, N, VectorConstant<Real, N>, Expression>
				(VectorConstant<Real, N>(left, right.size()),
				(const Expression&)right);
		}

		// Subtraction

		template <int RightN, typename RightExpression>
		VectorSubtraction<Real, ResultN<N, RightN>::N, Expression, 
			RightExpression>
			operator-(const VectorExpression

			<Real, RightN, RightExpression>& right) const
		{
			return VectorSubtraction
				<Real, ResultN<N, RightN>::N, Expression, 
				RightExpression>
				((const Expression&)*this,
				(const RightExpression&)right);
		}

		VectorSubtraction
			<Real, N, Expression, VectorConstant<Real, N> >

			operator-(const Real& right) const
		{
			return VectorSubtraction
				<Real, N, Expression, VectorConstant<Real, N> >
				((const Expression&)*this,
				VectorConstant<Real, N>(right, size()));
		}

		friend const VectorSubtraction
			<Real, N, VectorConstant<Real, N>, Expression>
			operator-(const Real& left, const VectorExpression& right)
		{
			return VectorSubtraction
				<Real, N, VectorConstant<Real, N>, Expression>
				(VectorConstant<Real, N>(left, right.size()),
				(const Expression&)right);
		}

		// Multiplication

		template <int RightN, typename RightExpression>
		VectorMultiplication<Real, ResultN<N, RightN>::N, Expression, 
			RightExpression>

			operator*(const VectorExpression
			<Real, RightN, RightExpression>& right) const
		{
			return VectorMultiplication
				<Real, ResultN<N, RightN>::N, Expression, 
				RightExpression>
				((const Expression&)*this,
				(const RightExpression&)right);
		}

		VectorMultiplication
			<Real, N, Expression, VectorConstant<Real, N> >

			operator*(const Real& right) const
		{
			return VectorMultiplication
				<Real, N, Expression, VectorConstant<Real, N> >
				((const Expression&)*this,
				VectorConstant<Real, N>(right, size()));
		}

		friend const VectorMultiplication
			<Real, N, VectorConstant<Real, N>, Expression>
			operator*(const Real& left, const VectorExpression& right)
		{
			return VectorMultiplication
				<Real, N, VectorConstant<Real, N>, Expression>
				(VectorConstant<Real, N>(left, right.size()),
				(const Expression&)right);
		}

		// Division

		template <int RightN, typename RightExpression>
		VectorDivision<Real, ResultN<N, RightN>::N, Expression, 
			RightExpression>
			operator/(const VectorExpression

			<Real, RightN, RightExpression>& right) const
		{
			return VectorDivision
				<Real, ResultN<N, RightN>::N, Expression, 
				RightExpression>
				((const Expression&)*this,
				(const RightExpression&)right);
		}

		VectorDivision
			<Real, N, Expression, VectorConstant<Real, N> >

			operator/(const Real& right) const
		{
			// You should not optimize here to use
			// VectorMultiplication with the reciprocal.
			// This is because we also want to support
			// the case where Real is an integer.

			return VectorDivision
				<Real, N, Expression, VectorConstant<Real, N> >
				((const Expression&)*this,
				VectorConstant<Real, N>(right, size()));
		}

		friend const VectorDivision
			<Real, N, VectorConstant<Real, N>, Expression>
			operator/(const Real& left, const VectorExpression& right)
		{
			return VectorDivision
				<Real, N, VectorConstant<Real, N>, Expression>
				(VectorConstant<Real, N>(left, right.size()),
				(const Expression&)right);
		}

		template <int N_ = N>
		EnableIfC<(N_ >= 1), Real> x() const
		{
			return (*this)[0];
		}

		template <int N_ = N>
		EnableIfC<(N_ >= 2), Real> y() const
		{
			return (*this)[1];
		}

		template <int N_ = N>
		EnableIfC<(N_ >= 3), Real> z() const
		{
			return (*this)[2];
		}

		template <int N_ = N>
		EnableIfC<(N_ >= 4), Real> w() const
		{
			return (*this)[3];
		}
	};

}

#include "pastel/sys/addition_vectorexpression.h"
#include "pastel/sys/constant_vectorexpression.h"
#include "pastel/sys/division_vectorexpression.h"
#include "pastel/sys/multiplication_vectorexpression.h"
#include "pastel/sys/negation_vectorexpression.h"
#include "pastel/sys/subtraction_vectorexpression.h"

#include "pastel/sys/array_vectorexpression.h"
#include "pastel/sys/binary_vectorexpression.h"
#include "pastel/sys/unary_vectorexpression.h"
#include "pastel/sys/point_vectorexpression.h"

#endif
