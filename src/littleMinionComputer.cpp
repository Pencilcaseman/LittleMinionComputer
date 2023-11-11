#include <lmc/littleMinionComputer.hpp>

namespace lmc {
	const ProgramState &LittleMinionComputer::state() const { return m_state; }
	ProgramState &LittleMinionComputer::state() { return m_state; }

	const ProgramState &LittleMinionComputer::initialState() const { return m_initialState; }
	ProgramState &LittleMinionComputer::initialState() { return m_initialState; }

	void LittleMinionComputer::execute() {
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
} // namespace lmc
