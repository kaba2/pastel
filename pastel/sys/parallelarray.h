// Description: ParallelArray class
// Detail: A parallel array

#ifndef PASTEL_PARALLELARRAY_H
#define PASTEL_PARALLELARRAY_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/memoryview.h"
#include "pastel/sys/unorderedmap.h"
#include "pastel/sys/propertyid.h"

#include <boost/operators.hpp>

#include <vector>
#include <memory>

namespace Pastel
{

	namespace Detail
	{

		struct UnknownArray
		{
		public:
			virtual ~UnknownArray();

			virtual bool resize(integer width) = 0;
			virtual std::auto_ptr<UnknownArray> clone() const = 0;
		};

	}

	//! A set of equal sized homogeneous typed arrays.

	template <typename Key>
	class ParallelArray;

	template <typename Key>
	class ParallelArray
	{
	private:
		typedef Detail::UnknownArray UnknownArray;
		typedef UnorderedMap<Key, UnknownArray*> Container;
		typedef typename Container::iterator Iterator;
		typedef typename Container::const_iterator ConstIterator;

	public:
		//! Constructs a zero-sized parallel array with no properties.
		/*!
		Time complexity: constant
		Exception safety: strong
		*/
		ParallelArray();

		//! Constructs a copy of the given parallel array.
		/*!
		Time complexity: O(properties() * size())
		Exception safety: strong
		*/
		ParallelArray(const ParallelArray<Key>& that);

		//! Destructs the parallel array.
		~ParallelArray();

		//! Copies the given parallel array.
		/*!
		Time complexity: O(properties() * size())
		Exception safety: strong
		*/
		ParallelArray<Key>& operator=(
			const ParallelArray<Key>& that);

		//! Calls clear() and removes all properties.
		/*!
		Time complexity: O(properties() * size())
		Exception safety: nothrow
		*/
		void freeMem();

		//! Swaps two ParallelArrays.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		void swap(ParallelArray<Key>& that);

		//! Calls resize(0).
		/*!
		Time complexity: O(properties() * size())
		Exception safety: nothrow
		*/
		void clear();

		//! Adds a property with the given key and type.

		/*!
		Time complexity: linear
		Exception safety: strong

		If the property with the given key and type
		is already added, returns false,
		otherwise returns true.
		*/
		template <typename Type>
		bool add(const PropertyId<Key, Type>& key,
			const Type& defaultValue = Type());

		//! Removes a property given by the key.

		/*!
		Time complexity: amortized constant
		Exception safety: nothrow

		If the property with the given key is
		not added, returns false,
		otherwise returns true.
		*/
		template <typename Type>
		bool remove(const PropertyId<Key, Type>& key);

		//! Retrieves the property given by the key.

		/*!
		Time complexity: amortized constant
		Exception safety: nothrow

		If the property with the given key and type
		is not found, returns false,
		otherwise returns true.
		*/
		template <typename Type>
		bool get(
			const PropertyId<Key, Type>& key,
			MemoryView<Type>& view);

		//! Retrieves the property given by the key.

		/*!
		FIX: This is a relic from older versions,
		consider removing.

		Time complexity: amortized constant
		Exception safety: nothrow

		If the property with the given key and type
		is not found, returns false,
		otherwise returns true.
		*/
		template <typename Type>
		bool get(
			const PropertyId<Key, Type>& key,
			ConstMemoryView<Type>& view);

		//! Retrieves the property given by the key.

		/*!
		Time complexity: amortized constant
		Exception safety: nothrow

		If the property with the given key and type
		is not found, returns false,
		otherwise returns true.
		*/
		template <typename Type>
		bool get(
			const PropertyId<Key, Type>& key,
			ConstMemoryView<Type>& view) const;

		//! Expands all of the property arrays if needed.
		/*!
		Preconditions:
		size >= 0

		Time complexity: linear
		Exception safety: basic

		If the reservation causes a reallocation even
		in one property array, returns true, otherwise
		returns false.

		If the requested size is smaller than the current
		size, does nothing. Otherwise resizes to the requested size.
		*/
		bool reserve(integer size);

		//! Resizes all of the property arrays.
		/*!
		Preconditions:
		size >= 0

		Time complexity: linear
		Exception safety: basic

		If the resizing causes a reallocation even
		in one property array, returns true, otherwise
		returns false.
		*/
		bool resize(integer size);

		//! Returns the number of properties.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		integer properties() const;

		//! Returns the size of the property arrays.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		integer size() const;

		//! Returns (size() == 0).
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		bool empty() const;

	private:
		template <typename Type>
		struct KnownArray
			: public UnknownArray
		{
		public:
			KnownArray();
			KnownArray(integer size,
				const Type& defaultValue);
			virtual ~KnownArray();

			virtual bool resize(integer width);
			virtual std::auto_ptr<UnknownArray> clone() const;

			std::vector<Type> data_;
			Type defaultValue_;

		private:
			KnownArray(const KnownArray<Type>& that);
			KnownArray<Type>& operator=(const KnownArray<Type>& that);
		};

		Container arrays_;
		integer size_;
	};

}

#include "pastel/sys/parallelarray.hpp"

#endif
