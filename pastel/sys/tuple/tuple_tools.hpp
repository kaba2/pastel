#ifndef PASTELSYS_TUPLE_TOOLS_HPP
#define PASTELSYS_TUPLE_TOOLS_HPP

#include "pastel/sys/tuple/tuple_tools.h"

namespace Pastel
{

	template <typename Type, int N>
	std::ostream& operator<<(std::ostream& stream, const Tuple<Type, N>& that)
	{
		if (that.empty())
		{
			return stream;
		}

		integer size = that.size();

		stream << that.front();

		for (integer i = 1;i < size;++i)
		{
			stream << ", " << that[i];
		}
		
		return stream;
	}

	template <typename Type>
	Tuple<Type, 1> makeTuple(
		const Type& a)
	{
		return Tuple<Type, 1>(a);
	}

	template <typename Type>
	Tuple<Type, 2> makeTuple(
		const Type& a, const Type& b)
	{
		return Tuple<Type, 2>(a, b);
	}

	template <typename Type>
	Tuple<Type, 3> makeTuple(
		const Type& a, const Type& b, const Type& c)
	{
		return Tuple<Type, 3>(a, b, c);
	}

	template <typename Type>
	Tuple<Type, 4> makeTuple(
		const Type& a, const Type& b, const Type& c, const Type& d)
	{
		return Tuple<Type, 4>(a, b, c, d);
	}

	// Comparison functions

	template <typename Real, int N>
	inline bool allEqual(
		const Tuple<Real, N>& that)
	{
		integer size = that.size();

		for (integer i = 1;i < size;++i)
		{
			if (!(that[i] == that[0]))
			{
				return false;
			}
		}

		return true;
	}

	template <typename Real, int N>
	inline bool allEqual(
		const Tuple<Real, N>& left,
		const Tuple<Real, N>& right)
	{
		PENSURE_OP(left.size(), ==, right.size());

		integer size = left.size();

		for (integer i = 0;i < size;++i)
		{
			if (!(left[i] == right[i]))
			{
				return false;
			}
		}

		return true;
	}

	template <typename Real, int N>
	inline bool allEqual(
		const Tuple<Real, N>& left,
		const NoDeduction<Real>& right)
	{
		integer size = left.size();

		for (integer i = 0;i < size;++i)
		{
			if (!(left[i] == right))
			{
				return false;
			}
		}

		return true;
	}

	template <typename Real, int N>
	inline bool allEqual(
		const NoDeduction<Real>& left,
		const Tuple<Real, N>& right)
	{
		integer size = left.size();

		for (integer i = 0;i < size;++i)
		{
			if (!(left == right[i]))
			{
				return false;
			}
		}

		return true;
	}

	template <typename Real, int N>
	inline bool anyEqual(
		const Tuple<Real, N>& left,
		const Tuple<Real, N>& right)
	{
		PENSURE_OP(left.size(), ==, right.size());

		integer size = left.size();

		for (integer i = 0;i < size;++i)
		{
			if (left[i] == right[i])
			{
				return true;
			}
		}

		return false;
	}

	template <typename Real, int N>
	inline bool anyEqual(
		const Tuple<Real, N>& left,
		const NoDeduction<Real>& right)
	{
		integer size = left.size();

		for (integer i = 0;i < size;++i)
		{
			if (left[i] == right)
			{
				return true;
			}
		}

		return false;
	}

	template <typename Real, int N>
	inline bool anyEqual(
		const NoDeduction<Real>& left,
		const Tuple<Real, N>& right)
	{
		integer size = right.size();

		for (integer i = 0;i < size;++i)
		{
			if (left == right[i])
			{
				return true;
			}
		}

		return false;
	}

	template <typename Real, int N>
	inline bool anyLess(
		const Tuple<Real, N>& left,
		const Tuple<Real, N>& right)
	{
		PENSURE_OP(left.size(), ==, right.size());

		integer size = left.size();

		for (integer i = 0;i < size;++i)
		{
			if (left[i] < right[i])
			{
				return true;
			}
		}

		return false;
	}

	template <typename Real, int N>
	inline bool anyLess(
		const NoDeduction<Real>& left,
		const Tuple<Real, N>& right)
	{
		integer size = right.size();

		for (integer i = 0;i < size;++i)
		{
			if (left < right[i])
			{
				return true;
			}
		}

		return false;
	}

	template <typename Real, int N>
	inline bool anyLess(
		const Tuple<Real, N>& left,
		const NoDeduction<Real>& right)
	{
		integer size = left.size();

		for (integer i = 0;i < size;++i)
		{
			if (left[i] < right)
			{
				return true;
			}
		}

		return false;
	}

	template <typename Real, int N>
	inline bool anyGreater(
		const Tuple<Real, N>& left,
		const Tuple<Real, N>& right)
	{
		return anyLess(right, left);
	}

	template <typename Real, int N>
	inline bool anyGreater(
		const NoDeduction<Real>& left,
		const Tuple<Real, N>& right)
	{
		return anyLess(right, left);
	}

	template <typename Real, int N>
	inline bool anyGreater(
		const Tuple<Real, N>& left,
		const NoDeduction<Real>& right)
	{
		return anyLess(right, left);
	}

	template <typename Real, int N>
	inline bool anyLessEqual(
		const Tuple<Real, N>& left,
		const Tuple<Real, N>& right)
	{
		return !(allLess(right, left));
	}

	template <typename Real, int N>
	inline bool anyLessEqual(
		const NoDeduction<Real>& left,
		const Tuple<Real, N>& right)
	{
		return !(allLess(right, left));
	}

	template <typename Real, int N>
	inline bool anyLessEqual(
		const Tuple<Real, N>& left,
		const NoDeduction<Real>& right)
	{
		return !(allLess(right, left));
	}

	template <typename Real, int N>
	inline bool anyGreaterEqual(
		const Tuple<Real, N>& left,
		const Tuple<Real, N>& right)
	{
		return !(allLess(left, right));
	}

	template <typename Real, int N>
	inline bool anyGreaterEqual(
		const NoDeduction<Real>& left,
		const Tuple<Real, N>& right)
	{
		return !(allLess(left, right));
	}

	template <typename Real, int N>
	inline bool anyGreaterEqual(
		const Tuple<Real, N>& left,
		const NoDeduction<Real>& right)
	{
		return !(allLess(left, right));
	}

	template <typename Real, int N>
	inline bool allLess(
		const Tuple<Real, N>& left,
		const Tuple<Real, N>& right)
	{
		PENSURE_OP(left.size(), ==, right.size());

		integer size = left.size();

		for (integer i = 0;i < size;++i)
		{
			if (!(left[i] < right[i]))
			{
				return false;
			}
		}

		return true;
	}

	template <typename Real, int N>
	inline bool allLess(
		const NoDeduction<Real>& left,
		const Tuple<Real, N>& right)
	{
		integer size = right.size();

		for (integer i = 0;i < size;++i)
		{
			if (!(left < right[i]))
			{
				return false;
			}
		}

		return true;
	}

	template <typename Real, int N>
	inline bool allLess(
		const Tuple<Real, N>& left,
		const NoDeduction<Real>& right)
	{
		integer size = right.size();

		for (integer i = 0;i < size;++i)
		{
			if (!(left[i] < right))
			{
				return false;
			}
		}

		return true;
	}

	template <typename Real, int N>
	inline bool allGreater(
		const Tuple<Real, N>& left,
		const Tuple<Real, N>& right)
	{
		return allLess(right, left);
	}

	template <typename Real, int N>
	inline bool allGreater(
		const NoDeduction<Real>& left,
		const Tuple<Real, N>& right)
	{
		return allLess(right, left);
	}

	template <typename Real, int N>
	inline bool allGreater(
		const Tuple<Real, N>& left,
		const NoDeduction<Real>& right)
	{
		return allLess(right, left);
	}

	template <typename Real, int N>
	inline bool allLessEqual(
		const Tuple<Real, N>& left,
		const Tuple<Real, N>& right)
	{
		return !(anyLess(right, left));
	}

	template <typename Real, int N>
	inline bool allLessEqual(
		const NoDeduction<Real>& left,
		const Tuple<Real, N>& right)
	{
		return !(anyLess(right, left));
	}

	template <typename Real, int N>
	inline bool allLessEqual(
		const Tuple<Real, N>& left,
		const NoDeduction<Real>& right)
	{
		return !(anyLess(right, left));
	}

	template <typename Real, int N>
	inline bool allGreaterEqual(
		const Tuple<Real, N>& left,
		const Tuple<Real, N>& right)
	{
		return !(anyLess(left, right));
	}

	template <typename Real, int N>
	inline bool allGreaterEqual(
		const NoDeduction<Real>& left,
		const Tuple<Real, N>& right)
	{
		return !(anyLess(left, right));
	}

	template <typename Real, int N>
	inline bool allGreaterEqual(
		const Tuple<Real, N>& left,
		const NoDeduction<Real>& right)
	{
		return !(anyLess(left, right));
	}

}

#endif
