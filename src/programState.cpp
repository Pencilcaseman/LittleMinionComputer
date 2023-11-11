#include <lmc/programState.hpp>

namespace lmc {
	const ProgramMemory &ProgramState::memory() const { return m_memory; }
	std::array<Datum, 100> &ProgramState::memory() { return m_memory; }

	const Datum &ProgramState::accumulator() const { return m_accumulator; }
	Datum &ProgramState::accumulator() { return m_accumulator; }

	const Datum &ProgramState::instructionCounter() const { return m_instructionCounter; }
	Datum &ProgramState::instructionCounter() { return m_instructionCounter; }

	const std::vector<Datum> &ProgramState::inputBuffer() const { return m_inputBuffer; }
	std::vector<Datum> &ProgramState::inputBuffer() { return m_inputBuffer; }

	const std::vector<Datum> &ProgramState::outputBuffer() const { return m_outputBuffer; }
	std::vector<Datum> &ProgramState::outputBuffer() { return m_outputBuffer; }

	const Datum &ProgramState::operator[](size_t index) const { return m_memory[index]; }
	Datum &ProgramState::operator[](size_t index) { return m_memory[index]; }

	const Datum &ProgramState::operator[](const Datum &index) const {
		return m_memory[index.value()];
	}
	Datum &ProgramState::operator[](const Datum &index) { return m_memory[index.value()]; }

	void ProgramState::increment() { m_instructionCounter = m_instructionCounter + 1; }

	void ProgramState::reset() {
		m_memory.fill(Datum(0));
		m_accumulator		 = Datum(0);
		m_instructionCounter = Datum(0);
	}
} // namespace lmc
