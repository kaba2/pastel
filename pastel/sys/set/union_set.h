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

		struct Index
		{
			Set_Index<A_Set> aIndex;
			Set_Index<B_Set> bIndex;
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

		Index index() const
		{
			return {aSet_.index(), bSet_.index()};
		}

		bool empty() const
		{
			return aSet_.empty() && bSet_.empty();
		}

		bool empty(const Index& index) const
		{
			return aSet_.empty(index.aIndex) && 
				bSet_.empty(index.bIndex);
		}

		const Element& element(const Index& index) const
		{
			PENSURE(!empty(index));

			if (aSet_.empty(index.aIndex))
			{
				return bSet_.element(index.bIndex);
			}
			
			return aSet_.element(index.aIndex);
		}

		/*
		void goto(Index& index, integer i)
		{
			PENSURE_RANGE(i, 0, n());
			if (i < aSet_.n())
			{
				aSet_.goto(index.aIndex, i);
			}
			else
			{
				bSet_.goto(index.bIndex, i - aSet_.n());
			}
		}
		*/

		void next(Index& index) const
		{
			PENSURE(!empty(index));
			if (aSet_.empty(index.aIndex))
			{
				bSet_.next(index.bIndex);
			}
			else
			{
				aSet_.next(index.aIndex);
			}
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
