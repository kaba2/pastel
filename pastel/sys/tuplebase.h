#ifndef PASTELSYS_TUPLEBASE_H
#define PASTELSYS_TUPLEBASE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"

#include <vector>

#include <boost/static_assert.hpp>
#include <boost/operators.hpp>
#include <boost/type_traits/is_base_of.hpp>

namespace Pastel
{

	template <int N, typename Type>
	class Tuple;

	template <int N, typename Type>
	class TemporaryTuple;

	namespace Detail
	{

		template <int N, typename Type>
		class TupleBase
			: boost::equality_comparable<Tuple<N, Type> >
		{
		public:
			// Using default constructor.
			// Using default copy constructor.
			// Using default assignment.
			// Using default destructor.

			template <int N, typename Type>
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

			template <typename ThatType>
			TupleBase(const TupleBase& that)
				: data_()
			{
				for (integer i = 0;i < N;++i)
				{
					data_[i] = that.data_[i];
				}
			}

			template <typename ThatType>
			TupleBase(const TupleBase<N, ThatType>& that)
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

				enum
				{
					IsBase = boost::is_base_of<TupleBase, Tuple<N, Type> >::value
				};

				BOOST_STATIC_ASSERT(IsBase);
			}

			void swap(Tuple<N, Type> & that)
			{
				using std::swap;

				for (integer i = 0;i < N;++i)
				{
					swap(data_[i], that.data_[i]);
				}
			}

			void setSize(integer size, const Type& that = Type())
			{
				ENSURE2(size >= 0 && size <= N, size, N);
				// Do nothing.
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
				PENSURE2(index >= 0 && index < size(), index, size());

				return data_[index];
			}

			const Type& at(integer index) const
			{
				PENSURE2(index >= 0 && index < size(), index, size());

				return data_[index];
			}

			Type& operator[](integer index)
			{
				PENSURE2(index >= 0 && index < size(), index, size());
				return data_[index];
			}

			const Type& operator[](integer index) const
			{
				PENSURE2(index >= 0 && index < size(), index, size());

				return data_[index];
			}

			bool operator==(const Tuple<N, Type> & that) const
			{
				const integer n = size();
				for (integer i = 0;i < n;++i)
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

		template <typename Type>
		class TupleBase<Unbounded, Type>
			: boost::equality_comparable<Tuple<Unbounded, Type> >
		{
		private:
			typedef std::vector<Type> Container;

		public:
			// Using default constructor.
			// Using default copy constructor.
			// Using default assignment.
			// Using default destructor.

			template <int N, typename Type>
			friend class TupleBase;

			typedef Type value_type;
			typedef typename Container::iterator iterator;
			typedef typename Container::const_iterator const_iterator;
			typedef typename Container::reverse_iterator reverse_iterator;
			typedef typename Container::const_reverse_iterator const_reverse_iterator;
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

			template <typename ThatType>
			TupleBase(const TupleBase& that)
				: data_(that.data_)
			{
			}

			template <typename ThatType>
			TupleBase(const TupleBase<Unbounded, ThatType>& that)
				: data_()
			{
				const integer n = that.size();
				data_.reserve(n);

				for (integer i = 0;i < n;++i)
				{
					data_.push_back(that.data_[i]);
				}
			}

			TupleBase(const TemporaryTuple<Unbounded, Type>& that)
				: data_()
			{
				TemporaryTuple<Unbounded, Type>& moveThat = 
					const_cast<TemporaryTuple<Unbounded, Type>&>(that);
				data_.swap(moveThat.data_);
			}

			~TupleBase()
			{
				enum
				{
					IsBase = boost::is_base_of<TupleBase, Tuple<Unbounded, Type> >::value
				};

				BOOST_STATIC_ASSERT(IsBase);
			}

			void setSize(integer size, const Type& that = Type())
			{
				ENSURE1(size >= 0, size);

				resize(size, that);
			}

			void resize(integer size, const Type& that = Type())
			{
				ENSURE1(size >= 0, size);

				data_.resize(size, that);
			}

			void swap(Tuple<Unbounded, Type>& that)
			{
				data_.swap(that.data_);
			}

			Tuple<Unbounded, Type>& operator=(const Tuple<Unbounded, Type>& that)
			{
				Tuple<Unbounded, Type> copy(that);
				swap(copy);
				return (Tuple<Unbounded, Type>&)*this;
			}

			TemporaryTuple<Unbounded, Type>& operator=(
				const TemporaryTuple<Unbounded, Type>& that)
			{
				Tuple<Unbounded, Type> copy(that);
				swap(copy);
				return (Tuple<Unbounded, Type>&)*this;
			}

			void set(const Type& that)
			{
				const integer n = size();

				for (integer i = 0;i < n;++i)
				{
					data_[i] = that;
				}
			}

			Type& front()
			{
				return data_.front();
			}

			const Type& front() const
			{
				return data_.front();
			}

			Type& back()
			{
				return data_.back();
			}

			const Type& back() const
			{
				return data_.back();
			}

			iterator begin()
			{
				return data.begin();
			}

			const_iterator begin() const
			{
				return data.begin();
			}

			iterator end()
			{
				return data.end();
			}

			const_iterator end() const
			{
				return data.end();
			}

			size_type capacity() const
			{
				return data_.capacity();
			}

			size_type size() const
			{
				return data_.size();
			}

			size_type max_size() const
			{
				return data_.max_size();
			}

			bool empty() const
			{
				return data_.empty();
			}

			Type& at(integer index)
			{
				PENSURE2(index >= 0 && index < size(), index, size());

				return data_[index];
			}

			const Type& at(integer index) const
			{
				PENSURE2(index >= 0 && index < size(), index, size());

				return data_[index];
			}

			Type& operator[](integer index)
			{
				PENSURE2(index >= 0 && index < size(), index, size());
				return data_[index];
			}

			const Type& operator[](integer index) const
			{
				PENSURE2(index >= 0 && index < size(), index, size());

				return data_[index];
			}

			bool operator==(const Tuple<Unbounded, Type> & that) const
			{
				PENSURE2(size() == that.size(), size(), that.size());

				const integer n = size();
				for (integer i = 0;i < n;++i)
				{
					if (!(data_[i] == that.data_[i]))
					{
						return false;
					}
				}

				return true;
			}

		private:
			Container data_;
		};

	}

}

#endif
