#ifndef PASTEL_OPTIONAL_H
#define PASTEL_OPTIONAL_H

#include "pastel/sys/object_forwarding.h"
#include "pastel/sys/hash.h"

#include <utility>

namespace Pastel
{

	class Epsilon {};

	template <typename Type>
	class Optional
		: public AsClass<Type>::type
	{
	public:
		typedef typename AsClass<Type>::type
			Type_Class;

		Optional()
			: Type_Class()
			, empty_(true)
		{
		}

		Optional(Epsilon)
			: Type_Class()
			, empty_(true)
		{
		}

		Optional(const Optional& that)
			: Type_Class(that.data())
			, empty_(that.empty_)
		{
		}

		template <typename That>
		Optional(const Optional<That>& that)
			: Type_Class(that.data())
			, empty_(that.empty_)
		{
			// Note that this function never 
			// matches the copy-constructor.
		}

		Optional(Optional&& that)
			: Type_Class(std::move(that.data()))
			, empty_(std::move(that.empty_))
		{
		}

		template <typename That>
		Optional(Optional<That>&& that)
			: Type_Class(std::move(that.data()))
			, empty_(std::move(that.empty_))
		{
			// Note that this function never 
			// matches the move-constructor.
		}

		Optional(Type that)
			: Type_Class(std::move(that))
			, empty_(false)
		{
		}

		Optional& operator=(Optional that)
		{
			Type_Class::operator=(std::move(that.data()));
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
			return !compare.that();
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

			swap((Type_Class&)*this, (Type_Class&)that);
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

		Type_Class& data()
		{
			return (Type_Class&)*this;
		}

		const Type_Class& data() const
		{
			return (const Type_Class&)*this;
		}

		bool empty_;
	};

}

namespace std
{

	template <typename Type>
	class hash<Pastel::Optional<Type>>
	{
	public:
		Pastel::hash_integer operator()(
			const Pastel::Optional<Type>& that) const
		{
			if (that.empty())
			{
				return Pastel::computeHash(that.empty());
			}
			
			return Pastel::computeHash(
				(const typename Pastel::AsClass<Type>::type&)that);
		}
	};

}


#endif
