#ifndef PASTEL_COPY_N_HPP
#define PASTEL_COPY_N_HPP

#include "pastel/sys/copy_n.h"

#include <algorithm>
#include <iterator>

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_same.hpp>

namespace Pastel
{

	namespace Detail_Copy_n
	{

		class RandomAccess_Version
		{
		public:
			template <typename Source_ForwardIterator,
			typename Target_ForwardIterator>
			static void work(
				const Source_ForwardIterator& sourceBegin,
				integer count,
				const Target_ForwardIterator& targetBegin)
			{
				std::copy(sourceBegin, sourceBegin + count, targetBegin);
			}
		};

		class General_Version
		{
		public:
			template <typename Source_ForwardIterator,
			typename Target_ForwardIterator>
			static void work(
				Source_ForwardIterator sourceBegin,
				integer count,
				Target_ForwardIterator targetBegin)
			{
				for (integer i = 0;i < count;++i)
				{
					*targetBegin = *sourceBegin;
					++targetBegin;
					++sourceBegin;
				}
			}
		};

	}

	template <typename Source_ForwardIterator,
	typename Target_ForwardIterator>
	void copy_n(
		const Source_ForwardIterator& sourceBegin,
		integer count,
		const Target_ForwardIterator& targetBegin)
	{
		ENSURE_OP(count, >=, 0);

		typedef typename boost::mpl::if_<
			std::is_same<
			typename std::iterator_traits<Source_ForwardIterator>::iterator_category,
			std::random_access_iterator_tag>,
			Detail_Copy_n::RandomAccess_Version,
			Detail_Copy_n::General_Version>::type Copy_n;
		
		Copy_n::work(sourceBegin, count, targetBegin);
	}
	
}

#endif
