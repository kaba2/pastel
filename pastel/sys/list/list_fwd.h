// Description: Types for the list

#ifndef PASTELSYS_LIST_FWD_H
#define PASTELSYS_LIST_FWD_H

#include "pastel/sys/range.h"

namespace Pastel
{

	namespace List_
	{
		
		template <typename>
		class Iterator;

		class Node;

		template <typename, typename>
		class End_Node;

		template <typename>
		class Data_Node;

		template <
			typename Data_,
			typename EndData_,
			typename EndBase_>
		class Node_Settings
		{
		public:
			using Data = Data_;
			using EndData = EndData_;
			using EndBase = EndBase_;
		};

		template <
			typename Node_Settings_,
			typename NodePtr_,
			bool UserDataInEndNode_>
		class Iterator_Settings
		{
		public:
			using Node_Settings = Node_Settings_;
			using NodePtr = NodePtr_;
			static constexpr bool UserDataInEndNode =
				UserDataInEndNode_;
		};

	}

	template <typename Settings>
	class Empty_List_Customization;

	template <
		typename, 
		template <typename> class = Empty_List_Customization>
	class List;

	template <typename Settings>
	class List_Fwd
	{
	public:
		using Fwd = Settings;
		PASTEL_FWD(Data);
		PASTEL_FWD(EndData);
		static constexpr bool UserDataInEndNode = 
			Settings::UserDataInEndNode;

		using Node = List_::Node;
		using Data_Node = List_::Data_Node<Data>;

		using EndBase = typename std::conditional<
			UserDataInEndNode,
			Data_Node, Node>::type;

		using End_Node = List_::End_Node<EndBase, EndData>;

		using Node_Settings =
			List_::Node_Settings<Data, EndData, Node>;

		using Iterator_Settings = List_::Iterator_Settings<
			Node_Settings, Node*, UserDataInEndNode>;
		using ConstIterator_Settings = List_::Iterator_Settings<
			Node_Settings, const Node*, UserDataInEndNode>;

		using Iterator = List_::Iterator<Iterator_Settings>;
		using ConstIterator = List_::Iterator<ConstIterator_Settings>;
		using Range = ranges::subrange<Iterator>;
		using ConstRange = ranges::subrange<ConstIterator>;

		// Compability with boost iterator ranges.

		using iterator = Iterator;
		using const_iterator = ConstIterator;
	};

}

namespace Pastel
{

	template <typename Settings>
	class Empty_List_Customization
	{
	protected:
		using Fwd = List_Fwd<Settings>;

		PASTEL_FWD(Iterator);
		PASTEL_FWD(ConstIterator);

		Empty_List_Customization() {}

		void onConstruction() {};
		void swap(Empty_List_Customization& that) {}

		void onClear() {}
		void onInsert(const Iterator& element) {}
		void onErase(const Iterator& element) {}
		void onSpliceFrom(const Iterator& element) {}
		void onSplice(const Iterator& element) {}

	private:
		Empty_List_Customization(const Empty_List_Customization& that) = delete;
		Empty_List_Customization(Empty_List_Customization&& that) = delete;
		Empty_List_Customization& operator=(Empty_List_Customization) = delete;
	};

}

namespace Pastel
{
	
	template <
		typename Data_ = Empty, 
		typename EndData_ = Empty,
		bool UserDataInEndNode_ = false>
	class List_Set_Settings
	{
	public:
		using Data = Data_;
		using EndData = EndData_;
		static constexpr bool UserDataInEndNode = UserDataInEndNode_;
	};

	template <
		typename Data_ = Empty, 
		typename EndData_ = Empty,
		bool UserDataInEndNode_ = false>
	using List_Set_Fwd = List_Fwd<
		List_Set_Settings<Data_, EndData_, UserDataInEndNode_>>;

}

#endif
