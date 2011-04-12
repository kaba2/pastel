#ifndef PASTEL_INTERLEAVE_HPP
#define PASTEL_INTERLEAVE_HPP

#include "pastel/sys/interleave.h"
#include "pastel/sys/ensure.h"

#include <algorithm>

namespace Pastel
{

	template <typename RandomAccessRange>
	void interleave(
		const RandomAccessRange& range,
		integer parts)
	{
		ENSURE_OP(parts, >, 0);
		ENSURE_OP(range.size() % parts, ==, 0);

		if (range.size() < 4 || parts == 1)
		{
			// Nothing to do.
			return;
		}

		const integer n = range.size() / parts;
		
		// The interleaving permutation function is given by:
		// f(i) = mod(i, s) * n + [i / s],
		// where
		// s is the number of parts,
		// n is the number of elements in a part.
		// [.] is the floor function
		//
		// The interleaved array is given by 
		// I[i] = range[f(i)].
		//
		// For example, if n = 2, and s = 3, then:
		//
		// i | mod(i, s) | [i / s] | f(i)
		// --+-----------+---------+-----
		// 0 |         0 |       0 |    0
		// 1 |         1 |       0 |    2
		// 2 |         2 |       0 |    4
		// 3 |         0 |       1 |    1
		// 4 |         1 |       1 |    3
		// 5 |         2 |       1 |    5
		//
		// Note that
		// f(0) = mod(0, s) * n + [0 / s] = 0
		// f(ns - 1) = mod(ns - 1, s) * n + [(ns - 1) / s]
		//           = (s - 1) * n + [n - 1 / s]
		//           = sn - n + n - 1
		//           = sn - 1
		// i.e. 0 and ns - 1 are fixed points of f.
		// It can be shown that they are the only fixed points
		// of f in the range [0, n * s[.
		//
		// The inverse of f is given by
		// f^-1(j) = mod(j, n) * s + [j / n]
		// which is interestingly symmetric.
		//
		// Therefore, all other elements can be traversed 
		// by repeated application of f starting from 1.
		// We can then make a swapping sequence swapping
		// range[i] and range[f(i)], and then updating
		// i := f(i).

		integer i = 1;
		while(true)
		{
			// Move to the previous element.

			const integer b = i / parts;
			// mod(i, parts) = i - b * parts
			const integer fi = (i - b * parts) * n + b;

			if (fi == 1)
			{
				break;
			}

			// Since n >= 4, we know that the swapping sequence
			// contains at least 2 elements, and thus the
			// swap here is always sensible.
			using std::swap;
			swap(range[fi], range[i]);

			i = fi;
		}
	}

}

#endif
