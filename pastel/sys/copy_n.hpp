#ifndef PASTELSYS_COPY_N_HPP
#define PASTELSYS_COPY_N_HPP

#include "pastel/sys/copy_n.h"

#include <boost/mpl/if.hpp>

#include <algorithm>
#include <iterator>
#include <type_traits>

namespace Pastel
{

	namespace CopyN_
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
			CopyN_::RandomAccess_Version,
			CopyN_::General_Version>::type Copy_n;
		
		Copy_n::work(sourceBegin, count, targetBegin);
	}
	
}

#endif
