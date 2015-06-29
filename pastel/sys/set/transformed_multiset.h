// Description: Transformed set

#ifndef PASTELSYS_TRANSFORMED_SET_H
#define PASTELSYS_TRANSFORMED_SET_H

#include "pastel/sys/set/set_concept.h"
#include "pastel/sys/function/function_concept.h"

namespace Pastel
{

	template <
		typename MultiSet,
		typename Transform,
		Requires<
			Models<MultiSet, MultiSet_Concept>/*,
			Models<Transform, 
				Function_Concept(void (&)(typename MultiSet::Element))
			>*/
		> = 0>
	class Transformed_MultiSet
	{
	public:
		using Element = 
			RemoveCvRef<
				decltype(
					std::declval<Transform>()(
						std::declval<typename MultiSet::Element>()
					)
				)
			>;

		using State =
			typename MultiSet::State;

		template <
			typename MultiSet_,
			typename Transform_>
		Transformed_MultiSet(
			MultiSet_ set,
			Transform_ transform)
		: set_(std::forward<MultiSet_>(set))
		, transform_(std::forward<Transform_>(transform))
		{
		}

		integer n() const
		{
			return set_.n();
		}

		State state() const
		{
			return set_.state();
		}

		bool empty(const State& state) const
		{
			return set_.empty(state);
		}

		decltype(auto) element(State& state) const
		{
			PENSURE(!empty(state));

			return transform_(set_.element(state));
		}

		template <typename Visit>
		bool forEach(const Visit& visit) const
		{
			return set_.forEach([&](auto&& element)
			{
				return visit(transform_(element));
			});
		}

	private:
		MultiSet set_;
		Transform transform_;
	};

	template <
		typename MultiSet,
		typename Transform,
		Requires<
			Models<MultiSet, MultiSet_Concept>/*,
			Models<Transform, 
				Function_Concept(int, typename RemoveCvRef<MultiSet>::Element)
			>*/
		> = 0>
	auto transformedMultiSet(
		MultiSet&& set,
		Transform&& transform)
	-> Transformed_MultiSet<RemoveCvRef<MultiSet>, RemoveCvRef<Transform>>
	{
		return
		{
			std::forward<MultiSet>(set),
			std::forward<Transform>(transform)
		};
	}

}

#endif
