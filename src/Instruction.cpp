/*
 * Instruction.cpp
 *
 *  Created on: 25-Dec-2023
 *      Author: karthik
 */

#include "instruction.h"
#include <iostream>
/*
 * Default Constructor
 */
Instr::Instr()
{
	opcode = 0;
	rs1 = 0;
	rs2 = 0;
	rd = 0;
	imm = 0;
	instruction.instructionUnion = 0;
}

/*
 * Constructor which breaks down the given instruction as i/p
 * parameter
 */
Instr::Instr(uint32_t instr)
{
	/* TODO instruction needs to genrated based on Instruction type
	 * will be updated in the Future
	 */
	opcode = (instr & 0x3F);

	//std::cout << "instr " << instr << std::endl;
	instruction.instructionUnion = instr;
	buildOperands();
}
/*
 * Function used to instruction set based of the type of it
 */
void Instr::buildOperands()
{
	uint32_t currInstr = instruction.instructionUnion;
	opcode = (currInstr & 0x3F);
	switch(findInstructionType())
	{
		case B_TYPE:
			rs1 = ((currInstr & 0x1F) << 6);
			rs2 = ((currInstr & 0x1F) << 11);
			rd = ((currInstr & 0x1F) << 16);
			break;
		case I_TYPE:
			rs1 = ((currInstr & 0x1F) << 6);
			rs2 = ((currInstr & 0x1F) << 11);
			imm = ((imm & 0x3FF) << 16);
			break;
		case UNKOWN_TYPE:
			break;
	}
}
/*
 * Return What type of instruction set it belongs to
 */
Instruction_TYPE Instr::findInstructionType()
{
	if( opcode < END_OF_B_TYPE)
	{
		return B_TYPE;
	}
	else if( opcode > END_OF_B_TYPE && opcode < END_OF_I_TYPE)
	{
		return I_TYPE;
	}
	// if the opcode is not correct, return unknown back
	return UNKOWN_TYPE;
}
/*
 * getter function to get the instruction
 */
uint32_t Instr::getInstruction()
{
	return instruction.instructionUnion;
}


/*
 * Build the Instruction based on the type and data already available
 */
void Instr::buildInstruction(Instruction_TYPE type)
{
	switch(type)
	{
		case B_TYPE:
			instruction.instructionUnion = ((opcode & 0x3F) << 0) | ((rs1 & 0x1F) << 6) | ((rs2 & 0x1F) << 11) | ((rd & 0x1F) << 16);
			break;
		case I_TYPE:
			instruction.instructionUnion = ((opcode & 0x3F) << 0) | ((rd & 0x1F) << 6) | ((rs1 & 0x1F) << 11) | ((imm & 0x3FF) << 16);
			break;
		case UNKOWN_TYPE:
					break;
	}

}


