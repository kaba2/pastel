// Description: Bounded array

#ifndef PASTELSYS_BOUNDED_ARRAY_H
#define PASTELSYS_BOUNDED_ARRAY_H

#include "pastel/sys/bounded_array_concepts.h"
#include "pastel/sys/bounded_array_fwd.h"
#include "pastel/sys/bounded_array_iterator.h"
#include "pastel/sys/destruct.h"
#include "pastel/sys/ensure.h"

#include <type_traits>

namespace Pastel
{

	template <typename Settings>
	using Empty_BoundedArray_Customization = 
		BoundedArray_Concepts::Customization<Settings>;

	template <
		typename Settings, 
		template <typename> class Customization = Empty_BoundedArray_Customization>
	class BoundedArray
	: public Customization<Settings>
	{
	public:
		using Fwd = BoundedArray_Fwd<Settings>;
		PASTEL_FWD(Element);
		PASTEL_FWD(Iterator);
		PASTEL_FWD(ConstIterator);
		PASTEL_FWD(Range);
		PASTEL_FWD(ConstRange);
		PASTEL_FWD(iterator);
		PASTEL_FWD(const_iterator);
		static constexpr int N = Fwd::N;

		#ifdef _MSC_VER
		// The std::is_trivially_copyable is not implemented in gcc 4.

		// We must require the Element to be trivially copyable,
		// or else the swap() cannot be implemented with the nothrow
		// guarantee.
		PASTEL_STATIC_ASSERT(std::is_trivially_copyable<Element>::value);
		#endif

		using Base = Customization<Settings>;

		//! Constructs an empty array.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		BoundedArray()
		{
			this->onConstruct();
		}

		//! Copy-constructs from another array.
		/*!
		Time complexity: O(min(that.size(), maxSize()))
		Exception safety: strong

		Exactly min(that.size(), maxSize()) elements
		will be copied.
		*/
		BoundedArray(const BoundedArray& that)
		{
			integer n = std::min(maxSize(), that.size());
			for (integer i = 0;i < n;++i)
			{
				emplaceBack(that[i]);
			}

			this->onConstruct();
		}

		//! Constructs from another array.
		/*!
		Time complexity: O(min(that.size(), maxSize()))
		Exception safety: strong

		Exactly min(that.size(), maxSize()) elements
		will be copied.
		*/
		template <
			typename That_Settings, 
			template <typename> class That_Customization>
		BoundedArray(const BoundedArray<That_Settings, That_Customization>& that)
		{
			integer n = std::min(maxSize(), that.size());
			for (integer i = 0;i < n;++i)
			{
				emplaceBack(that[i]);
			}

			this->onConstruct();
		}

		//! Move-constructs from another array.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		BoundedArray(BoundedArray&& that)
		{
			swap(that);

			this->onConstruct();
		}

		//! Constructs from an initializer list.
		/*!
		Time complexity: O(min(that.size(), maxSize()))
		Exception safety: strong

		Exactly min(that.size(), maxSize()) elements
		will be copied.
		*/
		template <typename That_Element>
		BoundedArray(const std::initializer_list<That_Element>& thatSet)
		{
			for (auto&& value : thatSet)
			{
				emplaceBack(value);
			}
		}

		//! Assigns from another array.
		/*!
		Time complexity: 
		O(1), if moved,
		O(size()), if copied.

		Exception safety: nothrow
		*/
		BoundedArray& operator=(BoundedArray that)
		{
			swap(that);
			return *this;
		}

		//! Swaps two arrays.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void swap(BoundedArray& that)
		{
			using std::swap;

			Base::swap(that);

			// Assumes std::is_trivially_copyable<Element>.
			swap(elementSet_, that.elementSet_);
			swap(size_, that.size_);
		}

		//! Removes all elements.
		/*!
		Time complexity: 
		O(size()), if non-trivial destruction,
		O(1), otherwise.

		Exception safety: nothrow
		*/
		void clear()
		{
			this->onClear();

			destruct(address(0), address(size()));
			size_ = 0;
		}

		//! Inserts an element at the back of the array.
		/*!
		Preconditions:
		!full()

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		template <typename... Type>
		void emplaceBack(Type&&... that)
		{
			PENSURE(!full());

			new (address(size())) Element(std::forward<Type>(that)...);
			++size_;

			this->onEmplaceBack();
		}

		//! Removes the last element from the array.
		/*!
		Preconditions:
		!empty()

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void popBack()
		{
			PENSURE(!empty());

			this->onPopBack();

			destruct(address(size() - 1));
			--size_;
		}

		PASTEL_ITERATOR_FUNCTIONS(begin, address(0));
		PASTEL_ITERATOR_FUNCTIONS(end, address(size()));
		PASTEL_RANGE_FUNCTIONS(range, begin, end);

		//! Returns the first element.
		/*!
		Preconditions:
		!empty()

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Element& front()
		{
			return removeConst(addConst(*this).front());
		}

		//! Returns the first element.
		/*!
		Preconditions:
		!empty()

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		const Element& front() const
		{
			PENSURE(!empty());
			return (*this)[0];
		}

		//! Returns the last element.
		/*!
		Preconditions:
		!empty()

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Element& back()
		{
			return removeConst(addConst(*this).back());
		}

		//! Returns the last element.
		/*!
		Preconditions:
		!empty()

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		const Element& back() const
		{
			PENSURE(!empty());
			return (*this)[size() - 1];
		}

		//! Returns the i:th element.
		/*!
		Preconditions:
		0 <= i < size()

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Element& operator[](integer i)
		{
			return removeConst(addConst(*this)[i]);
		}

		//! Returns the i:th element.
		/*!
		Preconditions:
		0 <= i < size()

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		const Element& operator[](integer i) const
		{
			PENSURE_OP(i, >=, 0);
			PENSURE_OP(i, <, size());

			return *address(i);
		}

		//! Returns the number of elements in the array.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer size() const
		{
			return size_;
		}

		//! Returns the maximum number of possible elements.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer maxSize() const
		{
			return N;
		}

		//! Returns whether the array is full.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		bool full() const
		{
			return size() == N;
		}

		//! Returns whether the array is empty.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		bool empty() const
		{
			return size() == 0;
		}

	private:
		Element* address(integer i)
		{
			return ((Element*)elementSet_) + i;
		}

		const Element* address(integer i) const
		{
			return ((const Element*)elementSet_) + i;
		}

		char elementSet_[sizeof(Element) * N];
		integer size_ = 0;
	};

}

namespace Pastel
{

	template <
		typename Element_,
		int N_>
	class BoundedArray_Settings
	{
	public:
		using Element = Element_;
		static constexpr int N = N_;
	};

}

#endif
