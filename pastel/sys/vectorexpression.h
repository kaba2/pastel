#ifndef PASTEL_VECTOREXPRESSION_H
#define PASTEL_VECTOREXPRESSION_H

#include "pastel/sys/vectorbase.h"

namespace Pastel
{

	template <
		int N,
		typename Real>
	class VectorConstant;

	template <
		int N,
		typename Real,
		typename Expression>
	class VectorNegation;

	template <
		int N,
		typename Real,
		typename LeftExpression,
		typename RightExpression>
	class VectorAddition;

	template <
		int N,
		typename Real,
		typename LeftExpression,
		typename RightExpression>
	class VectorSubtraction;

	template <
		int N,
		typename Real,
		typename LeftExpression,
		typename RightExpression>
	class VectorMultiplication;

	template <
		int N,
		typename Real,
		typename LeftExpression,
		typename RightExpression>
	class VectorDivision;

	template <
		int N,
		typename Real,
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
			void* address) const
		{
			return ((const Expression&)*this).involves(address);
		}

		bool involvesNonTrivially(
			void* address) const
		{
			return ((const Expression&)*this).involvesNonTrivially(address);
		}

		template <typename RightExpression>
		bool operator==(const VectorExpression<N, Real, RightExpression>& right) const
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

		template <typename RightExpression>
		bool operator!=(const VectorExpression<N, Real, RightExpression>& right) const
		{
			return !(*this == right);
		}

		// Negation

		const VectorNegation<N, Real, Expression> operator-() const
		{
			return VectorNegation<N, Real, Expression>((const Expression&)*this);
		}

		// Summation

		template <typename RightExpression>
		const VectorAddition<N, Real, Expression,
			RightExpression>
			operator+(const VectorExpression
			<N, Real, RightExpression>& right) const
		{
			return VectorAddition
				<N, Real, Expression,
				RightExpression >
				((const Expression&)*this,
				(const RightExpression&)right);
		}

		const VectorAddition
			<N, Real, Expression, VectorConstant<N, Real> >
			operator+(const Real& right) const
		{
			return VectorAddition
				<N, Real, Expression, VectorConstant<N, Real> >
				((const Expression&)*this,
				VectorConstant<N, Real>(right, size()));
		}

		friend const VectorAddition
			<N, Real, VectorConstant<N, Real>, Expression>
			operator+(const Real& left, const VectorExpression& right)
		{
			return VectorAddition
				<N, Real, VectorConstant<N, Real>, Expression>
				(VectorConstant<N, Real>(left, right.size()),
				(const Expression&)right);
		}

		// Subtraction

		template <typename RightExpression>
		const VectorSubtraction<N, Real, Expression,
			RightExpression>
			operator-(const VectorExpression
			<N, Real, RightExpression>& right) const
		{
			return VectorSubtraction
				<N, Real, Expression,
				RightExpression>
				((const Expression&)*this,
				(const RightExpression&)right);
		}

		const VectorSubtraction
			<N, Real, Expression, VectorConstant<N, Real> >
			operator-(const Real& right) const
		{
			return VectorSubtraction
				<N, Real, Expression, VectorConstant<N, Real> >
				((const Expression&)*this,
				VectorConstant<N, Real>(right, size()));
		}

		friend const VectorSubtraction
			<N, Real, VectorConstant<N, Real>, Expression>
			operator-(const Real& left, const VectorExpression& right)
		{
			return VectorSubtraction
				<N, Real, VectorConstant<N, Real>, Expression>
				(VectorConstant<N, Real>(left, right.size()),
				(const Expression&)right);
		}

		// Multiplication

		template <typename RightExpression>
		const VectorMultiplication<N, Real, Expression,
			RightExpression>
			operator*(const VectorExpression
			<N, Real, RightExpression>& right) const
		{
			return VectorMultiplication
				<N, Real, Expression,
				RightExpression>
				((const Expression&)*this,
				(const RightExpression&)right);
		}

		const VectorMultiplication
			<N, Real, Expression, VectorConstant<N, Real> >
			operator*(const Real& right) const
		{
			return VectorMultiplication
				<N, Real, Expression, VectorConstant<N, Real> >
				((const Expression&)*this,
				VectorConstant<N, Real>(right, size()));
		}

		friend const VectorMultiplication
			<N, Real, VectorConstant<N, Real>, Expression>
			operator*(const Real& left, const VectorExpression& right)
		{
			return VectorMultiplication
				<N, Real, VectorConstant<N, Real>, Expression>
				(VectorConstant<N, Real>(left, right.size()),
				(const Expression&)right);
		}

		// Division

		template <typename RightExpression>
		const VectorDivision<N, Real, Expression,
			RightExpression>
			operator/(const VectorExpression
			<N, Real, RightExpression>& right) const
		{
			return VectorDivision
				<N, Real, Expression,
				RightExpression>
				((const Expression&)*this,
				(const RightExpression&)right);
		}

		const VectorDivision
			<N, Real, Expression, VectorConstant<N, Real> >
			operator/(const Real& right) const
		{
			// You should not optimize here to use
			// VectorMultiplication with the reciprocal.
			// This is because we also want to support
			// the case where Real is an integer.

			return VectorDivision
				<N, Real, Expression, VectorConstant<N, Real> >
				((const Expression&)*this,
				VectorConstant<N, Real>(right, size()));
		}

		friend const VectorDivision
			<N, Real, VectorConstant<N, Real>, Expression>
			operator/(const Real& left, const VectorExpression& right)
		{
			return VectorDivision
				<N, Real, VectorConstant<N, Real>, Expression>
				(VectorConstant<N, Real>(left, right.size()),
				(const Expression&)right);
		}
	};

	// Concrete expressions

	template <
		int N,
		typename Real>
	class VectorConstant
		: public VectorExpression<N, Real,
		VectorConstant<N, Real> >
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
			void* address) const
		{
			return this == address;
		}

		bool involvesNonTrivially(
			void* address) const
		{
			return false;
		}

	private:
		const Real data_;
		const integer size_;
	};

	template <
		int N,
		typename Real,
		typename Expression>
	class VectorNegation
		: public VectorExpression<N, Real,
		VectorNegation<N, Real, Expression> >
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
			void* address) const
		{
			return this == address ||
				data_.involves(address);
		}

		bool involvesNonTrivially(
			void* address) const
		{
			return data_.involvesNonTrivially(address);
		}

	private:
		typename Expression::StorageType data_;
	};

	template <
		int N,
		typename Real,
		typename LeftExpression,
		typename RightExpression>
	class VectorAddition
		: public VectorExpression<N, Real,
		VectorAddition<N, Real,
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
			PENSURE2(N != Dynamic || left.size() == right.size(), 
				left.size(), right.size());
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
			void* address) const
		{
			return this == address ||
				left_.involves(address) ||
				right_.involves(address);
		}

		bool involvesNonTrivially(
			void* address) const
		{
			return left_.involvesNonTrivially(address) ||
				right_.involvesNonTrivially(address);
		}

	private:
		typename LeftExpression::StorageType left_;
		typename RightExpression::StorageType right_;
	};

	template <
		int N,
		typename Real,
		typename LeftExpression,
		typename RightExpression>
	class VectorSubtraction
		: public VectorExpression<N, Real,
		VectorSubtraction<N, Real,
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
			PENSURE2(N != Dynamic || left.size() == right.size(), 
				left.size(), right.size());
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
			void* address) const
		{
			return this == address ||
				left_.involves(address) ||
				right_.involves(address);
		}

		bool involvesNonTrivially(
			void* address) const
		{
			return left_.involvesNonTrivially(address) ||
				right_.involvesNonTrivially(address);
		}

	private:
		typename LeftExpression::StorageType left_;
		typename RightExpression::StorageType right_;
	};

	template <
		int N,
		typename Real,
		typename LeftExpression,
		typename RightExpression>
	class VectorMultiplication
		: public VectorExpression<N, Real,
		VectorMultiplication<N, Real,
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
			PENSURE2(N != Dynamic || left.size() == right.size(), 
				left.size(), right.size());
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
			void* address) const
		{
			return this == address ||
				left_.involves(address) ||
				right_.involves(address);
		}

		bool involvesNonTrivially(
			void* address) const
		{
			return left_.involvesNonTrivially(address) ||
				right_.involvesNonTrivially(address);
		}

	private:
		typename LeftExpression::StorageType left_;
		typename RightExpression::StorageType right_;
	};

	template <
		int N,
		typename Real,
		typename LeftExpression,
		typename RightExpression>
	class VectorDivision
		: public VectorExpression<N, Real,
		VectorDivision<N, Real,
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
			PENSURE2(N != Dynamic || left.size() == right.size(), 
				left.size(), right.size());
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
			void* address) const
		{
			return this == address ||
				left_.involves(address) ||
				right_.involves(address);
		}

		bool involvesNonTrivially(
			void* address) const
		{
			return left_.involvesNonTrivially(address) ||
				right_.involvesNonTrivially(address);
		}

	private:
		typename LeftExpression::StorageType left_;
		typename RightExpression::StorageType right_;
	};

}

#endif
