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
		struct Index_
		{
			Element_ element_;
			integer number_;
		};

		using Element = Element_;
		using Index = Index_;

		Interval_Set_Seq(
			Element begin,
			Element end)
		: begin_{begin, 0}
		, end_{end, 0}
		{
			integer count = 0;
			for (Element i{begin};i != end;++i)
			{
				++count;
			}
			end_.number_ = count;
		}

		integer n() const
		{
			return end_.number();
		}

		const Index& index() const
		{
			return begin_;
		}

		bool empty() const
		{
			return end_.number_ == 0;
		}

		bool empty(const Index& index) const
		{
			return index.number_ == end_.number_;
		}

		integer number(const Index& index) const
		{
			return index.number_;
		}

		Element element(const Index& index) const
		{
			return index.element_;
		}

		/*
		void goto(Index& index, integer i)
		{
			PENSURE_RANGE(i, 0, n() + 1);
			if (index.number_ > i)
			{
				index = this->index();
			}

			while (index.number_ < i)
			{
				next(index);
			}
		}
		*/

		void next(Index& index) const
		{
			PENSURE(!empty(index));
			++index.element_;
			++index.number_;
		}

		template <typename Visit>
		bool forEach(const Visit& visit) const
		{
			for (Element i{begin_.element_};i != end_.element_;++i)
			{
				if (!visit(i))
				{
					return false;
				}
			}

			return true;
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
			return number(end_);
		}

		Index index() const
		{
			return begin_;
		}

		bool empty() const
		{
			return n() == 0;
		}

		bool empty(const Index& index) const
		{
			return index == end_;
		}

		integer number(const Index& index) const
		{
			return index - begin_;
		}

		Element element(const Index& index) const
		{
			return index;
		}

		/*
		void goto(Index& index, integer i)
		{
			PENSURE_RANGE(i, 0, n());
			index.goto(i, begin_);
		}
		*/

		void next(Index& index) const
		{
			PENSURE(!empty(index));
			++index;
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
