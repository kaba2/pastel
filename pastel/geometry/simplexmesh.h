#ifndef PASTEL_SIMPLEXMESH_H
#define PASTEL_SIMPLEXMESH_H

#include "pastel/geometry/simplexmesh_datapolicies.h"

#include "pastel/sys/possiblyemptymember.h"
#include "pastel/sys/unorderedset.h"
#include "pastel/sys/tuple.h"

#include <boost/operators.hpp>

namespace Pastel
{

	//! A simplex mesh with arbitrary topology but minimal adjacency queries.
	/*!
	DataPolicy:
	A policy you use to define user data, interface extensions, and
	how to do react at the creation of elements. Predefined data policies
	can be found from 'pastel/geometry/simplexmesh_datapolicies.h'.

	class DataPolicy_Concept
	{
	public:
		// You can expand the interface
		// of SimplexMesh by defining your functions
		// here.
		void yourOwnInterfaceExtension();

	protected:
		// The following must be implemented
		// for the SimplexMesh to work.
		// Being implementation details, they should be protected.

		// Define a user data type as an EmptyClass
		// if no user data is desired.

		typedef undefined_type VertexData;
		typedef undefined_type SimplexData;

		void swap(DataPolicy_Concept& that);
		void clear();

		// If a user data type is defined as an
		// EmptyClass, a zero pointer is passed
		// to the following functions. The
		// implementation must handle these cases
		// correctly.

		void constructVertex(VertexData* vertex);
		void constructSimplex(SimplexData* simplex);

		void destructVertex(VertexData* vertex);
		void destructSimplex(SimplexData* simplex);
	};
	*/

	template <int N, typename DataPolicy = SimplexMesh_EmptyClassDataPolicy>
	class SimplexMesh
		: public DataPolicy
	{
	public:
		// Can't use 'using' because of a compiler bug.

		typedef typename DataPolicy::VertexData VertexData;
		typedef typename DataPolicy::SimplexData SimplexData;

		class Vertex;
		class Simplex;

	private:
		class VertexBody;
		class SimplexBody;

	public:
		class VertexHash
		{
		public:
			std::size_t operator()(const Vertex& vertex) const
			{
				return hash_(vertex.data_);
			}

		private:
			hash<VertexBody*> hash_;
		};

		class SimplexHash
		{
		public:
			std::size_t operator()(const Simplex& simplex) const
			{
				return hash_(simplex.data_);
			}

		private:
			hash<SimplexBody*> hash_;
		};

	private:
		typedef UnorderedSet<Vertex, VertexHash> VertexSet;
		typedef UnorderedSet<Simplex, SimplexHash> SimplexSet;

	public:
		typedef typename VertexSet::const_iterator ConstVertexIterator;
		typedef typename SimplexSet::const_iterator ConstSimplexIterator;

		//! Constructs an empty SimplexMesh.
		/*!
		Time complexity:
		constant

		Exception safety:
		strong
		*/
		SimplexMesh();

		//! Constructs a copy of another SimplexMesh.
		/*!
		Time complexity:
		O(that.vertices() + that.simplices())

		Exception safety:
		strong
		*/
		SimplexMesh(const SimplexMesh& that);

		//! Assigns from another SimplexMesh.
		/*!
		Time complexity:
		O(vertices() + simplices() +
		that.vertices() + that.simplices())

		Exception safety:
		strong
		*/
		SimplexMesh& operator=(const SimplexMesh& that);

		//! Destructs the mesh.
		/*!
		Time complexity:
		O(vertices() + simplices())

		Exception safety:
		nothrow
		*/
		~SimplexMesh();

		//! Swaps two SimplexMesh's.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		void swap(SimplexMesh& that);

		//! Removes all vertices and simplices.
		/*!
		Time complexity:
		O(vertices() + simplices())

		Exception safety:
		nothrow
		*/
		void clear();

		//! Adds a singular vertex to the mesh.
		/*!
		Time complexity:
		Average constant

		Exception safety:
		strong
		*/
		Vertex addVertex();

		//! Adds a triangle to the mesh.
		/*!
		Time complexity:
		Average constant

		Exception safety:
		strong
		*/
		Simplex addSimplex(
			const Tuple<Vertex, PASTEL_ADD_N(N, 1)>& vertexList);

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

		//! Returns the number of vertices in the mesh.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		integer vertices() const;

		//! Returns the number of simplices in the mesh.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		integer simplices() const;

	private:
		class VertexBody
			: public PossiblyEmptyMember<VertexData>
		{
		};

		class SimplexBody
			: public PossiblyEmptyMember<SimplexData>
		{
		public:
			Tuple<Vertex, PASTEL_ADD_N(N, 1)> vertexList_;
		};

	public:
		class Vertex
			: boost::equality_comparable<Vertex
			, boost::less_than_comparable<Vertex
			> >
		{
		public:
			friend class SimplexMesh<N, DataPolicy>;

			Vertex()
				: data_(0)
			{
			}

			VertexData& operator*() const
			{
				return *data_->data();
			}

			VertexData* operator->() const
			{
				return data_->data();
			}

			bool operator<(const Vertex& that) const
			{
				return data_ < that.data_;
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

		class Simplex
			: boost::equality_comparable<Simplex
			, boost::less_than_comparable<Simplex
			> >
		{
		public:
			friend class SimplexMesh<N, DataPolicy>;

			Simplex()
				: data_(0)
			{
			}

			SimplexData& operator*() const
			{
				return *data_->data();
			}

			SimplexData* operator->() const
			{
				return data_->data();
			}

			bool operator<(const Simplex& that) const
			{
				return data_ < that.data_;
			}

			bool operator==(const Simplex& that) const
			{
				return data_ == that.data_;
			}

			Vertex vertex(integer index) const
			{
				return data_->vertexList_[index];
			}

		private:
			explicit Simplex(SimplexBody* data)
				: data_(data)
			{
			}

			SimplexBody* data_;
		};

	private:
		// Allocates a vertex.
		/*
		Time complexity:
		average constant

		Exception safety:
		strong
		*/
		Vertex allocateVertex();

		// Allocates a simplex.
		/*
		Time complexity:
		average constant

		Exception safety:
		strong
		*/
		Simplex allocateSimplex();

		VertexSet vertexSet_;
		SimplexSet simplexSet_;
		PoolAllocator vertexAllocator_;
		PoolAllocator simplexAllocator_;
	};

}

#include "pastel/geometry/simplexmesh.hpp"

#endif
