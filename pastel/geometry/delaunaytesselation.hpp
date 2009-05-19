#ifndef PASTEL_DELAUNAYTESSELATION_HPP
#define PASTEL_DELAUNAYTESSELATION_HPP

#include "pastel/geometry/delaunaytesselation.h"
#include "pastel/geometry/bounding_sphere.h"
#include "pastel/geometry/overlaps_simplex_point.h"

namespace Pastel
{

	template <typename Real, typename DataPolicy>
	DelaunayTesselation<Real, DataPolicy>::DelaunayTesselation()
		: vertexAllocator_(sizeof(VertexBody))
		, simplexAllocator_(sizeof(SimplexBody))
		, root_(0)
		, vertexSet_()
		, simplexSet_()
	{
		initialize();
	}

	template <typename Real, typename DataPolicy>
	DelaunayTesselation<Real, DataPolicy>::DelaunayTesselation(
		const DelaunayTesselation& that)
		: vertexAllocator_(sizeof(VertexBody))
		, simplexAllocator_(sizeof(SimplexBody))
		, root_(0)
		, vertexSet_()
		, simplexSet_()
	{
		initialize();

		// TODO
		ENSURE(false);
	}

	template <typename Real, typename DataPolicy>
	DelaunayTesselation<Real, DataPolicy>::~DelaunayTesselation()
	{
		vertexAllocator_.clear();
		simplexAllocator_.clear();
		root_ = 0;
		vertexSet_.clear();
		simplexSet_.clear();
	}

	template <typename Real, typename DataPolicy>
	DelaunayTesselation<Real, DataPolicy>& DelaunayTesselation<Real, DataPolicy>::operator=(
		const DelaunayTesselation& that)
	{
		DelaunayTesselation<Real, DataPolicy> copy(that);
		swap(copy);
		return *this;
	}

	template <typename Real, typename DataPolicy>
	void DelaunayTesselation<Real, DataPolicy>::swap(
		DelaunayTesselation& that)
	{
		vertexAllocator_.swap(that.vertexAllocator_);
		simplexAllocator_.swap(that.simplexAllocator_);
		std::swap(root_, that.root_);
		vertexSet_.swap(that.vertexSet_);
		simplexSet_.swap(that.simplexSet_);
	}

	template <typename Real, typename DataPolicy>
	void DelaunayTesselation<Real, DataPolicy>::clear()
	{
		vertexAllocator_.clear();
		simplexAllocator_.clear();
		root_ = 0;
		vertexSet_.clear();
		simplexSet_.clear();

		initialize();
	}

	template <typename Real, typename DataPolicy>
	typename DelaunayTesselation<Real, DataPolicy>::ConstVertexIterator
		DelaunayTesselation<Real, DataPolicy>::vertexBegin() const
	{
		return vertexSet_.begin();
	}

	template <typename Real, typename DataPolicy>
	typename DelaunayTesselation<Real, DataPolicy>::ConstVertexIterator
		DelaunayTesselation<Real, DataPolicy>::vertexEnd() const
	{
		return vertexSet_.end();
	}

	template <typename Real, typename DataPolicy>
	typename DelaunayTesselation<Real, DataPolicy>::ConstSimplexIterator
		DelaunayTesselation<Real, DataPolicy>::simplexBegin() const
	{
		return simplexSet_.begin();
	}

	template <typename Real, typename DataPolicy>
	typename DelaunayTesselation<Real, DataPolicy>::ConstSimplexIterator
		DelaunayTesselation<Real, DataPolicy>::simplexEnd() const
	{
		return simplexSet_.end();
	}

	template <typename Real, typename DataPolicy>
	typename DelaunayTesselation<Real, DataPolicy>::Simplex
		DelaunayTesselation<Real, DataPolicy>::root() const
	{
		return Simplex(root_);
	}

	template <typename Real, typename DataPolicy>
	typename DelaunayTesselation<Real, DataPolicy>::Vertex
		DelaunayTesselation<Real, DataPolicy>::addVertex(
		const Point<2, Real>& position)
	{
		{
			const Tuple<3, VertexBody*>& vertex = root_->vertex();
			if (!overlaps(
				Simplex2(
				vertex[0]->position(),
				vertex[1]->position(),
				vertex[2]->position()), position))
			{
				return Vertex();
			}
		}

		SimplexBody* simplexData = root_;
		while(!simplexData->leaf())
		{
			bool foundChild = false;
			for (integer i = 0;i < 3;++i)
			{
				if (simplexData->child(i))
				{
					const Tuple<3, VertexBody*>& vertex =
						simplexData->child(i)->vertex();

					if (overlaps(
						Simplex2(
						vertex[0]->position(),
						vertex[1]->position(),
						vertex[2]->position()), position))
					{
						simplexData = simplexData->child(i);
						foundChild = true;
						break;
					}
				}
			}

			if (REPORT(!foundChild))
			{
				return Vertex();
			}
		}

		VertexBody* vertexData = allocateVertex(position);

		subdivide(
			simplexData,
			vertexData);

		return Vertex(vertexData);
	}

	template <typename Real, typename DataPolicy>
	integer DelaunayTesselation<Real, DataPolicy>::vertices() const
	{
		return vertexSet_.size();
	}

	template <typename Real, typename DataPolicy>
	integer DelaunayTesselation<Real, DataPolicy>::simplices() const
	{
		return simplexSet_.size();
	}

	template <typename Real, typename DataPolicy>
	AlignedBox<2, Real> DelaunayTesselation<Real, DataPolicy>::bound() const
	{
		using Pastel::Simplex;

		return boundingAlignedBox(
			Simplex<2, Real, 2>(
			Tuple<3, Point<2, Real> >(
			root_->vertex(0)->position(),
			root_->vertex(1)->position(),
			root_->vertex(2)->position())));
	}

	// Private

	template <typename Real, typename DataPolicy>
	void DelaunayTesselation<Real, DataPolicy>::initialize()
	{
		root_ = allocateSimplex();

		VertexBody* aVertex = allocateVertex(
			Point<2, Real>(0, 50));
		VertexBody* bVertex = allocateVertex(
			Point<2, Real>(-50, -50));
		VertexBody* cVertex = allocateVertex(
			Point<2, Real>(50, -50));

		root_->setVertex(makeTuple(
			aVertex, bVertex, cVertex));

		simplexSet_.insert(Simplex(root_));
	}

	template <typename Real, typename DataPolicy>
	void DelaunayTesselation<Real, DataPolicy>::subdivide(
		SimplexBody* simplexData,
		VertexBody* vertexData)
	{
		//        c
		//        x
		//       /|\
		//      / | \
		//     /C | B\
		//    /  _d_  \
		//   /_/  A  \_\
		//  x___________x
		// a             b

		VertexBody* aVertex = simplexData->vertex(0);
		VertexBody* bVertex = simplexData->vertex(1);
		VertexBody* cVertex = simplexData->vertex(2);
		VertexBody* dVertex = vertexData;

		SimplexBody* aChild = allocateSimplex();

		aChild->setVertex(
			makeTuple(
			aVertex,
			bVertex,
			dVertex));

		SimplexBody* bChild = allocateSimplex();

		bChild->setVertex(
			makeTuple(
			bVertex,
			cVertex,
			dVertex));

		SimplexBody* cChild = allocateSimplex();

		cChild->setVertex(
			makeTuple(
			cVertex,
			aVertex,
			dVertex));

		link(aChild, 0, simplexData->pair(0), simplexData->pairIndex(0));
		link(aChild, 1, bChild, 2);
		link(aChild, 2, cChild, 1);

		link(bChild, 0, simplexData->pair(1), simplexData->pairIndex(1));
		link(bChild, 1, cChild, 2);

		link(cChild, 0, simplexData->pair(2), simplexData->pairIndex(2));

		simplexData->setChild(
			makeTuple(
			aChild,
			bChild,
			cChild));

		simplexSet_.erase(Simplex(simplexData));
		simplexSet_.insert(Simplex(aChild));
		simplexSet_.insert(Simplex(bChild));
		simplexSet_.insert(Simplex(cChild));

		legalizeEdge(aChild, 0);
		legalizeEdge(bChild, 0);
		legalizeEdge(cChild, 0);
	}

	template <typename Real, typename DataPolicy>
	void DelaunayTesselation<Real, DataPolicy>::flipEdge(
		SimplexBody* aSimplex, integer aEdge,
		SimplexBody* bSimplex, integer bEdge)
	{
		ASSERT(aSimplex);
		ASSERT(bSimplex);

		//   +-----+
		//   |\  B |
		//   |  \  |
		//   | A  \|
		//   +-----+

		//  a2    b1     b3
		//   +     +-----+
		//   |\     \  B |
		//   |  \     \  |
		//   | A  \     \|
		//   +-----+     +
		//  a3     a1    b2

		const integer a1 = aEdge;
		const integer a2 = mod(aEdge + 1, 3);
		const integer a3 = mod(aEdge + 2, 3);

		const integer b1 = bEdge;
		const integer b2 = mod(bEdge + 1, 3);
		const integer b3 = mod(bEdge + 2, 3);

		//   +-----+
		//   | D  /|
		//   |  /  |
		//   |/  C |
		//   +-----+

		//  b1     b3    b3
		//   +-----+     +
		//   | D  /     /|
		//   |  /     /  |
		//   |/     /  C |
		//   +     +-----+
		//  a3    a3     a1

		SimplexBody* cSimplex = allocateSimplex();
		cSimplex->setVertex(
			makeTuple(
			aSimplex->vertex(a1),
			bSimplex->vertex(b3),
			aSimplex->vertex(a3)));
		link(cSimplex, 0, bSimplex->pair(b2), bSimplex->pairIndex(b2));
		link(cSimplex, 2, aSimplex->pair(a3), aSimplex->pairIndex(a3));

		SimplexBody* dSimplex = allocateSimplex();
		dSimplex->setVertex(
			makeTuple(
			bSimplex->vertex(b1),
			aSimplex->vertex(a3),
			bSimplex->vertex(b3)));
		link(dSimplex, 0, aSimplex->pair(a2), aSimplex->pairIndex(a2));
		link(dSimplex, 2, bSimplex->pair(b3), bSimplex->pairIndex(b3));

		link(cSimplex, 1, dSimplex, 1);

		aSimplex->setChild(
			makeTuple(
			cSimplex,
			dSimplex,
			(SimplexBody*)0));

		bSimplex->setChild(
			makeTuple(
			cSimplex,
			dSimplex,
			(SimplexBody*)0));

		simplexSet_.erase(Simplex(aSimplex));
		simplexSet_.erase(Simplex(bSimplex));

		simplexSet_.insert(Simplex(cSimplex));
		simplexSet_.insert(Simplex(dSimplex));

		legalizeEdge(cSimplex, 0);
		legalizeEdge(dSimplex, 2);
	}

	template <typename Real, typename DataPolicy>
	void DelaunayTesselation<Real, DataPolicy>::legalizeEdge(
		SimplexBody* simplex,
		integer edge)
	{
		SimplexBody* pairSimplex = simplex->pair(edge);
		if (!pairSimplex)
		{
			return;
		}

		using Pastel::Simplex;

		const integer pairIndex = simplex->pairIndex(edge);

		VertexBody* pairVertex = pairSimplex->vertex(mod(pairIndex + 2, 3));

		const Sphere<2, Real> illegalSphere =
			circumscribedSphere(
			Simplex<2, Real, 2>(
			simplex->vertex(0)->position(),
			simplex->vertex(1)->position(),
			simplex->vertex(2)->position()));

		if (overlaps(illegalSphere, pairVertex->position()))
		{
			// Illegal edge, flip it.

			flipEdge(simplex, edge,
				pairSimplex, pairIndex);
		}
	}


	template <typename Real, typename DataPolicy>
	typename DelaunayTesselation<Real, DataPolicy>::SimplexBody*
		DelaunayTesselation<Real, DataPolicy>::allocateSimplex()
	{
		integer rollBackIndex = 0;
		SimplexBody* simplex = 0;

		try
		{
			simplex = (SimplexBody*)simplexAllocator_.allocate();
			++rollBackIndex;

			new(simplex) SimplexBody;
			++rollBackIndex;

			if (simplex->data())
			{
				new(simplex->data()) SimplexData;
			}
			++rollBackIndex;

			constructSimplex(simplex->data());
			++rollBackIndex;
		}
		catch(...)
		{
			switch(rollBackIndex)
			{
			case 3:
				if (simplex->data())
				{
					simplex->data()->~SimplexData();
				}
				// Fall-through
			case 2:
				simplex->~SimplexBody();
				// Fall-through
			case 1:
				simplexAllocator_.deallocate(simplex);
				break;
			};

			throw;
		};

		return simplex;
	}

	template <typename Real, typename DataPolicy>
	typename DelaunayTesselation<Real, DataPolicy>::VertexBody*
		DelaunayTesselation<Real, DataPolicy>::allocateVertex(
		const Point<2, Real>& position)
	{
		integer rollBackIndex = 0;
		VertexBody* vertex = 0;

		try
		{
			vertex = (VertexBody*)vertexAllocator_.allocate();
			++rollBackIndex;

			new(vertex) VertexBody(position);
			++rollBackIndex;

			if (vertex->data())
			{
				new(vertex->data()) VertexData;
			}
			++rollBackIndex;

			constructVertex(vertex->data());
			++rollBackIndex;

			vertexSet_.insert(Vertex(vertex));
			++rollBackIndex;
		}
		catch(...)
		{
			switch(rollBackIndex)
			{
			case 4:
				destructVertex(vertex->data());
				// Fall-through
			case 3:
				if (vertex->data())
				{
					vertex->data()->~VertexData();
				}
				// Fall-through
			case 2:
				vertex->~VertexBody();
				// Fall-through
			case 1:
				vertexAllocator_.deallocate(vertex);
				break;
			};

			throw;
		};

		return vertex;
	}

}

#endif
