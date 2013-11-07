#ifndef PASTELSYS_OBJECT_FORWARDING_HPP
#define PASTELSYS_OBJECT_FORWARDING_HPP

#include "pastel/sys/object_forwarding.h"
#include "pastel/sys/mytypes.h"
#include "pastel/sys/hashing.h"

#include <utility>
#include <type_traits>

namespace Pastel
{

	template <typename Type>
	class Class_Forward
	{
	public:
		// Only scalar types are allowed to be forwarded.
		PASTEL_STATIC_ASSERT(std::is_scalar<Type>::value);

		Class_Forward()
			: member_()
		{
		}

		template <typename That>
		Class_Forward(That&& that)
			: member_(std::forward<That>(that))
		{
		}

		operator Type&()
		{
			return member_;
		}

		operator const Type&() const
		{
			return member_;
		}

	private:
		Type member_;
	};

}

namespace std
{

	template <typename Type>
	struct hash<Pastel::Class_Forward<Type>>
	{
	public:
		Pastel::hash_integer operator()(
			const Pastel::Class_Forward<Type>& that) const
		{
			return Pastel::computeHash<Type>(that);
		}
	};

	template <>
	struct hash<Pastel::Class_Forward<void>>
	{
	public:
		Pastel::hash_integer operator()(
			const Pastel::Class_Forward<void>& that) const
		{
			return Pastel::computeHash<void*>(0);
		}
	};

}

#endif
