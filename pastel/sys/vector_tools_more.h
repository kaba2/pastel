// Description: Tools for array programming with vectors
// Detail: min, max, clamp, abs, sin, cos, floor, ceil, etc.

/*!
\file
\brief Tools for array programming with vectors
*/

#ifndef PASTEL_VECTOR_TOOLS_MORE_H
#define PASTEL_VECTOR_TOOLS_MORE_H

#include "pastel/sys/vector_tools.h"
#include "pastel/sys/vector.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/tuple.h"

#include "pastel/sys/math_functions.h"

namespace Pastel
{

	// Permutation

	template <int N, typename Real, typename Expression>
	inline TemporaryVector<N, Real> permute(
		const VectorExpression<N, Real, Expression>& that,
		const Tuple<N, integer>& permutation);

	// Minimum functions

	//! Returns the index of the minimum element.

	template <int N, typename Real, typename Expression>
	inline integer minIndex(
		const VectorExpression<N, Real, Expression>& that);

	//! Returns the minimum of elements.

	template <int N, typename Real, typename Expression>
	inline Real min(
		const VectorExpression<N, Real, Expression>& that);

	//! Returns the minimum of (left[i], right[i]), elementwise.

	template <int N, typename Real, 
		typename LeftExpression, typename RightExpression>
	inline TemporaryVector<N, Real> min(
		const VectorExpression<N, Real, LeftExpression>& left,
		const VectorExpression<N, Real, RightExpression>& right);

	//! Returns the minimum of (left, right[i]), elementwise.

	template <int N, typename Real, typename Expression>
	inline TemporaryVector<N, Real> min(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const VectorExpression<N, Real, Expression>& right);

	//! Returns the minimum of (left[i], right), elementwise.

	template <int N, typename Real, typename Expression>
	inline TemporaryVector<N, Real> min(
		const VectorExpression<N, Real, Expression>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);



	// Maximum functions

	//! Returns the index of the maximum element.

	template <int N, typename Real, typename Expression>
	inline integer maxIndex(
		const VectorExpression<N, Real, Expression>& that);

	//! Returns the maximum of elements.

	template <int N, typename Real, typename Expression>
	inline Real max(
		const VectorExpression<N, Real, Expression>& that);

	//! Returns the maximum of (left[i], right[i]), elementwise.

	template <int N, typename Real,
	typename LeftExpression, typename RightExpression>
	inline TemporaryVector<N, Real> max(
		const VectorExpression<N, Real, LeftExpression>& left,
		const VectorExpression<N, Real, RightExpression>& right);

	//! Returns the maximum of (left, right[i]), elementwise.

	template <int N, typename Real, typename Expression>
	inline TemporaryVector<N, Real> max(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const VectorExpression<N, Real, Expression>& right);

	//! Returns the maximum of (left[i], right), elementwise.

	template <int N, typename Real, typename Expression>
	inline TemporaryVector<N, Real> max(
		const VectorExpression<N, Real, Expression>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);


	template <int N, typename Real,
	typename ThatExpression, typename MinExpression,
	typename MaxExpression>
	inline TemporaryVector<N, Real> clamp(
		const VectorExpression<N, Real, ThatExpression>& that,
		const VectorExpression<N, Real, MinExpression>& minimum,
		const VectorExpression<N, Real, MaxExpression>& maximum);

	// Optimization functions

	//! Returns x[i] * 2^power[i], elementwise.

	template <int N, typename Real>
	inline void multiplyByPowerOf2(
		Vector<N, Real>& x,
		const Vector<N, Real>& power);

	//! Returns x[i] * 2^power, elementwise.

	template <int N, typename Real>
	inline void multiplyByPowerOf2(
		Vector<N, Real>& x,
		const Real& power);

	//! Returns x[i] / 2^power[i], elementwise.

	template <int N, typename Real>
	inline void divideByPowerOf2(
		Vector<N, Real>& x,
		const Vector<N, Real>& power);

	//! Returns x[i] / 2^power, elementwise.

	template <int N, typename Real>
	inline void divideByPowerOf2(
		Vector<N, Real>& x,
		const Real& power);

	// Expression templates for array computation

	template <
		int N,
		typename Real,
		typename Expression>
	class VectorAbs
		: public VectorExpression<N, Real,
		VectorAbs<N, Real, Expression> >
	{
	public:
		typedef const VectorAbs& StorageType;

		explicit VectorAbs(
			const Expression& data)
			: data_(data)
		{
		}

		Real operator[](integer index) const
		{
			const Real data(data_[index]);
			return positive(data) ? data : -data;
		}

		integer size() const
		{
			const Expression& expression =
				(const Expression&)data_;
			return expression.size();
		}

		bool involves(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involves(memoryBegin, memoryEnd);
		}

		bool involvesNonTrivially(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involvesNonTrivially(memoryBegin, memoryEnd);
		}

	private:
		typename Expression::StorageType data_;
	};

	template <
		int N,
		typename Real,
		typename Expression>
	class VectorSquare
		: public VectorExpression<N, Real,
		VectorSquare<N, Real, Expression> >
	{
	public:
		typedef const VectorSquare& StorageType;

		explicit VectorSquare(
			const Expression& data)
			: data_(data)
		{
		}

		Real operator[](integer index) const
		{
			return Pastel::square(data_[index]);
		}

		integer size() const
		{
			const Expression& expression =
				(const Expression&)data_;
			return expression.size();
		}

		bool involves(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involves(memoryBegin, memoryEnd);
		}

		bool involvesNonTrivially(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involvesNonTrivially(memoryBegin, memoryEnd);
		}

	private:
		typename Expression::StorageType data_;
	};

	template <
		int N,
		typename Real,
		typename Expression>
	class VectorExp
		: public VectorExpression<N, Real,
		VectorExp<N, Real, Expression> >
	{
	public:
		typedef const VectorExp& StorageType;

		explicit VectorExp(
			const Expression& data)
			: data_(data)
		{
		}

		Real operator[](integer index) const
		{
			return std::exp(data_[index]);
		}

		integer size() const
		{
			const Expression& expression =
				(const Expression&)data_;
			return expression.size();
		}

		bool involves(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involves(memoryBegin, memoryEnd);
		}

		bool involvesNonTrivially(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involvesNonTrivially(memoryBegin, memoryEnd);
		}

	private:
		typename Expression::StorageType data_;
	};

	template <
		int N,
		typename Real,
		typename Expression>
	class VectorLog
		: public VectorExpression<N, Real,
		VectorLog<N, Real, Expression> >
	{
	public:
		typedef const VectorLog& StorageType;

		explicit VectorLog(
			const Expression& data)
			: data_(data)
		{
		}

		Real operator[](integer index) const
		{
			return std::log(data_[index]);
		}

		integer size() const
		{
			const Expression& expression =
				(const Expression&)data_;
			return expression.size();
		}

		bool involves(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involves(memoryBegin, memoryEnd);
		}

		bool involvesNonTrivially(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involvesNonTrivially(memoryBegin, memoryEnd);
		}

	private:
		typename Expression::StorageType data_;
	};

	template <
		int N,
		typename Real,
		typename LeftExpression,
		typename RightExpression>
	class VectorPow
		: public VectorExpression<N, Real,
		VectorPow<N, Real,
		LeftExpression, RightExpression> >
	{
	public:
		typedef const VectorPow& StorageType;

		VectorPow(
			const LeftExpression& left,
			const RightExpression& right)
			: left_(left)
			, right_(right)
		{
		}

		Real operator[](integer index) const
		{
			return std::pow(left_[index], right_[index]);
		}

		integer size() const
		{
			const LeftExpression& expression =
				(const LeftExpression&)left_;
			return expression.size();
		}

		bool involves(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return left_.involves(memoryBegin, memoryEnd) ||
				right_.involves(memoryBegin, memoryEnd);
		}

		bool involvesNonTrivially(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return left_.involvesNonTrivially(memoryBegin, memoryEnd) ||
				right_.involvesNonTrivially(memoryBegin, memoryEnd);
		}

	private:
		typename LeftExpression::StorageType left_;
		typename RightExpression::StorageType right_;
	};

	template <
		int N,
		typename Real,
		typename Expression>
	class VectorSqrt
		: public VectorExpression<N, Real,
		VectorSqrt<N, Real, Expression> >
	{
	public:
		typedef const VectorSqrt& StorageType;

		explicit VectorSqrt(
			const Expression& data)
			: data_(data)
		{
		}

		Real operator[](integer index) const
		{
			return std::sqrt(data_[index]);
		}

		integer size() const
		{
			const Expression& expression =
				(const Expression&)data_;
			return expression.size();
		}

		bool involves(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involves(memoryBegin, memoryEnd);
		}

		bool involvesNonTrivially(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involvesNonTrivially(memoryBegin, memoryEnd);
		}

	private:
		typename Expression::StorageType data_;
	};

	template <
		int N,
		typename Real,
		typename Expression>
	class VectorFloor
		: public VectorExpression<N, Real,
		VectorFloor<N, Real, Expression> >
	{
	public:
		typedef const VectorFloor& StorageType;

		explicit VectorFloor(
			const Expression& data)
			: data_(data)
		{
		}

		Real operator[](integer index) const
		{
			return std::floor(data_[index]);
		}

		integer size() const
		{
			const Expression& expression =
				(const Expression&)data_;
			return expression.size();
		}

		bool involves(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involves(memoryBegin, memoryEnd);
		}

		bool involvesNonTrivially(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involvesNonTrivially(memoryBegin, memoryEnd);
		}

	private:
		typename Expression::StorageType data_;
	};

	template <
		int N,
		typename Real,
		typename Expression>
	class VectorCeil
		: public VectorExpression<N, Real,
		VectorCeil<N, Real, Expression> >
	{
	public:
		typedef const VectorCeil& StorageType;

		explicit VectorCeil(
			const Expression& data)
			: data_(data)
		{
		}

		Real operator[](integer index) const
		{
			return std::ceil(data_[index]);
		}

		integer size() const
		{
			const Expression& expression =
				(const Expression&)data_;
			return expression.size();
		}

		bool involves(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involves(memoryBegin, memoryEnd);
		}

		bool involvesNonTrivially(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involvesNonTrivially(memoryBegin, memoryEnd);
		}

	private:
		typename Expression::StorageType data_;
	};

	template <
		int N,
		typename Real,
		typename Expression>
	class VectorSin
		: public VectorExpression<N, Real,
		VectorSin<N, Real, Expression> >
	{
	public:
		typedef const VectorSin& StorageType;

		explicit VectorSin(
			const Expression& data)
			: data_(data)
		{
		}

		Real operator[](integer index) const
		{
			return std::sin(data_[index]);
		}

		integer size() const
		{
			const Expression& expression =
				(const Expression&)data_;
			return expression.size();
		}

		bool involves(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involves(memoryBegin, memoryEnd);
		}

		bool involvesNonTrivially(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involvesNonTrivially(memoryBegin, memoryEnd);
		}

	private:
		typename Expression::StorageType data_;
	};

	template <
		int N,
		typename Real,
		typename Expression>
	class VectorCos
		: public VectorExpression<N, Real,
		VectorCos<N, Real, Expression> >
	{
	public:
		typedef const VectorCos& StorageType;

		explicit VectorCos(
			const Expression& data)
			: data_(data)
		{
		}

		Real operator[](integer index) const
		{
			return std::cos(data_[index]);
		}

		integer size() const
		{
			const Expression& expression =
				(const Expression&)data_;
			return expression.size();
		}

		bool involves(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involves(memoryBegin, memoryEnd);
		}

		bool involvesNonTrivially(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involvesNonTrivially(memoryBegin, memoryEnd);
		}

	private:
		typename Expression::StorageType data_;
	};

	template <
		int N,
		typename Real,
		typename Expression>
	class VectorTan
		: public VectorExpression<N, Real,
		VectorTan<N, Real, Expression> >
	{
	public:
		typedef const VectorTan& StorageType;

		explicit VectorTan(
			const Expression& data)
			: data_(data)
		{
		}

		Real operator[](integer index) const
		{
			return std::tan(data_[index]);
		}

		integer size() const
		{
			const Expression& expression =
				(const Expression&)data_;
			return expression.size();
		}

		bool involves(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involves(memoryBegin, memoryEnd);
		}

		bool involvesNonTrivially(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involvesNonTrivially(memoryBegin, memoryEnd);
		}

	private:
		typename Expression::StorageType data_;
	};

	template <
		int N,
		typename Real,
		typename Expression>
	class VectorAsin
		: public VectorExpression<N, Real,
		VectorAsin<N, Real, Expression> >
	{
	public:
		typedef const VectorAsin& StorageType;

		explicit VectorAsin(
			const Expression& data)
			: data_(data)
		{
		}

		Real operator[](integer index) const
		{
			return std::asin(data_[index]);
		}

		integer size() const
		{
			const Expression& expression =
				(const Expression&)data_;
			return expression.size();
		}

		bool involves(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involves(memoryBegin, memoryEnd);
		}

		bool involvesNonTrivially(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involvesNonTrivially(memoryBegin, memoryEnd);
		}

	private:
		typename Expression::StorageType data_;
	};

	template <
		int N,
		typename Real,
		typename Expression>
	class VectorAcos
		: public VectorExpression<N, Real,
		VectorAcos<N, Real, Expression> >
	{
	public:
		typedef const VectorAcos& StorageType;

		explicit VectorAcos(
			const Expression& data)
			: data_(data)
		{
		}

		Real operator[](integer index) const
		{
			return std::acos(data_[index]);
		}

		integer size() const
		{
			const Expression& expression =
				(const Expression&)data_;
			return expression.size();
		}

		bool involves(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involves(memoryBegin, memoryEnd);
		}

		bool involvesNonTrivially(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involvesNonTrivially(memoryBegin, memoryEnd);
		}

	private:
		typename Expression::StorageType data_;
	};

	template <
		int N,
		typename Real,
		typename LeftExpression,
		typename RightExpression>
	class VectorAtan2
		: public VectorExpression<N, Real,
		VectorAtan2<N, Real,
		LeftExpression, RightExpression> >
	{
	public:
		typedef const VectorAtan2& StorageType;

		VectorAtan2(
			const LeftExpression& left,
			const RightExpression& right)
			: left_(left)
			, right_(right)
		{
		}

		Real operator[](integer index) const
		{
			return std::atan2(left_[index], right_[index]);
		}

		integer size() const
		{
			const LeftExpression& expression =
				(const LeftExpression&)left_;
			return expression.size();
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
		int N,
		typename Real,
		typename Expression>
	class VectorInverse
		: public VectorExpression<N, Real,
		VectorInverse<N, Real, Expression> >
	{
	public:
		typedef const VectorInverse& StorageType;

		explicit VectorInverse(
			const Expression& data)
			: data_(data)
		{
		}

		Real operator[](integer index) const
		{
			return inverse(data_[index]);
		}

		integer size() const
		{
			const Expression& expression =
				(const Expression&)data_;
			return expression.size();
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

	// Arithmetic functions

	//! Returns 1 / x[i], elementwise.

	template <int N, typename Real, typename Expression>
	inline const VectorInverse<N, Real, Expression>
		inverse(const VectorExpression<N, Real, Expression>& x);

	//! Returns the absolute value of x[i], elementwise.

	template <int N, typename Real, typename Expression>
	inline const VectorAbs<N, Real, Expression>
		mabs(const VectorExpression<N, Real, Expression>& x);

	//! Returns the square of x[i], elementwise.

	template <int N, typename Real, typename Expression>
	inline const VectorSquare<N, Real, Expression>
		squarev(const VectorExpression<N, Real, Expression>& x);

	//! Returns e^x[i], elementwise.

	template <int N, typename Real, typename Expression>
	inline const VectorExp<N, Real, Expression>
		exp(const VectorExpression<N, Real, Expression>& x);

	//! Returns the natural logarithm of x[i], elementwise.

	template <int N, typename Real, typename Expression>
	inline const VectorLog<N, Real, Expression>
		log(const VectorExpression<N, Real, Expression>& x);

	//! Returns x[i]^y[i], elementwise.

	template <int N, typename Real,
		typename LeftExpression, typename RightExpression>
		inline const VectorPow<N, Real,
		LeftExpression, RightExpression>
		pow(const VectorExpression<N, Real, LeftExpression>& left,
		const VectorExpression<N, Real, RightExpression>& right);

	//! Returns x[i]^y, elementwise.

	template <int N, typename Real,
		typename LeftExpression>
		inline const VectorPow<N, Real,
		LeftExpression,
		VectorConstant<N, Real> >
		pow(const VectorExpression<N, Real, LeftExpression>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns the square root of x[i], elementwise.

	template <int N, typename Real, typename Expression>
	inline const VectorSqrt<N, Real, Expression>
		sqrt(const VectorExpression<N, Real, Expression>& x);

	//! Returns the floor of x[i], elementwise.

	template <int N, typename Real, typename Expression>
	inline const VectorFloor<N, Real, Expression>
		floor(const VectorExpression<N, Real, Expression>& x);

	//! Returns the ceiling of x[i], elementwise.

	template <int N, typename Real, typename Expression>
	inline const VectorCeil<N, Real, Expression>
		ceil(const VectorExpression<N, Real, Expression>& x);

	//! Returns the sine of x[i], elementwise.

	template <int N, typename Real, typename Expression>
	inline const VectorSin<N, Real, Expression>
		sin(const VectorExpression<N, Real, Expression>& x);

	//! Returns the cosine of x[i], elementwise.

	template <int N, typename Real, typename Expression>
	inline const VectorCos<N, Real, Expression>
		cos(const VectorExpression<N, Real, Expression>& x);

	//! Returns the tangent of x[i], elementwise.

	template <int N, typename Real, typename Expression>
	inline const VectorTan<N, Real, Expression>
		tan(const VectorExpression<N, Real, Expression>& x);

	//! Returns the arcsine of x[i], elementwise.

	template <int N, typename Real, typename Expression>
	inline const VectorAsin<N, Real, Expression>
		asin(const VectorExpression<N, Real, Expression>& x);

	//! Returns the arccosine of x[i], elementwise.

	template <int N, typename Real, typename Expression>
	inline const VectorAcos<N, Real, Expression>
		acos(const VectorExpression<N, Real, Expression>& x);

	//! Returns the arctangent of x[i], elementwise.

	template <int N, typename Real,
		typename LeftExpression, typename RightExpression>
		inline const VectorAtan2<N, Real,
		LeftExpression,
		RightExpression>
		atan2(const VectorExpression<N, Real, LeftExpression>& left,
		const VectorExpression<N, Real, RightExpression>& right);

}

#endif
