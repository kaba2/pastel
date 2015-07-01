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
	class Interval_Set_Seq
	{
	public:
		using Element = Element_;
		using Index = Element_;

		Interval_Set_Seq(
			Element begin,
			Element end)
		: begin_(begin)
		, end_(end)
		{
		}

		integer n() const
		{
			integer count = 0;
			for (Element i{begin_};i != end_;++i)
			{
				++count;
			}
			return count;
		}

		const Index& index() const
		{
			return begin_;
		}

		bool empty() const
		{
			return begin_ == end_;
		}

		bool empty(const Index& index) const
		{
			return index == end_;
		}

		Element element(const Index& index) const
		{
			return index;
		}

		integer next(Index& index, integer steps = 1) const
		{
			PENSURE_OP(steps, >=, 0);

			while (steps > 0 && index != end_)
			{
				++index;
				--steps;
			}

			return steps;
		}

	private:
		Index begin_;
		Index end_;
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
	class Interval_Set_Ra
	{
	public:
		using Element = Element_;
		using Index = Element_;

		Interval_Set_Ra(
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

		Index index() const
		{
			return begin_;
		}

		bool empty() const
		{
			return begin_ == end_;
		}

		bool empty(const Index& index) const
		{
			return index == end_;
		}

		Element element(const Index& index) const
		{
			return index;
		}

		integer next(Index& index, integer steps = 1) const
		{
			PENSURE_OP(steps, >=, 0);

			integer excess = steps - (end_ - index);
			if (excess > 0)
			{
				index = end_;
				return excess;
			}

			index += steps;
			return 0;
		}

	private:
		Element begin_;
		Element end_;
	};

}

namespace Pastel
{

	template <typename Element>
	using Interval_Set =
		std::conditional_t<
			Is_Subtractable<Element>::value,
			Interval_Set_Ra<Element>,
			Interval_Set_Seq<Element>>;

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
