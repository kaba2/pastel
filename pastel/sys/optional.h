#ifndef PASTEL_OPTIONAL_H
#define PASTEL_OPTIONAL_H

#include "pastel/sys/object_forwarding.h"
#include "pastel/sys/hash.h"

#include <utility>

namespace Pastel
{

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

		Optional(const Optional& that)
			: Type_Class(that)
			, empty_(that.empty_)
		{
		}

		Optional(Optional&& that)
			: Type_Class(std::move((Type_Class&)that))
			, empty_(that.empty_)
		{
		}

		template <typename That>
		Optional(That&& that)
			: Type_Class(std::forward<That>(that))
			, empty_(false)
		{
		}

		Optional& operator=(Optional that)
		{
			Type_Class::operator=(that);
			empty_ = that.empty_;
			return *this;
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
