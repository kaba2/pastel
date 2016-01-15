// Description: Dereferenced predicates
// Documentation: derived_predicates.txt

#ifndef PASTELSYS_DEREFERENCED_PREDICATE_H
#define PASTELSYS_DEREFERENCED_PREDICATE_H

#include "pastel/sys/predicate/predicate_concept.h"

#include "pastel/sys/operators.h"

namespace Pastel
{

	template <typename Predicate>
	class Dereferenced_Predicate
	{
	public:
		template <
			typename Left, 
			typename Right>
		bool operator()(
			const Left& left, 
			const Right& right) const
		{
			return Predicate()(*left, *right);
		}
	};

	using Dereferenced_LessThan = Dereferenced_Predicate<LessThan>;
	using Dereferenced_LessEqual = Dereferenced_Predicate<LessEqual>;
	using Dereferenced_GreaterThan = Dereferenced_Predicate<GreaterThan>;
	using Dereferenced_GreaterEqual = Dereferenced_Predicate<GreaterEqual>;
	using Dereferenced_EqualTo = Dereferenced_Predicate<EqualTo>;
	using Dereferenced_NotEqualTo = Dereferenced_Predicate<NotEqualTo>;

	template <typename Predicate>
	Dereferenced_Predicate<Predicate> dereferencedPredicate(
		const Predicate& predicate)
	{
		return Dereferenced_Predicate<Predicate>();
	}

}

#endif
