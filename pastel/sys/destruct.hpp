#ifndef PASTELSYS_DESTRUCT_HPP
#define PASTELSYS_DESTRUCT_HPP

#include "pastel/sys/destruct.h"
#include <boost/type_traits/is_pod.hpp>
#include <boost/mpl/if.hpp>

namespace Pastel
{

		namespace Detail
		{

			class PodTag {};
			class NonPodTag {};

			template <
				typename Type>
			void destructHelper(
			Type* from, Type* to, NonPodTag)
			{
				// General version

				Type* current(from);
				while (current != to)
				{
					current->~Type();
					++current;
				}
			}

			template <typename Type>
			void destructHelper(
			Type*, Type*, PodTag)
			{
				// POD version
			}

		}

		template <typename Type>
		void destruct(Type* from, Type* to)
		{
			typedef typename boost::mpl::if_<
				boost::is_pod<Type>,
				Detail::PodTag,
				Detail::NonPodTag>::type Tag;

			Detail::destructHelper(
				from, to, Tag());
		}

}

#endif
