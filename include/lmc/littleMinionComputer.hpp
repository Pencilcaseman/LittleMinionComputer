#pragma once

#include <librapid>
#include <lmc/instructionSet.hpp>
#include <lmc/assembler.hpp>

namespace lmc {
	class LittleMinionComputer {
	public:
		LIBRAPID_ALWAYS_INLINE LittleMinionComputer()							  = default;
		LIBRAPID_ALWAYS_INLINE LittleMinionComputer(const LittleMinionComputer &) = default;
		LIBRAPID_ALWAYS_INLINE LittleMinionComputer(LittleMinionComputer &&)	  = default;

		LIBRAPID_ALWAYS_INLINE LittleMinionComputer &
		operator=(const LittleMinionComputer &)											= default;
		LIBRAPID_ALWAYS_INLINE LittleMinionComputer &operator=(LittleMinionComputer &&) = default;

		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE const ProgramState &state() const {
			return m_state;
		}
		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE ProgramState &state() { return m_state; }

		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE const ProgramState &initialState() const {
			return m_initialState;
		}
		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE ProgramState &initialState() {
			return m_initialState;
		}

		LIBRAPID_ALWAYS_INLINE void reset() { m_state = m_initialState; }

		LIBRAPID_ALWAYS_INLINE void execute() {
			// Loop until HLT is called
			while (!m_state.instructionCounter().overflow()) {
				// Get the current value of the instruction counter
				Datum counter = m_state.instructionCounter();

				// Increment the instruction counter
				m_state.increment();

				// Find the instruction to execute
				const Datum &instruction = m_state[counter];

				// Decode and execute the instruction
				Opcode opcode = decodeInstruction(instruction);
				executeInstruction(m_state, opcode);
			}
		}

	private:
		ProgramState m_state;
		ProgramState m_initialState;
	};
} // namespace lmc
