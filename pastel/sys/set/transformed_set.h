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

		using Index =
			typename Set::Index;

		template <
			typename Set_,
			typename Transform_>
		Transformed_Set(
			Set_&& set,
			Transform_&& transform)
		: set_(std::forward<Set_>(set))
		, transform_(std::forward<Transform_>(transform))
		{
		}

		const Set& set() const
		{
			return set_;
		}

		integer n() const
		{
			return set_.n();
		}

		Index index() const
		{
			return set_.index();
		}

		bool empty() const
		{
			return set_.empty();
		}

		bool empty(const Index& index) const
		{
			return set_.empty(index);
		}

		Element element(const Index& index) const
		{
			PENSURE(!empty(index));
			return transform_(set_.element(index));
		}

		integer next(Index& index, integer steps = 1) const
		{
			return set_.next(index, steps);
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
				Function_Concept<int>(typename RemoveCvRef<Set>::Element)
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
