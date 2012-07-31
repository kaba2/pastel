// Description: Finite-state automaton

#ifndef PASTEL_AUTOMATON_H
#define PASTEL_AUTOMATON_H

#include "pastel/sys/automaton_fwd.h"
#include "pastel/sys/automaton_state_label.h"
#include "pastel/sys/automaton_transition_label.h"

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
			, startSet_()
			, finalSet_()
		{
		}

		//! Move-constructs from another automaton.
		Automaton(Automaton&& that)
			: graph_()
			, startSet_()
			, finalSet_()
		{
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
			startSet_.clear();
			finalSet_.clear();
		}

		//! Removes all transitions.
		void clearTransitions()
		{
			graph_.clearEdges();
		}

		//! Removes all start-state marks.
		void clearStart()
		{
			while(startStates() > 0)
			{
				removeStart(startSet_.front());
			}
		}

		//! Removes all final-state marks.
		void clearFinal()
		{
			while(finalStates() > 0)
			{
				removeFinal(finalSet_.front());
			}
		}

		//! Swaps two automata.
		void swap(Automaton& that)
		{
			using std::swap;

			graph_.swap(that.graph_);
			startSet_.swap(that.startSet_);
			finalSet_.swap(that.finalSet_);
		}

		// States

		//! Adds a new state.
		State_Iterator addState(StateData_Class stateData = StateData_Class())
		{
			auto iter = graph_.addVertex(
				StateLabel(std::move(stateData)));

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

			if (state->final())
			{
				// Remove designation of a final state.
				removeFinal(state);
			}

			if (state->start())
			{
				// Remove designation of a start state.
				removeStart(state);
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

		// Start states

		//! Adds the given state to the start states.
		void addStart(
			const State_ConstIterator& state)
		{
			if (!state->start())
			{
				cast(state)->startPosition_ = 
					startSet_.insert(
					startSet_.cend(), state);
			
				cast(state)->setStart(true);
			}
		}
		
		//! Removes the given state from the start states.
		void removeStart(
			const State_ConstIterator& state)
		{
			if (state->start())
			{
				startSet_.erase(state->startPosition_);
				cast(state)->setStart(false);
			}
		}

		Start_Iterator startBegin()
		{
			return startSet_.begin();
		}

		Start_ConstIterator cStartBegin() const
		{
			return startSet_.cbegin();
		}

		Start_Iterator startEnd()
		{
			return startSet_.end();
		}

		Start_ConstIterator cStartEnd() const
		{
			return startSet_.cend();
		}

		//! Returns the number of start states.
		integer startStates() const
		{
			return startSet_.size();
		}

		// Final states

		//! Makes the given state final.
		void addFinal(
			const State_ConstIterator& state)
		{
			if (!state->final())
			{
				cast(state)->finalPosition_ = 
					finalSet_.insert(
					finalSet_.cend(), state);
			
				cast(state)->setFinal(true);
			}
		}
		
		//! Removes finality from the given state.
		void removeFinal(
			const State_ConstIterator& state)
		{
			if (state->final())
			{
				finalSet_.erase(state->finalPosition_);
				cast(state)->finalPosition_ = finalSet_.end();
				cast(state)->setFinal(false);
			}
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

		//! Returns the number of final states.
		integer finalStates() const
		{
			return finalSet_.size();
		}

		// All states

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
			return graph_.addEdge(
				fromState, toState, 
				TransitionLabel(symbol, std::move(transitionData)));
		}

		//! Removes a transition.
		Transition_Iterator removeTransition(
			const Transition_ConstIterator& transition)
		{
			return graph_.removeEdge(transition);
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

		// Global operations

		void merge(Automaton& that)
		{
			// Bring in the graph.
			graph_.merge(that.graph_);
			
			// Bring in the start states.
			startSet_.splice(
				startSet_.cend(),
				that.startSet_);

			// Bring in the final states.
			finalSet_.splice(
				finalSet_.cend(),
				that.finalSet_);
		}
		
	private:
		// Deleted
		Automaton(const Automaton& that) PASTEL_DELETE;

		//! The underlying graph.
		Graph graph_;
		
		//! The set of start states.
		StartSet startSet_;

		//! The set of final states.
		FinalSet finalSet_;
	};

}

#endif
