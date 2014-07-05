// Description: An associative container for arbitrary data.
// Documentation: undocumented.txt

#ifndef PASTELSYS_ANYSTORAGE_H
#define PASTELSYS_ANYSTORAGE_H

#include <boost/any.hpp>

#include <string>
#include <unordered_map>

namespace Pastel
{

	class AnyStorage
	{
	private:
		typedef std::unordered_map<std::string, boost::any>
			DataSet;
		using ConstIterator = DataSet::const_iterator;

	public:
		template <typename Type>
		void set(const std::string& name, const Type& data)
		{
			dataSet_.insert(std::make_pair(name, boost::any(data)));
		}

		template <typename Type>
		Type get(const std::string& name, const Type& defaultReturn = Type())
		{
			ConstIterator iter = dataSet_.find(name);
			if (iter == dataSet_.end())
			{
				return defaultReturn;
			}
			try
			{
				return boost::any_cast<Type>(iter->second);
			}
			catch(boost::bad_any_cast&)
			{
				// Absorb.
			}

			return defaultReturn;
		}
		
	private:
		std::unordered_map<std::string, boost::any> dataSet_;
	};

}

#endif
