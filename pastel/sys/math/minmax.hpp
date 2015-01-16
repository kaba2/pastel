#ifndef PASTELSYS_MINMAX_HPP
#define PASTELSYS_MINMAX_HPP

#include "pastel/sys/minmax.h"

namespace Pastel
{

	template <typename Type>
		void minMax(
		const NoDeduction<Type>& a,
		const NoDeduction<Type>& b,
		Type& min, Type& max)
	{
		if (b < a)
		{
			min = b;
			max = a;
		}
		else
		{
			min = a;
			max = b;
		}
	}

	template <typename Type>
		void minMax(
		const NoDeduction<Type>& a,
		const NoDeduction<Type>& b,
		const NoDeduction<Type>& c,
		Type& min, Type& max)
	{
		if (b < a)
		{
			if (c < b)
			{
				// c < b < a
				min = c;
				max = a;
			}
			else
			{
				// a > b <= c
				min = b;
				if (c < a)
				{
					max = a;
				}
				else
				{
					max = c;
				}
			}
		}
		else
		{
			if (c < b)
			{
				// c < b >= a
				max = b;
				if (c < a)
				{
					min = c;
				}
				else
				{
					min = a;
				}
			}
			else
			{
				// c >= b >= a
				min = a;
				max = c;
			}
		}
	}

}

#endif
