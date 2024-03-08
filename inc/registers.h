/*
 * registers.h
 *
 *  Created on: 25-Dec-2023
 *      Author: karthik
 */

#ifndef REGISTERS_H_
#define REGISTERS_H_

#include <mainMemory.h>

/*
 * Our Little Register bank used for the setup
 * Consists of 12 common Registers used as R[0]-R[11]
 * One Stack Pointer (SP) 9 mostly will be used for future updates
 * One Program Counter (PC) used to point to code where it is executing
 * Status registers
 * Instruction Register contains details on which Instruction is executing
 */
struct Registers
{

	int R[16];
	int SP;    /* Stack Pointer */
	int LR;    /* Return Address */
	int PC;    /* Program Counter */
	int SR;    /* Status Register */ // TODO current assumption this is a flag think about overflow
	int IR;    /* Instruction Register */

	/*std::unordered_map<std::string, int*> register_addres =
							{
							  {"R0", &R[0]},   {"R1", &R[1]},
							  {"R2", &R[2]},   {"R3", &R[3]},
							  {"R4", &R[4]},   {"R5", &R[5]},
							  {"R6", &R[6]},   {"R7", &R[7]},
							  {"R8", &R[8]},   {"R9", &R[9]},
							  {"R10", &R[10]}, {"R11",&R[11]},
							  {"R12", &R[12]}, {"R13",&R[13]},
							  {"R14", &R[14]}, {"R15",&R[15]},
							  {"LR", &LR},     {"SP",&SP},
							  {"PC", &PC},     {"SR",&SR},
							  {"IR", &IR},
							};*/
public:
	Registers();
	void dumpRegisters();
	void saveCallerRegisters(Memory &gMemory);
	void RestoreCallerRegisters(Memory &gMemory);

	void setGeneralRegister(int regNum, int value);
	void setStackPointerRegister(int value);
	void setLinkRegister(int value);
	void setStatusRegister(int value);
	void setInstructionRegister(int value);

	int setGeneralRegister(int regNum);
	int setStackPointerRegister();
	int setLinkRegister();
	int setStatusRegister();
	int setInstructionRegister();

};


#endif /* REGISTERS_H_ */
