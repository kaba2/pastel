#ifndef PASTEL_UNARY_VECTOREXPRESSION_DETAIL_H
#define PASTEL_UNARY_VECTOREXPRESSION_DETAIL_H

#include "pastel/sys/unary_vectorexpression.h"

#define PASTEL_UNARY_VECTOREXPRESSION_CLASS(CLASS, UNARY_FUNCTION) \
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
			return UNARY_FUNCTION(data_[index]); \
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

#define PASTEL_UNARY_VECTOREXPRESSION_FUNCTION(CLASS, VECTOR_FUNCTION) \
	template <typename Real, int N, typename Expression> \
	inline const CLASS<Real, N, Expression> \
		VECTOR_FUNCTION(const VectorExpression<Real, N, Expression>& x) \
	{ \
		return CLASS<Real, N, Expression>((const Expression&)x); \
	}

#define PASTEL_UNARY_VECTOREXPRESSION(CLASS, UNARY_FUNCTION, VECTOR_FUNCTION) \
	PASTEL_UNARY_VECTOREXPRESSION_CLASS(CLASS, UNARY_FUNCTION); \
	PASTEL_UNARY_VECTOREXPRESSION_FUNCTION(CLASS, VECTOR_FUNCTION);

#endif
