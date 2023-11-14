#pragma once

#include <librapid>
#include <lmc/datum.hpp>
#include <lmc/programState.hpp>

/*
 * Mnemonic		op code		Description
 * ----------------------------------------
 * ADD			1xx			Add
 * SUB			2xx			Subtract
 * STO			3xx			Store
 * LDA			5xx 		Load
 * BR			6xx			Branch
 * BRZ			7xx			Branch on zero
 * BRP			8xx			Branch on positive
 * IN			901			Input
 * OUT			902			Output
 * HLT			000			Halt or Stop
 * DAT						Data storage location
 */

namespace lmc {
	enum class InstructionSet {
		ADD = 100, // Add XX to accumulator
		SUB = 200, // Subtract XX from accumulator.
				   // If subtrahend is greater than minuend,
				   // NEG flag is set and the result in the
				   // accumulator is ((minuend - subtrahend) + 1000)
		STO = 300, // Store accumulator in XX
		LDA = 500, // Load accumulator with XX
		BR	= 600, // Branch to XX
		BRZ = 700, // Branch to XX if accumulator is zero
		BRP = 800, // Branch to XX if accumulator is positive
				   // (branch if NEG flag is NOT set)
		IN	= 901, // Input to accumulator from device
		OUT = 902, // Output accumulator to device
		HLT = 000, // Halt program execution
		DAT = 1000 // Not a real instruction
	};

//	Datum instructionToDatum(const std::string &instruction, Datum ) {
//		if (instruction == "ADD") return static_cast<Datum::ValueType>(InstructionSet::ADD);
//		if (instruction == "SUB") return static_cast<Datum::ValueType>(InstructionSet::SUB);
//		if (instruction == "STO") return static_cast<Datum::ValueType>(InstructionSet::STO);
//		if (instruction == "LDA") return static_cast<Datum::ValueType>(InstructionSet::LDA);
//	}

	struct Opcode {
		InstructionSet instruction;
		Datum operand;
	};

	LIBRAPID_ALWAYS_INLINE void instructionAdd(ProgramState &state, const Datum &operand) {
		Datum val			= state[operand];
		state.accumulator() = state.accumulator().value() + val.value();
	}

	LIBRAPID_ALWAYS_INLINE void instructionSub(ProgramState &state, const Datum &operand) {
		Datum val			= state[operand];
		state.accumulator() = state.accumulator().value() - val.value();
	}

	LIBRAPID_ALWAYS_INLINE void instructionSto(ProgramState &state, const Datum &operand) {
		state.memory()[operand.value()] = state.accumulator();
	}

	LIBRAPID_ALWAYS_INLINE void instructionLda(ProgramState &state, const Datum &operand) {
		state.accumulator() = state[operand];
	}

	LIBRAPID_ALWAYS_INLINE void instructionBr(ProgramState &state, const Datum &operand) {
		state.instructionCounter() = operand.value();
	}

	LIBRAPID_ALWAYS_INLINE void instructionBrz(ProgramState &state, const Datum &operand) {
		if (state.accumulator().value() == 0) { state.instructionCounter() = operand.value(); }
	}

	LIBRAPID_ALWAYS_INLINE void instructionBrp(ProgramState &state, const Datum &operand) {
		if (!state.accumulator().underflow()) { state.instructionCounter() = operand.value(); }
	}

	LIBRAPID_ALWAYS_INLINE void instructionIn(ProgramState &state) {
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

	LIBRAPID_ALWAYS_INLINE void instructionOut(ProgramState &state) {
		state.outputBuffer().emplace_back(state.accumulator());
	}

	LIBRAPID_ALWAYS_INLINE void instructionHlt(ProgramState &state) {
		// This will overflow the instruction counter, which we
		// treat as a halt instruction. This also has the nice
		// side effect of wrapping back to the start of the program.
		// To restart the program, simply reset the overflow flag.
		state.instructionCounter() = 1000;
	}

	LIBRAPID_ALWAYS_INLINE Opcode decodeInstruction(const Datum &instruction) {
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
			default: return {InstructionSet::DAT, -1};
		}
	}

	LIBRAPID_ALWAYS_INLINE void executeInstruction(ProgramState &state, const Opcode &opcode) {
		switch (opcode.instruction) {
			case InstructionSet::ADD: instructionAdd(state, opcode.operand); break;
			case InstructionSet::SUB: instructionSub(state, opcode.operand); break;
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
