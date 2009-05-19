#ifndef PASTEL_SMALLFIXEDSET_H
#define PASTEL_SMALLFIXEDSET_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	template <typename Type>
	class SmallFixedSet
	{
	public:
		typedef Type* iterator;
		typedef const Type* const_iterator;

		explicit SmallFixedSet(integer size);
		SmallFixedSet(const SmallFixedSet& that);
		~SmallFixedSet();

		void swap(SmallFixedSet& that);

		SmallFixedSet<Type>& operator=(
			const SmallFixedSet& that);

		iterator begin();
		const_iterator begin() const;

		iterator end();
		const_iterator end() const;

		integer size() const;
		integer capacity() const;
		bool empty() const;
		bool full() const;

		Type& front();
		const Type& front() const;
		Type& back();
		const Type& back() const;

		iterator insert(const Type& that);

		void clear();

		Type& operator[](integer index);
		const Type& operator[](integer index) const;

	private:
		Type* data_;
		integer capacity_;
		integer size_;
	};

}

#include "pastel/sys/smallfixedset.hpp"

#endif
