#pragma once

#include <librapid>
#include <lmc/instructionSet.hpp>

namespace lmc {
	class LittleMinionComputer {
	public:
		LittleMinionComputer()							   = default;
		LittleMinionComputer(const LittleMinionComputer &) = default;
		LittleMinionComputer(LittleMinionComputer &&)	   = default;

		LittleMinionComputer &operator=(const LittleMinionComputer &) = default;
		LittleMinionComputer &operator=(LittleMinionComputer &&)	  = default;

		LIBRAPID_NODISCARD const ProgramState &state() const;
		LIBRAPID_NODISCARD ProgramState &state();

		LIBRAPID_NODISCARD const ProgramState &initialState() const;
		LIBRAPID_NODISCARD ProgramState &initialState();

		void execute();

	private:
		ProgramState m_state;
		ProgramState m_initialState;
	};
} // namespace lmc
