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
		class End_Node;

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
		PASTEL_FWD(EndData);

		using Data_Class = As_Class<Data>;
		using EndData_Class = As_Class<EndData>;

		class Node_Settings;
		using Node = List_::Node<Node_Settings>;
		using End_Node = List_::End_Node<Node_Settings>;
		using Data_Node = List_::Data_Node<Node_Settings>;

		class Node_Settings
		{
		public:
			using Data_Class = Data_Class;
			using EndData_Class = EndData_Class;
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

namespace Pastel
{
	
	template <
		typename Data_ = void, 
		typename EndData_ = void>
	class List_Set_Settings
	{
	public:
		using Data = Data_;
		using EndData = EndData_;
	};

	template <
		typename Data_ = void, 
		typename EndData_ = void>
	using List_Set_Fwd = List_Fwd<List_Set_Settings<Data_, EndData_>>;

}

#endif
