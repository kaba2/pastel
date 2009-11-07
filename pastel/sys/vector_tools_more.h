// Description: Array programming for vectors
// Detail: min, max, clamp, abs, sin, cos, floor, ceil, etc.

#ifndef PASTEL_VECTOR_TOOLS_MORE_H
#define PASTEL_VECTOR_TOOLS_MORE_H

#include "pastel/sys/vector_tools.h"
#include "pastel/sys/vector.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/tuple.h"

#include "pastel/sys/math_functions.h"

namespace Pastel
{

#define PASTEL_VECTOR_EXPRESSION_CLASS(CLASS, SCALAR_FUNCTION) \
	template <  \
		typename Real,  \
		int N,  \
		typename Expression>  \
	class CLASS \
		: public VectorExpression<Real, N, CLASS<Real, N, Expression> > \
	{ \
	public: \
		typedef const CLASS& StorageType; \
		\
		explicit CLASS( \
			const Expression& data) \
			: data_(data) \
		{ \
		} \
		\
		Real operator[](integer index) const \
		{ \
			return SCALAR_FUNCTION(data_[index]); \
		} \
		\
		integer size() const \
		{ \
			const Expression& expression = \
				(const Expression&)data_; \
			return expression.size(); \
		} \
		\
		bool involves( \
			const void* memoryBegin, const void* memoryEnd) const \
		{ \
			return data_.involves(memoryBegin, memoryEnd); \
		} \
		\
		bool involvesNonTrivially( \
			const void* memoryBegin, const void* memoryEnd) const \
		{ \
			return data_.involvesNonTrivially(memoryBegin, memoryEnd); \
		} \
		 \
	private: \
		typename Expression::StorageType data_; \
	};

#define PASTEL_VECTOR_EXPRESSION_FUNCTION(CLASS, VECTOR_FUNCTION) \
	template <typename Real, int N, typename Expression> \
	inline const CLASS<Real, N, Expression> \
		VECTOR_FUNCTION(const VectorExpression<Real, N, Expression>& x) \
	{ \
		return CLASS<Real, N, Expression>((const Expression&)x); \
	}

#define PASTEL_VECTOR_EXPRESSION(CLASS, SCALAR_FUNCTION, VECTOR_FUNCTION) \
	PASTEL_VECTOR_EXPRESSION_CLASS(CLASS, SCALAR_FUNCTION); \
	PASTEL_VECTOR_EXPRESSION_FUNCTION(CLASS, VECTOR_FUNCTION);

	// Permutation

	template <typename Real, int N, typename Expression>
	inline Vector<Real, N> permute(
		const VectorExpression<Real, N, Expression>& that,
		const Tuple<integer, N>& permutation);

	// Minimum functions

	//! Returns the index of the minimum element.

	template <typename Real, int N, typename Expression>
	inline integer minIndex(
		const VectorExpression<Real, N, Expression>& that);

	//! Returns the minimum of elements.

	template <typename Real, int N, typename Expression>
	inline Real min(
		const VectorExpression<Real, N, Expression>& that);

	//! Returns the minimum of (left[i], right[i]), elementwise.

	template <typename Real, int N, 
		typename LeftExpression, typename RightExpression>
	inline Vector<Real, N> min(
		const VectorExpression<Real, N, LeftExpression>& left,
		const VectorExpression<Real, N, RightExpression>& right);

	//! Returns the minimum of (left, right[i]), elementwise.

	template <typename Real, int N, typename Expression>
	inline Vector<Real, N> min(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const VectorExpression<Real, N, Expression>& right);

	//! Returns the minimum of (left[i], right), elementwise.

	template <typename Real, int N, typename Expression>
	inline Vector<Real, N> min(
		const VectorExpression<Real, N, Expression>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);



	// Maximum functions

	//! Returns the index of the maximum element.

	template <typename Real, int N, typename Expression>
	inline integer maxIndex(
		const VectorExpression<Real, N, Expression>& that);

	//! Returns the maximum of elements.

	template <typename Real, int N, typename Expression>
	inline Real max(
		const VectorExpression<Real, N, Expression>& that);

	//! Returns the maximum of (left[i], right[i]), elementwise.

	template <typename Real, int N,
	typename LeftExpression, typename RightExpression>
	inline Vector<Real, N> max(
		const VectorExpression<Real, N, LeftExpression>& left,
		const VectorExpression<Real, N, RightExpression>& right);

	//! Returns the maximum of (left, right[i]), elementwise.

	template <typename Real, int N, typename Expression>
	inline Vector<Real, N> max(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const VectorExpression<Real, N, Expression>& right);

	//! Returns the maximum of (left[i], right), elementwise.

	template <typename Real, int N, typename Expression>
	inline Vector<Real, N> max(
		const VectorExpression<Real, N, Expression>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);


	template <typename Real, int N,
	typename ThatExpression, typename MinExpression,
	typename MaxExpression>
	inline Vector<Real, N> clamp(
		const VectorExpression<Real, N, ThatExpression>& that,
		const VectorExpression<Real, N, MinExpression>& minimum,
		const VectorExpression<Real, N, MaxExpression>& maximum);

	// Optimization functions

	//! Returns x[i] * 2^power[i], elementwise.

	template <typename Real, int N>
	inline void multiplyByPowerOf2(
		Vector<Real, N>& x,
		const Vector<Real, N>& power);

	//! Returns x[i] * 2^power, elementwise.

	template <typename Real, int N>
	inline void multiplyByPowerOf2(
		Vector<Real, N>& x,
		const Real& power);

	//! Returns x[i] / 2^power[i], elementwise.

	template <typename Real, int N>
	inline void divideByPowerOf2(
		Vector<Real, N>& x,
		const Vector<Real, N>& power);

	//! Returns x[i] / 2^power, elementwise.

	template <typename Real, int N>
	inline void divideByPowerOf2(
		Vector<Real, N>& x,
		const Real& power);

	// Expression templates for array computation

	PASTEL_VECTOR_EXPRESSION(VectorInverse, inverse, inverse);
	PASTEL_VECTOR_EXPRESSION(VectorAbs, mabs, mabs);
	PASTEL_VECTOR_EXPRESSION(VectorSquare, square, squarev);
	PASTEL_VECTOR_EXPRESSION(VectorExp, std::exp, exp);
	PASTEL_VECTOR_EXPRESSION(VectorLog, std::log, log);
	PASTEL_VECTOR_EXPRESSION(VectorSqrt, std::sqrt, sqrt);
	PASTEL_VECTOR_EXPRESSION(VectorFloor, std::floor, floor);
	PASTEL_VECTOR_EXPRESSION(VectorCeil, std::ceil, ceil);
	PASTEL_VECTOR_EXPRESSION(VectorMod, mod, mod);
	PASTEL_VECTOR_EXPRESSION(VectorSin, std::sin, sin);
	PASTEL_VECTOR_EXPRESSION(VectorCos, std::cos, cos);
	PASTEL_VECTOR_EXPRESSION(VectorTan, std::tan, tan);
	PASTEL_VECTOR_EXPRESSION(VectorAsin, std::asin, asin);
	PASTEL_VECTOR_EXPRESSION(VectorAcos, std::acos, acos);
	PASTEL_VECTOR_EXPRESSION(VectorAtan, std::atan, atan);

	template <
		typename Real,
		int N,
		typename LeftExpression,
		typename RightExpression>
	class VectorPow
		: public VectorExpression<Real, N, VectorPow<Real, N, 
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
		typename Real,
		int N,
		typename LeftExpression,
		typename RightExpression>
	class VectorAtan2
		: public VectorExpression<Real, N, VectorAtan2<Real, N, 
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

	// Arithmetic functions

	//! Returns x[i]^y[i], elementwise.

	template <typename Real, int N,
		typename LeftExpression, typename RightExpression>
		inline const VectorPow<Real, N, 
		LeftExpression, RightExpression>
		pow(const VectorExpression<Real, N, LeftExpression>& left,
		const VectorExpression<Real, N, RightExpression>& right);

	//! Returns x[i]^y, elementwise.

	template <typename Real, int N,
		typename LeftExpression>
		inline const VectorPow<Real, N, 
		LeftExpression, 
		VectorConstant<Real, N> >
		pow(const VectorExpression<Real, N, LeftExpression>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	template <typename Real, int N,
		typename LeftExpression, typename RightExpression>
		inline const VectorAtan2<Real, N, 
		LeftExpression, 
		RightExpression>
		atan2(const VectorExpression<Real, N, LeftExpression>& left,
		const VectorExpression<Real, N, RightExpression>& right);

}

#endif
