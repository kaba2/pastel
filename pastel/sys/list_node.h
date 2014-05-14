// Description: List node

#ifndef PASTELSYS_LIST_NODE_H
#define PASTELSYS_LIST_NODE_H

#include "pastel/sys/list_fwd.h"

namespace Pastel
{

	namespace List_
	{

		template <typename, typename>
		class Iterator;

		template <typename Node_Settings>
		class Node
		{
		public:
			Node()
				: next_()
			{
				isolateSelf();
			}

		private:
			Node(const Node&) = delete;
			Node& operator=(Node) = delete;

			template <typename, typename>
			friend class Iterator;

			template <typename, template <typename> class>
			friend class List;

			bool isSentinel() const
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

		template <typename Node_Settings>
		class Sentinel_Node
			: public Node<Node_Settings>
			, public Node_Settings::SentinelData_Class
		{
		public:
			using Base = Node<Node_Settings>;

			using Fwd = Node_Settings;
			PASTEL_FWD(SentinelData_Class);

			Sentinel_Node()
				: Base()
				, SentinelData_Class()
			{
			}

			explicit Sentinel_Node(const SentinelData_Class& data)
				: Base()
				, SentinelData_Class(data)
			{
			}

		private:
			Sentinel_Node(const Sentinel_Node&) = delete;
			Sentinel_Node& operator=(Sentinel_Node) = delete;
		};

		template <typename Node_Settings>
		class Data_Node
			: public Node<Node_Settings>
			, public Node_Settings::Data_Class
		{
		public:
			using Base = Node<Node_Settings>;

			using Fwd = Node_Settings;
			PASTEL_FWD(Data_Class);

			template <typename... Type>
			explicit Data_Node(Type&&... data)
				: Base()
				, Data_Class(std::forward<Type>(data)...)
			{
			}

		private:
			Data_Node(const Data_Node&) = delete;
			Data_Node& operator=(Data_Node) = delete;
		};

	}

}

#endif
