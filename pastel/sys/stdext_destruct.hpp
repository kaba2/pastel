#ifndef PASTEL_STDEXT_DESTRUCT_HPP
#define PASTEL_STDEXT_DESTRUCT_HPP

#include "pastel/sys/stdext_destruct.h"

#include <boost/type_traits/has_trivial_destructor.hpp>
#include <boost/mpl/if.hpp>

namespace Pastel
{

	namespace StdExt
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
				boost::has_trivial_destructor<Type>,
				Detail_Destruct::Trivial_Version,
				Detail_Destruct::General_Version>::type Destruct;

			Destruct::work(begin, end);
		}

		template <typename Type>
		void destruct(Type* that)
		{
			if (!boost::has_trivial_destructor<Type>())
			{
				that->~Type();
			}
		}

	}

}

#endif
