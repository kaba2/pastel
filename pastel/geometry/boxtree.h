#ifndef PASTELGEOMETRY_BOXTREE_H
#define PASTELGEOMETRY_BOXTREE_H

namespace Pastel
{

	template <int N, typename Real, typename UserData>
	class BoxTree
	{
	public:
		typedef int32 Index;

		class Node
		{
		public:
			Node(const AlignedBox<N, Real>& positiveBox,
				Index positiveIndex,
				const AlignedBox<N, Real>& negativeBox,
				Index negativeIndex)
				: positiveBox_(positiveBox)
				, positiveIndex_(positiveIndex)
				, negativeBox_(negativeBox)
				, negativeIndex_(negativeIndex)
			{
			}

			enum
			{
				LeafMask = 0x80000000
			};

			// if (positiveIndex_ & LeafMask), then
			// the positive child is a leaf node
			// and 'positiveIndex_ - LeafMask' is an index
			// to the 'dataList_'.
			// Otherwise 'positiveIndex_' is an
			// index to the 'nodeList_'.
			// Similarly for negativeIndex.

			AlignedBox<N, Real> positiveBox_;
			Index positiveIndex_;
			AlignedBox<N, Real> negativeBox_;
			Index negativeIndex_;
		};

		class Cursor
		{
		public:
			Cursor()
				: tree_(0)
				, node_(0)
			{
			}

			Cursor(BoxTree<N, Real, UserData>* tree,
				Node* node)
				: tree_(tree)
				, node_(node)
			{
				ENSURE(tree_);
				ENSURE(node_);
			}

			Cursor positive() const
			{
				PENSURE(!positiveLeaf());

				return Cursor(tree_, &(tree_->nodeList_[node_->positiveIndex_]));
			}

			Cursor negative() const
			{
				PENSURE(!negativeLeaf());

				return Cursor(tree_, &(tree_->nodeList_[node_->negativeIndex_]));
			}

			const AlignedBox<N, Real>& positiveBox() const
			{
				PENSURE(node_);
				return node_->positiveBox_;
			}

			const AlignedBox<N, Real>& negativeBox() const
			{
				PENSURE(node_);
				return node_->negativeBox_;
			}

			bool positiveLeaf() const
			{
				PENSURE(node_);
				return (node_->positiveIndex_ & LeafMask) != 0;
			}

			bool negativeLeaf() const
			{
				PENSURE(node_);
				return (node_->negativeIndex_ & LeafMask) != 0;
			}

			UserData& positiveData() const
			{
				PENSURE(positiveLeaf());
				return tree_->dataList_[node_->positiveIndex_ - LeafMask];
			}

			UserData& negativeData() const
			{
				PENSURE(negativeLeaf());
				return tree_->dataList_[node_->negativeIndex_ - LeafMask];
			}

		private:
			BoxTree<N, Real, UserData>* tree_;
			Node* node_;
		};

		class DepthFirstEntry
		{
		public:
			DepthFirstEntry(
				integer start,
				integer end,
				const AlignedBox<N, Real>& bound,
				bool positiveChild)
				: start_(start)
				, end_(end)
				, bound_(bound)
				, positiveChild_(positiveChild)
			{
			}

			integer start_;
			integer end_;
			AlignedBox<N, Real> bound_;
			bool positiveChild_;
		};

		void construct(
			const std::vector<AlignedBox<N, Real> >& boxList,
			const std::vector<UserData>& dataList)
		{
			nodeList_.clear();
			dataList_.clear();

			// The data structure relies on recursing down to a
			// single box.

			const integer size = boxList.size();
			ENSURE2(dataList.size() == size, dataList.size(), size);

			// Rather than arranging the 'boxList',
			// we arrange indices into it.

			std::vector<Index> permutation;
			permutation.reserve(size);

			for (integer i = 0;i < size;++i)
			{
				permutation.push_back(i);
			}

			// Find out the bounding box for all boxes.

			AlignedBox<N, Real> treeBound;
			for (integer i = 0;i < size;++i)
			{
				treeBound = boundingAlignedBox(boxList[i], treeBound);
			}

			treeBound_ = treeBound;

			// Form the box tree by a depth first traversal.
			// Here recursion is replaced by iteration
			// and a stack.

			std::list<DepthFirstEntry> depthFirstList;

			// Start by dividing the whole box set.

			depthFirstList.push_back(
				DepthFirstEntry(0, size, treeBound, false));

			while(!depthFirstList.empty())
			{
				// Retrieve the next set of boxes to split.

				const DepthFirstEntry entry = depthFirstList.back();
				const integer depth = entry.depth_;
				const integer startIndex = entry.start_;
				const integer endIndex = entry.end_;
				const AlignedBox<N, Real>& bound = entry.bound_;

				depthFirstList.pop_back();

				// Split along the longest dimension of the bounding box.

				const Vector<N, Real> extent = bound.max() - bound.min();

				const integer splitDimension = maxIndex(extent);

				// Split from the center of the box.

				const Real split =
					linear(bound.min()[splitDimension], bound.max()[splitDimension], 0.5);

				// Arrange the indices on the range so
				// that those boxes that have their centers below
				// the split position are arranged to the start
				// of the range.

				AlignedBox<N, Real> negativeBox;
				AlignedBox<N, Real> positiveBox;

				integer negativeEndIndex = startIndex;

				for (integer i = startIndex;i < endIndex;++i)
				{
					const AlignedBox<N, Real>& box =
						boxList[permutation[i]];

					const Real boxCenter =
						linear(box.min()[splitDimension], box.max()[splitDimension], 0.5);

					if (boxCenter < split)
					{
						// This box goes to the negative child.
						std::swap(permutation[i], permutation[negativeEndIndex]);
						++negativeEndIndex;

						// Update the negative child bound.
						negativeBox = boundingAlignedBox(negativeBox, box);
					}
					else
					{
						// This box goes to the positive child.
						// Update the positive child bound.
						positiveBox = boundingAlignedBox(positiveBox, box);
					}
				}

				if (negativeEndIndex == startIndex ||
					negativeEndIndex == endIndex)
				{
					// The other child would be left empty.

					negativeEndIndex = (startIndex + endIndex) / 2;
				}

				// Add the node.

				// In the case of intermediate nodes,
				// the indices are to the node list.

				integer negativeIndex = startIndex;
				integer positiveIndex = negativeEndIndex;

				if (negativeEndIndex - startIndex <= 1)
				{
					// The negative child is a leaf node,
					// change the index to the userdata list.

					negativeIndex = permutation[negativeIndex] + LeafMask;
				}
				if (endIndex - positiveIndex <= 1)
				{
					// The positive child is a leaf node,
					// change the index to the userdata list.

					positiveIndex = permutation[positiveIndex] + LeafMask;
				}

				nodeList_.push_back(
					Node(negativeBox, negativeIndex, positiveBox, positiveIndex, entry.positiveChild_));

				// Recurse

				if (startIndex != negativeEndIndex)
				{
					depthFirstList_.push_back(
						DepthFirstEntry(startIndex, negativeEndIndex, negativeBox, false));
				}
				if (negativeEndIndex != endIndex)
				{
					depthFirstList_.push_back(
						DepthFirstEntry(negativeEndIndex, endIndex, positiveBox, true));
				}
			}

			// Now add the user data in permuted order.

			dataList_.reserve(size);
			for (integer i = 0;i < size;++i)
			{
				dataList_.push_back(dataList[permutation[i]]);
			}
		}

		const AlignedBox<N, Real>& bound() const
		{
			return treeBound_;
		}

		bool empty() const
		{
			return nodeList_.empty();
		}

		Cursor root() const
		{
			if (nodeList_.empty())
			{
				return Cursor(this, 0);
			}

			return Cursor(this, &nodeList_.front());
		}

		void swap(BoxTree& that)
		{
			nodeList_.swap(that.nodeList_);
			dataList_.swap(that.dataList_);
			treeBound_.swap(that.treeBound_);
		}

		void clear()
		{
			nodeList_.clear();
			dataList_.clear();
			treeBound_ = AlignedBox<N, Real>();
		}

	private:
		typedef std::vector<Node> NodeContainer;
		typedef std::vector<UserData> DataContainer;

		NodeContainer nodeList_;
		DataContainer dataList_;
		AlignedBox<N, Real> treeBound_;
	};

	/*
	- Noodi tallettaa lapsiensa laatikot
	- Lehti-solmujen läpikäynti
	*/

}

#endif
