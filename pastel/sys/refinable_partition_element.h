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

		//! Move-constructs from another element.
		/*!
		Time complexity: constant
		Exception safety: strong

		FIX: This function is needed solely because Visual Studio 2010
		does not support the emplace function properly. Remove this 
		function when support for emplace becomes available.
		*/
		Element(Element&& that)
			: set_(that.set_)
			, member_(that.member_)
			, type_(that.type_)
		{
			if (Data::data())
			{
				new(Data::data()) ElementData(std::move(that.data()));
			}
		}

		//! Destructs an element.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		~Element()
		{
			if (Data::data())
			{
				Data::data()->~ElementData();
			}
		}

		//! Returns the contained data.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		ElementData& data()
		{
			PENSURE(Data::data());
			return *Data::data();
		}

		//! Returns the contained data.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		const ElementData& data() const
		{
			PENSURE(Data::data());
			return *Data::data();
		}

		//! Returns the containing set.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		Set_ConstIterator set() const
		{
			return set_;
		}

		//! Returns the member in the member-set.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		Member_ConstIterator member() const
		{
			return member_;
		}

		//! Returns whether the element is marked.
		/*!
		Time complexity: constant
		Exception safety: nothrow
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
		Element(const Element& that);

		// Deleted.
		Element& operator=(Element that);

		//! Constructs the element.
		/*!
		Time complexity: constant
		Exception safety: strong
		*/
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

		//! Marks the element.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
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
		
		//! The corresponding member in the member-set.
		Member_Iterator member_;

		//! The type of the element.
		/*!
		The element is marked if and only if
		the type of the element is different
		than the type of the containing set.
		*/
		bool type_;
	};

}

#endif
