#ifndef PASTEL_REFINABLE_PARTITION_ELEMENT_H
#define PASTEL_REFINABLE_PARTITION_ELEMENT_H

#include "pastel/sys/refinable_partition.h"
#include "pastel/sys/possiblyemptymember.h"

namespace Pastel
{

	template <typename Type>
	class RefinablePartition_Fwd<Type>::Element
		: public PossiblyEmptyMember<Type>
	{
	public:
		typedef PossiblyEmptyMember<Type> Data;

		Element(const Element& that)
			: set_(that.set_)
			, position_(that.position_)
			, type_(that.type_)
		{
			if (Data::data())
			{
				new(Data::data()) Type(that.data());
			}
		}

		Element(Element&& that)
			: set_()
			, position_()
			, type_(false)
		{
			if (Data::data())
			{
				new(Data::data()) Type;
			}

			swap(that);
		}

		~Element()
		{
			if (Data::data())
			{
				Data::data()->~Type();
			}
		}

		Element& operator=(Element that)
		{
			swap(that);
			return *this;
		}

		//! Returns the contained data.
		Type& data()
		{
			PENSURE(Data::data());
			return *Data::data();
		}

		//! Returns the contained data.
		const Type& data() const
		{
			PENSURE(Data::data());
			return *Data::data();
		}

		//! Returns the containing set.
		Set_ConstIterator set() const
		{
			return set_;
		}

		//! Returns the position in the position-set.
		Position_ConstIterator position() const
		{
			return position_;
		}

		//! Returns whether the element is marked.
		/*!
		The element is marked if and only if
		the type of the element is different
		than the type of the containing set.
		*/
		bool marked() const
		{
			return type_ != set_->type();
		}

	private:
		template <typename Type>
		friend class RefinablePartition;

		template <typename Type>
		friend class RefinablePartition<Type>::Set;

		Element()
			: set_()
			, position_()
			, type_(false)
		{
			if (Data::data())
			{
				new(Data::data()) Type;
			}
		}

		Element(
			Set_Iterator set,
			Position_Iterator position,
			Type data)
			: set_(set)
			, position_(position)
			, type_(set->type())
		{
			if (Data::data())
			{
				new(Data::data()) Type(std::move(data));
			}
		}

		void swap(Element& that)
		{
			using std::swap;
			swap(set_, that.set_);
			swap(position_, that.position_);
			swap(type_, that.type_);
			if (Data::data())
			{
				swap(data(), that.data());
			}
		}

		void mark(bool markIt)
		{
			// If the element is to be marked,
			// then give it the type which is
			// the negation of the set type.
			// Otherwise give the type set
			// type.

			type_ = markIt ^ set_->type();

			ASSERT(marked() == markIt);
		}

		//! The set which contains this element.
		Set_Iterator set_;
		
		//! The position in the position-set.
		Position_Iterator position_;

		//! The type of the element.
		bool type_;
	};

}

#endif
