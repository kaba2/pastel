// Description: Types for the list

#ifndef PASTELSYS_LIST_FWD_H
#define PASTELSYS_LIST_FWD_H

#include "pastel/sys/class.h"
#include "pastel/sys/range.h"

namespace Pastel
{

	namespace List_
	{
		
		template <typename>
		class Iterator;

		template <typename>
		class Node;

		template <typename>
		class End_Node;

		template <typename>
		class Data_Node;

		template <
			typename Data_Class_,
			typename EndData_Class_>
		class Node_Settings
		{
		public:
			using Data_Class = Data_Class_;
			using EndData_Class = EndData_Class_;
		};

		template <
			typename Node_Settings_,
			typename NodePtr_>
		class Iterator_Settings
		{
		public:
			using Node_Settings = Node_Settings_;
			using NodePtr = NodePtr_;
		};

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

		using Node_Settings = List_::Node_Settings<Data_Class, EndData_Class>;
		using Node = List_::Node<Node_Settings>;
		using End_Node = List_::End_Node<Node_Settings>;
		using Data_Node = List_::Data_Node<Node_Settings>;

		using Iterator_Settings = List_::Iterator_Settings<
			Node_Settings, Node*>;
		using ConstIterator_Settings = List_::Iterator_Settings<
			Node_Settings, const Node*>;

		using Iterator = List_::Iterator<Iterator_Settings>;
		using ConstIterator = List_::Iterator<ConstIterator_Settings>;
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
