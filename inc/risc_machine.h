/*
 * Risc_machine.h
 *
 *  Created on: 22-Dec-2023
 *      Author: karthik
 */

#ifndef RISC_MACHINE_H_
#define RISC_MACHINE_H_

#include <stdint.h>
#include <unordered_map>
#include <string>

#include "registers.h"
#include "symbolTable.h"
#include "cache.hpp"

#define OPERATION_SUCCESS   0
#define PROGRAM_ERROR      -1
#define NO_OPCODE_FOUND    -2
#define NO_SYMBOL_FOUND    -1 // TODO fix all return code values
#define NO_REGISTER_FOUND  -4

struct Operands
{
	int opcode;
	int operand1;
	int operand2;
	int operand3;

public:
	Operands(): opcode(0), operand1(0), operand2(0), operand3(0) {}

	void reInit()
	{
		opcode   = 0;
		operand1 = 0;
		operand2 = 0;
		operand3 = 0;
	}

};

struct Machine
{
	Memory      memory;
	Registers   registers;
	SymbolTable symbolTable;
	LRU         LRUCache;
	Operands    operands;
};


// Exposing function for testing
void executeInstruction(Operands &operands);

#endif /* RISC_MACHINE_H_ */
