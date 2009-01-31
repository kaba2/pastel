#ifndef PASTELSYS_PROTOTYPEFACTORY_HPP
#define PASTELSYS_PROTOTYPEFACTORY_HPP

#include "pastel/sys/prototypefactory.h"

namespace Tune
{

	template <typename Type>
	PrototypeFactory<Type>& PrototypeFactor<Type>::operator=(
		const PrototypeFactory& that)
	{
		PrototypeFactory copy(that);
		swap(copy);
		return *this;
	}

	template <typename Type>
	void PrototypeFactor<Type>::swap(PrototypeFactory& that)
	{
		prototypeMap_.swap(that.prototypeMap_);
	}

	template <typename Type>
	void PrototypeFactor<Type>::clear()
	{
		prototypeMap_.clear();
	}

	template <typename Type>
	CountedPtr<Type> PrototypeFactor<Type>::create(
		const ConstIterator& iter) const
	{
		if (iter == prototypeMap_.end())
		{
			return CountedPtr<Type>();
		}

		return (*iter)->clone();
	}

	template <typename Type>
	CountedPtr<Type> PrototypeFactor<Type>::create(
		const std::string& name) const
	{
		return create(prototypeMap_.find(name));
	}

	template <typename Type>
	void PrototypeFactor<Type>::insert(
		const std::string& name,
		const CountedPtr<Type>& prototype)
	{
		prototypeMap_.insert(std::make_pair(name, prototype));
	}

	template <typename Type>
	void PrototypeFactor<Type>::erase(
		const std::string& name)
	{
		prototypeMap_.erase(name);
	}

	template <typename Type>
	integer PrototypeFactor<Type>::size() const
	{
		return prototypeMap_.size();
	}
	
	template <typename Type>
	typename PrototypeFactor<Type>::ConstIterator
		PrototypeFactor<Type>::begin() const
	{
		return prototypeMap_.begin();
	}

	template <typename Type>
	typename PrototypeFactor<Type>::ConstIterator 
		PrototypeFactor<Type>::end() const
	{
		return prototypeMap_.end();
	}

}

#endif
