#ifndef PASTELSYS_CLASS_HPP
#define PASTELSYS_CLASS_HPP

#include "pastel/sys/class.h"
#include "pastel/sys/mytypes.h"
#include "pastel/sys/hashing.h"

#include <boost/operators.hpp>

#include <utility>
#include <type_traits>

namespace Pastel
{

	template <typename Type>
	class Class
	{
	public:
		PASTEL_STATIC_ASSERT(std::is_scalar<Type>::value);

		Class()
			: member_()
		{
		}

		Class(Type that)
			: member_(that)
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

	template <>
	class Class<void>
	: boost::less_than_comparable<Class<void>,
	boost::equality_comparable<Class<void>
	> >
	{
	public:
		Class()
		{
		}

		Class(std::nullptr_t)
		{
		}

		bool operator<(const Class& that) const
		{
			return false;
		}

		bool operator==(const Class& that)
		{
			return true;
		}
	};

}

namespace std
{

	template <typename Type>
	struct hash<Pastel::Class<Type>>
	{
	public:
		Pastel::hash_integer operator()(
			const Pastel::Class<Type>& that) const
		{
			return Pastel::computeHash<Type>(that);
		}
	};

	template <>
	struct hash<Pastel::Class<void>>
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
