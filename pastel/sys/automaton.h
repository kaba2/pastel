// Description: Finite-state automaton

#ifndef PASTEL_AUTOMATON_H
#define PASTEL_AUTOMATON_H

#include "pastel/sys/automaton_concepts.h"

#include "pastel/sys/automaton_fwd.h"
#include "pastel/sys/automaton_state_label.h"
#include "pastel/sys/automaton_transition_label.h"
#include "pastel/sys/optional.h"

#include <unordered_map>

namespace Pastel
{

	//! Finite-state automaton
	template <
		typename Symbol, 
		typename StateData = void, 
		typename TransitionData = void,
		typename Customization = Automaton_Concepts::Customization<Symbol, StateData, TransitionData>>
	class Automaton
		: public Customization
	{
	public:
		typedef Automaton_Fwd<Symbol, StateData, TransitionData> Fwd;

		PASTEL_FWD(StateLabel);
		PASTEL_FWD(TransitionLabel);
		PASTEL_FWD(Graph);
		
		PASTEL_FWD(State_Iterator);
		PASTEL_FWD(State_ConstIterator);
		PASTEL_FWD(StateData_Class);
		
		PASTEL_FWD(Transition_Iterator);
		PASTEL_FWD(Transition_ConstIterator);
		PASTEL_FWD(TransitionData_Class);
		
		PASTEL_FWD(StartSet);
		PASTEL_FWD(Start_Iterator);
		PASTEL_FWD(Start_ConstIterator);
		
		PASTEL_FWD(FinalSet);
		PASTEL_FWD(Final_Iterator);
		PASTEL_FWD(Final_ConstIterator);

		//! Constructs an empty automaton.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		Automaton()
			: graph_()
			, startSet_()
			, finalSet_()
		{
		}

		//! Copy-constructs from another automaton.
		/*!
		Time complexity: 
		O(that.states() + that.transitions())

		Exception safety: 
		strong
		*/
		Automaton(const Automaton& that)
			: graph_()
			, startSet_()
			, finalSet_()
		{
			// This will map states from 'that' automaton
			// to this automaton.
			std::unordered_map<
				State_ConstIterator,
				State_ConstIterator,
				IteratorAddress_Hash> stateMap;

			// Create the states and the state-mapping.
			for (auto state = that.cStateBegin();
				state != that.cStateEnd();
				++state)
			{
				stateMap[state] = addState(*state);
			}
			
			// Create the transitions.
			for (auto transition = that.cTransitionBegin();
				transition != that.cTransitionEnd();
				++transition)
			{
				addTransition(
					stateMap[transition->from()],
					transition->symbol(),
					stateMap[transition->to()],
					*transition);
			}

			// Add the start states.
			std::for_each(
				that.cStartBegin(),
				that.cStartEnd(),
				[&](const State_ConstIterator& state)
			{
				addStart(stateMap[state]);
			});

			// Add the final states.
			std::for_each(
				that.cFinalBegin(),
				that.cFinalEnd(),
				[&](const State_ConstIterator& state)
			{
				addFinal(stateMap[state]);
			});
		}

		//! Move-constructs from another automaton.
		/*!
		Time complexity: 
		O(1)

		Exception safety: 
		strong
		*/
		Automaton(Automaton&& that)
			: graph_()
			, startSet_()
			, finalSet_()
		{
			swap(that);
		}

		//! Move-assigns from another automaton.
		/*!
		Time complexity: 
		Move/copy-construction

		Exception safety: 
		Move/copy-construction
		*/
		Automaton& operator=(Automaton that)
		{
			swap(that);
			return *this;
		}

		//! Removes all states and transitions.
		/*!
		Time complexity:
		O(n + m) + customization,
		where 
		n is the number of states, and
		m is the number of transitions.

		Exception safety:
		nothrow
		*/
		void clear()
		{
			onClear();

			graph_.clear();
			startSet_.clear();
			finalSet_.clear();
		}

		//! Removes all transitions.
		/*!
		Time complexity:
		O(m) + customization,
		where
		m is the number of transitions.

		Exception safety:
		nothrow
		*/
		void clearTransitions()
		{
			onClearTransitions();

			graph_.clearEdges();
		}

		//! Removes all start-state marks.
		/*!
		Time complexity:
		O(k) + customization,
		where
		k is the number of start-states.

		Exception safety:
		nothrow
		*/
		void clearStart()
		{
			onClearStart();

			while(startStates() > 0)
			{
				removeStart(startSet_.front());
			}
		}

		//! Removes all final-state marks.
		/*!
		Time complexity:
		O(k) + customization
		where
		k is the number of start-states.

		Exception safety:
		nothrow
		*/
		void clearFinal()
		{
			onClearFinal();

			while(finalStates() > 0)
			{
				removeFinal(finalSet_.front());
			}
		}

		//! Swaps two automata.
		/*!
		Time complexity:
		O(1) + customization

		Exception safety:
		nothrow
		*/
		void swap(Automaton& that)
		{
			Customization::swap((Customization&)that);
			graph_.swap(that.graph_);
			startSet_.swap(that.startSet_);
			finalSet_.swap(that.finalSet_);
		}

		// States

		//! Adds a new state.
		/*!
		Time complexity:
		O(1) + customization

		Exception safety:
		strong + customization
		*/
		State_Iterator addState(StateData_Class stateData = StateData_Class())
		{
			State_Iterator state = graph_.addVertex(
				StateLabel(std::move(stateData)));

			try
			{
				onAddState(state);
			}
			catch(...)
			{
				graph_.removeVertex(state);
				throw;
			}

			return state;
		}

		//! Removes a state.
		/*!
		Time complexity:
		O(1) + customization

		Exception safety:
		nothrow
		*/
		State_Iterator removeState(
			const State_ConstIterator& state)
		{
			onRemoveState(state);

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
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		State_Iterator cast(
			const State_ConstIterator& state)
		{
			return graph_.cast(state);
		}

		// Start states

		//! Adds the given state to the start states.
		/*!
		Time complexity: 
		O(1) + customization
		
		Exception safety: 
		strong + customization
		*/
		void addStart(
			const State_ConstIterator& state)
		{
			if (state->start())
			{
				return;
			}
			
			Start_Iterator start = startSet_.insert(
				startSet_.cend(), state);

			cast(state)->startPosition_ = start; 
			cast(state)->setStart(true);

			try
			{
				onAddStart(state);
			}
			catch(...)
			{
				cast(state)->startPosition_ = startSet_.end(); 
				cast(state)->setStart(false);
				startSet_.erase(start);
				throw;
			}
		}
		
		//! Removes the given state from the start-states.
		/*!
		Time complexity:
		O(1) + customization

		Exception safety:
		nothrow
		*/
		Start_Iterator removeStart(
			const State_ConstIterator& state)
		{
			if (!state->start())
			{
				return state->startPosition_;
			}
			
			onRemoveStart(state);

			Start_Iterator next =
				startSet_.erase(state->startPosition_);
			cast(state)->setStart(false);

			return next;
		}

		//! Returns the first iterator of the start-states.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Start_Iterator startBegin()
		{
			return startSet_.begin();
		}

		//! Returns the first iterator of the start-states.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Start_ConstIterator cStartBegin() const
		{
			return startSet_.cbegin();
		}

		//! Returns the end-iterator of the start-states.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Start_Iterator startEnd()
		{
			return startSet_.end();
		}

		//! Returns the end-iterator of the start-states.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Start_ConstIterator cStartEnd() const
		{
			return startSet_.cend();
		}

		//! Returns the number of start states.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer startStates() const
		{
			return startSet_.size();
		}

		// Final states

		//! Makes the given state final.
		/*!
		Time complexity: 
		O(1) + customization
		
		Exception safety: 
		strong + customization
		*/
		void addFinal(
			const State_ConstIterator& state)
		{
			if (state->final())
			{
				return;
			}

			Final_Iterator final = finalSet_.insert(
				finalSet_.cend(), state);

			cast(state)->finalPosition_ = final;
			cast(state)->setFinal(true);

			try
			{
				onAddFinal(state);
			}
			catch(...)
			{
				cast(state)->finalPosition_ = finalSet_.end();
				cast(state)->setFinal(false);
				finalSet_.erase(final);
				throw;
			}
		}
		
		//! Removes finality from the given state.
		/*!
		Time complexity: 
		O(1) + customization
		
		Exception safety: 
		nothrow
		*/
		Final_Iterator removeFinal(
			const State_ConstIterator& state)
		{
			if (!state->final())
			{
				return state->finalPosition_;
			}

			onRemoveFinal(state);

			Final_Iterator next = 
				finalSet_.erase(state->finalPosition_);
			cast(state)->finalPosition_ = finalSet_.end();
			cast(state)->setFinal(false);

			return next;
		}

		//! Returns the first iterator of the final states.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Final_Iterator finalBegin()
		{
			return finalSet_.begin();
		}

		//! Returns the first iterator of the final states.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Final_ConstIterator cFinalBegin() const
		{
			return finalSet_.cbegin();
		}

		//! Returns the end-iterator of the final states.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Final_Iterator finalEnd()
		{
			return finalSet_.end();
		}

		//! Returns the end-iterator of the final states.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Final_ConstIterator cFinalEnd() const
		{
			return finalSet_.cend();
		}

		//! Returns the number of final states.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer finalStates() const
		{
			return finalSet_.size();
		}

		// All states

		//! Returns the first iterator of the state-set.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		State_Iterator stateBegin()
		{
			return graph_.vertexBegin();
		}

		//! Returns the first iterator of the state-set.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		State_ConstIterator cStateBegin() const
		{
			return graph_.cVertexBegin();
		}

		//! Returns the end-iterator of the state-set.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		State_Iterator stateEnd()
		{
			return graph_.vertexEnd();
		}

		//! Returns the end-iterator of the state-set.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		State_ConstIterator cStateEnd() const
		{
			return graph_.cVertexEnd();
		}

		//! Returns the number of states.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer states() const
		{
			return graph_.vertices();
		}

		// Transitions

		//! Adds a new transition.
		/*!
		Time complexity: 
		O(1) + customization

		Exception safety: 
		strong + customization
		*/
		Transition_Iterator addTransition(
			const State_ConstIterator& fromState,
			Optional<Symbol> symbol,
			const State_ConstIterator& toState,
			TransitionData_Class transitionData = TransitionData_Class())
		{
			if (!canAddTransition(fromState, symbol, toState))
			{
				return transitionEnd();
			}

			Transition_Iterator transition = graph_.addEdge(
				fromState, toState, 
				TransitionLabel(std::move(symbol), std::move(transitionData)));

			try
			{
				onAddTransition(transition);
			}
			catch(...)
			{
				graph_.removeEdge(transition);
				throw;
			}

			return transition;
		}

		//! Removes a transition.
		/*!
		Time complexity: 
		O(1) + customization

		Exception safety: 
		nothrow
		*/
		Transition_Iterator removeTransition(
			const Transition_ConstIterator& transition)
		{
			onRemoveTransition(transition);

			return graph_.removeEdge(transition);
		}

		//! Returns the first iterator of the transition-set.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Transition_Iterator transitionBegin()
		{
			return graph_.edgeBegin();
		}

		//! Returns the first iterator of the transition-set.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Transition_ConstIterator cTransitionBegin() const
		{
			return graph_.cEdgeBegin();
		}

		//! Returns the end-iterator of the transition-set.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Transition_Iterator transitionEnd()
		{
			return graph_.edgeEnd();
		}

		//! Returns the end-iterator of the transition-set.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Transition_ConstIterator cTransitionEnd() const
		{
			return graph_.cEdgeEnd();
		}

		//! Returns the first iterator of the transition-set of the state.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Transition_Iterator transitionBegin(
			const State_ConstIterator& state)
		{
			return state->begin();
		}

		//! Returns the first iterator of the transition-set of the state.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Transition_ConstIterator cTransitionBegin(
			const State_ConstIterator& state) const
		{
			return state->cbegin();
		}

		//! Returns the end-iterator of the transition-set of the state.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Transition_Iterator transitionEnd(
			const State_ConstIterator& state)
		{
			return state->end();
		}

		//! Returns the end-iterator of the transition-set of the state.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Transition_ConstIterator cTransitionEnd(
			const State_ConstIterator& state) const
		{
			return state->cend();
		}

		//! Removes constness from a transition-iterator.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Transition_Iterator cast(
			const Transition_ConstIterator& transition)
		{
			return graph_.cast(transition);
		}

		//! Returns the number of transitions.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer transitions() const
		{
			return graph_.edges();
		}

		// Global operations

		//! Merges two automata together.
		/*!
		Time complexity: 
		O(1) + customization

		Exception safety: 
		nothrow + customization
		*/
		void merge(Automaton& that)
		{
			onMerge(that);

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
		//! The underlying graph.
		Graph graph_;
		
		//! The set of start states.
		StartSet startSet_;

		//! The set of final states.
		FinalSet finalSet_;
	};

}

#include <ostream>
#include <unordered_map>

namespace Pastel
{

	template <
		typename Symbol, 
		typename StateData, 
		typename TransitionData,
		typename Customization>
	std::ostream& operator<<(
		std::ostream& stream,
		const Automaton<Symbol, StateData, TransitionData, Customization>& automaton)
	{
		typedef Automaton<Symbol, StateData, TransitionData, Customization>
			Automaton;
		typedef typename Automaton::State_ConstIterator
			State_ConstIterator;
		typedef typename Automaton::Transition_ConstIterator
			Transition_ConstIterator;

		stream << automaton.states() << " states, "
			<< automaton.transitions() << " transitions." 
			<< std::endl << std::endl;

		std::unordered_map<State_ConstIterator, integer,
			IteratorAddress_Hash> stateMap;

		integer stateId = 0;
		for (auto state = automaton.cStateBegin();
			state != automaton.cStateEnd();
			++state)
		{
			stateMap[state] = stateId;
			++stateId;
		}

		stream << "Transitions:" << std::endl
			<< std::endl;

		for (auto transition = automaton.cTransitionBegin();
			transition != automaton.cTransitionEnd();
			++transition)
		{
			stream << "(" << stateMap[transition->from()]
				<< ", " << transition->symbol()
				<< ") --> " << stateMap[transition->to()] << std::endl;
		}

		stream << std::endl;

		stream << "Start states: ";

		std::for_each(
			automaton.cStartBegin(),
			automaton.cStartEnd(),
			[&](const State_ConstIterator& state)
		{
			stream << stateMap[state] << " ";
		});

		stream << std::endl << std::endl;

		stream << "Final states: ";

		std::for_each(
			automaton.cFinalBegin(),
			automaton.cFinalEnd(),
			[&](const State_ConstIterator& state)
		{
			stream << stateMap[state] << " ";
		});

		stream << std::endl;

		return stream;
	}


}

#endif
