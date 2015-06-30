// Description: Union multi-set

#ifndef PASTELSYS_UNION_SET_H
#define PASTELSYS_UNION_SET_H

#include "pastel/sys/set/set_concept.h"

namespace Pastel
{

	template <
		typename A_Set,
		typename B_Set,
		Requires<
			Models<A_Set, Set_Concept>,
			Models<B_Set, Set_Concept>,
			std::is_same<
				Set_Element<A_Set>,
				// Using Set_Element here triggers a bug in
				// Visual Studio 2015 RC.
				typename RemoveCvRef<B_Set>::Element
			>
		> = 0>
	class Union_Set
	{
	public:
		using Element = Set_Element<A_Set>;

		struct State
		{
			A_Set aState;
			B_Set bState;
		};

		Union_Set(
			A_Set aSet,
			B_Set bSet)
		: aSet_(std::move(aSet))
		, bSet_(std::move(bSet))
		{
		}

		integer n() const
		{
			return aSet_.n() + bSet_.n();
		}

		State state() const
		{
			return {aSet_.state(), bSet_.state()};
		}

		bool empty(const State& state) const
		{
			return state.aState.empty() && 
				state.bState.empty();
		}

		const Element& element(State& state) const
		{
			PENSURE(!empty(state));

			if (state.aState.empty())
			{
				return bSet_.element(state.bState);
			}
			
			return aSet_.element(state.aState);
		}

		template <typename Visit>
		bool forEach(const Visit& visit) const
		{
			if (aSet_.forEach(visit))
			{
				return bSet_.forEach(visit);
			}

			return false;
		}

	private:
		A_Set aSet_;
		B_Set bSet_;
	};

	template <
		typename A_Set,
		typename B_Set,
		Requires<
			Models<A_Set, Set_Concept>,
			Models<B_Set, Set_Concept>,
			std::is_same<
				Set_Element<A_Set>,
				// Using Set_Element here triggers a bug in
				// Visual Studio 2015 RC.
				typename RemoveCvRef<B_Set>::Element
			>
		> = 0>
	auto unionSet(
		A_Set&& aSet,
		B_Set&& bSet)
	-> Union_Set<
		RemoveCvRef<A_Set>, 
		RemoveCvRef<B_Set>>
	{
		return 
		{
			std::forward<A_Set>(aSet), 
			std::forward<B_Set>(bSet)
		};
	}

}

#endif
