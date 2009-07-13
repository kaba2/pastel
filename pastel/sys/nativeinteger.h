// Documentation: math.txt

#ifndef PASTEL_NATIVEINTEGER_H
#define PASTEL_NATIVEINTEGER_H

#include "pastel/sys/mytypes.h"

#include <boost/operators.hpp>

namespace Pastel
{

	template <typename Type>
	class NativeInteger
		: boost::unit_steppable<NativeInteger<Type>
		, boost::ordered_euclidian_ring_operators<NativeInteger<Type>
		, boost::bitwise<NativeInteger<Type>
		, boost::shiftable2<NativeInteger<Type>, integer
		, boost::equality_comparable2<NativeInteger<Type>, integer
		> > > > >
	{
	public:
		NativeInteger();
		// Implicit conversion allowed.
		NativeInteger(Type that);

		void swap(NativeInteger& that);

		bool operator==(const NativeInteger& that) const;
		bool operator<(const NativeInteger& that) const;

		Type data() const;

		bool zero() const;
		bool negative() const;
		bool positive() const;

		NativeInteger<Type> operator+() const;
		NativeInteger<Type> operator-() const;

		NativeInteger<Type>& operator++();
		NativeInteger<Type>& operator--();

		NativeInteger<Type>& operator+=(
			const NativeInteger& that);
		NativeInteger<Type>& operator-=(
			const NativeInteger& that);
		NativeInteger<Type>& operator*=(
			const NativeInteger& that);
		NativeInteger<Type>& operator/=(
			const NativeInteger& that);

		NativeInteger<Type>& operator<<=(
			integer that);
		NativeInteger<Type>& operator>>=(
			integer that);

		NativeInteger<Type>& operator&=(
			const NativeInteger& that);
		NativeInteger<Type>& operator|=(
			const NativeInteger& that);
		NativeInteger<Type>& operator^=(
			const NativeInteger& that);

	private:
		Type data_;
	};

	template <typename Type>
	void swap(NativeInteger<Type>& left, NativeInteger<Type>& right);

	template <typename Type>
	bool zero(const NativeInteger<Type>& that);

	template <typename Type>
	bool negative(const NativeInteger<Type>& that);

	template <typename Type>
	bool positive(const NativeInteger<Type>& that);

}

#include "pastel/sys/nativeinteger.hpp"

#endif
