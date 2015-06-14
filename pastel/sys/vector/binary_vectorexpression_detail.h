#ifndef PASTELSYS_BINARY_VECTOREXPRESSION_DETAIL_H
#define PASTELSYS_BINARY_VECTOREXPRESSION_DETAIL_H

#include "pastel/sys/vector/binary_vectorexpression.h"

#define PASTEL_BINARY_VECTOREXPRESSION_CLASS(CLASS, BINARY_FUNCTION) \
	template <  \
		typename Real, integer N,  \
		typename LeftExpression,  \
		typename RightExpression>  \
	class CLASS \
		: public VectorExpression<Real, N, CLASS<Real, N, \
		LeftExpression, RightExpression> > \
	{ \
	public: \
		using StorageType = const CLASS&; \
		\
		CLASS( \
			const LeftExpression& left, \
			const RightExpression& right) \
			: left_(left) \
			, right_(right) \
		{ \
		} \
		\
		Real operator[](integer index) const \
		{ \
			return BINARY_FUNCTION(left_[index], right_[index]); \
		} \
		\
		integer size() const \
		{ \
			const LeftExpression& expression = \
				(const LeftExpression&)left_; \
			return expression.size(); \
		} \
		\
		bool involves( \
			const void* memoryBegin, const void* memoryEnd) const \
		{ \
			return left_.involves(memoryBegin, memoryEnd) || \
				right_.involves(memoryBegin, memoryEnd); \
		} \
		\
		bool evaluateBeforeAssignment( \
			const void* memoryBegin, const void* memoryEnd) const \
		{ \
			return left_.evaluateBeforeAssignment(memoryBegin, memoryEnd) || \
				right_.evaluateBeforeAssignment(memoryBegin, memoryEnd); \
		} \
		 \
	private: \
		typename LeftExpression::StorageType left_; \
		typename RightExpression::StorageType right_; \
	};

#define PASTEL_BINARY_VECTOREXPRESSION_FUNCTION(CLASS, VECTOR_FUNCTION) \
	template <typename Real, integer N, \
		typename LeftExpression, typename RightExpression> \
	inline const CLASS<Real, N, LeftExpression, RightExpression> \
		VECTOR_FUNCTION( \
		const VectorExpression<Real, N, LeftExpression>& left, \
		const VectorExpression<Real, N, RightExpression>& right) \
	{ \
		return CLASS<Real, N, LeftExpression, RightExpression>( \
			(const LeftExpression&)left, \
			(const RightExpression&)right); \
	}

#define PASTEL_BINARY_VECTOREXPRESSION_FUNCTION_RIGHTCONSTANT(CLASS, VECTOR_FUNCTION) \
	template <typename Real, integer N, typename LeftExpression> \
		const CLASS<Real, N, LeftExpression, VectorConstant<Real, N> > \
		VECTOR_FUNCTION( \
		const VectorExpression<Real, N, LeftExpression>& left, \
		const NoDeduction<Real>& right) \
	{ \
		return Pastel::VECTOR_FUNCTION(left, \
			VectorConstant<Real, N>(right, left.size())); \
	}

#define PASTEL_BINARY_VECTOREXPRESSION_FUNCTION_LEFTCONSTANT(CLASS, VECTOR_FUNCTION) \
	template <typename Real, integer N, typename RightExpression> \
		const CLASS<Real, N, VectorConstant<Real, N>, RightExpression> \
		VECTOR_FUNCTION( \
		const NoDeduction<Real>& left, \
		const VectorExpression<Real, N, RightExpression>& right) \
	{ \
		return Pastel::VECTOR_FUNCTION( \
			VectorConstant<Real, N>(left, right.size()), \
			right); \
	}

#define PASTEL_BINARY_VECTOREXPRESSION(CLASS, BINARY_FUNCTION, VECTOR_FUNCTION) \
	PASTEL_BINARY_VECTOREXPRESSION_CLASS(CLASS, BINARY_FUNCTION); \
	PASTEL_BINARY_VECTOREXPRESSION_FUNCTION(CLASS, VECTOR_FUNCTION); \
	PASTEL_BINARY_VECTOREXPRESSION_FUNCTION_LEFTCONSTANT(CLASS, VECTOR_FUNCTION); \
	PASTEL_BINARY_VECTOREXPRESSION_FUNCTION_RIGHTCONSTANT(CLASS, VECTOR_FUNCTION);

#define PASTEL_BINARY_VECTOREXPRESSION_RIGHTCONSTANT(CLASS, BINARY_FUNCTION, VECTOR_FUNCTION) \
	PASTEL_BINARY_VECTOREXPRESSION_CLASS(CLASS, BINARY_FUNCTION); \
	PASTEL_BINARY_VECTOREXPRESSION_FUNCTION(CLASS, VECTOR_FUNCTION); \
	PASTEL_BINARY_VECTOREXPRESSION_FUNCTION_RIGHTCONSTANT(CLASS, VECTOR_FUNCTION);

#define PASTEL_BINARY_VECTOREXPRESSION_LEFTCONSTANT(CLASS, BINARY_FUNCTION, VECTOR_FUNCTION) \
	PASTEL_BINARY_VECTOREXPRESSION_CLASS(CLASS, BINARY_FUNCTION); \
	PASTEL_BINARY_VECTOREXPRESSION_FUNCTION(CLASS, VECTOR_FUNCTION); \
	PASTEL_BINARY_VECTOREXPRESSION_FUNCTION_LEFTCONSTANT(CLASS, VECTOR_FUNCTION);

#endif
