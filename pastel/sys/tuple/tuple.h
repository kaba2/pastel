// Description: Tuple

#ifndef PASTELSYS_TUPLE_H
#define PASTELSYS_TUPLE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/destruct.h"

#include <vector>
#include <algorithm>
#include <memory>

#include <boost/operators.hpp>

namespace Pastel
{

	template <int N, int NewN>
	inline constexpr int ModifyN = (N == Dynamic) ? Dynamic : NewN;

	template <int N, int i = 1>
	inline constexpr int AddN = ModifyN<N, N + i>;

	template <int N, int i = 1>
	inline constexpr int SubN = ModifyN<N, N - i>;

	class Dimension
	{
	public:
		explicit Dimension(integer dimension)
			: dimension_(dimension)
		{
			PENSURE_OP(dimension, >=, 0);
		}

		operator integer() const
		{
			return dimension_;
		}

	private:
		Dimension() = delete;

		integer dimension_;
	};

	inline std::string asString(Dimension n)
	{
		using Pastel::asString;
		return asString((integer)n);
	}

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
		Alias() = delete;

		Type data_;
	};

	template <typename Type>
	class Copy
	{
	public:
		explicit Copy(Type data)
			: data_(data)
		{
		}

		operator Type() const
		{
			return data_;
		}

	private:
		Copy() = delete;

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

	template <typename Type, int N = Dynamic>
	class Tuple
		: boost::equality_comparable<Tuple<Type, N> >
	{
	public:
		template <typename, int>
		friend class Tuple;

		using value_type = Type;
		using iterator = Type*;
		using const_iterator = const Type*;
		using reverse_iterator = Type*;
		using const_reverse_iterator = const Type*;
		using pointer = Type*;
		using const_pointer = const Type*;
		using reference = Type&;
		using const_reference = const Type&;
		using difference_type = integer;
		using size_type = integer;

		using Iterator = iterator;
		using ConstIterator = const_iterator;

		Tuple()
			: data_()
		{
		}

		explicit Tuple(const Type& that)
			: data_()
		{
			set(that);
		}

		explicit Tuple(const Type* that)
			: data_()
		{
			std::copy(that, that + N, data_);
		}

		explicit Tuple(
			const Dimension& dimension,
			const Type& that = Type())
			: data_()
		{
			PENSURE_OP(dimension, ==, N);

			set(that);
		}

		Tuple(
			const Dimension& dimension,
			const Copy<const Type*>& that)
			: data_()
		{
			PENSURE_OP(dimension, ==, N);
			std::copy((const Type*)that, (const Type*)that + N, data_);
		}

		// Alias for static tuple is copying.
		// This is needed so that one does not have to
		// make special cases in generic programming.
		Tuple(
			const Dimension& dimension,
			const Alias<Type*>& dataAlias)
			: Tuple(dimension, Copy<const Type*>(dataAlias))
		{
		}

		// Using default copy constructor.
		// Maybe this is more efficient than doing
		// what's below, since 'data_' need not
		// be default initialized.
		Tuple(const Tuple& that) = default;
		/*
		Tuple(const Tuple& that)
			: data_()
		{
			std::copy(that.begin(), that.end(), begin());
		}
		*/

		// Note copy constructor won't match this function.
		template <typename ThatType, int ThatN>
		Tuple(const Tuple<ThatType, ThatN>& that)
			: data_()
		{
			PENSURE_OP(size(), ==, that.size());
			std::copy(that.begin(), that.end(), begin());
		}

		template <int N_ = N> requires (N == 2)
		Tuple(const Type& x, const Type& y)
		{
			set(x, y);
		}

		template <int N_ = N> requires (N == 3)
		Tuple(const Type& x, const Type& y, const Type& z)
		{
			set(x, y, z);
		}

		template <int N_ = N> requires (N == 4)
		Tuple(const Type& x, const Type& y, 
			const Type& z, const Type& w)
		{
			set(x, y, z, w);
		}

		// Using default operator=.

		~Tuple()
		{
			PASTEL_STATIC_ASSERT(N == Dynamic || N > 0);

			static constexpr bool IsBase =
				std::is_base_of<Tuple, Tuple<Type, N> >::value;

			PASTEL_STATIC_ASSERT(IsBase);
		}

		// The assignment need not be implemented, since
		// we settle for basic exception safety rather than strong
		// for performance (no element swapping).
		/*
		Tuple<Type, N>& operator=(const Tuple& that)
		{
			std::copy(that.begin(), that.end(), begin());
			
			return *this;
		}
		*/

		Tuple<Type, N>& operator=(
			const std::initializer_list<Type>& that)
		{
			integer n = std::min(size(), (integer)that.size());
			std::copy_n(that.begin(), n, begin());
			return *this;
		}

		void swap(Tuple<Type, N> & that)
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

		size_type n() const
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

		//! Returns the address of the first element.
		Type* rawBegin()
		{
			return data_;
		}

		//! Returns the address of the first element.
		const Type* rawBegin() const
		{
			return data_;
		}

		//! Returns the address of the one-past last element.
		Type* rawEnd()
		{
			return data_ + N;
		}

		//! Returns the address of the one-past last element.
		const Type* rawEnd() const
		{
			return data_ + N;
		}

		template <int N_>
		bool operator==(const Tuple<Type, N_> & that) const
		{
			return std::equal(
				begin(), end(), that.begin());
		}

		template <int N_ = N> requires (N == 2)
		void set(
			const Type& x, const Type& y)
		{
			Tuple& v = *this;
			v[0] = x;
			v[1] = y;
		}

		template <int N_ = N> requires (N == 3)
		void set(
			const Type& x, const Type& y, 
			const Type& z)
		{
			Tuple& v = *this;
			v[0] = x;
			v[1] = y;
			v[2] = z;
		}

		template <int N_ = N> requires (N == 4)
		void set(
			const Type& x, const Type& y, 
			const Type& z, const Type& w)
		{
			Tuple& v = *this;
			v[0] = x;
			v[1] = y;
			v[2] = z;
			v[3] = w;
		}

		template <int N_ = N> requires (N >= 1)
		Type& x()
		{
			return (*this)[0];
		}

		template <int N_ = N> requires (N >= 1)
		const Type& x() const
		{
			return (*this)[0];
		}

		template <int N_ = N> requires (N >= 2)
		Type& y()
		{
			return (*this)[1];
		}

		template <int N_ = N> requires (N >= 2)
		const Type& y() const
		{
			return (*this)[1];
		}

		template <int N_ = N> requires (N >= 3)
		Type& z()
		{
			return (*this)[2];
		}

		template <int N_ = N> requires (N >= 3)
		const Type& z() const
		{
			return (*this)[2];
		}

		template <int N_ = N> requires (N >= 4)
		Type& w()
		{
			return (*this)[3];
		}

		template <int N_ = N> requires (N >= 4)
		const Type& w() const
		{
			return (*this)[3];
		}

	private:
		Type data_[N];
	};

	template <typename Type>
	class Tuple<Type, Dynamic>
		: boost::equality_comparable<Tuple<Type, Dynamic> >
	{
	private:
		static constexpr int N = Dynamic;
	
	public:
		template <typename, int>
		friend class Tuple;

		using value_type = Type;
		using iterator = Type*;
		using const_iterator = Type const*;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		using pointer = Type*;
		using const_pointer = const Type*;
		using reference = Type&;
		using const_reference = const Type&;
		using difference_type = integer;
		using size_type = integer;
		using Iterator = iterator;
		using ConstIterator = const_iterator;

		explicit Tuple(
			const Dimension& dimension,
			const Type& that = Type())
			: data_(0)
			, size_(0)
			, deleteData_(true)
		{
			integer size = dimension;
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

		Tuple(
			const Tuple& that)
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

		Tuple(
			const Tuple& that,
			const Dimension& dimension,
			const Type& defaultData = Type())
			: data_(0)
			, size_(0)
			, deleteData_(true)
		{
			integer size = dimension;
			allocate(size);
			
			try
			{
				copyConstruct(that, ofDimension(size), defaultData);
			}
			catch(...)
			{
				deallocate();
				throw;
			};
		}

		template <typename ThatType, int ThatN>
		Tuple(
			const Tuple<ThatType, ThatN>& that)
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

		template <typename ThatType, int ThatN>
		Tuple(
			const Tuple<ThatType, ThatN>& that,
			const Dimension& dimension,
			const Type& defaultData = Type())
			: data_(0)
			, size_(0)
			, deleteData_(true)
		{
			integer size = dimension;
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

		Tuple(
			const Dimension& dimension,
			const Alias<Type*>& dataAlias)
			: data_(dataAlias)
			, size_(dimension)
			, deleteData_(false)
		{
		}

		Tuple(
			const Dimension& dimension,
			const Copy<const Type*>& that)
			: data_(0)
			, size_(0)
			, deleteData_(true)
		{
			integer size = dimension;
			allocate(size);

			std::copy((const Type*)that, (const Type*)that + size, data_);
		}

		~Tuple()
		{
			if (deleteData_)
			{
				destruct(data_, data_ + size_);
				deallocate();
			}

			static constexpr bool IsBase = std::is_base_of<Tuple, Tuple<Type, N> >::value;

			PASTEL_STATIC_ASSERT(IsBase);
		}

		Tuple<Type, N>& operator=(const Tuple& that)
		{
			// We settle for basic exception safety rather than strong
			// for performance (no memory reallocation).
			PENSURE_OP(size_, ==, that.size_);

			std::copy(that.begin(), that.end(), begin());
			
			return *this;
		}

		Tuple<Type, N>& operator=(
			const std::initializer_list<Type>& that)
		{
			integer n = std::min(size(), (integer)that.size());
			std::copy_n(that.begin(), n, begin());
			return *this;
		}

		void setSize(integer size, const Type& that = Type())
		{
			ENSURE_OP(size, >=, 0);

			resize(size, that);
		}

		void resize(integer size, const Type& that = Type())
		{
			ENSURE_OP(size, >=, 0);

			Tuple copy(*this, ofDimension(size), that);
			swap(copy);
		}

		void swap(Tuple& that)
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

		size_type n() const
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

		//! Returns the address of the first element.
		Type* rawBegin()
		{
			return data_;
		}

		//! Returns the address of the first element.
		const Type* rawBegin() const
		{
			return data_;
		}

		//! Returns the address of the one-past last element.
		Type* rawEnd()
		{
			return data_ + size_;
		}

		//! Returns the address of the one-past last element.
		const Type* rawEnd() const
		{
			return data_ + size_;
		}

		bool operator==(const Tuple<Type, N> & that) const
		{
			PENSURE2(size() == that.size(), size(), that.size());

			return std::equal(
				begin(), end(), that.begin());
		}

	private:
		void allocate(integer size)
		{
			PENSURE_OP(size, >=, 0);
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

		template <typename ThatType, int ThatN>
		void copyConstruct(
			const Tuple<ThatType, ThatN>& that)
		{
			integer size = that.size();
			ASSERT_OP(size, ==, size_);

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

		template <typename ThatType, int ThatN>
		void copyConstruct(
			const Tuple<ThatType, ThatN>& that,
			const Dimension& dimension,
			const Type& defaultData)
		{
			integer size = dimension;

			integer minSize = std::min(
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

		Tuple() = delete;

		Type* data_;
		integer size_;
		bool deleteData_;
	};

}

namespace Pastel
{

	using Integer1 = Tuple<integer, 1>;
	using Type1 = Tuple<dreal, 1>;

	using Integer2 = Tuple<integer, 2>;
	using Type2 = Tuple<dreal, 2>;

	using Integer3 = Tuple<integer, 3>;
	using Type3 = Tuple<dreal, 3>;

	using Integer4 = Tuple<integer, 4>;
	using Type4 = Tuple<dreal, 4>;

	using IntegerD = Tuple<integer, Dynamic>;
	using TypeD = Tuple<dreal, Dynamic>;

}

#include "pastel/sys/tuple/tuple_tools.h"

#endif
