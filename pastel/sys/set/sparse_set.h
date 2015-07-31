// Description: Sparse set

#ifndef PASTELSYS_SPARSE_SET_H
#define PASTELSYS_SPARSE_SET_H

#include "pastel/sys/set/set_concept.h"

namespace Pastel
{

	template <
		typename Set,
		Requires<
			Models<Set, Set_Concept>
		> = 0>
	class Sparse_Set
	{
	public:
		using Element = Set_Element<Set>;
		using Index = Set_Index<Set>;

		template <typename Set_>
		Sparse_Set(
			Set_&& set,
			integer factor)
		: set_(std::forward<Set_>(set))
		, factor_(factor)
		{
			PENSURE_OP(factor, >, 0);
		}

		const Set& set() const
		{
			return set_;
		}

		integer factor() const
		{
			return factor_;
		}

		integer n() const
		{
			return set_.n() / factor_;
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

		// FIX: Change to use decltype(auto) when possible.
		// decltype(auto) in the return-type triggers
		// a bug in Visual Studio 2015
		Element element(const Index& index) const
		{
			return set_.element(index);
		}

		integer next(Index& index, integer steps = 1) const
		{
			return set_.next(index, steps * factor_);
		}

	private:
		Set set_;
		integer factor_;
	};

	template <
		typename Set,
		Requires<
			Models<Set, Set_Concept>
		> = 0>
	auto sparseSet(
		Set&& set,
		integer factor)
	-> Sparse_Set<RemoveCvRef<Set>>
	{
		return
		{
			std::forward<Set>(set),
			factor
		};
	}

}

#endif
