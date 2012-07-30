// Description: Finite-state automaton

#ifndef PASTEL_AUTOMATON_H
#define PASTEL_AUTOMATON_H

#include "pastel/sys/automaton_fwd.h"
#include "pastel/sys/automaton_label.h"

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
			, startState_(rejectGraph_.vertexEnd())
			, rejectState_(rejectGraph_.vertexEnd())
			, emptyBranchSet_()
			, finalSet_()
		{
			construct();
		}

		//! Move-constructs from another automaton.
		Automaton(Automaton&& that)
			: graph_()
			, searchSet_()
			, rejectGraph_()
			, startState_(rejectGraph_.vertexEnd())
			, rejectState_(rejectGraph_.vertexEnd())
			, emptyBranchSet_()
			, finalSet_()
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
			// The rejectGraph, rejectState, 
			// and emptyBranchSet are kept the same.

			graph_.clear();
			searchSet_.clear();
			startState_ = rejectState_;
			finalSet_.clear();
		}

		//! Removes all transitions.
		void clearTransitions()
		{
			graph_.clearEdges();
			searchSet_.clear();
		}

		//! Swaps two automata.
		void swap(Automaton& that)
		{
			using std::swap;

			graph_.swap(that.graph_);
			searchSet_.swap(that.searchSet_);
			rejectGraph_.swap(that.rejectGraph_);
			swap(startState_, that.startState_);
			swap(rejectState_, that.rejectState_);
			emptyBranchSet_.swap(that.emptyBranchSet_);
			finalSet_.swap(that.finalSet_);
		}

		// States

		//! Adds a new state.
		State_Iterator addState(StateData_Class stateData = StateData_Class())
		{
			auto iter = graph_.addVertex(std::move(stateData));
			return iter;
		}

		//! Removes a state.
		State_Iterator removeState(
			const State_ConstIterator& state)
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
		State_Iterator cast(
			const State_ConstIterator& state)
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

		// Final states

		//! Makes the given state final.
		void addFinal(
			const State_ConstIterator& state)
		{
			finalSet_.insert(state);
		}
		
		//! Removes finality from the given state.
		void removeFinal(
			const State_ConstIterator& state)
		{
			finalSet_.erase(state);
		}

		//! Returns whether the state is final.
		bool final(const State_ConstIterator& state) const
		{
			return finalSet_.count(state);
		}

		Final_Iterator finalBegin()
		{
			return finalSet_.begin();
		}

		Final_ConstIterator cFinalBegin() const
		{
			return finalSet_.cbegin();
		}

		Final_Iterator finalEnd()
		{
			return finalSet_.end();
		}

		Final_ConstIterator cFinalEnd() const
		{
			return finalSet_.cend();
		}

		integer finalStates() const
		{
			return finalSet_.size();
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
			const State_ConstIterator& fromState,
			const Symbol& symbol,
			const State_ConstIterator& toState,
			TransitionData_Class transitionData = TransitionData_Class())
		{
			Transition_Iterator transition =
				findTransition(fromState, symbol, toState);
			if (transition != graph_.edgeEnd())
			{
				// The transition already exists.
				return transition;
			}

			integer rollback = 0;

			transition = graph_.addEdge(
				fromState, toState, 
				Label(symbol, std::move(transitionData)));
			++rollback;

			Search_Iterator search;
			try
			{
				// Either the branch already exists, and then it
				// is returned, or it will be created. 
				search = searchSet_.emplace(
					std::make_pair(
					StateSymbol(fromState, symbol), 
					BranchSet())).first;
				++rollback;
										
				BranchSet& branch = search->second;

				// Add the transition to the branch.
				branch.insert(
					std::make_pair(toState, transition));
			}
			catch(...)
			{
				switch(rollback)
				{
				case 2:
					searchSet_.erase(search);
					// Fall-through
				case 1:
					graph_.removeEdge(transition);
					break;
				};
				throw;
			}

			return transition;
		}

		//! Removes a transition.
		Transition_Iterator removeTransition(
			const Transition_ConstIterator& transition)
		{
			searchSet_.erase(
				StateSymbol(transition->from(), transition->symbol()));
			
			return graph_.removeEdge(transition);
		}

		//! Searches for the given transition.
		Transition_Iterator findTransition(
			const State_ConstIterator& fromState,
			const Symbol& symbol)
		{
			return cast(((const Automaton&)*this).findTransition(
				fromState, symbol));
		}

		//! Searches for a given transition.
		/*!
		Note:
		If the automaton is non-deterministic,
		there may be many transitions for a given
		from-state and symbol. In this case this
		function returns one of those transitions.
		If you need all transitions, see the
		findTransitions() function.
		*/
		Transition_ConstIterator findTransition(
			const State_ConstIterator& fromState,
			const Symbol& symbol) const
		{
			// Find all branches.
			auto range = findTransitions(
				fromState, symbol);

			if (range.first == range.second)
			{
				// There are no branches.
				return graph_.cEdgeEnd();
			}

			// Return the first branch.
			return *range.first;
		}

		//! Searches for the given transition.
		Transition_Iterator findTransition(
			const State_ConstIterator& fromState,
			const Symbol& symbol,
			const State_ConstIterator& toState)
		{
			return cast(((const Automaton&)*this).findTransition(
				fromState, symbol, toState));
		}

		//! Searches for the given transition.
		/*!
		Note:
		By the definition of a finite-state automaton,
		there can be at most one transition from a state
		to another with a given symbol.
		*/
		Transition_ConstIterator findTransition(
			const State_ConstIterator& fromState,
			const Symbol& symbol,
			const State_ConstIterator& toState) const
		{
			// See if this state-symbol pair has any
			// branches.
			Search_ConstIterator search = 
				searchSet_.find(
				StateSymbol(fromState, symbol));
			if (search == searchSet_.cend())
			{
				// There are no branches.
				return graph_.cEdgeEnd();
			}
			
			// Obtain the branch set.
			const BranchSet& branchSet = 
				search->second;
			
			// Search the branch set for a branch
			// with the given to-state.
			Branch_ConstIterator branch =
				branchSet.find(toState);
			if (branch == branchSet.cend())
			{
				// None of the branches end up in 
				// the to-state.
				return graph_.cEdgeEnd();
			}

			// The branch was found.
			return branch->second;
		}

		//! Returns the branches of a given state-symbol pair.
		/*!
		returns:
		A set of Transition_ConstIterator's by an iterator range.

		A set of transitions sharing the same from-state
		and symbol is called a branch set, with the
		transitions in it called branches (since in the 
		non-deterministic case the computation branches).

		Note:
		If the automaton is deterministic, then there is at most
		one transition from a given state and symbol. In this
		case it may be more convenient to use the findTransition() 
		function.
		*/
		std::pair<Branch_ConstIterator, Branch_ConstIterator>
			findTransitions(
			const State_ConstIterator& fromState,
			const Symbol& symbol) const
		{
			auto search = searchSet_.find(
				StateSymbol(fromState, symbol));
			if (search == searchSet_.cend())
			{
				return std::make_pair(
					emptyBranchSet_.cbegin(),
					emptyBranchSet_.cend());
			}

			const BranchSet& branch = search.second;
			
			return std::make_pair(
				branch.cbegin(), 
				branch.cend());
		}

		//! Tests for the existence of a transition.
		bool existsTransition(
			const State_ConstIterator& state,
			const Symbol& symbol) const
		{
			return findTransition(state, symbol) != cTransitionEnd();
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
			return state->begin();
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
			return state->end();
		}

		//! Returns the end-iterator of the transition-set of the state.
		Transition_ConstIterator cTransitionEnd(
			const State_ConstIterator& state) const
		{
			return state->cend();
		}

		//! Removes constness from a transition-iterator.
		Transition_Iterator cast(
			const Transition_ConstIterator& transition)
		{
			return graph_.cast(transition);
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
		Given a pair (q, X), where q is a state and X is a symbol, 
		this map returns the set of transitions from state q with 
		symbol X. There might be many such transitions if the automaton
		is non-deterministic. However, by the definition of a 
		finite-state automaton, there will not be multiple transitions 
		between two states with the same symbol; the relation from 
		state-symbol pairs to states is a partial function.
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

		//! The set of final states.
		FinalSet finalSet_;

		//! The reject state.
		/*!
		The reject state ensures that each transition in the automaton 
		always has a dereferencable state. This gets rid of boundary
		cases in the code.
		*/
		State_Iterator rejectState_;

		//! An empty set of transitions.
		BranchSet emptyBranchSet_;
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
