#ifndef PASTEL_PROTOTYPEFACTORY_H
#define PASTEL_PROTOTYPEFACTORY_H

#include "pastel/sys/unorderedmap.h"
#include "pastel/sys/countedptr.h"

#include <string>

namespace Pastel
{

	/*!
	Requirements:
	- 'Type' default-constructible.
	- 'Type' has been derived from ReferenceCounted.
	- 'Type' has a 'CountedPtr<Type> clone() const' member function.
	*/

	template <typename Type>
	class PrototypeFactory
	{
	private:
		typedef UnorderedMap<std::string, CountedPtr<Type> > 
			PrototypeMap;
		typedef typename PrototypeMap::iterator Iterator;

	public:
		// Using default constructor.
		// Using default copy constructor.
		// Using default destructor.

		typedef typename PrototypeMap::const_iterator ConstIterator;

		PrototypeFactory<Type>& operator=(const PrototypeFactory& that);

		void swap(PrototypeFactory& that);
		void clear();

		CountedPtr<Type> create(
			const ConstIterator& iter) const;

		CountedPtr<Type> create(
			const std::string& name) const;

		void insert(
			const std::string& name,
			const CountedPtr<Type>& prototype);

		void erase(
			const std::string& name);

		integer size() const;
		
		ConstIterator begin() const;
		ConstIterator end() const;

	private:
		 PrototypeMap prototypeMap_;
	};

}

#include "pastel/sys/prototypefactory.hpp"

#endif
