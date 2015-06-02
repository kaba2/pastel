#ifndef PASTELSYS_RATIONAL_REAL_HPP
#define PASTELSYS_RATIONAL_REAL_HPP

#include "pastel/sys/rational/rational.h"
#include "pastel/sys/bit/lowest_bit.h"
#include "pastel/sys/bit/highest_bit.h"
#include "pastel/sys/real/ieee_float.h"
#include "pastel/sys/string/digit.h"
#include "pastel/sys/sequence/exponential_binary_search.h"

namespace Pastel
{

	template <typename Integer>
	template <
		typename Real,
		Requires<std::is_floating_point<Real>>>
	Real Rational<Integer>::asReal() const
	{
		// Handle the degenerate cases.
		switch(classify())
		{
			case NumberType::Infinity:
				return infinity<Real>();
			case NumberType::MinusInfinity:
				return -infinity<Real>();
			case NumberType::Nan:
				return nan<Real>();
			default:
				// Fall-through
				;
		};

		// Finding the closest floating-point number is
		// tricky, because we do not know the internal
		// representation of the floating-point number.
		// Therefore, we convert the rational number to
		// a string, which is exact up to rounding, and 
		// then use the C++ Standard Library to convert
		// the string to a floating-point number.

		// We choose the number of digits in the string
		// as two digits more than what the precision
		// of the floating-number is. This accounts for
		// the fact that the number of correct digits has 
		// been rounded down.
		enum : integer 
		{
			digits = 
				std::numeric_limits<Real>::digits10 + 2
		};

		return stringAsReal<Real>(
			asString(PASTEL_TAG(digits), digits));
	}

	template <typename Integer>
	template <
		typename Real,
		typename... ArgumentSet,
		Requires<std::is_floating_point<Real>>>
	Rational<Integer>::Rational(
		Real that,
		ArgumentSet&&... argumentSet)
		: m_(0)
		, n_(1)
	{
		Integer nMax = PASTEL_ARG_S(nMax, infinity<Integer>());

		ENSURE(nMax >= 1);

		Real logAbs = std::log2(abs(that));
		if (logAbs < -(bits(m()) - 1))
		{
			// The rational number underflows.
			
			// Zero
			m_ = 0;
			n_ = 1;
			return;
		}

		if (logAbs > bits(m()) - 1)
		{
			// The rational number overflows.
			if (negative(that))
			{
				// -Infinity
				m_ = -1;
				n_ = 0;
				return;
			}

			// +Infinity
			m_ = 1;
			n_ = 0;
			return;
		}

		bool nonNegative = (that >= 0);
		if (!nonNegative)
		{
			// Reduce to the non-negative case.
			that = -that;
		}

		// A _rational pair_ is (m, n) in NN^{>= 0} such that
		// gcd(m, n) = 1. The _value_ of a rational pair is m / n.
		// We define n / 0 = infinity, for all n > 0.
		//
		// Let (a, b) and (c, d) be rational pairs, with a / b < c / d.
		// Then (a + c, b + d) --- called the _mediant_ of (a, b) and (c, d) 
		// --- is a rational pair, such that
		//
		//     a / b < (a + c) / (b + d) < c / d.
		//
		// The mediants --- incrementally computed between subsequent rational 
		// pairs --- form a bijection with the rational numbers in the open interval 
		// ]a / b, c / d[. Combined with the order-property, this means that
		// the mediant can be used in a search analogous to a binary search.
		// The corresponding binary tree is called the Stern-Brocot tree.
		// The levels of this tree form the Farey's sequences.

		// Since we wish to search over all non-negative rational numbers,
		// we have 
		//
		//     (a, b) = (0, 1) = 0,
		//     (c, d) = (1, 0) = infinity.
		//
		// A naive mediant search in the binary tree will not do.
		// For example, consider x = 0. Then the mediant will always
		// be chosen as the new upper bound, and the terms are
		// of the form
		//
		//     (k a + c, k b + d) = (1, k),
		//
		// which converges really slowly towards zero.
		//
		// Instead, we can compute the smallest k so that the 
		// mediant at the k:th iteration is smaller than x:
		//
		//         (ka + c) / (kb + d) <= x
		//     <=> k a + c <= x (kb + d)
		//     <=> k (a - xb) <= xd - c
		//     <=> k <= (xd - c) / (a - xb)
		//
		// Since k is integer, it is given by
		//
		//     k = floor[(xd - c) / (a - xb)].
		//
		// For example, with (a, b) = (0, 1), and (c, d) = (1, 0), we have
		//
		//     k = infinity.
		//
		// It follows that
		//
		//     (ka + c) / (kb + d) <= x <= ((k - 1)a + c) / ((k - 1) b + d).
		//
		// Unfortunately, we cannot use the formula for k, because
		// we cannot do the computations accurately (they involve x).
		// Therefore, we use the condition
		//
		//     (ka + c) / (kb + d) <= x
		//
		// in an exponential binary search for k. This we can do,
		// because we can convert a rational number to a floating
		// point number accurately.
		//
		// The process repeats with new bounds
		//
		//      (ka + c, kb + d) and ((k - 1)a + c, (k - 1)b + d).
		//
		// However, this time we make an analogous move towards greater 
		// numbers.
		//
		// We use two conditions to stop the process, both controlled by the user:
		//
		// * the divisor of the mediants gets too big, or
		// * a mediant is very close to x.
		//
		// After we get the new bounds, we check to see if they are close to x. 
		// If so, the one with the smaller divisor is returned. If not, we
		// continue recursively.

		// Let n = floor(x).
		//
		// Suppose we start searching for the best mediant
		// from [(0, 1), (1, 0)[. The mediant-binary search will
		// then do the following steps:
		// 
		// 		[(0, 1), (1, 0)[
		//      [(1, 1), (1, 0)[
		//      [(2, 1), (1, 0)[
		//      ...
		//      [(n, 1), (1, 0)[
		//      [(n, 1), (n + 1, 1)[
		//
		// Therefore, we should start straight from 
		//
		// 		[(n, 1), (1, 0)[.
		//
		// We do not start from [(n, 1), (n + 1, 1)[, since
		// then the initialization would need to check whether
		// (n + 1, 1) is better than (n, 1).  

		Real n = floor(that);
		Rational left = (integer)n;
		Rational right = infinity<Rational>();
		
		Rational& best = *this;
		best = left;		
		Real minError = that - left.asReal<Real>();

		auto leftMediant = [&](const Integer& k)
		{
			return Rational(
				k * left.m() + right.m(), 
				k * left.n() + right.n(),
				SkipSimplify());
		};

		auto consider = [&](const Rational& candidate)
		{
			Real error = abs(candidate.asReal<Real>() - that);
			if (error < minError)
			{
				best = candidate;
				minError = error;
			}
		};

		auto pass = [&](auto&& indicator)
		{
			Integer kMax = 
				std::min(
					// Avoid overflowing the divisor.
					zero(left.n()) ? infinity<Integer>() - 1 : (nMax - right.n()) / left.n(),
					// Avoid overflowing the numerator.
					zero(left.m()) ? infinity<Integer>() - 1 : (infinity<Integer>() - right.m()) / left.m()
				) + 1;
			
			Integer k = exponentialBinarySearch(
				Integer(1), kMax, indicator);

			Rational newRight = leftMediant(k - 1);
			consider(newRight);
			right = newRight;

			return k == kMax;
		};

		while(minError > 0)
		{
			ASSERT(left.asReal<Real>() <= that);
			ASSERT(right.asReal<Real>() >= that);

			bool leftDone = pass(				
				[&](Integer k)
				{
					return leftMediant(k).asReal<Real>() < that;
				});
			left.swap(right);

			if (minError == 0)
			{
				break;
			}

			bool rightDone = pass(
				[&](Integer k)
				{
					return leftMediant(k).asReal<Real>() > that;
				});
			left.swap(right);

			if (leftDone && rightDone)
			{
				// The left and right bound cannot be improved
				// further, because they would overflow either
				// the numerator or the divisor. Stop here.
				break;
			}
		}

		if (!nonNegative)
		{
			// Negate the number to give the correct sign.
			negate();
		}
    }

	template <typename Integer>
	template <
		typename Real,
		typename... ArgumentSet,
		Requires<std::is_floating_point<Real>>>
	Rational<Integer>::Rational(
		Real that,
		Simplest simplest,
		ArgumentSet&&... argumentSet)
		: m_(0)
		, n_(0)
	{
		// "Approximating Rational Numbers by Fractions",
		// Michael Forisek,
		// Fun with Algorithms
		// Lecture Notes in Computer Science Volume 4475, 
		// 2007, pp 156-165.

		// Paper's problem
		// ---------------
		//
		// Let q_m in ZZ, d_m in ZZ^{>= 0}, 
		// and q_n, d_n in ZZ^{> 0}. 
		// Find p_m in ZZ, and p_n in ZZ^{> 0},
		// with minimal p_n, such that
		//
		//         (q_m / q_n) - (d_m / d_n) 
		//      <= p_m / p_n 
		//      <= (q_m / q_n) + (d_m / d_n).

		// Our problem
		// -----------
		//
		// Let q in RR, and d in RR^{>= 0}. 
		// Find p_m in ZZ, and p_n in ZZ^{> 0},
		// with minimal |p_n|, such that
		//
		//         q - d <= p_m / p_n <= q + d.
		//
		// To adapt the paper to this problem, we
		// need to replace direct computation of 
		// the number of mediant-iterations with an
		// exponential binary search.

		Real maxError = PASTEL_ARG_S(maxError, 0);

		Real logAbs = std::log2(abs(that));
		if (logAbs < -(bits(m()) - 1))
		{
			// The rational number underflows.
			
			// Zero
			m_ = 0;
			n_ = 1;
			return;
		}

		if (logAbs > bits(m()) - 1)
		{
			// The rational number overflows.
			if (negative(that))
			{
				// -Infinity
				m_ = -1;
				n_ = 0;
				return;
			}

			// +Infinity
			m_ = 1;
			n_ = 0;
			return;
		}

		bool nonNegative = (that >= 0);
		if (!nonNegative)
		{
			// Reduce to the non-negative case.
			that = -that;
		}

		Real xMin = that - maxError;
		Real xMax = that + maxError;

		Real n = floor(that);
		Rational left = (integer)n;
		Rational right = infinity<Rational>();
		Rational& best = *this;
		
		bool found = false;

		auto consider = [&](const Rational& candidate)
		{
			Real x = candidate.asReal<Real>();

			// Because of rounding errors, it is important
			// to do this comparison exactly as in the
			// exponential binary search. In particular,
			// abs(x - that) <= maxError would give
			// inconsistent results.
			if (xMin <= x && x <= xMax)
			{
				best = candidate;
				found = true;
			}
		};

		auto leftMediant = [&](const Integer& k)
		{
			return Rational(
				k * left.m() + right.m(), 
				k * left.n() + right.n(),
				SkipSimplify());
		};

		auto pass = [&](auto&& indicator)
		{
			Integer kMax = 
				std::min(
					// Avoid overflowing the divisor.
					zero(left.n()) ? infinity<Integer>() - 1 : (infinity<Integer>() - right.n()) / left.n(),
					// Avoid overflowing the numerator.
					zero(left.m()) ? infinity<Integer>() - 1 : (infinity<Integer>() - right.m()) / left.m()
				) + 1;
			
			Integer k = exponentialBinarySearch(
				Integer(1), kMax, indicator);

			if (k < kMax)
			{
				consider(leftMediant(k));
			}

			right = leftMediant(k - 1);

			return k == kMax;
		};

		consider(left);

		while(!found)
		{
			ASSERT(left.asReal<Real>() < xMin);
			ASSERT(right.asReal<Real>() > xMax);

			bool leftDone = pass(
				[&](Integer k)
				{
					return leftMediant(k).asReal<Real>() <= xMax;
				});
			left.swap(right);

			if (found)
			{
				break;
			}

			bool rightDone = pass(
				[&](Integer k)
				{
					return leftMediant(k).asReal<Real>() >= xMin;
				});
			left.swap(right);

			if (leftDone && rightDone)
			{
				// The left and right bound cannot be improved
				// further, because they would overflow either
				// the numerator or the divisor. Stop here.
				break;
			}
		}

		if (!nonNegative)
		{
			// Negate the number to give the correct sign.
			negate();
		}
    }

}

#endif
