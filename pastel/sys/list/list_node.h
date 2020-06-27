// Description: List node

#ifndef PASTELSYS_LIST_NODE_H
#define PASTELSYS_LIST_NODE_H

#include "pastel/sys/list/list_fwd.h"

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
			friend class Pastel::List;

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

		template <typename Base, typename EndData>
		class End_Node : public Base
		{
		public:
			End_Node() = default;

			explicit End_Node(const EndData& data)
				: Base()
				, data_(data)
			{
			}

			EndData& data() {
				return data_;
			}

			const EndData& data() const {
				return data_;
			}

		private:
			End_Node(const End_Node&) = delete;
			End_Node& operator=(End_Node) = delete;

			BOOST_ATTRIBUTE_NO_UNIQUE_ADDRESS
			EndData data_;
		};

		template <typename Data>
		class Data_Node	: public Node
		{
		public:
			template <typename... Type>
			explicit Data_Node(Type&&... data)
				: Node()
				, data_(std::forward<Type>(data)...)
			{
			}

			Data& data() {
				return data_;
			}

			const Data& data() const {
				return data_;
			}

		private:
			Data_Node(const Data_Node&) = delete;
			Data_Node& operator=(Data_Node) = delete;

			BOOST_ATTRIBUTE_NO_UNIQUE_ADDRESS
			Data data_;
		};

	}

}

#endif
