// Description: Named tuples

#ifndef PASTELSYS_NAMED_TUPLES_H
#define PASTELSYS_NAMED_TUPLES_H

#include "pastel/sys/hashing.h"

#include <boost/operators.hpp>

#define PASTEL_NAMED_TUPLE2(Type, A, a, B, b) \
	class Type \
		: boost::equality_comparable<Type> \
	{ \
	public: \
		Type( \
			PASTEL_REMOVE_BRACKETS(A) a##_, \
			PASTEL_REMOVE_BRACKETS(B) b##_) \
			: a(std::move(a##_)) \
			, b(std::move(b##_)) \
		{ \
		} \
		\
		bool operator==(const Type& that) const \
		{ \
			return  \
				a == that.a && \
				b == that.b; \
		} \
		\
		Pastel::hash_integer hash() const \
		{ \
			return Pastel::combineHash( \
				Pastel::computeHash(a), \
				Pastel::computeHash(b)); \
		} \
		\
		PASTEL_REMOVE_BRACKETS(A) a; \
		PASTEL_REMOVE_BRACKETS(B) b; \
	}

#define PASTEL_NAMED_TUPLE3(Type, A, a, B, b, C, c) \
	class Type \
		: boost::equality_comparable<Type> \
	{ \
	public: \
		Type( \
			PASTEL_REMOVE_BRACKETS(A) a##_, \
			PASTEL_REMOVE_BRACKETS(B) b##_, \
			PASTEL_REMOVE_BRACKETS(C) c##_) \
			: a(std::move(a##_)) \
			, b(std::move(b##_)) \
			, c(std::move(c##_)) \
		{ \
		} \
		\
		bool operator==(const Type& that) const \
		{ \
			return  \
				a == that.a && \
				b == that.b && \
				c == that.c; \
		} \
		\
		Pastel::hash_integer hash() const \
		{ \
			return Pastel::combineHash( \
				Pastel::combineHash( \
				Pastel::computeHash(a), \
				Pastel::computeHash(b)), \
				Pastel::computeHash(c)); \
		} \
		\
		PASTEL_REMOVE_BRACKETS(A) a; \
		PASTEL_REMOVE_BRACKETS(B) b; \
		PASTEL_REMOVE_BRACKETS(C) c; \
	}

#endif
