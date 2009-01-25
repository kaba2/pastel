#ifndef PASTELSYS_CONFIG_H
#define PASTELSYS_CONFIG_H

#include "pastel/sys/syslibrary.h"
#include "pastel/sys/mytypes.h"

#include <map>
#include <string>
#include <vector>
#include <boost/any.hpp>

namespace Pastel
{

	class PASTELSYS Config
	{
	private:
		typedef std::map<std::string, boost::any> Container;

	public:
		typedef Container::const_iterator ConstIterator;

		// Using default constructor.
		// Using default copy constructor.
		// Using default destructor.
		
		Config& operator=(const Config& that);

		Config& operator+=(const Config& that);

		void swap(Config& that);
		void clear();

		ConstIterator begin() const;
		ConstIterator end() const;

		bool created(const std::string& key) const;

		template <typename Type>
		bool ofType(const ConstIterator& iter) const;

		integer properties() const;

		template <typename Type>
		void assign(
			const std::string& key,
			const PASTEL_NO_DEDUCTION(Type)& value);

		template <typename Type>
		void insert(
			const std::string& key,
			const PASTEL_NO_DEDUCTION(Type)& value);

		void erase(const std::string& key);

		template <typename Type>
		const Type& property(const std::string& key) const;
		
		template <typename Type>
		const Type& property(const ConstIterator& iter) const;

		template <typename Type>
		const std::vector<Type>& propertyList(const std::string& key) const;
	
		template <typename Type>
		const std::vector<Type>& propertyList(const ConstIterator& iter) const;

	private:
		template <typename Type>
		std::vector<Type>& writePropertyList(const std::string& key);

		template <typename Type>
		std::vector<Type>& writePropertyList(const ConstIterator& iter);

		Container data_;
	};

}

#include "pastel/sys/config.hpp"

#endif
