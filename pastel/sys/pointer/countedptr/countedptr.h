// Description: CountedPtr class
// Detail: A reference counted smart-pointer

#ifndef PASTELSYS_COUNTEDPTR_H
#define PASTELSYS_COUNTEDPTR_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/pointer/countedptr/referencecounted.h"
#include "pastel/sys/pointer/weakptr.h"

#include <compare>

namespace Pastel
{

	//! A reference counting smart-pointer
	/*!
	Any class wishing to use CountedPtr for reference counting
	must derive from ReferenceCounted. CountedPtr can't be
	used to reference count non-class types. See boost::shared_ptr
	for this capability. CountedPtr can't handle cyclic
	references: if A references B and B references A, then
	A and B will never be destructed. This is a common problem
	in reference counting which you must take care of.
	*/
	template <typename Type>
	class CountedPtr
	{
	public:
		template <typename ThatType>
		friend class CountedPtr;

		//! Constructs an empty pointer.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		CountedPtr();

		//! Constructs a copy of the given pointer.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		CountedPtr(const CountedPtr<Type>& that);

		//! Constructs a copy of the given pointer.
		/*!
		Time complexity: constant
		Exception safety: nothrow

		Note implicit conversion is allowed.
		*/
		template <typename ThatType>
		CountedPtr(const CountedPtr<ThatType>& that);

		template <typename ThatType>
		CountedPtr(const WeakPtr<ThatType>& that);

		//! Constructs a copy of the given pointer.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		explicit CountedPtr(Type* that);

		//! Destructs the pointer.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		~CountedPtr();

		//! Copies the given pointer.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		CountedPtr<Type>& operator=(
			const CountedPtr<Type>& that);

		//! Copies the given pointer.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		template <typename ThatType>
		CountedPtr<Type>& operator=(
			const CountedPtr<ThatType>& that);

		//! Copies the given pointer.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		CountedPtr<Type>& operator=(Type* that);

		//! Compares two pointers.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		template <typename ThatType>
		bool operator<=>(const CountedPtr<ThatType>& that) const {
			return data_ <=> that.data_;
		}

		//! Returns the contained pointer.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		Type* get() const;

		//! Dereferences the object pointed to.
		/*!
		Preconditions:
		!empty()

		Time complexity: constant
		Exception safety: nothrow
		*/
		Type* operator->() const;

		//! Dereferences the object pointed to.
		/*!
		Preconditions:
		!empty()

		Time complexity: constant
		Exception safety: nothrow
		*/
		Type& operator*() const;

		//! Swaps two pointers.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		void swap(CountedPtr<Type>& that);

		//! Sets the current pointer to null pointer.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		void clear();

		//! Returns the number of active CountedPtr's to the current pointee.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		integer count() const;

		//! Returns true if this object points to a null pointer.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		bool empty() const;

	private:
		void increaseCount() const;
		void decreaseCount() const;

		Type* data_;
	};

	template <typename Type>
	void swap(CountedPtr<Type>& left,
		CountedPtr<Type>& right);

}

namespace std
{

	template <typename Type>
	struct hash<Pastel::CountedPtr<Type>>
	{
	public:
		Pastel::hash_integer operator()(
			const Pastel::CountedPtr<Type>& that) const
		{
			return Pastel::computeHash(that.get());
		}
	};

}

#include "pastel/sys/pointer/countedptr/countedptr.hpp"

#endif
