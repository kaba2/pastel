// Description: Transformed set

#ifndef PASTELSYS_TRANSFORMED_SET_H
#define PASTELSYS_TRANSFORMED_SET_H

#include "pastel/sys/set/set_concept.h"
#include "pastel/sys/function/function_concept.h"

namespace Pastel
{

	template <
		typename Set,
		typename Transform,
		Requires<
			Models<Set, Set_Concept>/*,
			Models<Transform, 
				Function_Concept(void (&)(typename Set::Element))
			>*/
		> = 0>
	class Transformed_Set
	{
	public:
		using Element = 
			RemoveCvRef<
				decltype(
					std::declval<Transform>()(
						std::declval<typename Set::Element>()
					)
				)
			>;

		using State =
			typename Set::State;

		template <
			typename Set_,
			typename Transform_>
		Transformed_Set(
			Set_ set,
			Transform_ transform)
		: set_(std::forward<Set_>(set))
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
		Set set_;
		Transform transform_;
	};

	template <
		typename Set,
		typename Transform,
		Requires<
			Models<Set, Set_Concept>/*,
			Models<Transform, 
				Function_Concept(int, typename RemoveCvRef<Set>::Element)
			>*/
		> = 0>
	auto transformedSet(
		Set&& set,
		Transform&& transform)
	-> Transformed_Set<RemoveCvRef<Set>, RemoveCvRef<Transform>>
	{
		return
		{
			std::forward<Set>(set),
			std::forward<Transform>(transform)
		};
	}

}

#endif
