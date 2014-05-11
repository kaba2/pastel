#ifndef PASTELSYS_REFINABLE_PARTITION_ELEMENT_H
#define PASTELSYS_REFINABLE_PARTITION_ELEMENT_H

#include "pastel/sys/refinable_partition.h"
#include "pastel/sys/class.h"

// Visual Studio generates "multiple assignment operators" warning,
// because it does not implement the deletion of functions 
// (which we simulate below).
#if (defined _WIN32 || defined _WIN64)
#	pragma warning(push)
#	pragma warning(disable: 4522)
#endif

namespace Pastel
{

	template <typename ElementData, typename SetData>
	class RefinablePartition_Fwd<ElementData, SetData>::Element
		: public ElementData_Class
	{
	public:
		template <typename, typename>
		friend class RefinablePartition;

		template <typename ElementData_, typename SetData_>
		friend class RefinablePartition_Fwd<ElementData_, SetData_>::Set;

		using ElementData_Class::operator=;

		//! Move-constructs from another element.
		/*!
		Time complexity: constant
		Exception safety: strong

		FIX: Delete after emplace becomes available in Visual Studio.
		*/
		Element(Element&& that)
			: ElementData_Class(std::move((ElementData_Class&&)that))
			, set_(std::move(that.set_))
			, member_(std::move(that.member_))
			, type_(std::move(that.type_))
		{
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
		Element() = delete;
		Element(const Element& that) = delete;
		Element& operator=(Element that) = delete;

		//! Constructs the element.
		/*!
		Time complexity: constant
		Exception safety: strong
		*/
		Element(
			Set_Iterator set,
			Member_Iterator member,
			ElementData_Class data)
			: ElementData_Class(std::move(data))
			, set_(set)
			, member_(member)
			, type_(set->type())
		{
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

#if (defined _WIN32 || defined _WIN64)
#	pragma warning(pop)
#endif

#endif
