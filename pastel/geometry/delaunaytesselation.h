#ifndef PASTEL_DELAUNAYTESSELATION_H
#define PASTEL_DELAUNAYTESSELATION_H

#include "pastel/sys/tuple.h"
#include "pastel/sys/poolallocator.h"
#include "pastel/sys/point.h"
#include "pastel/sys/unorderedset.h"
#include "pastel/sys/possiblyemptymember.h"

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/simplexmesh_datapolicies.h"

namespace Pastel
{

	//! Delaunay tesselation
	/*!
	DataPolicy:
	See the documentation for SimplexMesh's DataPolicy concept.
	Predefined data policies can be found from
	'pastel/geometry/simplexmesh_datapolicies.h'.
	*/

	template <typename Real, typename DataPolicy = SimplexMesh_EmptyDataPolicy>
	class DelaunayTesselation
		: public DataPolicy
	{
	public:
		class VertexBody;
		class SimplexBody;

		class Vertex;
		class Simplex;

		class VertexHash;
		class SimplexHash;

	private:
		typedef UnorderedSet<Vertex, VertexHash> VertexSet;
		typedef UnorderedSet<Simplex, SimplexHash> SimplexSet;

	public:
		typedef typename VertexSet::const_iterator ConstVertexIterator;
		typedef typename SimplexSet::const_iterator ConstSimplexIterator;

		//! Constructs an empty triangulation.
		/*!
		Time complexity:
		constant

		Exception safety:
		strong
		*/
		DelaunayTesselation();

		//! Destructs the triangulation.
		/*!
		Time complexity:
		linear

		Exception safety:
		nothrow
		*/
		~DelaunayTesselation();

		//! Constructs a copy from another triangulation.
		/*!
		Time complexity:
		linear

		Exception safety:
		strong
		*/
		DelaunayTesselation(
			const DelaunayTesselation& that);

		//! Assigns another triangulation.
		/*!
		Time complexity:
		linear

		Exception safety:
		strong
		*/
		DelaunayTesselation<Real, DataPolicy>& operator=(
			const DelaunayTesselation& that);

		//! Swaps with another triangulation.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		void swap(
			DelaunayTesselation& that);

		//! Removes all simplices.
		/*!
		Time complexity:
		linear

		Exception safety:
		nothrow
		*/
		void clear();

		//! Returns an iterator to the beginning of the vertex set.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		ConstVertexIterator vertexBegin() const;

		//! Returns an iterator to the end of the vertex set.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		ConstVertexIterator vertexEnd() const;

		//! Returns an iterator to the beginning of the simplex set.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		ConstSimplexIterator simplexBegin() const;

		//! Returns an iterator to the end of the simplex set.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		ConstSimplexIterator simplexEnd() const;

		//! Removes all simplices.
		/*!
		Time complexity:
		expected log n

		Exception safety:
		strong
		*/
		Vertex addVertex(
			const Point<2, Real>& position);

		//! Returns the root simplex.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow

		The root simplex contains a hierarchy of simplices such
		that each simplex contains its children. The hierarchy
		can then be traversed by following the children
		simplices. The simplices at the leaf nodes are given
		access through the simplexBegin() and simplexEnd() functions.
		*/
		Simplex root() const;

		//! Returns the number of vertices.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		integer vertices() const;

		//! Returns the number of simplices.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		integer simplices() const;

		//! Returns an aligned box that bounds the tesselation.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		AlignedBox<2, Real> bound() const;

	private:
		void initialize();

		void subdivide(
			SimplexBody* simplex,
			VertexBody* vertex);

		void flipEdge(
			SimplexBody* aSimplex, integer aEdge,
			SimplexBody* bSimplex, integer bEdge);

		void legalizeEdge(
			SimplexBody* simplex,
			integer edge);

		SimplexBody* allocateSimplex();
		VertexBody* allocateVertex(const Point<2, Real>& position);

		PoolAllocator vertexAllocator_;
		PoolAllocator simplexAllocator_;
		SimplexBody* root_;
		VertexSet vertexSet_;
		SimplexSet simplexSet_;
	};

	template <typename Real, typename DataPolicy>
	class DelaunayTesselation<Real, DataPolicy>::Vertex
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		friend class DelaunayTesselation<Real, DataPolicy>;

		Vertex()
			: data_(0)
		{
		}

		bool empty() const
		{
			return data_ == 0;
		}

		const Point<2, Real>& position() const
		{
			PENSURE(data_);
			return data_->position();
		}

		typename DataPolicy::VertexData& data()
		{
			return *data_->data();
		}

		const typename DataPolicy::VertexData& data() const
		{
			return *data_->data();
		}

		bool operator==(const Vertex& that) const
		{
			return data_ == that.data_;
		}

	private:
		explicit Vertex(VertexBody* data)
			: data_(data)
		{
		}

		VertexBody* data_;
	};

	template <typename Real, typename DataPolicy>
	class DelaunayTesselation<Real, DataPolicy>::Simplex
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		friend class DelaunayTesselation<Real, DataPolicy>;

		Simplex()
			: data_(0)
		{
		}

		bool operator==(const Simplex& that) const
		{
			return data_ == that.data_;
		}

		bool empty() const
		{
			return data_ == 0;
		}

		typename DataPolicy::SimplexData& data()
		{
			return *data_->data();
		}

		const typename DataPolicy::SimplexData& data() const
		{
			return *data_->data();
		}

		Tuple<3, Vertex> vertex() const
		{
			return Tuple<3, Vertex>(
				Vertex(data_->vertex(0)),
				Vertex(data_->vertex(1)),
				Vertex(data_->vertex(2)));
		}

		Tuple<3, Simplex> pair() const
		{
			return Tuple<3, Simplex>(
				Simplex(data_->pair(0)),
				Simplex(data_->pair(1)),
				Simplex(data_->pair(2)));
		}

		Tuple<3, Simplex> child() const
		{
			return Tuple<3, Simplex>(
				Simplex(data_->child(0)),
				Simplex(data_->child(1)),
				Simplex(data_->child(2)));
		}

		Vertex vertex(integer index) const
		{
			PENSURE(data_);
			return Vertex(data_->vertex(index));
		}

		Simplex pair(integer index) const
		{
			PENSURE(data_);
			return Simplex(data_->pair(index));
		}

		Simplex child(integer index) const
		{
			PENSURE(data_);
			return Simplex(data_->child(index));
		}

	private:
		explicit Simplex(
			SimplexBody* data)
			: data_(data)
		{
		}

		SimplexBody* data_;
	};

	template <typename Real, typename DataPolicy>
	class DelaunayTesselation<Real, DataPolicy>::VertexBody
		: public PossiblyEmptyMember<typename DataPolicy::VertexData>
	{
	private:
		typedef PossiblyEmptyMember<typename DataPolicy::VertexData> Base;

		friend class DelaunayTesselation<Real, DataPolicy>;
		friend class DelaunayTesselation<Real, DataPolicy>::Vertex;

	public:
		VertexBody()
			: position_()
		{
		}

		explicit VertexBody(
			const Point<2, Real>& position)
			: position_(position)
		{
		}

		const Point<2, Real>& position() const
		{
			return position_;
		}

	private:
		Point<2, Real> position_;
	};

	template <typename Real, typename DataPolicy>
	class DelaunayTesselation<Real, DataPolicy>::SimplexBody
		: public PossiblyEmptyMember<typename DataPolicy::SimplexData>
	{
	private:
		typedef PossiblyEmptyMember<typename DataPolicy::SimplexData> Base;

		friend class DelaunayTesselation<Real, DataPolicy>;
		friend class DelaunayTesselation<Real, DataPolicy>::Simplex;

	public:
		/*
		The data has been packed for better memory usage.

		* Because nowadays data is aligned on 4-byte
		boundaries, the 2 lowest bits of pointers are left
		unused and can be used for data storage.

		* Because pair-pointers are only needed at
		leaf nodes, and the child-pointers are
		only needed at intermediate nodes, these
		data can be stored in a common memory space
		which we call 'link_'.

		* We also need pair-indices which range [0, 2].
		These can be stored to the 2 lowest bits
		of the pair-pointers.

		* To distinguish leaf-nodes from intermediate
		nodes, we store a constant '3' to the 2 lowest bits
		of the child-pointers.
		*/

		SimplexBody()
			: vertex_(0)
			, link_(0)
		{
		}

		bool leaf() const
		{
			return unpackIndex(link_[0]) < 3;
		}

		void setVertex(
			const Tuple<3, VertexBody*>& vertex)
		{
			vertex_ = vertex;
		}

		const Tuple<3, VertexBody*>& vertex() const
		{
			return vertex_;
		}

		VertexBody* vertex(integer index) const
		{
			return vertex_[index];
		}

		void setChild(
			const Tuple<3, SimplexBody*>& child)
		{
			for (integer i = 0;i < 3;++i)
			{
				link_[i] = pack(child[i], 3);
			}
		}

		Tuple<3, SimplexBody*> child() const
		{
			return Tuple<3, SimplexBody*>(
				child(0),
				child(1),
				child(2));
		}

		SimplexBody* child(integer index) const
		{
			return unpack(link_[index]);
		}

		Tuple<3, SimplexBody*> pair() const
		{
			return Tuple<3, SimplexBody*>(
				pair(0),
				pair(1),
				pair(2));
		}

		SimplexBody* pair(
			integer index) const
		{
			return unpack(link_[index]);
		}

		integer pairIndex(
			integer index) const
		{
			return unpackIndex(link_[index]);
		}

		friend void link(
			SimplexBody* aSimplex,
			integer aIndex,
			SimplexBody* bSimplex,
			integer bIndex)
		{
			if (aSimplex)
			{
				aSimplex->link_[aIndex] = pack(bSimplex, bIndex);
			}

			if (bSimplex)
			{
				bSimplex->link_[bIndex] = pack(aSimplex, aIndex);
			}
		}

	private:
		friend integer unpackIndex(SimplexBody* simplex)
		{
			return ((pointer_integer)simplex) & 0x3;
		}

		friend SimplexBody* pack(SimplexBody* simplex, integer index)
		{
			ASSERT1(index >= 0 && index <= 3, index);

			const pointer_integer simplexInteger =
				((pointer_integer)simplex) + index;

			return (SimplexBody*)simplexInteger;
		}

		friend SimplexBody* unpack(SimplexBody* simplex)
		{
			const pointer_integer simplexInteger =
				((pointer_integer)simplex) & ~0x3;

			return (SimplexBody*)simplexInteger;
		}

		Tuple<3, VertexBody*> vertex_;
		Tuple<3, SimplexBody*> link_;
	};

	template <typename Real, typename DataPolicy>
	class DelaunayTesselation<Real, DataPolicy>::VertexHash
	{
	public:
		std::size_t operator()(const Vertex& vertex) const
		{
			const hash<VertexBody*> hash_;
			return hash_(vertex.data_);
		}
	};

	template <typename Real, typename DataPolicy>
	class DelaunayTesselation<Real, DataPolicy>::SimplexHash
	{
	public:
		std::size_t operator()(const Simplex& simplex) const
		{
			const hash<SimplexBody*> hash_;
			return hash_(simplex.data_);
		}
	};

}

#include "pastel/geometry/delaunaytesselation.hpp"

#endif
