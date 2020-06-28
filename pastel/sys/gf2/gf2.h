// Description: Finite field arithmetic in GF(2^n).

#ifndef PASTELSYS_GF2_H
#define PASTELSYS_GF2_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"

#include <boost/operators.hpp>

namespace Pastel
{

	template <int N>
	class Gf2
	: boost::ring_operators<Gf2<N>
	, boost::left_shiftable2<Gf2<N>, integer
	, boost::equality_comparable<Gf2<N>
	> > >
	{
	public:
		PASTEL_STATIC_ASSERT(1 <= N && N <= 64);

		static constexpr uint64 ValidMask = 
			(uint64)(1 << N) - 1;

		explicit Gf2(uint64 polynomial = 0)
		: polynomial_(polynomial)
		{
			PENSURE_OP((polynomial & ValidMask), ==, polynomial);
		}

		Gf2(const Gf2& that)
		: polynomial_(that.polynomial_)
		{
		}

		Gf2& operator=(const Gf2& that)
		{
			polynomial_ = that.polynomial_;
			return *this;
		}

		Gf2& operator*=(const Gf2& that)
		{
			*this = multiply(*this, that);
			return *this;
		}

		Gf2& operator<<=(integer amount)
		{
			PENSURE_OP(amount, >=, 0);

			// The following table gives the bit-representation
			// of X^N mod F_N(X), where F_N is an irreducible 
			// polynomial of order N over GF(2). This is given in 
			// highestPowerSet[N]. Example:
			// An irreducible polynomial of order 32 over GF(2):
			// F_32(X) = X^32 + X^7 + X^3 + X^2 + 1
			// Then X^32 = X^7 + X^3 + X^2 + 1 (mod F_32(X))
			// which in bit-representation is 
			// 10001101, and in hexadecimal 0x8d.
			// These irreducible polynomials were obtained from the paper
			// "Table of Low-Weight Binary Irreducible Polynomials",
			// Gadiel Seroussi, 1998.

			uint64 highestPowerSet[] =
			{
				0x0, 0x0, 0x3, 0x3, 
				0x3, 0x5, 0x3, 0x3, 
				0x1B, 0x3, 0x9, 0x5, 
				0x9, 0x1B, 0x21, 0x3, 
				0x2B, 0x9, 0x9, 0x27, 
				0x9, 0x5, 0x3, 0x21, 
				0x1B, 0x9, 0x1B, 0x27, 
				0x3, 0x5, 0x3, 0x9, 
				0x8D, 0x401, 0x81, 0x5, 
				0x201, 0x53, 0x63, 0x11, 
				0x39, 0x9, 0x81, 0x59, 
				0x21, 0x1B, 0x3, 0x21, 
				0x2D, 0x201, 0x1D, 0x4B, 
				0x9, 0x47, 0x201, 0x81, 
				0x95, 0x11, 0x80001, 0x95, 
				0x3, 0x27, 0x20000001, 0x3,
				0x1B
			};

			uint64 highestPower = highestPowerSet[N];

			for (integer i = 0;i < amount;++i)
			{
				bool carry = ((polynomial_ >> (N - 1)) == 1);
				polynomial_ <<= 1;
				if (carry)
				{
					polynomial_ ^= highestPower;
					polynomial_ &= ValidMask;
				}
			}

			return *this;
		}

		//! Adds 'that' to this element.
		Gf2& operator+=(const Gf2& that)
		{
			polynomial_ ^= that.polynomial_;
			return *this;
		}

		//! Subtracts 'that' from this element.
		/*!
		In GF(2^n) subtraction is equal to addition.
		*/
		Gf2& operator-=(const Gf2& that)
		{
			return *this += that;
		}

		//! Returns the additive inverse.
		/*
		In GF(2^n) the additive inverse is the element
		itself.
		*/
		Gf2 operator-() const
		{
			return *this;
		}

		//! Returns whether 'that' is equal to this.
		bool operator==(const Gf2& that) const
		{
			return polynomial_ == that.polynomial_;
		}

		//! Returns the polynomial representation.
		/*!
		An element of GF(2^n) is represented by a
		polynomial over GF(2). This polynomial is
		encoded as a bit-field, where the coefficient
		of X^i is given by the i:th bit.
		*/
		uint64 polynomial() const
		{
			return polynomial_;
		}

	private:
		Gf2 multiply(Gf2 left, const Gf2& right) const
		{
			Gf2 result;
			for (integer i = 0;i < N;++i)
			{
				uint64 ithBitMask =
					(uint64)1 << (uint64)i;
				if (right.polynomial() & ithBitMask)
				{
					result += left;
				}

				left <<= 1;
			}

			return result;
		}

		uint64 polynomial_;
	};

}

#endif
