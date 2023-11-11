#pragma once

#include <librapid>
#include <lmc/datum.hpp>

namespace lmc {
	using ProgramMemory = std::array<Datum, 100>;

	class ProgramState {
	public:
		ProgramState()					   = default;
		ProgramState(const ProgramState &) = default;
		ProgramState(ProgramState &&)	   = default;

		ProgramState &operator=(const ProgramState &) = default;
		ProgramState &operator=(ProgramState &&)	  = default;

		LIBRAPID_NODISCARD const ProgramMemory &memory() const;
		ProgramMemory &memory();

		LIBRAPID_NODISCARD const Datum &accumulator() const;
		Datum &accumulator();

		LIBRAPID_NODISCARD const Datum &instructionCounter() const;
		Datum &instructionCounter();

		LIBRAPID_NODISCARD const std::vector<Datum> &inputBuffer() const;
		std::vector<Datum> &inputBuffer();

		LIBRAPID_NODISCARD const std::vector<Datum> &outputBuffer() const;
		std::vector<Datum> &outputBuffer();

		LIBRAPID_NODISCARD const Datum &operator[](size_t index) const;
		Datum &operator[](size_t index);

		LIBRAPID_NODISCARD const Datum &operator[](const Datum &index) const;
		Datum &operator[](const Datum &index);

		void increment();

		void reset();

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
