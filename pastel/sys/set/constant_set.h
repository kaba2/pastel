// Description: Constant multi-set

#ifndef PASTELSYS_CONSTANT_SET_H
#define PASTELSYS_CONSTANT_SET_H

#include "pastel/sys/set/set_concept.h"

namespace Pastel
{

	template <typename Element_>
	class Constant_Set
	{
	public:
		using Element = Element_;
		using Index = integer;

		template <typename... ArgumentSet>
		explicit Constant_Set(
			integer n,
			ArgumentSet&&... argumentSet)
		: element_(std::forward<ArgumentSet>(argumentSet)...)
		, n_(n)
		{
			PENSURE_OP(n, >=, 0);
		}

		integer n() const
		{
			return n_;
		}

		Index index() const
		{
			return 0;
		}

		bool empty() const
		{
			return n() == 0;
		}

		bool empty(const Index& index) const
		{
			return index == n();
		}

		const Element& element(const Index& index) const
		{
			PENSURE(!empty(index));
			return element_;
		}

		integer next(Index& index, integer steps = 1) const
		{
			integer excess = steps - (n() - index);
			if (excess > 0)
			{
				index = n();
				return excess;
			}

			index += steps;
			return 0;
		}

	private:
		Element element_;
		integer n_;
	};

	template <
		typename Element,
		typename... ArgumentSet>
	Constant_Set<Element> constantSet(integer n, ArgumentSet&&... argumentSet)
	{
		return {n, std::forward<ArgumentSet>(argumentSet)...};
	}

}

#endif
