#ifndef PASTELGEOMETRY_BOXCOLLIDER_H
#define PASTELGEOMETRY_BOXCOLLIDER_H

#include "pastel/sys/primes.h"
#include "pastel/sys/smallset.h"
#include "pastel/sys/unorderedset.h"
#include "pastel/sys/unorderedmap.h"
#include "pastel/sys/tuple.h"

namespace Pastel
{

	template <int N, typename Real, typename Object>
	class BoxCollider
	{
	private:
		class Vertex
		{
		public:
			Vertex()
				: position_(0)
				, start_(true)
				, index_(0)
				, stabbingNumber_(0)
			{
			}

			Vertex(const Real& position, bool start, integer index, integer stabbingNumber)
				: position_(position)
				, start_(start)
				, index_(index)
				, stabbingNumber_(stabbingNumber)
			{
			}

			void setStabbingNumber(integer stabbingNumber)
			{
				stabbingNumber_ = stabbingNumber;
			}

			integer stabbingNumber() const
			{
				return stabbingNumber_;
			}

			integer indexIndex() const
			{
				return index_;
			}

			integer object() const
			{
				return index_ / (2 * N);
			}

			const Real& position() const
			{
				return position_;
			}

			bool start() const
			{
				return start_;
			}

			bool operator<(const Vertex& right) const
			{
				if (position_ < right.position_)
				{
					return true;
				}
				if (position_ > right.position_)
				{
					return false;
				}
				if (start_ && !right.start_)
				{
					return true;
				}
				if (!start_ && right.start_)
				{
					return false;
				}

				return index_ < right.index_;
			}

		private:
			Real position_;
			bool start_;
			integer index_;
			integer stabbingNumber_;
		};

		class CollisionHash
		{
		public:
			CollisionHash()
				: prime_(followingPrime(50000))
			{
			}
			integer operator()(const std::pair<integer, integer>& that) const
			{
				return that.first * prime_ + that.second;
			}

			integer prime_;
		};

		typedef UnorderedMap<std::pair<integer, integer>, integer, CollisionHash>
			CounterContainer;
		typedef typename CounterContainer::iterator CounterIterator;
		typedef typename CounterContainer::const_iterator ConstCounterIterator;

		typedef UnorderedSet<std::pair<integer, integer>, CollisionHash>
			CollisionContainer;
		typedef typename CollisionContainer::iterator CollisionIterator;

	public:
		typedef typename CollisionContainer::const_iterator ConstCollisionIterator;

		void clear()
		{
			for (integer i = 0;i < N;++i)
			{
				vertexList_[i].clear();
			}

			objectList_.clear();
			indexList_.clear();
			counterSet_.clear();
			collisionSet_.clear();
		}

		void swap(BoxCollider& that)
		{
			vertexList_.swap(that.vertexList_);
			objectList_.swap(that.objectList_);
			indexList_.swap(that.indexList_);
			counterSet_.swap(that.counterSet_);
			collisionSet_.swap(that.collisionSet_);
		}

		ConstCollisionIterator collisionBegin() const
		{
			return collisionSet_.begin();
		}

		ConstCollisionIterator collisionEnd() const
		{
			return collisionSet_.end();
		}

		template <typename BoundCallback>
		void updateAll(BoundCallback bound)
		{
			const integer objects = objectList_.size();
			for (integer i = 0;i < objects;++i)
			{
				update(i, bound);
			}
		}

		template <typename BoundCallback>
		integer insert(const Object& object, BoundCallback bound)
		{
			const integer objectIndex = objectList_.size();
			objectList_.push_back(object);

			for (integer i = 0;i < N;++i)
			{
				VertexContainer& vertexList = vertexList_[i];

				const integer vertexIndex = 2 * N * objectIndex + 2 * i;

				const integer startIndex =
					vertexList.insert(
					Vertex(bound(i, true, object), true, vertexIndex, 0));

				ASSERT2(indexList_.size() == vertexIndex, indexList_.size(), vertexIndex);
				indexList_.push_back(startIndex);

				// Note: because the end-point will come after
				// the start-point, startIndex will remain correct
				// after this insertion.
				const integer endIndex = vertexList.insert(
					Vertex(bound(i, false, object), false, vertexIndex + 1, 0));

				ASSERT2(indexList_.size() == vertexIndex + 1, indexList_.size(), vertexIndex + 1);
				indexList_.push_back(endIndex);

				// Update the indices.

				updateIndices(i, startIndex, vertexList.size());

				// Update the stabbing numbers.

				updateStabbingNumbers(i, startIndex, endIndex + 1);

				// Update the collision and counter sets.

				for (integer k = startIndex + 1;k < endIndex;++k)
				{
					Vertex& vertex = vertexList[k];
					if (vertex.start())
					{
						increaseOverlap(vertex.object(), objectIndex);
					}
				}

				Vertex& startVertex = vertexList[startIndex];
				Vertex& endVertex = vertexList[endIndex];
				integer stabbingNumber = startVertex.stabbingNumber();

				integer k = startIndex - 1;
				while(stabbingNumber > 0)
				{
					ASSERT1(k >= 0, k);
					Vertex& vertex = vertexList[k];
					if (vertex.start())
					{
						Vertex& otherVertex = vertexList[indexList_[vertex.indexIndex() + 1]];
						ASSERT(otherVertex.object() == vertex.object());
						if (startVertex < otherVertex)
						{
							increaseOverlap(vertex.object(), objectIndex);
							--stabbingNumber;
						}
					}

					--k;
				}
			}

			//check();

			return objectIndex;
		}

		Object object(integer objectIndex) const
		{
			return objectList_[objectIndex];
		}

	private:
		void updateVertex(integer i, integer vertexIndex, const Real& newPosition)
		{
			VertexContainer& vertexList = vertexList_[i];
			Vertex& vertex = vertexList[vertexIndex];

			if (newPosition != vertex.position())
			{
				const integer newVertexIndex = vertexList.set(vertexIndex,
					Vertex(newPosition, vertex.start(), vertex.indexIndex(), vertex.stabbingNumber()));
				ASSERT(newVertexIndex != -1);

				if (newVertexIndex != vertexIndex)
				{
					const integer jorma = newVertexIndex;
				}

				fixRange(i, vertexIndex, newVertexIndex);
			}
		}

		template <typename BoundCallback>
		void update(integer objectIndex, BoundCallback bound)
		{
			const Object& object = objectList_[objectIndex];
			for (integer i = 0;i < N;++i)
			{
				VertexContainer& vertexList = vertexList_[i];

				const Real newMinPosition = bound(i, true, object);
				const Real newMaxPosition = bound(i, false, object);

				const integer startIndex = indexList_[objectIndex * 2 * N + 2 * i];

				const Real oldMinPosition = vertexList[startIndex].position();

				if (newMinPosition <= oldMinPosition)
				{
					updateVertex(i, startIndex, newMinPosition);

					const integer newEndIndex = indexList_[objectIndex * 2 * N + 2 * i + 1];
					updateVertex(i, newEndIndex, newMaxPosition);
				}
				else
				{
					const integer endIndex = indexList_[objectIndex * 2 * N + 2 * i + 1];
					updateVertex(i, endIndex, newMaxPosition);

					integer newStartIndex = indexList_[objectIndex * 2 * N + 2 * i];
					updateVertex(i, newStartIndex, newMinPosition);
				}
			}

			//check();
		}

		void updateIndices(integer i, integer startIndex, integer endIndex)
		{
			VertexContainer& vertexList = vertexList_[i];

			for (integer k = startIndex;k < endIndex;++k)
			{
				Vertex& vertex = vertexList[k];
				indexList_[vertex.indexIndex()] = k;
			}
		}

		void updateStabbingNumbers(integer i, integer startIndex, integer endIndex)
		{
			ASSERT(startIndex <= endIndex);

			VertexContainer& vertexList = vertexList_[i];

			integer updateStartIndex = startIndex;
			if (updateStartIndex == 0)
			{
				vertexList[0].setStabbingNumber(0);
				++updateStartIndex;
			}

			for (integer k = updateStartIndex;k < endIndex;++k)
			{
				Vertex& previousVertex = vertexList[k - 1];
				Vertex& currentVertex = vertexList[k];
				integer stab = previousVertex.stabbingNumber();
				if (previousVertex.start())
				{
					++stab;
				}
				else
				{
					--stab;
				}

				currentVertex.setStabbingNumber(stab);
			}
		}

		void check()
		{
			for (integer i = 0;i < N;++i)
			{
				VertexContainer& vertexList = vertexList_[i];
				for (integer k = 1;k < vertexList.size();++k)
				{
					ASSERT(vertexList[k - 1] < vertexList[k]);

					integer stabbingNumber = vertexList[k - 1].stabbingNumber();
					if (vertexList[k - 1].start())
					{
						++stabbingNumber;
					}
					else
					{
						--stabbingNumber;
					}

					ASSERT2(vertexList[k].stabbingNumber() == stabbingNumber,
						vertexList[k].stabbingNumber(), stabbingNumber);
				}
				for (integer k = 0;k < objectList_.size();++k)
				{
					const integer startIndex = indexList_[k * 2 * N + 2 * i];
					const integer endIndex = indexList_[k * 2 * N + 2 * i + 1];

					ASSERT(vertexList[startIndex] < vertexList[endIndex]);
				}
			}
		}

		void fixRange(integer i, integer oldIndex, integer newIndex)
		{
			VertexContainer& vertexList = vertexList_[i];

			// Fix indices.

			const integer minIndex = std::min(oldIndex, newIndex);
			const integer maxIndex = std::max(oldIndex, newIndex);

			updateIndices(i, minIndex, maxIndex + 1);

			// Fix stabbing numbers.

			updateStabbingNumbers(i, minIndex, maxIndex + 1);

			// Fix overlap status.
			if (newIndex < oldIndex)
			{
				Vertex& right = vertexList[newIndex];
				for (integer k = oldIndex;k > newIndex;--k)
				{
					Vertex& left = vertexList[k];
					updateSwap(left, right);
				}
			}
			else if (newIndex > oldIndex)
			{
				Vertex& left = vertexList[newIndex];
				for (integer k = oldIndex;k < newIndex;++k)
				{
					Vertex& right = vertexList[k];
					updateSwap(left, right);
				}
			}
		}

		void updateSwap(Vertex& left, Vertex& right)
		{
			ASSERT(left.object() != right.object());

			if (left.start() && !right.start())
			{
				decreaseOverlap(left.object(), right.object());
			}
			else if (!left.start() && right.start())
			{
				increaseOverlap(left.object(), right.object());
			}
		}

		void increaseOverlap(integer a, integer b)
		{
			ASSERT(a != b);

			const std::pair<integer, integer> overlap =
				std::make_pair(std::min(a, b), std::max(a, b));

			CounterIterator iter = counterSet_.find(overlap);
			if (iter != counterSet_.end())
			{
				REPORT(iter->second == N);
				++(iter->second);
				if (iter->second == N)
				{
					collisionSet_.insert(overlap);
				}
			}
			else
			{
				counterSet_.insert(
					std::make_pair(overlap, (integer)1));
			}
		}

		void decreaseOverlap(integer a, integer b)
		{
			ASSERT(a != b);

			const std::pair<integer, integer> overlap =
				std::make_pair(std::min(a, b), std::max(a, b));

			CounterIterator iter = counterSet_.find(overlap);
			ASSERT(iter != counterSet_.end());

			if (iter->second == N)
			{
				collisionSet_.erase(overlap);
			}

			ASSERT1(iter->second > 0, iter->second);

			--(iter->second);
			if (iter->second == 0)
			{
				counterSet_.erase(iter);
			}
		}

		typedef SmallSet<Vertex> VertexContainer;
		typedef std::vector<Object> ObjectContainer;
		typedef std::vector<integer> IndexContainer;

		Tuple<N, VertexContainer> vertexList_;
		IndexContainer indexList_;
		ObjectContainer objectList_;
		CounterContainer counterSet_;
		CollisionContainer collisionSet_;
	};

}

#endif
