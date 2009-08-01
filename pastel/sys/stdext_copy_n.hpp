#ifndef PASTEL_STDEXT_COPY_N_HPP
#define PASTEL_STDEXT_COPY_N_HPP

#include "pastel/sys/stdext_copy_n.h"

#include <algorithm>

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_same.hpp>

namespace Pastel
{

	namespace StdExt
	{

		namespace Detail_Copy_n
		{

			class RandomAccess
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

			class General
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

			boost::mpl::if_<
				boost::mpl::is_same<
				typename Source_ForwardIterator::iterator_category,
				std::random_access_iterator_tag>,
				Detail_Copy_n::RandomAccess,
				Detail_Copy_n::General>::work(
				sourceBegin, count, targetBegin);
		}
	
	}

}

#endif
