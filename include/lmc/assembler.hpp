#pragma once

#include <librapid>
#include <exception>
#include <lmc/datum.hpp>
#include <lmc/instructionSet.hpp>

namespace lmc {
	Datum instructionToDatum(const std::string &txt) {
		if (txt == "ADD") return static_cast<Datum::ValueType>(InstructionSet::ADD);
		if (txt == "SUB") return static_cast<Datum::ValueType>(InstructionSet::SUB);
		if (txt == "STO") return static_cast<Datum::ValueType>(InstructionSet::STO);
		if (txt == "LDA") return static_cast<Datum::ValueType>(InstructionSet::LDA);
		if (txt == "BR") return static_cast<Datum::ValueType>(InstructionSet::BR);
		if (txt == "BRZ") return static_cast<Datum::ValueType>(InstructionSet::BRZ);
		if (txt == "BRP") return static_cast<Datum::ValueType>(InstructionSet::BRP);
		if (txt == "IN") return static_cast<Datum::ValueType>(InstructionSet::IN);
		if (txt == "OUT") return static_cast<Datum::ValueType>(InstructionSet::OUT);
		if (txt == "HLT") return static_cast<Datum::ValueType>(InstructionSet::HLT);

		throw std::invalid_argument(fmt::format("Unknown operation '{}'", txt));
	}

	// Need to keep track of:
	// 1. label/variable
	// 2. operation
	// 3. operand
	struct Operation {
		std::string name;
		Datum operation;
		std::string operand;
	};

	std::array<Datum, 100> assemble(const std::string &program, bool verbose = false) {
		std::vector<std::string> reducedLines;

		std::string current;
		for (const auto &c : program) {
			if (c == '\n') {
				if (!current.empty()) {
					reducedLines.emplace_back(current);
					current = "";
				}
			} else {
				if ((c == ' ' || c == '\t')) {
					if (current.length() > 0) {
						if (current.back() == ' ') {
							continue;
						} else {
							current += ' ';
						}
					}
				} else {
					current += c;
				}
			}
		}

		if (!current.empty()) reducedLines.emplace_back(current);

		for (const auto &line : reducedLines) { std::cout << std::flush; }

		std::vector<Operation> ops;
		librapid::Map<std::string, Datum::ValueType> namedEntities;
		namedEntities[""]	   = 0; // Ignore empty strings
		Datum::ValueType index = 0;
		for (const auto &line : reducedLines) {
			// Parse the line
			// 0. # Comment
			// 1. IN
			// 2. OUT
			// 3. <OP> <var>
			// 4. <var> DAT <val>
			// 5. <label> <OP> <var>

			std::vector<std::string> split;
			std::string tmp;
			Operation newOp;
			for (const auto &c : line) {
				if (c == '#') { // Ignore comments
					if (!tmp.empty()) split.emplace_back(tmp);
					tmp = "";
					goto lineContinue;
				} else if (c == ' ') { // Split by whitespace
					split.emplace_back(tmp);
					tmp = "";
				} else {
					tmp += c;
				}
			}

			if (!tmp.empty()) split.emplace_back(tmp);
			switch (split.size()) {
				case 1: {
					// Input/Output -- no operands
					newOp.operation = instructionToDatum(split[0]);
					break;
				}
				case 2: {
					// <val> DAT
					// <label> <IN/OUT>
					// <op> <operand>

					if (split[1] == "DAT") {
						newOp.name				  = split[0];
						newOp.operation			  = Datum(1000); // Force overflow
						namedEntities[newOp.name] = index;
					} else if (split[1] == "IN" || split[1] == "OUT") {
						newOp.name		= split[0];
						newOp.operation = instructionToDatum(split[1]);

						namedEntities[newOp.name] = index;
					} else {
						newOp.operation = instructionToDatum(split[0]);
						newOp.operand	= split[1];
					}
					break;
				}
				case 3: {
					if (split[1] == "DAT") { // <var> DAT <val>
						newOp.name		= split[0];
						newOp.operation = Datum(1000); // Force overflow
						newOp.operand	= split[2];

						namedEntities[newOp.name] = index;
					} else { // <label> <op> <operand>
						newOp.name		= split[0];
						newOp.operation = instructionToDatum(split[1]);
						newOp.operand	= split[2];

						namedEntities[newOp.name] = index;
					}

					break;
				}
				default: {
					throw std::invalid_argument(fmt::format("Syntax error in line '{}'", line));
				}
			}

			ops.emplace_back(newOp);
			index++;
		lineContinue:
		}

		std::array<Datum, 100> memory;

		index = 0;
		for (const auto &op : ops) {
			Datum::ValueType memVal;
			if (!op.operation.overflow()) {
				// Set opcode and value
				Datum::ValueType entityLocation = namedEntities.get(op.operand, -1);
				if (entityLocation < 0)
					throw std::runtime_error(fmt::format("Unknown entity: '{}'", op.operand));
				memVal = op.operation.value() + namedEntities[op.operand];
			} else if (!op.operand.empty()) {
				// Data declaration

				std::stringstream ss(op.operand);

				if (ss >> memVal) {
					if (ss.eof()) {
					} else {
						throw std::invalid_argument("Extra characters after number");
					}
				} else {
					// Parsing failed, throw an exception.
					throw std::invalid_argument("Syntax error. Must be a 3-digit integer.");
				}
			} else {
				// HLT is the only command that should take us here
				memVal = op.operation.value();
			}

			memory[index] = memVal;

			if (verbose) {
				auto col1 = fmt::fg(fmt::color::green) | fmt::emphasis::bold;
				auto col2 = fmt::fg(fmt::color::orange) | fmt::emphasis::bold;

				fmt::print(col1, "Writing ");
				fmt::print(col2, "{:0>3}", memVal);
				fmt::print(col1, " to memory index ");
				fmt::print(col2, "{:0>3}\n", index);
			}

			index++;
		}

		return memory;
	}
} // namespace lmc
