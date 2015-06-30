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
		using State = integer;

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

		State state() const
		{
			return 0;
		}

		bool empty() const
		{
			return n() == 0;
		}

		bool empty(const State& state) const
		{
			return state == n();
		}

		const Element& element(const State& state) const
		{
			PENSURE(!empty(state));
			return element_;
		}

		void next(State& state) const
		{
			PENSURE(!empty(state));
			++state;
		}

		template <typename Visit>
		bool forEach(const Visit& visit) const
		{
			for (integer i = 0;i < n();++i)
			{
				if (!visit(element_))
				{
					return false;
				}
			}

			return true;
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
