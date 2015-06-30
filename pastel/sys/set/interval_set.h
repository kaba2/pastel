// Description: Interval multi-set

#ifndef PASTELSYS_INTERVAL_SET_H
#define PASTELSYS_INTERVAL_SET_H

#include "pastel/sys/set/set_concept.h"
#include "pastel/sys/algebra/element_concept.h"

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
				++t,
				//! Computes the distance between elements.
				Concept::convertsTo<integer>(t - t)
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

		integer n() const
		{
			return end_ - begin_;
		}

		State state() const
		{
			return begin_;
		}

		bool empty(const State& state) const
		{
			return state == end_;
		}

		Element element(State& state) const
		{
			Element element = state;
			++state;
			return element;
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
