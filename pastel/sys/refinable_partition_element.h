#ifndef PASTEL_REFINABLE_PARTITION_ELEMENT_H
#define PASTEL_REFINABLE_PARTITION_ELEMENT_H

#include "pastel/sys/refinable_partition.h"
#include "pastel/sys/possiblyemptymember.h"

namespace Pastel
{

	template <typename ElementData, typename SetData>
	class RefinablePartition_Fwd<ElementData, SetData>::Element
		: public PossiblyEmptyMember<ElementData>
	{
	public:
		typedef PossiblyEmptyMember<ElementData> Data;

		Element(const Element& that)
			: set_(that.set_)
			, member_(that.member_)
			, type_(that.type_)
		{
			if (Data::data())
			{
				new(Data::data()) ElementData(that.data());
			}
		}

		Element(Element&& that)
			: set_()
			, member_()
			, type_(false)
		{
			if (Data::data())
			{
				new(Data::data()) ElementData;
			}

			swap(that);
		}

		~Element()
		{
			if (Data::data())
			{
				Data::data()->~ElementData();
			}
		}

		//! Returns the contained data.
		ElementData& data()
		{
			PENSURE(Data::data());
			return *Data::data();
		}

		//! Returns the contained data.
		const ElementData& data() const
		{
			PENSURE(Data::data());
			return *Data::data();
		}

		//! Returns the containing set.
		Set_ConstIterator set() const
		{
			return set_;
		}

		//! Returns the member in the member-set.
		Member_ConstIterator member() const
		{
			return member_;
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
		template <typename ElementData, typename SetData>
		friend class RefinablePartition;

		template <typename ElementData, typename SetData>
		friend class RefinablePartition<ElementData, SetData>::Set;

		// Deleted.
		Element();

		// Deleted.
		Element& operator=(Element that);

		Element(
			Set_Iterator set,
			Member_Iterator member,
			ElementData data)
			: set_(set)
			, member_(member)
			, type_(set->type())
		{
			if (Data::data())
			{
				new(Data::data()) ElementData(std::move(data));
			}
		}

		void swap(Element& that)
		{
			using std::swap;
			swap(set_, that.set_);
			swap(member_, that.member_);
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
		
		//! The member in the member-set.
		Member_Iterator member_;

		//! The type of the element.
		bool type_;
	};

}

#endif
