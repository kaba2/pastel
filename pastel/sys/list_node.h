// Description: List node

#ifndef PASTELSYS_LIST_NODE_H
#define PASTELSYS_LIST_NODE_H

#include "pastel/sys/list_fwd.h"

namespace Pastel
{

	namespace List_
	{

		template <typename>
		class Iterator;

		class Node
		{
		public:
			Node()
				: next_()
			{
				isolateSelf();
			}

			template <typename>
			friend class Iterator;

			template <typename, template <typename> class>
			friend class List;

		private:
			Node(const Node&) = delete;
			Node& operator=(Node) = delete;

			bool isEnd() const
			{
				return next() == 0;
			}

			void isolateSelf()
			{
				next() = (Node*)this;
				prev() = (Node*)this;
			}

			Node*& next(bool right)
			{
				return next_[right];
			}

			Node* next(bool right) const
			{
				return next_[right];
			}

			Node*& next()
			{
				return next_[1];
			}

			const Node* next() const
			{
				return next_[1];
			}

			Node*& prev()
			{
				return next_[0];
			}

			const Node* prev() const
			{
				return next_[0];
			}

			Node* next_[2];
		};

		// Note: Writing EndData_Class instead of EndData_Class_
		// triggers a bug in Visual Studio 2013.
		template <typename Base, typename EndData_Class_>
		class End_Node
			: public Base
			, public EndData_Class_
		{
		public:
			End_Node()
				: Base()
				, EndData_Class_()
			{
			}

			explicit End_Node(const EndData_Class_& data)
				: Base()
				, EndData_Class_(data)
			{
			}

		private:
			End_Node(const End_Node&) = delete;
			End_Node& operator=(End_Node) = delete;
		};

		// Note: Writing Data_Class instead of Data_Class_
		// triggers a bug in Visual Studio 2013.
		template <typename Data_Class_>
		class Data_Node
			: public Node
			, public Data_Class_
		{
		public:
			template <typename... Type>
			explicit Data_Node(Type&&... data)
				: Node()
				, Data_Class_(std::forward<Type>(data)...)
			{
			}

		private:
			Data_Node(const Data_Node&) = delete;
			Data_Node& operator=(Data_Node) = delete;
		};

	}

}

#endif
