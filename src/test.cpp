/*
 * test.cpp
 *
 *  Created on: 27-Dec-2023
 *      Author: karthik
 */

#include <unordered_map>

#include "test.h"
#include "risc_machine.h"
#include "registers.h"
#include "symbolTable.h"
#include "memory.h"
#include "programLoad.h"
#include "instruction.h"

using namespace std;

extern Machine gMachine;

static unordered_map<string,Instruction_Opcode> tOperand =
						{
						  {"NOP", Instruction_Opcode::NOP},   {"HALT", Instruction_Opcode::HALT},
						  {"CMP", Instruction_Opcode::CMP},   {"JMP", Instruction_Opcode::JMP},
						  {"JS", Instruction_Opcode::JS},     {"JNS", Instruction_Opcode::JNS},
						  {"LOAD", Instruction_Opcode::LOAD}, {"STORE", Instruction_Opcode::STORE},
						  {"ADD", Instruction_Opcode::ADD},   {"SUB", Instruction_Opcode::SUB},
						  {"JAL", Instruction_Opcode::JAL},   {"RET", Instruction_Opcode::RET},
						  {"ADDI", Instruction_Opcode::ADDI},
						};
static unordered_map<string,int> tRegister_number =
						{
						  {"R0", 0},   {"R1", 1},
						  {"R2", 2},   {"R3", 3},
						  {"R4", 4},   {"R5", 5},
						  {"R6", 6},   {"R7", 7},
						  {"R8", 8},   {"R9", 9},
						  {"R10", 10}, {"R11",11},
						  {"R12", 12}, {"R13",13},
						  {"R14", 14}, {"R15",15},
						  {"LR", 16},  {"SP",17},
						};


/*
 * Start of the testing setup. It
 * Currently all test cases.
 */
int testOperandRegister()
{
	int textCounter = 0;
	string s = "R1";
	SymbolTable tSymbol;

	loadOperand(gMachine.memory, tSymbol, textCounter, s);

	if(gMachine.memory.text[textCounter] == 1) // R1 register is mapped to 1
	{
		return 0;
	}
	return -1;
}

int testExecutionOfCMP(int condition)
{
	Operands tOp;

	gMachine.registers.IR = CMP;
	if(condition == TEST_CMP_EQUAL)
	{
		gMachine.registers.R[1] = 10;
		gMachine.registers.R[2] = 10;
	}
	else if (condition == TEST_CMP_NOT_EQUAL)
	{
		gMachine.registers.R[1] = 10;
		gMachine.registers.R[2] = 20;
	}
		tOp.operand1 = 1;
		tOp.operand2 = 2;

	executeInstruction(tOp);

	if(gMachine.registers.SR == true)
	{
		return 0;
	}
	return -1;
}


int testMain()
{
	int testCnt = 0; // Increment the variable every time a new test case is added
	int testPassed = 0; // Increment the value only if the test case passes

	testCnt++;
	if( testOperandRegister() == 0)
	{
		testPassed++;
	}
	testCnt++;
	if ( testExecutionOfCMP(TEST_CMP_EQUAL) == 0)
	{
		testPassed++;
	}
	return (testCnt << 16 | testPassed);
}




