#ifndef PASTEL_TUPLE_TOOLS_HPP
#define PASTEL_TUPLE_TOOLS_HPP

#include "pastel/sys/tuple_tools.h"

namespace Pastel
{

	template <typename Type>
	Tuple<1, Type> makeTuple(
		const Type& a)
	{
		return Tuple<1, Type>(a);
	}

	template <typename Type>
	Tuple<2, Type> makeTuple(
		const Type& a, const Type& b)
	{
		return Tuple<2, Type>(a, b);
	}

	template <typename Type>
	Tuple<3, Type> makeTuple(
		const Type& a, const Type& b, const Type& c)
	{
		return Tuple<3, Type>(a, b, c);
	}

	template <typename Type>
	Tuple<4, Type> makeTuple(
		const Type& a, const Type& b, const Type& c, const Type& d)
	{
		return Tuple<4, Type>(a, b, c, d);
	}

	// Comparison functions

	template <int N, typename Real>
	inline bool allEqual(
		const Tuple<N, Real>& that)
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
		const Tuple<N, Real>& left,
		const Tuple<N, Real>& right)
	{
		PENSURE2(left.size() == right.size(), 
			left.size(), right.size());

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
		const Tuple<N, Real>& left,
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
		const Tuple<N, Real>& right)
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
		const Tuple<N, Real>& left,
		const Tuple<N, Real>& right)
	{
		PENSURE2(left.size() == right.size(), 
			left.size(), right.size());

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
		const Tuple<N, Real>& left,
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
		const Tuple<N, Real>& right)
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
		const Tuple<N, Real>& left,
		const Tuple<N, Real>& right)
	{
		PENSURE2(left.size() == right.size(), 
			left.size(), right.size());

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
		const Tuple<N, Real>& right)
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
		const Tuple<N, Real>& left,
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
		const Tuple<N, Real>& left,
		const Tuple<N, Real>& right)
	{
		return anyLess(right, left);
	}

	template <int N, typename Real>
	inline bool anyGreater(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Tuple<N, Real>& right)
	{
		return anyLess(right, left);
	}

	template <int N, typename Real>
	inline bool anyGreater(
		const Tuple<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return anyLess(right, left);
	}

	template <int N, typename Real>
	inline bool anyLessEqual(
		const Tuple<N, Real>& left,
		const Tuple<N, Real>& right)
	{
		return !(allLess(right, left));
	}

	template <int N, typename Real>
	inline bool anyLessEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Tuple<N, Real>& right)
	{
		return !(allLess(right, left));
	}

	template <int N, typename Real>
	inline bool anyLessEqual(
		const Tuple<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return !(allLess(right, left));
	}

	template <int N, typename Real>
	inline bool anyGreaterEqual(
		const Tuple<N, Real>& left,
		const Tuple<N, Real>& right)
	{
		return !(allLess(left, right));
	}

	template <int N, typename Real>
	inline bool anyGreaterEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Tuple<N, Real>& right)
	{
		return !(allLess(left, right));
	}

	template <int N, typename Real>
	inline bool anyGreaterEqual(
		const Tuple<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return !(allLess(left, right));
	}

	template <int N, typename Real>
	inline bool allLess(
		const Tuple<N, Real>& left,
		const Tuple<N, Real>& right)
	{
		PENSURE2(left.size() == right.size(), 
			left.size(), right.size());

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
		const Tuple<N, Real>& right)
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
		const Tuple<N, Real>& left,
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
		const Tuple<N, Real>& left,
		const Tuple<N, Real>& right)
	{
		return allLess(right, left);
	}

	template <int N, typename Real>
	inline bool allGreater(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Tuple<N, Real>& right)
	{
		return allLess(right, left);
	}

	template <int N, typename Real>
	inline bool allGreater(
		const Tuple<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return allLess(right, left);
	}

	template <int N, typename Real>
	inline bool allLessEqual(
		const Tuple<N, Real>& left,
		const Tuple<N, Real>& right)
	{
		return !(anyLess(right, left));
	}

	template <int N, typename Real>
	inline bool allLessEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Tuple<N, Real>& right)
	{
		return !(anyLess(right, left));
	}

	template <int N, typename Real>
	inline bool allLessEqual(
		const Tuple<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return !(anyLess(right, left));
	}

	template <int N, typename Real>
	inline bool allGreaterEqual(
		const Tuple<N, Real>& left,
		const Tuple<N, Real>& right)
	{
		return !(anyLess(left, right));
	}

	template <int N, typename Real>
	inline bool allGreaterEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Tuple<N, Real>& right)
	{
		return !(anyLess(left, right));
	}

	template <int N, typename Real>
	inline bool allGreaterEqual(
		const Tuple<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return !(anyLess(left, right));
	}

}

#endif
