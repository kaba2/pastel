#ifndef PASTEL_TUPLE_TOOLS_HPP
#define PASTEL_TUPLE_TOOLS_HPP

#include "pastel/sys/tuple_tools.h"

namespace Pastel
{

	template <int N, typename Type>
	std::ostream& operator<<(std::ostream& stream, const Tuple<Type, N>& that)
	{
		if (that.empty())
		{
			return stream;
		}

		const integer size = that.size();

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

	template <int N, typename Real>
	inline bool allEqual(
		const Tuple<Real, N>& that)
	{
		const integer size = that.size();

		for (integer i = 1;i < size;++i)
		{
			if (!(that[i] == that[0]))
			{
				return false;
			}
		}

		return true;
	}

	template <int N, typename Real>
	inline bool allEqual(
		const Tuple<Real, N>& left,
		const Tuple<Real, N>& right)
	{
		PENSURE_OP(left.size(), ==, right.size());

		const integer size = left.size();

		for (integer i = 0;i < size;++i)
		{
			if (!(left[i] == right[i]))
			{
				return false;
			}
		}

		return true;
	}

	template <int N, typename Real>
	inline bool allEqual(
		const Tuple<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		const integer size = left.size();

		for (integer i = 0;i < size;++i)
		{
			if (!(left[i] == right))
			{
				return false;
			}
		}

		return true;
	}

	template <int N, typename Real>
	inline bool allEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Tuple<Real, N>& right)
	{
		const integer size = left.size();

		for (integer i = 0;i < size;++i)
		{
			if (!(left == right[i]))
			{
				return false;
			}
		}

		return true;
	}

	template <int N, typename Real>
	inline bool anyEqual(
		const Tuple<Real, N>& left,
		const Tuple<Real, N>& right)
	{
		PENSURE_OP(left.size(), ==, right.size());

		const integer size = left.size();

		for (integer i = 0;i < size;++i)
		{
			if (left[i] == right[i])
			{
				return true;
			}
		}

		return false;
	}

	template <int N, typename Real>
	inline bool anyEqual(
		const Tuple<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		const integer size = left.size();

		for (integer i = 0;i < size;++i)
		{
			if (left[i] == right)
			{
				return true;
			}
		}

		return false;
	}

	template <int N, typename Real>
	inline bool anyEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Tuple<Real, N>& right)
	{
		const integer size = right.size();

		for (integer i = 0;i < size;++i)
		{
			if (left == right[i])
			{
				return true;
			}
		}

		return false;
	}

	template <int N, typename Real>
	inline bool anyLess(
		const Tuple<Real, N>& left,
		const Tuple<Real, N>& right)
	{
		PENSURE_OP(left.size(), ==, right.size());

		const integer size = left.size();

		for (integer i = 0;i < size;++i)
		{
			if (left[i] < right[i])
			{
				return true;
			}
		}

		return false;
	}

	template <int N, typename Real>
	inline bool anyLess(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Tuple<Real, N>& right)
	{
		const integer size = right.size();

		for (integer i = 0;i < size;++i)
		{
			if (left < right[i])
			{
				return true;
			}
		}

		return false;
	}

	template <int N, typename Real>
	inline bool anyLess(
		const Tuple<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		const integer size = left.size();

		for (integer i = 0;i < size;++i)
		{
			if (left[i] < right)
			{
				return true;
			}
		}

		return false;
	}

	template <int N, typename Real>
	inline bool anyGreater(
		const Tuple<Real, N>& left,
		const Tuple<Real, N>& right)
	{
		return anyLess(right, left);
	}

	template <int N, typename Real>
	inline bool anyGreater(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Tuple<Real, N>& right)
	{
		return anyLess(right, left);
	}

	template <int N, typename Real>
	inline bool anyGreater(
		const Tuple<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return anyLess(right, left);
	}

	template <int N, typename Real>
	inline bool anyLessEqual(
		const Tuple<Real, N>& left,
		const Tuple<Real, N>& right)
	{
		return !(allLess(right, left));
	}

	template <int N, typename Real>
	inline bool anyLessEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Tuple<Real, N>& right)
	{
		return !(allLess(right, left));
	}

	template <int N, typename Real>
	inline bool anyLessEqual(
		const Tuple<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return !(allLess(right, left));
	}

	template <int N, typename Real>
	inline bool anyGreaterEqual(
		const Tuple<Real, N>& left,
		const Tuple<Real, N>& right)
	{
		return !(allLess(left, right));
	}

	template <int N, typename Real>
	inline bool anyGreaterEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Tuple<Real, N>& right)
	{
		return !(allLess(left, right));
	}

	template <int N, typename Real>
	inline bool anyGreaterEqual(
		const Tuple<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return !(allLess(left, right));
	}

	template <int N, typename Real>
	inline bool allLess(
		const Tuple<Real, N>& left,
		const Tuple<Real, N>& right)
	{
		PENSURE_OP(left.size(), ==, right.size());

		const integer size = left.size();

		for (integer i = 0;i < size;++i)
		{
			if (!(left[i] < right[i]))
			{
				return false;
			}
		}

		return true;
	}

	template <int N, typename Real>
	inline bool allLess(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Tuple<Real, N>& right)
	{
		const integer size = right.size();

		for (integer i = 0;i < size;++i)
		{
			if (!(left < right[i]))
			{
				return false;
			}
		}

		return true;
	}

	template <int N, typename Real>
	inline bool allLess(
		const Tuple<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		const integer size = right.size();

		for (integer i = 0;i < size;++i)
		{
			if (!(left[i] < right))
			{
				return false;
			}
		}

		return true;
	}

	template <int N, typename Real>
	inline bool allGreater(
		const Tuple<Real, N>& left,
		const Tuple<Real, N>& right)
	{
		return allLess(right, left);
	}

	template <int N, typename Real>
	inline bool allGreater(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Tuple<Real, N>& right)
	{
		return allLess(right, left);
	}

	template <int N, typename Real>
	inline bool allGreater(
		const Tuple<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return allLess(right, left);
	}

	template <int N, typename Real>
	inline bool allLessEqual(
		const Tuple<Real, N>& left,
		const Tuple<Real, N>& right)
	{
		return !(anyLess(right, left));
	}

	template <int N, typename Real>
	inline bool allLessEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Tuple<Real, N>& right)
	{
		return !(anyLess(right, left));
	}

	template <int N, typename Real>
	inline bool allLessEqual(
		const Tuple<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return !(anyLess(right, left));
	}

	template <int N, typename Real>
	inline bool allGreaterEqual(
		const Tuple<Real, N>& left,
		const Tuple<Real, N>& right)
	{
		return !(anyLess(left, right));
	}

	template <int N, typename Real>
	inline bool allGreaterEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Tuple<Real, N>& right)
	{
		return !(anyLess(left, right));
	}

	template <int N, typename Real>
	inline bool allGreaterEqual(
		const Tuple<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return !(anyLess(left, right));
	}

}

#endif
