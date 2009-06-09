#ifndef PASTEL_TUPLEBASE_H
#define PASTEL_TUPLEBASE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/log.h"
#include "pastel/sys/destruct.h"

#include <vector>
#include <algorithm>

#include <boost/static_assert.hpp>
#include <boost/operators.hpp>
#include <boost/type_traits/is_base_of.hpp>

namespace Pastel
{

	class Dimension
	{
	public:
		explicit Dimension(integer dimension)
			: dimension_(dimension)
		{
			PENSURE1(dimension >= 0, dimension);
		}

		operator integer() const
		{
			return dimension_;
		}

	private:
		// Prohibited.
		Dimension();

		integer dimension_;
	};

	template <typename Type>
	class Alias
	{
	public:
		explicit Alias(Type data)
			: data_(data)
		{
		}

		operator Type() const
		{
			return data_;
		}

	private:
		// Prohibited.
		Alias();

		Type data_;
	};

	inline Dimension ofDimension(integer dimension)
	{
		return Dimension(dimension);
	}

	template <typename Type>
	inline Alias<Type*> withAliasing(Type* data)
	{
		return Alias<Type*>(data);
	}

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

			explicit TupleBase(
				const Dimension& dimension,
				const Type& that)
				: data_()
			{
				PENSURE2(dimension == N, dimension, N);

				set(that);
			}

			// Using default copy constructor.
			// Maybe this is more efficient than doing
			// what's below, since 'data_' need not
			// be default initialized.
			/*
			TupleBase(const TupleBase& that)
				: data_()
			{
				std::copy(that.begin(), that.end(), begin());
			}
			*/

			// Note copy constructor won't match this function.
			template <typename ThatType>
			TupleBase(const TupleBase<N, ThatType>& that)
				: data_()
			{
				std::copy(that.begin(), that.end(), begin());
			}

			// Using default operator=.

			~TupleBase()
			{
				BOOST_STATIC_ASSERT(N == Dynamic || N > 0);

				enum
				{
					IsBase = boost::is_base_of<TupleBase, Tuple<N, Type> >::value
				};

				BOOST_STATIC_ASSERT(IsBase);
			}

			// The assignment need not be implemented, since
			// we settle for basic exception safety rather than strong
			// for performance (no element swapping).
			/*
			TupleBase<N, Type>& operator=(const TupleBase& that)
			{
				std::copy(that.begin(), that.end(), begin());
				
				return *this;
			}
			*/

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
				std::fill(begin(), end(), that);
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

			size_type dimension() const
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

			TemporaryTuple<N, Type>& asTemporary()
			{
				return (TemporaryTuple<N, Type>&)*this;
			}

		private:
			Type data_[N];
		};

		template <typename Type>
		class TupleBase<Dynamic, Type>
			: boost::equality_comparable<Tuple<Dynamic, Type> >
		{
		private:
			enum
			{
				N = Dynamic
			};
		public:
			template <int N, typename Type>
			friend class TupleBase;

			typedef Type value_type;
			typedef Type* iterator;
			typedef Type const* const_iterator;
			typedef std::reverse_iterator<iterator> reverse_iterator;
			typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
			typedef Type* pointer;
			typedef const Type* const_pointer;
			typedef Type& reference;
			typedef const Type& const_reference;
			typedef integer difference_type;
			typedef integer size_type;

			explicit TupleBase(
				const Dimension& dimension,
				const Type& that = Type())
				: data_(0)
				, size_(0)
				, deleteData_(true)
			{
				const integer size = dimension;
				allocate(size);
				
				try
				{
					std::uninitialized_fill_n(data_, size, that);
				}
				catch(...)
				{
					deallocate();
					throw;
				};
			}

			TupleBase(
				const TupleBase& that)
				: data_(0)
				, size_(0)
				, deleteData_(true)
			{
				allocate(that.size());
				
				try
				{
					copyConstruct(that);
				}
				catch(...)
				{
					deallocate();
					throw;
				};
			}

			TupleBase(
				const TupleBase& that,
				const Dimension& dimension,
				const Type& defaultData = Type())
				: data_(0)
				, size_(0)
				, deleteData_(true)
			{
				const integer size = dimension;
				allocate(size);
				
				try
				{
					copyConstruct(that, size, defaultData);
				}
				catch(...)
				{
					deallocate();
					throw;
				};
			}

			template <typename ThatType>
			TupleBase(
				const TupleBase<N, ThatType>& that)
				: data_(0)
				, size_(0)
				, deleteData_(true)
			{
				allocate(that.size());
				
				try
				{
					copyConstruct(that);
				}
				catch(...)
				{
					deallocate();
					throw;
				};
			}

			template <typename ThatType>
			TupleBase(
				const TupleBase<N, ThatType>& that,
				const Dimension& dimension,
				const Type& defaultData = Type())
				: data_(0)
				, size_(0)
				, deleteData_(true)
			{
				const integer size = dimension;
				allocate(size);
				
				try
				{
					copyConstruct(that, size, defaultData);
				}
				catch(...)
				{
					deallocate();
					throw;
				};
			}

			TupleBase(const TemporaryTuple<N, Type>& that)
				: data_(0)
				, size_(0)
				, deleteData_(true)
			{
				TemporaryTuple<N, Type>& moveThat =
					const_cast<TemporaryTuple<N, Type>&>(that);

				swap(moveThat);
			}

			TupleBase(
				const Dimension& dimension,
				const Alias<Type*> dataAlias)
				: data_(dataAlias)
				, size_(dimension)
				, deleteData_(false)
			{
			}

			~TupleBase()
			{
				if (deleteData_)
				{
					destruct(data_, data_ + size_);
					deallocate();
				}

				enum
				{
					IsBase = boost::is_base_of<TupleBase, Tuple<N, Type> >::value
				};

				BOOST_STATIC_ASSERT(IsBase);
			}

			TupleBase<N, Type>& operator=(const TupleBase& that)
			{
				// We settle for basic exception safety rather than strong
				// for performance (no memory reallocation).
				PENSURE2(size_ == that.size_, size_, that.size_);

				std::copy(that.begin(), that.end(), begin());
				
				return *this;
			}

			void setSize(integer size, const Type& that = Type())
			{
				ENSURE1(size >= 0, size);

				resize(size, that);
			}

			void resize(integer size, const Type& that = Type())
			{
				ENSURE1(size >= 0, size);

				TupleBase copy(*this, size, that);
				swap(copy);
			}

			void swap(Tuple<N, Type>& that)
			{
				std::swap(data_, that.data_);
				std::swap(size_, that.size_);
				std::swap(deleteData_, that.deleteData_);
			}

			void set(const Type& that)
			{
				std::fill(begin(), end(), that);
			}

			Type& front()
			{
				PENSURE(!empty());
				return *data_;
			}

			const Type& front() const
			{
				PENSURE(!empty());
				return *data_;
			}

			Type& back()
			{
				PENSURE(!empty());
				return *(data_ + size_ - 1);
			}

			const Type& back() const
			{
				PENSURE(!empty());
				return *(data_ + size_ - 1);
			}

			iterator begin()
			{
				return data_;
			}

			const_iterator begin() const
			{
				return data_;
			}

			iterator end()
			{
				return data_ + size_;
			}

			const_iterator end() const
			{
				return data_ + size_;
			}

			size_type capacity() const
			{
				return size_;
			}

			size_type size() const
			{
				return size_;
			}

			size_type dimension() const
			{
				return size_;
			}

			size_type max_size() const
			{
				return size_;
			}

			bool empty() const
			{
				return size_ == 0;
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

			TemporaryTuple<N, Type>& asTemporary()
			{
				return (TemporaryTuple<N, Type>&)*this;
			}

		private:
			void allocate(integer size)
			{
				PENSURE1(size >= 0, size);
				ASSERT(data_ == 0);
				ASSERT1(size_ == 0, size_);

				data_ = (Type*)allocateRaw(sizeof(Type) * size);
				size_ = size;
			}

			void deallocate()
			{
				if (data_)
				{
					deallocateRaw((void*)data_);
				}
				data_ = 0;
				size_ = 0;
			}

			template <typename ThatType>
			void copyConstruct(
				const TupleBase<N, ThatType>& that)
			{
				const integer size = that.size();
				ASSERT(size == size_);

				try
				{
					std::uninitialized_copy(
						that.data_, 
						that.data_ + size, 
						data_);
				}
				catch(...)
				{
					deallocate();
					throw;
				};
			}

			template <typename ThatType>
			void copyConstruct(
				const TupleBase<N, ThatType>& that,
				const Dimension& dimension,
				const Type& defaultData)
			{
				const integer size = dimension;

				const integer minSize = std::min(
					that.size(), size);
				
				integer rollBackIndex = 0;
				try
				{
					std::uninitialized_copy(
						that.data_, 
						that.data_ + minSize, 
						data_);
					++rollBackIndex;

					if (size > minSize)
					{
						std::uninitialized_fill_n(
							data_ + minSize,
							size - minSize,
							defaultData);
					}
				}
				catch(...)
				{
					switch(rollBackIndex)
					{
					case 1:
						destruct(data_, data_ + minSize);
						// Fall-through.
					case 0:
						deallocate();
						break;
					};
					throw;
				};
			}

			// Prohibited
			TupleBase();

			Type* data_;
			integer size_;
			bool deleteData_;
		};

	}

}

#endif
