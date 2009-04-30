#ifndef PASTELSYS_VECTOREXPRESSION_H
#define PASTELSYS_VECTOREXPRESSION_H

#include "pastel/sys/vectorbase.h"

namespace Pastel
{

	template <int N, typename Real,
		template <int, typename> class DerivedT>
	class VectorBase;

	template <typename Type>
	class StorageType
	{
	public:
		typedef const Type Result;
	};

	template <int N, typename Real,
		template <int, typename> class DerivedT>
	class StorageType<VectorBase<N, Real, DerivedT> >
	{
	public:
		typedef const VectorBase<N, Real, DerivedT>& Result;
	};

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
			const Expression& expression =
				(const Expression&)*this;
			return expression[index];
		}

		integer size() const
		{
			const Expression& expression =
				(const Expression&)*this;
			return expression.size();
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
			const Expression& expression =
				(Expression&)data_;

			return data_.size();
		}

	private:
		typedef typename StorageType<Expression>::
			Result ExpressionType;

		ExpressionType data_;
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
		VectorAddition(
			const LeftExpression& left,
			const RightExpression& right)
			: left_(left)
			, right_(right)
		{
			PENSURE2(N != Unbounded || left.size() == right.size(), 
				left.size(), right.size());
		}

		Real operator[](integer index) const
		{
			return left_[index] + right_[index];
		}

		integer size() const
		{
			const LeftExpression& expression =
				(LeftExpression&)left_;
			return expression.size();
		}

	private:
		typedef typename StorageType<LeftExpression>::
			Result LeftType;
		typedef typename StorageType<RightExpression>::
			Result RightType;

		LeftType left_;
		RightType right_;
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
		VectorSubtraction(
			const LeftExpression& left,
			const RightExpression& right)
			: left_(left)
			, right_(right)
		{
			PENSURE2(N != Unbounded || left.size() == right.size(), 
				left.size(), right.size());
		}

		Real operator[](integer index) const
		{
			return left_[index] - right_[index];
		}

		integer size() const
		{
			const LeftExpression& expression =
				(LeftExpression&)left_;
			return expression.size();
		}

	private:
		typedef typename StorageType<LeftExpression>::
			Result LeftType;
		typedef typename StorageType<RightExpression>::
			Result RightType;

		LeftType left_;
		RightType right_;
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
		VectorMultiplication(
			const LeftExpression& left,
			const RightExpression& right)
			: left_(left)
			, right_(right)
		{
			PENSURE2(N != Unbounded || left.size() == right.size(), 
				left.size(), right.size());
		}

		Real operator[](integer index) const
		{
			return left_[index] * right_[index];
		}

		integer size() const
		{
			const LeftExpression& expression =
				(LeftExpression&)left_;
			return expression.size();
		}

	private:
		typedef typename StorageType<LeftExpression>::
			Result LeftType;
		typedef typename StorageType<RightExpression>::
			Result RightType;

		LeftType left_;
		RightType right_;
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
		VectorDivision(
			const LeftExpression& left,
			const RightExpression& right)
			: left_(left)
			, right_(right)
		{
			PENSURE2(N != Unbounded || left.size() == right.size(), 
				left.size(), right.size());
		}

		Real operator[](integer index) const
		{
			return left_[index] / right_[index];
		}

		integer size() const
		{
			const LeftExpression& expression =
				(LeftExpression&)left_;
			return expression.size();
		}

	private:
		typedef typename StorageType<LeftExpression>::
			Result LeftType;
		typedef typename StorageType<RightExpression>::
			Result RightType;

		LeftType left_;
		RightType right_;
	};

}

#endif
