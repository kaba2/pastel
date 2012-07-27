#ifndef PASTEL_OBJECT_FORWARDING_HPP
#define PASTEL_OBJECT_FORWARDING_HPP

#include "pastel/sys/object_forwarding.h"
#include "pastel/sys/mytypes.h"

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

}

#endif
