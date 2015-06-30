// Description: Interval multi-set

#ifndef PASTELSYS_INTERVAL_SET_H
#define PASTELSYS_INTERVAL_SET_H

#include "pastel/sys/set/set_concept.h"
#include "pastel/sys/algebra/element_concept.h"
#include "pastel/sys/type_traits/is_subtractable.h"

namespace Pastel
{

	//! An interval element.
	struct Interval_Element_Concept
		: Refines<Element_Concept>
	{
		template <typename Type>
		auto requires(Type&& t) -> decltype
		(
			conceptCheck(
				//! Constructs from another element.
				Type{t},
				//! Advances to the next element.
				++t
				//! Computes the distance between elements.
				/*!
				This is optional, and can be used to accelerate
				the computation of n().
				*/
				//Concept::convertsTo<integer>(t - t)
			)
		);
	};

}

namespace Pastel
{

	template <
		typename Element_,
		Requires<
			Models<Element_, Interval_Element_Concept>
		> = 0
	>
	class Interval_Set
	{
	public:
		using Element = Element_;
		using State = Element;

		Interval_Set(
			Element begin,
			Element end)
		: begin_(begin)
		, end_(end)
		{
		}

		template <
			typename Type = Element,
			Requires<
				Is_Subtractable<Type>
			> = 0
		>
		integer n() const
		{
			return end_ - begin_;
		}

		template <
			typename Type = Element,
			Requires<
				Not<Is_Subtractable<Type>>
			> = 0
		>
		integer n() const
		{
			integer count = 0;
			forEach([&](auto&&)
			{
				++count;
				return true;
			});

			return count;
		}

		State state() const
		{
			return begin_;
		}

		bool empty() const
		{
			return n() == 0;
		}

		bool empty(const State& state) const
		{
			return state == end_;
		}

		Element element(const State& state) const
		{
			Element element = state;
			return element;
		}

		void next(State& state) const
		{
			PENSURE(!empty(state));
			++state;
		}

		template <typename Visit>
		bool forEach(const Visit& visit) const
		{
			for (Element i{begin_};i != end_;++i)
			{
				if (!visit(i))
				{
					return false;
				}
			}

			return true;
		}

	private:
		Element begin_;
		Element end_;
	};

	template <typename Element>
	Interval_Set<RemoveCvRef<Element>> intervalSet(
		Element&& begin, 
		Element&& end)
	{
		return 
		{
			std::forward<Element>(begin), 
			std::forward<Element>(end)
		};
	}

}

#endif
