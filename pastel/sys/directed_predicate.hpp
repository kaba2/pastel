#ifndef PASTELSYS_DIRECTED_PREDICATE_HPP
#define PASTELSYS_DIRECTED_PREDICATE_HPP

#include "pastel/sys/directed_predicate.h"

namespace Pastel
{

	template <typename Predicate>
	class Directed_Predicate<Predicate, true>
	{
	public:
		template <typename Left, typename Right>
		bool operator()(const Left& left, const Right& right)
		{
			return Predicate()(left, right);
		}
	};

	template <typename Predicate>
	class Directed_Predicate<Predicate, false>
	{
	public:
		template <typename Left, typename Right>
		bool operator()(const Left& left, const Right& right)
		{
			return Predicate()(right, left);
		}
	};

}

#endif
