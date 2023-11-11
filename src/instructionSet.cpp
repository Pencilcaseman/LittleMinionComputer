#include <lmc/instructionSet.hpp>

namespace lmc {
	void instructionAdd(ProgramState &state, const Datum &operand) {
		Datum val			= state[operand];
		state.accumulator() = state.accumulator().value() + val.value();
	}

	void instructionSubtract(ProgramState &state, const Datum &operand) {
		Datum val			= state[operand];
		state.accumulator() = state.accumulator().value() - val.value();
	}

	void instructionSto(ProgramState &state, const Datum &operand) {
		state.memory()[operand.value()] = state.accumulator();
	}

	void instructionLda(ProgramState &state, const Datum &operand) {
		state.accumulator() = state[operand];
	}

	void instructionBr(ProgramState &state, const Datum &operand) {
		state.instructionCounter() = operand.value();
	}

	void instructionBrz(ProgramState &state, const Datum &operand) {
		if (state.accumulator().value() == 0) { state.instructionCounter() = operand.value(); }
	}

	void instructionBrp(ProgramState &state, const Datum &operand) {
		if (!state.accumulator().underflow()) { state.instructionCounter() = operand.value(); }
	}

	void instructionIn(ProgramState &state) {
		// If input buffer is empty, ask for input.
		// Otherwise, take the first element from the input buffer.

		if (state.inputBuffer().empty()) {
			std::cout << "Input: ";
			Datum::ValueType input;
			std::cin >> input;
			state.accumulator() = input;
		} else {
			state.accumulator() = state.inputBuffer().front();
			state.inputBuffer().erase(state.inputBuffer().begin());
		}
	}

	void instructionOut(ProgramState &state) {
		state.outputBuffer().emplace_back(state.accumulator());
	}

	void instructionHlt(ProgramState &state) {
		// This will overflow the instruction counter, which we
		// treat as a halt instruction. This also has the nice
		// side effect of wrapping back to the start of the program.
		// To restart the program, simply reset the overflow flag.
		state.instructionCounter() = 1000;
	}

	Opcode decodeInstruction(const Datum &instruction) {
		switch (instruction.value() / 100) {
			case 0: return {InstructionSet::HLT, -1};
			case 1: return {InstructionSet::ADD, instruction.value() % 100};
			case 2: return {InstructionSet::SUB, instruction.value() % 100};
			case 3: return {InstructionSet::STO, instruction.value() % 100};
			case 5: return {InstructionSet::LDA, instruction.value() % 100};
			case 6: return {InstructionSet::BR, instruction.value() % 100};
			case 7: return {InstructionSet::BRZ, instruction.value() % 100};
			case 8: return {InstructionSet::BRP, instruction.value() % 100};
			case 9:
				switch (instruction.value() % 100) {
					case 1: return {InstructionSet::IN, -1};
					case 2: return {InstructionSet::OUT, -1};
					default: return {InstructionSet::DAT, -1};
				}
		}
	}

	void executeInstruction(ProgramState &state, const Opcode &opcode) {
		switch (opcode.instruction) {
			case InstructionSet::ADD: instructionAdd(state, opcode.operand); break;
			case InstructionSet::SUB: instructionSubtract(state, opcode.operand); break;
			case InstructionSet::STO: instructionSto(state, opcode.operand); break;
			case InstructionSet::LDA: instructionLda(state, opcode.operand); break;
			case InstructionSet::BR: instructionBr(state, opcode.operand); break;
			case InstructionSet::BRZ: instructionBrz(state, opcode.operand); break;
			case InstructionSet::BRP: instructionBrp(state, opcode.operand); break;
			case InstructionSet::IN: instructionIn(state); break;
			case InstructionSet::OUT: instructionOut(state); break;
			case InstructionSet::HLT: instructionHlt(state); break;
			case InstructionSet::DAT: break;
		}
	}
} // namespace lmc
