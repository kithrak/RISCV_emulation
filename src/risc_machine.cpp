//============================================================================
// Name        : RISC.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <mainMemory.h>
#include <iostream>
#include <fstream>

#include "risc_machine.h"
#include "programLoad.h"
#include "instruction.h"
#include "test.h"

using namespace std;

Machine gMachine;
Operands gOperands;
/*
 * Will be used in future to move it to compile time
 * parameter to enable disable Test environment functions
 */
int gTestEnvironment = TEST_DISABLED;

/*
 * Write Obj code or text code to the output file
 * object.dat
 */
void writeObjectCodeToFile()
{
	ofstream myFile;
	myFile.open("object.dat",ios::out);
	if(!myFile)
	{
		cout << "INFO:writeObjectCodeToFile: File was not created\n";
	}
	else
	{
		for(int i = 0; i< gMachine.memory.nextFreeText; i++)
		{
			myFile << gMachine.memory.text[i] << endl;
		}
		myFile.close();
	}
}

/*
 * Below function decodes instruction for B_Type instruction set
 */
void decode_b_type_instr(uint32_t instruction, Operands &operands)
{
	/*
	 * update according to I instruction set
	 *  opcode:6;
	 *	rd:4;
	 *	rs1:4;
	 *	imm:12;
	 *	reserved:8;
	 */
	if(gMachine.registers.IR == ADD || gMachine.registers.IR == SUB || gMachine.registers.IR == AND)
	{
		operands.operand3     = (instruction >> 16) & 0x1F;
	}
	if(gMachine.registers.IR != JMP && gMachine.registers.IR != JNS && gMachine.registers.IR != JS && gMachine.registers.IR != JAL)
	{
		operands.operand1     = (instruction >> 6) & 0x1F;
	}
	operands.operand2     = (instruction >> 11) & 0x1F;
}
/*
 * Below function decodes instruction for I_Type instruction set
 */
void decode_i_type_instr(uint32_t instruction, Operands &operands)
{
	/*
	 * update according to I instr set
	 *  opcode:6;
	 *  rd:5;
	 *	rs1:5;
	 *	imm:12;
	 *	reserved:6;
	 */
	/* TODO update the code when Adding more instr below
	 * is just a template
	 */
	if(gMachine.registers.IR == LOAD || gMachine.registers.IR == LOADA)
	{
		operands.operand1     = (instruction >> 16) & 0x3FF;
		operands.operand2     = (instruction >> 11) & 0x1F;
	}
	if(gMachine.registers.IR == STORE)
	{
		operands.operand2     = (instruction >> 16) & 0x3FF;
		operands.operand1     = (instruction >> 11) & 0x1F;
	}
	if(gMachine.registers.IR == JMP || gMachine.registers.IR == JNS || gMachine.registers.IR == JS || gMachine.registers.IR == JAL)
	{
		operands.operand2     = (instruction >> 16) & 0x3FF;
	}
	if(gMachine.registers.IR == ADDI)
	{
		operands.operand3     = (instruction >> 6) & 0x1F;
		operands.operand2     = (instruction >> 16) & 0x3FF;
		operands.operand1     = (instruction >> 11) & 0x1F;
	}
}

/*
 * Fetches the required operand as per RISC-KB machine
 * operation till now have supported up to three operand.
 */
void fetchNextInstruction(Operands &operands)
{
	gMachine.registers.IR = gMachine.memory.text[gMachine.registers.PC++];

	if(gMachine.registers.IR == RET)
	{
		return;
	}
	if(gMachine.registers.IR == ADD || gMachine.registers.IR == SUB || gMachine.registers.IR == ADDI || gMachine.registers.IR == AND)
	{
		operands.operand3     = gMachine.memory.text[gMachine.registers.PC++];
	}
	if(gMachine.registers.IR != JMP && gMachine.registers.IR != JNS && gMachine.registers.IR != JS && gMachine.registers.IR != JAL)
	{
		operands.operand1     = gMachine.memory.text[gMachine.registers.PC++];
	}
	operands.operand2     = gMachine.memory.text[gMachine.registers.PC++];

}

/*
 * Fetches the required operand as per RISC-KB machine
 * operation till now have supported up to three operand.
 */
void fetchNextInstAndDecode(Operands &operands)
{
	uint32_t instruction = gMachine.memory.text[gMachine.registers.PC++];

	// Extract fields from the 32-bit instruction
	uint32_t opcode = instruction & 0x3F;
	gMachine.registers.IR = opcode;

	if( opcode < END_OF_B_TYPE)
	{
		decode_b_type_instr(instruction, operands);
	}
	else if( opcode > END_OF_B_TYPE && opcode < END_OF_I_TYPE)
	{
		decode_i_type_instr(instruction, operands);
	}
	else
	{
		cout << "ERR: fetchNextInstAndDecode: Instruction Set not yet Implemented or Wrong Opcode " << opcode << endl;
	}
}

/*
 * Heart of the execution of the all opcode fetched from the memory
 */
void executeInstruction(Operands &operands)
{
	int value = 0;
	switch (gMachine.registers.IR)
	{
		case NOP:
			/*
			 * Add a little extra  delay
			 */
			for(int i = 0; i< 100; i++){}
			break;

		case HALT:
			/*
			 * Exit the program by print a msg along:)
			 */

			writeObjectCodeToFile();
			cout << "Execution completed/Halted\n";
			gMachine.registers.dumpRegisters();

			cout << "Output of the function is " << gMachine.registers.R[2];

		    exit(0);
			break;

		case CMP:
			/*
			 * Compare two register and update the SR register if equal
			 */
			if(gMachine.registers.R[operands.operand2] == gMachine.registers.R[operands.operand1])
			{
				gMachine.registers.SR =  true;
			}
			break;

		case LOAD:
			/*
			 * Load data from the gMemory to register mentioned in the instruction
			 */
			if(operands.operand2 == 17)
			{
				/*
				 * Get address from the SP
				 */
				operands.operand1 = gMachine.registers.SP;
			}
			gMachine.registers.R[operands.operand2] = gMachine.LRUCache.getDatafromCache(gMachine.memory,operands.operand1);
			break;

		case LOADA:
			/*
			 * Load address from the gMemory to register mentioned in the instruction
			 */
			gMachine.registers.R[operands.operand2] = operands.operand1;
			break;
		case STORE:
			/*
			 * Load data from the register to gMemory mentioned in the instruction
			 */
			if(operands.operand2 == 17)
			{
				/*
				 * Get address from the SP
				 */
				operands.operand1 = gMachine.registers.SP;
			}
			/*
			 * Currently the system supports only WB so new data will be put
			 * on the cache and memory
			 */
			value = gMachine.registers.R[operands.operand1];
			gMachine.LRUCache.putDataToMemoryAndCache(gMachine.memory,operands.operand2,value);
			//gMemory.data[operand2] = gMachine.registers.R[operand1];
			break;

		case ADD:

			/*
			 * Add two numbers
			 * TODO check if we need to think of overflow
			 */
			gMachine.registers.R[operands.operand3] = gMachine.registers.R[operands.operand1] + gMachine.registers.R[operands.operand2];
			break;
		case ADDI:
			gMachine.registers.R[operands.operand3] = gMachine.registers.R[operands.operand1] + operands.operand2;
			break;
		case SUB:
			/*
			 * Add two numbers
			 * TODO check if we need to think of overflow
			 */
			gMachine.registers.R[operands.operand3] = gMachine.registers.R[operands.operand1] - gMachine.registers.R[operands.operand2];
			break;

		case AND:

			/*
			 * AND two numbers
			 */
			gMachine.registers.R[operands.operand3] = gMachine.registers.R[operands.operand1] & gMachine.registers.R[operands.operand2];
			break;

		case JMP:
			gMachine.registers.PC = operands.operand2;
			break;

		case JS:
			if(gMachine.registers.SR == true)
			{
				gMachine.registers.PC = operands.operand2;
				gMachine.registers.SR = false;
			}
			break;
		case JNS:
			if(gMachine.registers.SR == false)
			{
				gMachine.registers.PC = operands.operand2;
			}
			break;
		case JAL:
			// Save registers required
			gMachine.registers.saveCallerRegisters(gMachine.memory);
			// Jump to the address
			gMachine.registers.PC = operands.operand2;

			break;
		case RET:
			// Jump back to next execution before the called function
			// restore the registers
			gMachine.registers.RestoreCallerRegisters(gMachine.memory);
			break;
		default:
			/*
			 * Exit the program with appropriate message
			 */
			cout << "ERR Program is exiting due to Invalid Instruction at " << gMachine.registers.PC << endl;
			exit(0);
	}
	operands.reInit();
}


/*
 * Starting point of the execution
 * It will support 3 modes till now
 * 1) Text format( ASsembly language) loaded directly to the memory
 * 2) Assembly Language converted to instruction format and decoded during execution
 * 3) Test Script which can be used to Test if addition of code breaks existing code
 */
int main(int argc, char **argv)
{
	string inputFileNames;
	string inputFileType;

	/*
	 * Program takes 2 inputs
	 * format of the decoding required or if test needs to executed
	 * 1) format of the code
	 * 2) file name
	 * Default we will load factorial 7 code into the execution path
	 */
	if(argc <= 2)
	{
		inputFileNames = "Program.txt";
		inputFileType  = "dat";
	}
	else
	{
		inputFileNames = argv[2];
		inputFileType = argv[1];
	}
	if(inputFileType == "text")
	{
		/*
		 * Load the code into text segment
		 */
		if(loadProgramToMemory(gMachine, inputFileNames) == -1)
		{
			cout << "ERR:main: Something went wrong during execution\n";
			return 0;
		}

		/* Infinite Loop till no more code to execute */
		while(true)
		{
			fetchNextInstruction(gOperands);
			executeInstruction(gOperands);
		}
	}
	else if(inputFileType == "TEST" || inputFileType == "test")
	{
		gTestEnvironment =  1;
		int testoutput = testMain();
		cout << " Number of Tests Passed : " << (testoutput & 0xFFFF) << endl;
		cout << " Total Tests Executed : " << ((testoutput >> 16) & 0xFFFF) << endl;
	}
	else
	{
		/*
		 * Converts the code into instruction format and be ready for execution
		 * after that
		 */
		if(loadInstructionSetToMemory(gMachine, inputFileNames) == -1)
		{
			cout << "ERR:main: Something went wrong during execution\n";
			return 0;
		}

		/* Infinite Loop till no more code to execute */
		while(true)
		{
			fetchNextInstAndDecode(gOperands);
			executeInstruction(gOperands);
		}
	}
	return 0;
}
