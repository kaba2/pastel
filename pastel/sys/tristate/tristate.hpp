#ifndef PASTELSYS_TRISTATE_HPP
#define PASTELSYS_TRISTATE_HPP

#include "pastel/sys/tristate.h"

#include <algorithm>

namespace Pastel
{

	inline void swap(TriState& left, TriState& right)
	{
		std::swap(left, right);
	}

	inline TriState triNot(TriState that)
	{
		TriState result = TriState::Maybe;

		if (that == TriState::True)
		{
			result = TriState::False;
		}
		else if (that == TriState::False)
		{
			result = TriState::True;
		}

		return result;
	}

	inline TriState triMix(TriState left, TriState right)
	{
		TriState result = TriState::Maybe;

		if (left == right &&
			left != TriState::Maybe)
		{
			result = left;
		}

		return result;
	}

	inline TriState triEqual(TriState left, TriState right)
	{
		TriState result = TriState::Maybe;

		// Note:
		// equal(TriState::Maybe, TriState::Maybe) = TriState::Maybe

		if (left == right)
		{
			result = TriState::True;
		}
		else if (left != TriState::Maybe &&
			right != TriState::Maybe)
		{
			result = TriState::False;
		}

		return result;
	}

	inline TriState triNotEqual(TriState left, TriState right)
	{
		TriState result = TriState::Maybe;

		// Note:
		// notEqual(TriState::Maybe, TriState::Maybe) = TriState::Maybe

		if (left == right)
		{
			result = TriState::False;
		}
		else if (left != TriState::Maybe &&
			right != TriState::Maybe)
		{
			result = TriState::True;
		}

		return result;
	}

	inline TriState triAnd(TriState left, TriState right)
	{
		TriState result = TriState::True;

		if (left == TriState::False || right == TriState::False)
		{
			result = TriState::False;
		}
		else if (left == TriState::Maybe || right == TriState::Maybe)
		{
			result = TriState::Maybe;
		}

		return result;
	}

	inline TriState triOr(
		TriState left, TriState right)
	{
		TriState result = TriState::False;

		if (left == TriState::True || right == TriState::True)
		{
			result = TriState::True;
		}
		else if (left == TriState::Maybe || right == TriState::Maybe)
		{
			result = TriState::Maybe;
		}

		return result;
	}

	inline TriState triXor(TriState left, TriState right)
	{
		TriState result = TriState::True;

		if (left == TriState::Maybe || right == TriState::Maybe)
		{
			result = TriState::Maybe;
		}
		else
		{
			if (left == right)
			{
				result = TriState::False;
			}
		}

		return result;
	}

	template <typename Type>
	TriState triLess(
		const Type& left, const Type& right)
	{
		if (left < right)
		{
			return TriState::True;
		}

		if (right < left)
		{
			return TriState::False;
		}

		return TriState::Both;
	}

}

#endif
