// Description: Basic vector expressions
// Detail: addition, subtraction, multiplication, etc.

#ifndef PASTEL_VECTOREXPRESSION_H
#define PASTEL_VECTOREXPRESSION_H

#include "pastel/sys/vectorbase.h"

namespace Pastel
{

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

		bool involvesNonTrivially(
			const void* memoryBegin, const void* memoryEnd) const
		{
			return ((const Expression&)*this).involvesNonTrivially(memoryBegin, memoryEnd);
		}

		template <typename RightExpression>
		bool operator==(const VectorExpression<Real, N, RightExpression>& right) const
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
		bool operator!=(const VectorExpression<Real, N, RightExpression>& right) const
		{
			return !(*this == right);
		}

		// Negation

		const VectorNegation<Real, N, Expression> operator-() const
		{
			return VectorNegation<Real, N, Expression>((const Expression&)*this);
		}

		// Summation

		template <typename RightExpression>
		const VectorAddition<Real, N, Expression, 
			RightExpression>
			operator+(const VectorExpression
			<Real, N, RightExpression>& right) const
		{
			return VectorAddition
				<Real, N, Expression, 
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

		template <typename RightExpression>
		const VectorSubtraction<Real, N, Expression, 
			RightExpression>
			operator-(const VectorExpression
			<Real, N, RightExpression>& right) const
		{
			return VectorSubtraction
				<Real, N, Expression, 
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

		template <typename RightExpression>
		const VectorMultiplication<Real, N, Expression, 
			RightExpression>
			operator*(const VectorExpression
			<Real, N, RightExpression>& right) const
		{
			return VectorMultiplication
				<Real, N, Expression, 
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

		template <typename RightExpression>
		const VectorDivision<Real, N, Expression, 
			RightExpression>
			operator/(const VectorExpression
			<Real, N, RightExpression>& right) const
		{
			return VectorDivision
				<Real, N, Expression, 
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

		bool involvesNonTrivially(
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

		bool involvesNonTrivially(
			const void* memoryBegin, const void* memoryEnd) const
		{
			return data_.involvesNonTrivially(memoryBegin, memoryEnd);
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
			const void* memoryBegin, const void* memoryEnd) const
		{
			return left_.involves(memoryBegin, memoryEnd) ||
				right_.involves(memoryBegin, memoryEnd);
		}

		bool involvesNonTrivially(
			const void* memoryBegin, const void* memoryEnd) const
		{
			return left_.involvesNonTrivially(memoryBegin, memoryEnd) ||
				right_.involvesNonTrivially(memoryBegin, memoryEnd);
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
			const void* memoryBegin, const void* memoryEnd) const
		{
			return left_.involves(memoryBegin, memoryEnd) ||
				right_.involves(memoryBegin, memoryEnd);
		}

		bool involvesNonTrivially(
			const void* memoryBegin, const void* memoryEnd) const
		{
			return left_.involvesNonTrivially(memoryBegin, memoryEnd) ||
				right_.involvesNonTrivially(memoryBegin, memoryEnd);
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
			const void* memoryBegin, const void* memoryEnd) const
		{
			return left_.involves(memoryBegin, memoryEnd) ||
				right_.involves(memoryBegin, memoryEnd);
		}

		bool involvesNonTrivially(
			const void* memoryBegin, const void* memoryEnd) const
		{
			return left_.involvesNonTrivially(memoryBegin, memoryEnd) ||
				right_.involvesNonTrivially(memoryBegin, memoryEnd);
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
			const void* memoryBegin, const void* memoryEnd) const
		{
			return left_.involves(memoryBegin, memoryEnd) ||
				right_.involves(memoryBegin, memoryEnd);
		}

		bool involvesNonTrivially(
			const void* memoryBegin, const void* memoryEnd) const
		{
			return left_.involvesNonTrivially(memoryBegin, memoryEnd) ||
				right_.involvesNonTrivially(memoryBegin, memoryEnd);
		}

	private:
		typename LeftExpression::StorageType left_;
		typename RightExpression::StorageType right_;
	};

	template <typename Real, int N>
	class ConstVectorView
		: public VectorExpression<Real, N, ConstVectorView<Real, N> >
	{
	public:
		typedef const ConstVectorView StorageType;

		typedef const Real* ConstIterator;

		ConstVectorView()
			: data_(0)
			, size_(0)
		{
		}

		ConstVectorView(
			const Real* data,
			integer size)
			: data_(data)
			, size_(size)
		{
			PENSURE_OP(size, >=, 0);
			PENSURE2(N == Dynamic || size == N, N, size);
		}

		integer size() const
		{
			return size_;
		}

		bool involves(
			const void* memoryBegin, const void* memoryEnd) const
		{
			return Pastel::memoryOverlaps(
				memoryBegin, memoryEnd,
				data_, data_ + size_);
		}

		bool involvesNonTrivially(
			const void* memoryBegin, const void* memoryEnd) const
		{
			return false;
		}

		integer dimension() const
		{
			return size_;
		}

		void swap(ConstVectorView<Real, N>& that)
		{
			std::swap(data_, that.data_);
			std::swap(size_, that.size_);
		}

		bool operator==(const ConstVectorView& that) const
		{
			return std::equal(
				begin(), end(), that.begin());
		}

		const Real& operator[](integer index) const
		{
			PENSURE2(index >= 0 && index < size_, 
				index, size_);

			return data_[index];
		}

		ConstIterator begin() const
		{
			return data_;
		}

		ConstIterator end() const
		{
			return data_ + size_;
		}

	protected:
		// Must be protected for
		// VectorView to access

		const Real* data_;
		integer size_;
	};

	template <typename Real, int N>
	class VectorView
		: public ConstVectorView<Real, N>
	{
	private:
		typedef ConstVectorView<Real, N> Base;
		using Base::data_;
		using Base::size_;

	public:
		typedef const VectorView StorageType;
		typedef Real* Iterator;

		using Base::size;

		VectorView()
			: Base()
		{
		}

		VectorView(
			const Real* data,
			integer size)
			: Base(data, size)
		{
			PENSURE_OP(size, >=, 0);
			PENSURE2(N == Dynamic || size == N, N, size);
		}

		// The parameter to this function
		// is deliberately not a reference,
		// because the reference could point
		// to this vector.
		void set(const Real that) const
		{
			std::fill(begin(), end(), that);
		}

		Iterator begin() const
		{
			return (Iterator)data_;
		}

		Iterator end() const
		{
			return (Iterator)(data_ + size_);
		}

		const VectorView& operator=(const Real& that) const
		{
			// We accept basic exception safety for performance.
			set(that);

			return *this;
		}

		const VectorView& operator=(
			const VectorView& that) const
		{
			ENSURE_OP(size(), ==, that.size());

			std::copy(begin(), end(), data_);

			return *this;
		}

		template <typename ThatReal, typename Expression>
		const VectorView& operator=(
			const VectorExpression<ThatReal, N, Expression>& that) const
		{
			ENSURE_OP(size(), ==, that.size());

			if (that.involvesNonTrivially(
				data_, data_ + size_))
			{
				// In the case we must reallocate, we can
				// as well copy construct, so that there
				// is no redundant initialization.
				
				// Of course, if the expression involves
				// this vector as a non-trivial subexpression,
				// we must copy construct anyway.

				*this = Vector<Real, N>(that);
			}
			else
			{				
				// We accept basic exception safety for performance.

				Iterator iter = begin();
				const integer n = size();

				for (integer i = 0;i < n;++i)
				{
					*iter = that[i];
					++iter;
				}
			}

			return *this;
		}

		Real& operator[](integer index) const
		{
			PENSURE2(index >= 0 && index < size_, 
				index, size_);

			return (Real&)data_[index];
		}

		// The parameter to this function
		// is deliberately not a reference,
		// because the reference could point
		// to this vector.
		const VectorView& operator+=(const Real that) const
		{
			Iterator iter = begin();
			const Iterator iterEnd = end();

			while(iter != iterEnd)
			{
				*iter += that;
				++iter;
			}

			return *this;
		}

		// The parameter to this function
		// is deliberately not a reference,
		// because the reference could point
		// to this vector.
		const VectorView& operator-=(const Real that) const
		{
			Iterator iter = begin();
			const Iterator iterEnd = end();

			while(iter != iterEnd)
			{
				*iter -= that;
				++iter;
			}

			return *this;
		}

		// The parameter to this function
		// is deliberately not a reference,
		// because the reference could point
		// to this vector.
		const VectorView& operator*=(const Real that) const
		{
			Iterator iter = begin();
			const Iterator iterEnd = end();

			while(iter != iterEnd)
			{
				*iter *= that;
				++iter;
			}

			return *this;
		}

		// Here the reference is ok because we actually
		// use the parameter's inverse.
		const VectorView& operator/=(const Real& that) const
		{
			return (*this *= Pastel::inverse(that));
		}

		template <typename ThatReal, typename Expression>
		const VectorView& operator+=(
			const VectorExpression<ThatReal, N, Expression>& that) const
		{
			PENSURE2(that.size() == size(), that.size(), size());

			if (that.involvesNonTrivially(
				data_, data_ + size_))
			{
				*this += Vector<Real, N>(that);
			}
			else
			{
				Iterator iter = begin();
				const integer n = size();

				for (integer i = 0;i < n;++i)
				{
					*iter += that[i];
					++iter;
				}
			}

			return *this;
		}

		template <typename ThatReal, typename Expression>
		const VectorView& operator-=(
			const VectorExpression<ThatReal, N, Expression>& that) const
		{
			PENSURE2(that.size() == size(), that.size(), size());

			if (that.involvesNonTrivially(
				data_, data_ + size_))
			{
				*this -= Vector<Real, N>(that);
			}
			else
			{
				Iterator iter = begin();
				const integer n = size();

				for (integer i = 0;i < n;++i)
				{
					*iter -= that[i];
					++iter;
				}
			}

			return *this;
		}

		template <typename ThatReal, typename Expression>
		const VectorView& operator*=(
			const VectorExpression<ThatReal, N, Expression>& that) const
		{
			PENSURE2(that.size() == size(), that.size(), size());

			if (that.involvesNonTrivially(
				data_, data_ + size_))
			{
				*this *= Vector<Real, N>(that);
			}
			else
			{
				Iterator iter = begin();
				const integer n = size();

				for (integer i = 0;i < n;++i)
				{
					*iter *= that[i];
					++iter;
				}
			}

			return *this;
		}

		template <typename ThatReal, typename Expression>
		const VectorView& operator/=(
			const VectorExpression<ThatReal, N, Expression>& that) const
		{
			PENSURE2(that.size() == size(), that.size(), size());

			if (that.involvesNonTrivially(
				data_, data_ + size_))
			{
				*this /= Vector<Real, N>(that);
			}
			else
			{
				Iterator iter = begin();
				const integer n = size();

				for (integer i = 0;i < n;++i)
				{
					*iter /= that[i];
					++iter;
				}
			}

			return *this;
		}
	};

}

#endif
