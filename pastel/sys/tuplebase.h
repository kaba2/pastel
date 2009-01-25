#ifndef PASTELSYS_TUPLEBASE_H
#define PASTELSYS_TUPLEBASE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"

#include <boost/static_assert.hpp>
#include <boost/operators.hpp>

namespace Pastel
{

	namespace Detail
	{

		template <int N, typename Type, typename Derived>
		class TupleBase
			: boost::equality_comparable<Derived>
		{
		public:
			// Using default constructor.
			// Using default copy constructor.
			// Using default assignment.
			// Using default destructor.

			template <int N, typename Type, typename Derived>
			friend class TupleBase;

			typedef Type value_type;
			typedef Type* iterator;
			typedef const Type* const_iterator;
			typedef Type* reverse_iterator;
			typedef const Type* const_reverse_iterator;
			typedef Type* pointer;
			typedef const Type* const_pointer;
			typedef Type& reference;
			typedef const Type& const_reference;
			typedef integer difference_type;
			typedef integer size_type;

			TupleBase()
				: data_()
			{
			}

			explicit TupleBase(const Type& that)
				: data_()
			{
				set(that);
			}

			template <typename ThatType, typename ThatDerived>
			TupleBase(const TupleBase<N, ThatType, ThatDerived>& that)
				: data_()
			{
				for (integer i = 0;i < N;++i)
				{
					data_[i] = that.data_[i];
				}
			}

			~TupleBase()
			{
				BOOST_STATIC_ASSERT(N > 0);
			}

            void swap(Derived& that)
            {
				using std::swap;

				for (integer i = 0;i < N;++i)
				{
                    swap(data_[i], that.data_[i]);
				}
            }

			void set(const Type& that)
			{
				for (integer i = 0;i < N;++i)
				{
					data_[i] = that;
				}
			}

			Type& front()
			{
				return data_[0];
			}

			const Type& front() const
			{
				return data_[0];
			}

			Type& back()
			{
				return data_[N - 1];
			}

			const Type& back() const
			{
				return data_[N - 1];
			}

			Type* begin()
			{
				return &data_[0];
			}

			const Type* begin() const
			{
				return &data_[0];
			}

			Type* end()
			{
				return &data_[0] + N;
			}

			const Type* end() const
			{
				return &data_[0] + N;
			}

			size_type capacity() const
			{
				return N;
			}

			size_type size() const
			{
				return N;
			}

			size_type max_size() const
			{
				return N;
			}

			bool empty() const
			{
				return false;
			}

			Type& at(integer index)
			{
				PENSURE2(index >= 0 && index < N, index, N);

				return data_[index];
			}

			const Type& at(integer index) const
			{
				PENSURE2(index >= 0 && index < N, index, N);

				return data_[index];
			}

			Type& operator[](integer index)
			{
				PENSURE2(index >= 0 && index < N, index, N);
				return data_[index];
			}

			const Type& operator[](integer index) const
			{
				PENSURE2(index >= 0 && index < N, index, N);

				return data_[index];
			}

			bool operator==(const Derived& that) const
			{
				for (integer i = 0;i < N;++i)
				{
					if (!(data_[i] == that.data_[i]))
					{
						return false;
					}
				}

				return true;
			}

		private:
			Type data_[N];
		};

	}

}

#endif
