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

	template <
		typename Type, 
		typename Tag>
	class Member_Class
	{
	public:
		// We require that Type be a non-class, since otherwise
		// the constructor forwarding turns all constructors
		// implicit.
		PASTEL_STATIC_ASSERT(!std::is_class<Type>::value);

		template <
			typename... That,
			// Forward only those parameters which can be used to construct Type.
			typename = PASTEL_ENABLE_IF((std::is_constructible<Type, That...>), void)
			>
		Member_Class(That&&... that)
			: member_(std::forward<That>(that)...)
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

	template <typename Tag>
	class Member_Class<void, Tag>
	: boost::less_than_comparable<Member_Class<void, Tag>,
	boost::equality_comparable<Member_Class<void, Tag>
	> >
	{
	public:
		Member_Class()
		{
		}

		Member_Class(std::nullptr_t)
		{
		}

		bool operator<(const Member_Class& that) const
		{
			return false;
		}

		bool operator==(const Member_Class& that)
		{
			return true;
		}
	};

}

namespace std
{

	template <
		typename Type,
		typename Tag>
	struct hash<Pastel::Member_Class<Type, Tag>>
	{
	public:
		Pastel::hash_integer operator()(
			const Pastel::Member_Class<Type, Tag>& that) const
		{
			return Pastel::computeHash<Type>(that);
		}
	};

	template <
		typename Tag>
	struct hash<Pastel::Member_Class<void, Tag>>
	{
	public:
		Pastel::hash_integer operator()(
			const Pastel::Member_Class<void, Tag>& that) const
		{
			return Pastel::computeHash<void*>(0);
		}
	};

}

namespace Pastel
{

	template <
		typename Type, 
		typename Tag>
	class Inherited_Class
	: public Type
	{
	public:
		// We require Type to be a class; we inherit from Type.
		PASTEL_STATIC_ASSERT(std::is_class<Type>::value);

		//! Inherit most of the constructors from Type.
		/*!
		It is important to use inheritance --- rather than perfect 
		forwarding --- to preserve the explicitness of Type's
		constructors.
		*/
		using Type::Type;

		//! Constructs an empty class.
		/*!
		The default-constructor is not lifted from Type.
		*/
		Inherited_Class()
		: Type()
		{
		}

		//! Forwards copy-construction to Type.
		/*! 
		The copy-constructor is not lifted from Type.
		*/
		Inherited_Class(const Type& that)
		: Type(that)
		{
		}

		//! Forwards move-construction to Type.
		/*! 
		The move-constructor is not lifted from Type.
		*/
		Inherited_Class(Type&& that)
		: Type(std::move(that))
		{
		}

		//! Forwards assignment to Type.
		template <typename That>
		Inherited_Class& operator=(That&& that)
		{
			*static_cast<Type*>(this) = std::forward<That>(that);
			return *this;
		}
	};

}

namespace std
{

	template <
		typename Type,
		typename Tag>
	struct hash<Pastel::Inherited_Class<Type, Tag>>
	{
	public:
		Pastel::hash_integer operator()(
			const Pastel::Inherited_Class<Type, Tag>& that) const
		{
			return Pastel::computeHash<Type>(that);
		}
	};

}

#endif
