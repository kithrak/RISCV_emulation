/*
 * registers.cpp
 *
 *  Created on: 25-Dec-2023
 *      Author: karthik
 */

#include "registers.h"
#include "risc_machine.h"
#include "iostream"

using namespace std;

Registers::Registers()
{
	R[0] = 0;
	R[1] = 0;
	R[2] = 0;
	R[3] = 0;
	R[4] = 0;
	R[5] = 0;
	R[6] = 0;
	R[7] = 0;
	R[8] = 0;
	R[9] = 0;
	R[10] = 0;
	R[11] = 0;
	R[12] = 0;
	R[13] = 0;
	R[14] = 0;
	R[15] = 0;

	SP = MAX_DATA_MEMORY - 1; //  As memory starts from 0-Max-1
	PC = 0;
	LR = 0;
	SR = 0;
	IR = 0;
}
/*
 * Debug Function dumping all the function
 */
void Registers::dumpRegisters()
{
	std::cout << "--------------------------------" << endl;
	cout << "---------Register Bank----------" << endl;
	cout << "--------------------------------" << endl;
	cout << endl;
	cout << "---------General Registers------" << endl;
	cout << "R0  => " << R[0] << endl;
	cout << "R1  => " << R[1] << endl;
	cout << "R2  => " << R[2] << endl;
	cout << "R3  => " << R[3] << endl;
	cout << "R4  => " << R[4] << endl;
	cout << "R5  => " << R[5] << endl;
	cout << "R6  => " << R[6] << endl;
	cout << "R7  => " << R[7] << endl;
	cout << "R8  => " << R[8] << endl;
	cout << "R9  => " << R[9] << endl;
	cout << "R10 => " << R[10] << endl;
	cout << "R11 => " << R[11] << endl;
	cout << "R12 => " << R[12] << endl;
	cout << "R13 => " << R[13] << endl;
	cout << "R14 => " << R[14] << endl;
	cout << "R15 => " << R[15] << endl;
	cout << "--------------------------------" << endl;
	cout << "Stack Pointer   => " << SP << endl;
	cout << "Status Register => " << SR << endl;
	cout << "Program Counter => " << PC << endl;
	cout << "Link Register => " << LR << endl;
}

/*
 * Function called to save the program Counter
 * just before jumping to the function
 */
void Registers::saveCallerRegisters(Memory &gMemory)
{
	/*
	 * LR to point to the next execution address after the calling jump
	 */
	gMemory.data[SP--] = LR;
	LR = PC; // PC will be pointing to instruction after call

}

/*
 * Function called to Restore the program Counter
 * after function call
 */
void Registers::RestoreCallerRegisters(Memory &gMemory)
{
	/*
	 * LR to point to the next execution address after the calling jump
	 * SP pointer
	 */
	PC = LR;
	SP++;
	LR = gMemory.data[SP];
}

