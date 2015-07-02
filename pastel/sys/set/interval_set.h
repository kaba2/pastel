// Description: Interval multi-set

#ifndef PASTELSYS_INTERVAL_SET_H
#define PASTELSYS_INTERVAL_SET_H

#include "pastel/sys/set/set_concept.h"
#include "pastel/sys/algebra/element_concept.h"
#include "pastel/sys/type_traits/is_subtractable.h"

namespace Pastel
{

	//! A random-access interval element.
	struct Interval_Set_Element_Seq_Concept
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
			)
		);
	};

}

namespace Pastel
{

	//! A random-access interval element.
	struct Interval_Set_Element_Ra_Concept
		: Refines<Interval_Set_Element_Seq_Concept>
	{
		template <typename Type>
		auto requires(Type&& t) -> decltype
		(
			conceptCheck(
				//! Computes the distance between elements.
				Concept::convertsTo<integer>(
					t - t),
				//! Advances an element by the given number of steps.
				Concept::convertsTo<Type>(
					t + std::declval<integer>())
			)
		);
	};

}

namespace Pastel
{

	template <
		typename Element_
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
		typename Element_
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
			Models<Element, Interval_Set_Element_Ra_Concept>::value,
			Interval_Set_Ra<Element>,
			std::conditional_t<
				Models<Element, Interval_Set_Element_Seq_Concept>::value,
				Interval_Set_Seq<Element>,
				void
			>
		>;

	template <typename Element>
	Interval_Set<RemoveCvRef<Element>> intervalSet(
		Element begin, 
		Element end)
	{
		return 
		{
			std::move(begin), 
			std::move(end)
		};
	}

}

#endif
