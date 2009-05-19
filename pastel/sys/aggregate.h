#ifndef PASTEL_AGGREGATE_H
#define PASTEL_AGGREGATE_H

#include <map>
#include <memory>

namespace Pastel
{

	template <typename Key, typename Type>
	class Aggregate
	{
	private:
		typedef std::map<Key, Type*> Container;
		typedef typename Container::iterator Iterator;
	public:
		typedef typename Container::const_iterator ConstIterator;

		Aggregate();
		~Aggregate();

		void clear();
		void swap(Aggregate<Key, Type>& that);

		ConstIterator begin() const;
		ConstIterator end() const;

		void add(const Key& key, std::auto_ptr<Type>& that);
		std::auto_ptr<Type> remove(const Key& key);

		Type* get(const Key& key) const;

	private:
		// Undefined
		Aggregate(const Aggregate&);
		// Undefined
		Aggregate& operator=(const Aggregate&);

		Container data_;
	};

}

#include "pastel/sys/aggregate.hpp"

#endif
