/*!
\file
\brief A class for a reference counting smart-pointer.
*/

#ifndef PASTEL_WEAKPTR_H
#define PASTEL_WEAKPTR_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/syslibrary.h"
#include "pastel/sys/functional.h"
#include "pastel/sys/referencecounted.h"

#include <boost/operators.hpp>

namespace Pastel
{

	//! A base class for WeakPtr reference counted objects.

	/*!
	Any class wishing to use WeakPtr for reference counting
	must derive from ReferenceCounted. This base class
	simply includes a counter that the WeakPtr implementation
	increases and decreases.
	*/

	//! A reference counting smart-pointer

	/*!
	Any class wishing to use WeakPtr for reference counting
	must derive from ReferenceCounted. WeakPtr can't be
	used to reference count non-class types. See boost::shared_ptr
	for this capability. WeakPtr can't handle cyclic
	references: if A references B and B references A, the
	A and B will never be destructed. This is a common problem
	in reference counting, which you must take care of.
	*/

	template <typename Type>
	class CountedPtr;

	template <typename Type>
	class WeakPtr
		: boost::less_than_comparable<
		WeakPtr<Type>
		, boost::equality_comparable<
		WeakPtr<Type>
		> >
	{
	public:
		template <typename ThatType>
		friend class WeakPtr;

		//! Constructs an empty pointer.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		WeakPtr();

		//! Constructs a copy of the given pointer.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		WeakPtr(const WeakPtr<Type>& that);

		//! Constructs a copy of the given pointer.
		/*!
		Time complexity: constant
		Exception safety: nothrow

		Note implicit conversion is allowed.
		*/
		template <typename ThatType>
		WeakPtr(const WeakPtr<ThatType>& that);

		template <typename ThatType>
		WeakPtr(const CountedPtr<ThatType>& that);

		//! Constructs a copy of the given pointer.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		explicit WeakPtr(Type* that);

		//! Destructs the pointer.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		~WeakPtr();

		//! Copies the given pointer.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		WeakPtr<Type>& operator=(
			const WeakPtr<Type>& that);

		//! Copies the given pointer.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		template <typename ThatType>
		WeakPtr<Type>& operator=(
			const WeakPtr<ThatType>& that);

		//! Copies the given pointer.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		WeakPtr<Type>& operator=(Type* that);

		//! Compares two pointers.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		template <typename ThatType>
		bool operator==(const WeakPtr<ThatType>& that) const;

		//! Compares two pointers.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		template <typename ThatType>
		bool operator<(const WeakPtr<ThatType>& that) const;

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
		void swap(WeakPtr<Type>& that);

		//! Sets the current pointer to null pointer.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		void clear();

		//! Returns the number of active WeakPtr's to the current pointee.
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
		Type* data_;
	};

	template <typename Type>
	void swap(WeakPtr<Type>& left,
		WeakPtr<Type>& right);

	// Hash function for unordered containers
	template <typename Type>
	struct hash<WeakPtr<Type> >
	{
		std::size_t operator()(const WeakPtr<Type>& that) const
		{
			return (std::size_t)(that.get());
		}
	};
}

#include "pastel/sys/weakptr.hpp"

#endif
