// Description: Optional object

#ifndef PASTELSYS_OPTIONAL_H
#define PASTELSYS_OPTIONAL_H

#include "pastel/sys/hashing.h"

#include <utility>

namespace Pastel
{

	class Epsilon {};

	template <typename Type>
	class Optional
	{
	public:
		Optional()
			: data_()
			, empty_(true)
		{
		}

		Optional(Epsilon)
			: data_()
			, empty_(true)
		{
		}

		Optional(const Optional& that)
			: data_(that.data())
			, empty_(that.empty_)
		{
		}

		template <typename That>
		Optional(const Optional<That>& that)
			: data_(that.data())
			, empty_(that.empty_)
		{
			// Note that this function never 
			// matches the copy-constructor.
		}

		Optional(Optional&& that)
			: data_(std::move(that.data()))
			, empty_(std::move(that.empty_))
		{
		}

		template <typename That>
		Optional(Optional<That>&& that)
			: data_(std::move(that.data()))
			, empty_(std::move(that.empty_))
		{
			// Note that this function never 
			// matches the move-constructor.
		}

		Optional(Type that)
			: data_(std::move(that))
			, empty_(false)
		{
		}

		Type& operator*()
		{
			return data_;
		}

		const Type& operator*() const
		{
			return data_;
		}
	
		Type* operator->()
		{
			return &data_;
		}

		const Type* operator->() const
		{
			return &data_;
		}

		Type& data()
		{
			return data_;
		}

		const Type& data() const
		{
			return data_;
		}

		Optional& operator=(Optional that)
		{
			data_ = std::move(that.data());
			empty_ = std::move(that.empty_);
			return *this;
		}

		template <typename That>
		bool operator==(const Optional<That>& that) const
		{
			return compare(that);
		}

		template <typename That>
		bool operator!=(const Optional<That>& that) const
		{
			return !compare(that);
		}

		template <typename That>
		bool operator==(const That& that) const
		{
			// Treat 'that' as if it was a non-empty
			// optional.

			if (empty())
			{
				return false;
			}

			return data() == that;
		}

		template <typename That>
		bool operator!=(const That& that) const
		{
			return !(*this == that);
		}

		void swap(Optional& that)
		{
			using std::swap;

			swap((Type&)*this, (Type&)that);
			swap(empty_, that.empty_);
		}

		void clear()
		{
			empty_ = true;
		}

		bool empty() const
		{
			return empty_;
		}

	private:
		template <typename That>
		bool compare(const Optional<That>& that) const
		{
			if (empty() != that.empty())
			{
				return false;
			}
			
			if (empty())
			{
				return true;
			}

			return data() == that.data();
		}

		BOOST_ATTRIBUTE_NO_UNIQUE_ADDRESS
		Type data_;
		bool empty_;
	};

}

namespace std
{

	template <typename Type>
	struct hash<Pastel::Optional<Type>>
	{
	public:
		Pastel::hash_integer operator()(
			const Pastel::Optional<Type>& that) const
		{
			if (that.empty())
			{
				return Pastel::computeHash(that.empty());
			}
			
			return Pastel::computeHash(that.data());
		}
	};

}

#endif
