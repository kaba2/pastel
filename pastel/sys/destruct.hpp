#ifndef PASTELSYS_DESTRUCT_HPP
#define PASTELSYS_DESTRUCT_HPP

#include "pastel/sys/destruct.h"

#include <type_traits>

#include <boost/mpl/if.hpp>
#include <boost/type_traits/has_trivial_destructor.hpp>

namespace Pastel
{

	namespace Destruct_
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
			boost::has_trivial_destructor<Type>,
			Destruct_::Trivial_Version,
			Destruct_::General_Version>::type Destruct;

		Destruct::work(begin, end);
	}

	template <typename Type>
	void destruct(Type* that)
	{
		if (!boost::has_trivial_destructor<Type>::value && that)
		{
			that->~Type();
		}
	}

}

#endif
