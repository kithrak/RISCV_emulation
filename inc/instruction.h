/*
 * Instruction.h
 *
 *  Created on: 25-Dec-2023
 *      Author: karthik
 */

#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include <stdint.h>

/* Instruction supported by RISC machine */
enum Instruction_Opcode
{
	NOP=0,
	HALT,
	CMP,
	ADD,
	SUB,
	RET,
	AND,
	END_OF_B_TYPE,
	LOAD,
	LOADA,
	STORE,
	JMP,
	JS,
	JNS,
	JAL,
	ADDI,
	END_OF_I_TYPE
};

/*
 * Different type of Instruction Set supported
 */
enum Instruction_TYPE
{
	B_TYPE=0,
	I_TYPE,
	UNKOWN_TYPE
};

/*
 * ---------------------------------------------------------------
 *  RISC machine current consists of 2 instruction type
 * Basic
 * Immediate
 *---------------------------------------------------------------
 */
typedef union
{
	/*
	 * Basic Instruction type
	 * Opcode : 5 bytes
	 * rs1    : 4 bytes  Source1
	 * rs2    : 4 bytes  Source2
	 * rd     : 4 bytes  Target
	 */
	typedef struct
	{
		uint32_t opcode:6;
		uint32_t rs1:5;
		uint32_t rs2:5;
		uint32_t rd:5;
		uint32_t reserved:11; // future expansion if we want to add more subtype function for same opcode
	}RISC_B_INST;

	/*
	 * Immediate Instruction type
	 * Opcode : 5 bytes
	 * rs1    : 4 bytes  Source1
	 * rs2    : 4 bytes  Source2
	 * rd     : 4 bytes  Target
	 */
	typedef struct RISC_I_INST
	{
		uint32_t opcode:6;
		uint32_t rd:5;
		uint32_t rs1:5;
		uint32_t imm:12;
		uint32_t reserved:6; // future expansion if we want to add more subtype function for same opcode
	}RISC_I_INST;

	uint32_t instructionUnion;
}Instruction_set;

struct Instr
{
	uint32_t opcode;
	uint32_t rs1;
	uint32_t rs2;
	uint32_t rd;
	uint32_t imm;
	Instruction_set instruction;

public:
	Instr();
	Instr(uint32_t instr);
	void buildInstruction(Instruction_TYPE type);
	Instruction_TYPE findInstructionType();
	uint32_t getInstruction();
	void buildOperands();

};

#endif /* INSTRUCTION_H_ */
