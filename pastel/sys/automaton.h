// Description: Finite-state automaton

#ifndef PASTEL_AUTOMATON_H
#define PASTEL_AUTOMATON_H

#include "pastel/sys/automaton_fwd.h"
#include "pastel/sys/automaton_transition.h"

namespace Pastel
{

	//! Finite-state automaton
	template <
		typename Symbol, 
		typename StateData = void, 
		typename TransitionData = void>
	class Automaton
		: public Automaton_Fwd<Symbol, StateData, TransitionData>
	{
	public:
		//! Constructs an empty automaton.
		Automaton()
			: graph_()
			, searchSet_()
			, rejectGraph_()
			, startState_(graph_.vertexEnd())
			, rejectState_(rejectGraph_.vertexEnd())
		{
			construct();
		}

		//! Move-constructs from another automaton.
		Automaton(Automaton&& that)
			: graph_()
			, searchSet_()
			, rejectGraph_()
			, startState_(graph_.end())
			, rejectState_(rejectGraph_.vertexEnd())
		{
			construct();
			swap(that);
		}

		//! Move-assigns from another automaton.
		Automaton& operator=(Automaton that)
		{
			swap(that);
			return *this;
		}

		//! Removes all states and transitions.
		void clear()
		{
			// The rejectGraph and rejectState
			// are kept the same.

			graph_.clear();
			searchSet_.clear();
			startState_ = rejectState_;
		}

		//! Removes all transitions.
		void clearTransitions()
		{
			searchSet_.clear();
		}

		//! Swaps two parse-tables.
		void swap(Automaton& that)
		{
			using std::swap;

			graph_.swap(that.graph_);
			searchSet_.swap(that.searchSet_);
			rejectGraph_.swap(that.rejectGraph_);
			swap(startState_, that.startState_);
			swap(rejectState_, that.rejectState_);
		}

		// States

		//! Adds a new state.
		State_Iterator addState(StateData_Class stateData = StateData_Class())
		{
			auto iter = graph_.addVertex(std::move(stateData));
			return iter;
		}

		//! Removes a state.
		State_Iterator removeState(State_ConstIterator state)
		{
			// Remove all transitions that are 
			// incoming or outgoing at this vertex.
			while(state->incidentEdges() > 0)
			{
				removeTransition(
					state->cbegin()->edge());
			}

			// Remove the vertex.
			return graph_.removeVertex(state);
		}

		//! Removes constness from a state-iterator.
		State_Iterator cast(State_ConstIterator state)
		{
			return graph_.cast(state);
		}

		//! Sets the start-state.
		void setStartState(
			State_ConstIterator state)
		{
			startState_ = cast(state);
		}

		//! Returns the start-state.
		State_Iterator startState()
		{
			return startState_;
		}

		//! Returns the start-state.
		State_ConstIterator startState() const
		{
			return startState_;
		}

		//! Returns the reject-state.
		State_Iterator rejectState()
		{
			return rejectState_;
		}

		//! Returns the reject-state.
		State_ConstIterator rejectState() const
		{
			return rejectState_;
		}

		//! Returns the first iterator of the state-set.
		State_Iterator stateBegin()
		{
			return graph_.vertexBegin();
		}

		//! Returns the first iterator of the state-set.
		State_ConstIterator cStateBegin() const
		{
			return graph_.cVertexBegin();
		}

		//! Returns the end-iterator of the state-set.
		State_Iterator stateEnd()
		{
			return graph_.vertexEnd();
		}

		//! Returns the end-iterator of the state-set.
		State_ConstIterator cStateEnd() const
		{
			return graph_.cVertexEnd();
		}

		//! Returns the number of states.
		integer states() const
		{
			return graph_.vertices();
		}

		// Transitions

		//! Adds a new transition.
		Transition_Iterator addTransition(
			State_ConstIterator fromState,
			Symbol symbol,
			State_ConstIterator toState,
			TransitionData_Class transitionData = TransitionData_Class())
		{
			/*
			if (searchSet_.count(
				StateSymbol(fromState, symbol)))
			{
				return graph_.cEdgeEnd();
			}
			*/

			auto transition = graph_.addEdge(
				fromState, toState, 
				Transition(symbol, std::move(transitionData)));

			searchSet_.emplace(
				std::make_pair(StateSymbol(fromState, symbol), 
				transition));

			return transition;
		}

		//! Removes a transition.
		void removeTransition(
			const Transition_ConstIterator& transition)
		{
			searchSet_.erase(
				StateSymbol(transition->from(), *transition));
			graph_.erase(transition);
		}

		//! Searches for the given transition.
		Transition_Iterator findTransition(
			const StateSymbol& transition)
		{
			auto iter = searchSet_.find(transition);
			return iter->second;
		}

		//! Searches for the given transition.
		Transition_ConstIterator findTransition(
			const StateSymbol& transition) const
		{
			auto iter = searchSet_.find(transition);
			return iter->second;
		}

		//! Tests for the existence of a transition.
		bool existsTransition(
			const StateSymbol& transition) const
		{
			return findTransition(transition) != cTransitionEnd();
		}

		//! Returns the first iterator of the transition-set.
		Transition_Iterator transitionBegin()
		{
			return graph_.edgeBegin();
		}

		//! Returns the first iterator of the transition-set.
		Transition_ConstIterator cTransitionBegin() const
		{
			return graph_.cEdgeBegin();
		}

		//! Returns the end-iterator of the transition-set.
		Transition_Iterator transitionEnd()
		{
			return graph_.edgeEnd();
		}

		//! Returns the end-iterator of the transition-set.
		Transition_ConstIterator cTransitionEnd() const
		{
			return graph_.cEdgeEnd();
		}

		//! Returns the first iterator of the transition-set of the state.
		Transition_Iterator transitionBegin(
			const State_ConstIterator& state)
		{
			return state->cbegin();
		}

		//! Returns the first iterator of the transition-set of the state.
		Transition_ConstIterator cTransitionBegin(
			const State_ConstIterator& state) const
		{
			return state->cbegin();
		}

		//! Returns the end-iterator of the transition-set of the state.
		Transition_Iterator transitionEnd(
			const State_ConstIterator& state)
		{
			return state->cend();
		}

		//! Returns the end-iterator of the transition-set of the state.
		Transition_ConstIterator cTransitionEnd(
			const State_ConstIterator& state) const
		{
			return state->cend();
		}

		//! Returns the number of transitions.
		integer transitions() const
		{
			return graph_.edges();
		}

	private:
		// Deleted
		Automaton(const Automaton& that) PASTEL_DELETE;

		//! Initialization common to all constructors.
		/*!
		This function creates the reject state.
		*/
		void construct()
		{
			// Construct the reject state.
			rejectState_ = rejectGraph_.addVertex();

			// The start-state is the reject-state
			// in the beginning.
			startState_ = rejectState_;
		}

		//! The underlying graph.
		Graph graph_;
		
		//! Fast associative searches of transitions.
		/*!
		Given a transition (q, X), where q is a state
		and X is a symbol, this multi-map contains all the
		edges from state q with symbol X. There might be
		many such edges if the automaton is non-deterministic.
		However, there must not be multiple transitions between
		two states with the same symbol; the relation from
		transitions to states is a partial function.
		*/
		SearchSet searchSet_;

		//! Storage for the reject state.
		/*!
		The reject-state is stored here rather than in 
		the 'graph' to make it simpler to preserve the 
		identity of the reject state.
		*/
		Graph rejectGraph_;

		//! The start state.
		State_Iterator startState_;

		//! The reject state.
		/*!
		The reject state ensures that each transition in the automaton 
		always has a dereferencable state. This gets rid of boundary
		cases in the code.
		*/
		State_Iterator rejectState_;
	};

}

namespace Pastel
{

	//! Returns the target-state of a state-symbol transition.
	/*!
	If the transition does not exists, the 
	automaton.rejectState() is returned instead.
	*/
	template <typename State, typename Symbol>
	auto transition(
		const Automaton<State, Symbol>& automaton,
		typename Automaton<State, Symbol>::State_ConstIterator state,
		Symbol symbol)
		-> typename Automaton<State, Symbol>::State_ConstIterator;

	//! Returns the target-state of a state-symbol-list transition.
	/*!
	If some of the transitions does not exists, the 
	automaton.rejectState() is returned instead.
	*/
	template <typename State, typename Symbol,
		typename Symbol_ConstRange>
	auto transition(
		const Automaton<State, Symbol>& automaton,
		typename Automaton<State, Symbol>::State_ConstIterator state,
		Symbol_ConstRange symbolSet)
		-> typename Automaton<State, Symbol>::State_ConstIterator;

}

#include "pastel/sys/automaton.hpp"

#endif
