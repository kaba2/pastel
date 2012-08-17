#ifndef PASTEL_NAMED_TUPLES_H
#define PASTEL_NAMED_TUPLES_H

#include "pastel/sys/hash.h"

#include <boost/operators.hpp>

#define PASTEL_NAMED_TUPLE2(Type, A, a, B, b) \
	class Type \
		: boost::equality_comparable<Type> \
	{ \
	public: \
		Type( \
			A a##_, \
			B b##_) \
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
		hash_integer hash() const \
		{ \
			return Pastel::combineHash( \
				Pastel::computeHash(a), \
				Pastel::computeHash(b)); \
		} \
		\
		A a; \
		B b; \
	}

#define PASTEL_NAMED_TUPLE3(Type, A, a, B, b, C, c) \
	class Type \
		: boost::equality_comparable<Type> \
	{ \
	public: \
		Type( \
			A a##_, \
			B b##_, \
			C c##_) \
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
		hash_integer hash() const \
		{ \
			return Pastel::combineHash( \
				Pastel::combineHash( \
				Pastel::computeHash(a), \
				Pastel::computeHash(b)), \
				Pastel::computeHash(c)); \
		} \
		\
		A a; \
		B b; \
		C c; \
	}

#endif
