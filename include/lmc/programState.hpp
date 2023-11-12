#pragma once

#include <librapid>
#include <lmc/datum.hpp>

namespace lmc {
	using ProgramMemory = std::array<Datum, 100>;

	class ProgramState {
	public:
		LIBRAPID_ALWAYS_INLINE ProgramState()					  = default;
		LIBRAPID_ALWAYS_INLINE ProgramState(const ProgramState &) = default;
		LIBRAPID_ALWAYS_INLINE ProgramState(ProgramState &&)	  = default;

		LIBRAPID_ALWAYS_INLINE ProgramState &operator=(const ProgramState &) = default;
		LIBRAPID_ALWAYS_INLINE ProgramState &operator=(ProgramState &&)		 = default;

		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE const ProgramMemory &memory() const {
			return m_memory;
		}
		LIBRAPID_ALWAYS_INLINE ProgramMemory &memory() { return m_memory; }

		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE const Datum &accumulator() const {
			return m_accumulator;
		}
		LIBRAPID_ALWAYS_INLINE Datum &accumulator() { return m_accumulator; }

		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE const Datum &instructionCounter() const {
			return m_instructionCounter;
		}
		LIBRAPID_ALWAYS_INLINE Datum &instructionCounter() { return m_instructionCounter; }

		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE const std::vector<Datum> &inputBuffer() const {
			return m_inputBuffer;
		}
		LIBRAPID_ALWAYS_INLINE std::vector<Datum> &inputBuffer() { return m_inputBuffer; }

		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE const std::vector<Datum> &outputBuffer() const {
			return m_outputBuffer;
		}
		LIBRAPID_ALWAYS_INLINE std::vector<Datum> &outputBuffer() { return m_outputBuffer; }

		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE const Datum &operator[](size_t index) const {
			return m_memory[index];
		}
		LIBRAPID_ALWAYS_INLINE Datum &operator[](size_t index) { return m_memory[index]; }

		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE const Datum &
		operator[](const Datum &index) const {
			return m_memory[index.value()];
		}

		LIBRAPID_ALWAYS_INLINE Datum &operator[](const Datum &index) {
			return m_memory[index.value()];
		}

		LIBRAPID_ALWAYS_INLINE void increment() { m_instructionCounter = m_instructionCounter + 1; }

		LIBRAPID_ALWAYS_INLINE void reset() {
			m_memory.fill(Datum(0));
			m_accumulator		 = Datum(0);
			m_instructionCounter = Datum(0);
		}

	private:
		// 100 memory locations
		ProgramMemory m_memory;

		// Accumulator (the value stored by the calculator)
		Datum m_accumulator;

		// Instruction counter
		Datum m_instructionCounter;

		// Input and output buffers
		std::vector<Datum> m_inputBuffer;
		std::vector<Datum> m_outputBuffer;
	};
} // namespace lmc
