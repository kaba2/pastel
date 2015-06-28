// Description: Union multi-set

#ifndef PASTELSYS_UNION_MULTISET_H
#define PASTELSYS_UNION_MULTISET_H

#include "pastel/sys/set/multiset_concept.h"

namespace Pastel
{

	template <
		typename A_MultiSet,
		typename B_MultiSet,
		Requires<
			Models<A_MultiSet, MultiSet_Concept>,
			Models<B_MultiSet, MultiSet_Concept>,
			std::is_same<
				MultiSet_Element<A_MultiSet>,
				// Using MultiSet_Element here triggers a bug in
				// Visual Studio 2015 RC.
				typename RemoveCvRef<B_MultiSet>::Element
			>
		> = 0>
	class Union_MultiSet
	{
	public:
		using Element = MultiSet_Element<A_MultiSet>;

		Union_MultiSet(
			A_MultiSet aSet,
			B_MultiSet bSet)
		: aSet_(std::move(aSet))
		, bSet_(std::move(bSet))
		{
		}

		integer n() const
		{
			return aSet_.n() + bSet_.n();
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
		A_MultiSet aSet_;
		B_MultiSet bSet_;
	};

	template <
		typename A_MultiSet,
		typename B_MultiSet,
		Requires<
			Models<A_MultiSet, MultiSet_Concept>,
			Models<B_MultiSet, MultiSet_Concept>,
			std::is_same<
				MultiSet_Element<A_MultiSet>,
				// Using MultiSet_Element here triggers a bug in
				// Visual Studio 2015 RC.
				typename RemoveCvRef<B_MultiSet>::Element
			>
		> = 0>
	auto unionMultiSet(
		A_MultiSet&& aSet,
		B_MultiSet&& bSet)
	-> Union_MultiSet<
		RemoveCvRef<A_MultiSet>, 
		RemoveCvRef<B_MultiSet>>
	{
		return 
		{
			std::forward<A_MultiSet>(aSet), 
			std::forward<B_MultiSet>(bSet)
		};
	}

}

#endif
