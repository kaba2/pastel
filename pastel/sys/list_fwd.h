// Description: Types for the list

#ifndef PASTELSYS_LIST_FWD_H
#define PASTELSYS_LIST_FWD_H

#include "pastel/sys/class.h"
#include "pastel/sys/range.h"

namespace Pastel
{

	namespace List_
	{
		
		template <typename, typename>
		class Iterator;

		template <typename>
		class Node;

		template <typename>
		class Sentinel_Node;

		template <typename>
		class Data_Node;

	}

	template <typename, template <typename> class>
	class List;

	template <typename Settings>
	class List_Fwd
	{
	public:
		using Fwd = Settings;
		PASTEL_FWD(Data);
		PASTEL_FWD(SentinelData);

		using Data_Class = As_Class<Data>;
		using SentinelData_Class = As_Class<SentinelData>;

		class Node_Settings;
		using Node = List_::Node<Node_Settings>;
		using Sentinel_Node = List_::Sentinel_Node<Node_Settings>;
		using Data_Node = List_::Data_Node<Node_Settings>;

		class Node_Settings
		{
		public:
			using Data_Class = Data_Class;
			using SentinelData_Class = SentinelData_Class;
		};

		using Iterator = List_::Iterator<Node*, Node_Settings>;
		using ConstIterator = List_::Iterator<const Node*, Node_Settings>;
		using Range = boost::iterator_range<Iterator>;
		using ConstRange = boost::iterator_range<ConstIterator>;

		// Compability with boost iterator ranges.

		using iterator = Iterator;
		using const_iterator = ConstIterator;
	};
	
}

#endif
