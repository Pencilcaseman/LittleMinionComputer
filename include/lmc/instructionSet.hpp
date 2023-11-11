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

	struct Opcode {
		InstructionSet instruction;
		Datum operand;
	};

	void instructionAdd(ProgramState &state, const Datum &operand);
	void instructionSub(ProgramState &state, const Datum &operand);
	void instructionSto(ProgramState &state, const Datum &operand);
	void instructionLda(ProgramState &state, const Datum &operand);
	void instructionBr(ProgramState &state, const Datum &operand);
	void instructionBrz(ProgramState &state, const Datum &operand);
	void instructionBrp(ProgramState &state, const Datum &operand);
	void instructionIn(ProgramState &state, const Datum &operand);
	void instructionOut(ProgramState &state);
	void instructionHlt(ProgramState &state);

	Opcode decodeInstruction(const Datum &instruction);
	void executeInstruction(ProgramState &state, const Opcode &opcode);
} // namespace lmc
