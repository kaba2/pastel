#ifndef PASTEL_TRISTATE_HPP
#define PASTEL_TRISTATE_HPP

#include "pastel/sys/tristate.h"

#include <algorithm>

namespace Pastel
{

	inline void swap(TriState::Enum& left, TriState::Enum& right)
	{
		std::swap(left, right);
	}

	inline TriState::Enum triNot(TriState::Enum that)
	{
		TriState::Enum result = TriState::Maybe;

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

	inline TriState::Enum triMix(TriState::Enum left, TriState::Enum right)
	{
		TriState::Enum result = TriState::Maybe;

		if (left == right &&
			left != TriState::Maybe)
		{
			result = left;
		}

		return result;
	}

	inline TriState::Enum triEqual(TriState::Enum left, TriState::Enum right)
	{
		TriState::Enum result = TriState::Maybe;

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

	inline TriState::Enum triNotEqual(TriState::Enum left, TriState::Enum right)
	{
		TriState::Enum result = TriState::Maybe;

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

	inline TriState::Enum triAnd(TriState::Enum left, TriState::Enum right)
	{
		TriState::Enum result = TriState::True;

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

	inline TriState::Enum triOr(TriState::Enum left, TriState::Enum right)
	{
		TriState::Enum result = TriState::False;

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

	inline TriState::Enum triXor(TriState::Enum left, TriState::Enum right)
	{
		TriState::Enum result = TriState::True;

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
	TriState::Enum triLess(
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
