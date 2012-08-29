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
	class Class
	{
	public:
		// Only non-class types are allowed to be forwarded.
		PASTEL_STATIC_ASSERT(!std::is_class<Type>::value);

		Class()
			: member_()
		{
		}

		Class(Type that)
			: member_(std::move(that))
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
	class hash<Pastel::Class<Type>>
	{
	public:
		Pastel::hash_integer operator()(
			const Pastel::Class<Type>& that) const
		{
			return Pastel::computeHash<Type>(that);
		}
	};

	template <>
	class hash<Pastel::Class<void>>
	{
	public:
		Pastel::hash_integer operator()(
			const Pastel::Class<void>& that) const
		{
			return Pastel::computeHash<void*>(0);
		}
	};

}

#endif
