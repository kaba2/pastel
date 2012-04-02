#ifndef PASTEL_DESTRUCT_HPP
#define PASTEL_DESTRUCT_HPP

#include "pastel/sys/destruct.h"

#include <type_traits>

#include <boost/mpl/if.hpp>

namespace Pastel
{

	namespace Detail_Destruct
	{

		class General_Version
		{
		public:
			template <typename Type>
			static void work(Type* begin, Type* end)
			{
				Type* current = begin;
				while (current != end)
				{
					current->~Type();
					++current;
				}
			}
		};

		class Trivial_Version
		{
		public:
			template <typename Type>
			static void work(Type*, Type*)
			{
			}
		};

	}

	template <typename Type>
	void destruct(Type* begin, Type* end)
	{
		typedef typename boost::mpl::if_<
			std::has_trivial_destructor<Type>,
			Detail_Destruct::Trivial_Version,
			Detail_Destruct::General_Version>::type Destruct;

		Destruct::work(begin, end);
	}

	template <typename Type>
	void destruct(Type* that)
	{
		if (!std::has_trivial_destructor<Type>::value && that)
		{
			that->~Type();
		}
	}

}

#endif
