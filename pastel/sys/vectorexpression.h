// Description: VectorExpression CRTP base class

#ifndef PASTEL_VECTOREXPRESSION_H
#define PASTEL_VECTOREXPRESSION_H

#include "pastel/sys/vectorbase.h"
#include "pastel/sys/memory_overlaps.h"

namespace Pastel
{

	template <int LeftN, int RightN>
	class ResultN
	{
	public:
		enum
		{
			N = (LeftN == Dynamic) ? RightN : LeftN
		};
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

		integer dimension() const
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

			const integer n = size();
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

		const VectorNegation<Real, N, Expression> operator-() const
		{
			return VectorNegation<Real, N, Expression>((const Expression&)*this);
		}

		// Summation

		template <int RightN, typename RightExpression>
		const VectorAddition<Real, ResultN<N, RightN>::N, Expression, 
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

		const VectorAddition
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
		const VectorSubtraction<Real, ResultN<N, RightN>::N, Expression, 
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

		const VectorSubtraction
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
		const VectorMultiplication<Real, ResultN<N, RightN>::N, Expression, 
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

		const VectorMultiplication
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
		const VectorDivision<Real, ResultN<N, RightN>::N, Expression, 
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

		const VectorDivision
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
	};

	// Concrete expressions

	template <
		typename Real,
		int N>
	class VectorConstant
		: public VectorExpression<Real, N, VectorConstant<Real, N> >
	{
	public:
		typedef const VectorConstant StorageType;

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
		const Real data_;
		const integer size_;
	};

	template <
		typename Real,
		int N,
		typename Expression>
	class VectorNegation
		: public VectorExpression<Real, N, VectorNegation<Real, N, Expression> >
	{
	public:
		typedef const VectorNegation& StorageType;

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

	template <
		typename Real,
		int N,
		typename LeftExpression,
		typename RightExpression>
	class VectorAddition
		: public VectorExpression<Real, N, VectorAddition<Real, N, 
		LeftExpression, RightExpression> >
	{
	public:
		typedef const VectorAddition& StorageType;

		VectorAddition(
			const LeftExpression& left,
			const RightExpression& right)
			: left_(left)
			, right_(right)
		{
			PENSURE_OP(left.size(), ==, right.size());
		}

		Real operator[](integer index) const
		{
			return left_[index] + right_[index];
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

	template <
		typename Real,
		int N,
		typename LeftExpression,
		typename RightExpression>
	class VectorSubtraction
		: public VectorExpression<Real, N, VectorSubtraction<Real, N, 
		LeftExpression, RightExpression> >
	{
	public:
		typedef const VectorSubtraction& StorageType;

		VectorSubtraction(
			const LeftExpression& left,
			const RightExpression& right)
			: left_(left)
			, right_(right)
		{
			PENSURE_OP(left.size(), ==, right.size());
		}

		Real operator[](integer index) const
		{
			return left_[index] - right_[index];
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

	template <
		typename Real,
		int N,
		typename LeftExpression,
		typename RightExpression>
	class VectorMultiplication
		: public VectorExpression<Real, N, VectorMultiplication<Real, N, 
		LeftExpression, RightExpression> >
	{
	public:
		typedef const VectorMultiplication& StorageType;

		VectorMultiplication(
			const LeftExpression& left,
			const RightExpression& right)
			: left_(left)
			, right_(right)
		{
			PENSURE_OP(left.size(), ==, right.size());
		}

		Real operator[](integer index) const
		{
			return left_[index] * right_[index];
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
		typedef const VectorDivision& StorageType;

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

#include "pastel/sys/array_vectorexpression.h"
#include "pastel/sys/binary_vectorexpression.h"
#include "pastel/sys/unary_vectorexpression.h"

#endif
