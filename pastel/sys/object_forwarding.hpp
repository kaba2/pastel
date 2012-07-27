#ifndef PASTEL_OBJECT_FORWARDING_HPP
#define PASTEL_OBJECT_FORWARDING_HPP

#include "pastel/sys/object_forwarding.h"
#include "pastel/sys/mytypes.h"

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

		template <typename P1>
		Class(P1&& p1)
			: member_(
			std::forward<P1>(p1))
		{
		}

		template <
			typename P1, typename P2>
			Class(P1&& p1, P2&& p2)
			: member_(
			std::forward<P1>(p1), std::forward<P2>(p2))
		{
		}

		template <
			typename P1, typename P2, 
			typename P3>
			Class(P1&& p1, P2&& p2, P3&& p3)
			: member_(
			std::forward<P1>(p1), std::forward<P2>(p2),
			std::forward<P3>(p3))
		{
		}

		template <
			typename P1, typename P2, 
			typename P3, typename P4>
			Class(P1&& p1, P2&& p2, P3&& p3, P4&& p4)
			: member_(
			std::forward<P1>(p1), std::forward<P1>(p2),
			std::forward<P3>(p3), std::forward<P4>(p4))
		{
		}

		template <
			typename P1, typename P2, 
			typename P3, typename P4,
			typename P5>
			Class(
			P1&& p1, P2&& p2, P3&& p3, P4&& p4, 
			P5&& p5)
			: member_(
			std::forward<P1>(p1), std::forward<P2>(p2),
			std::forward<P3>(p3), std::forward<P4>(p4),
			std::forward<P5>(p5))
		{
		}

		template <
			typename P1, typename P2, 
			typename P3, typename P4,
			typename P5, typename P6>
			Class(
			P1&& p1, P2&& p2, P3&& p3, P4&& p4, 
			P5&& p5, P6&& p6)
			: member_(
			std::forward<P1>(p1), std::forward<P2>(p2),
			std::forward<P3>(p3), std::forward<P4>(p4),
			std::forward<P5>(p5), std::forward<P6>(p6))
		{
		}

		~Class()
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
	{
	};

}

#endif
