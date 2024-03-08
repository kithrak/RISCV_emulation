/*
 * programLoad.cpp
 *
 *  Created on: 22-Dec-2023
 *      Author: karthik
 */

#include <vector>
#include <sstream>
#include <iterator>
#include <unordered_map>
#include <bits/stdc++.h>
#include <instruction.h>

#include "programLoad.h"
#include "risc_machine.h"
#include "symbolTable.h"
#include "instruction.h"

using namespace std;

int textCounter = 0;

static unordered_map<string,Instruction_Opcode> operand =
						{
						  {"NOP", Instruction_Opcode::NOP},   {"HALT", Instruction_Opcode::HALT},
						  {"CMP", Instruction_Opcode::CMP},   {"JMP", Instruction_Opcode::JMP},
						  {"JS", Instruction_Opcode::JS},     {"JNS", Instruction_Opcode::JNS},
						  {"LOAD", Instruction_Opcode::LOAD}, {"STORE", Instruction_Opcode::STORE},
						  {"ADD", Instruction_Opcode::ADD},   {"SUB", Instruction_Opcode::SUB},
						  {"JAL", Instruction_Opcode::JAL},   {"RET", Instruction_Opcode::RET},
						  {"ADDI", Instruction_Opcode::ADDI}, {"LOADA", Instruction_Opcode::LOADA},
						  {"AND", Instruction_Opcode::AND},
						};
static unordered_map<string,int> register_number =
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
 *  for string delimiter
 */
vector<std::string> split(string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

bool isdigits(string &s)
{
	return (s.find_first_not_of("0123456789") == string::npos);
}
/*
 * Function to find value for the Opcode
 */
int findOpcode(string opcode)
{
	auto it = operand.find(opcode);
	if( it != operand.end())
	{
		return it->second;
	}
	return NO_OPCODE_FOUND;
}
/*
 * Function to find the register Number for the Register string
 */
int findRegister(string Reg)
{
	auto it = register_number.find(Reg);
	if( it != register_number.end())
	{
		return it->second;
	}
	return NO_REGISTER_FOUND;
}

/*
 * Function to find existing SymbolTable
 */
int findSymAddress(SymbolTable &symbol, string sym)
{
	int address = symbol.getVariableValue(sym);
	if(address == NO_SYMBOL_FOUND)
	{
		cout << "ERR:findSymAddress: Invalid Symbol name " <<  sym << endl;
		return PROGRAM_ERROR;
	}
	return address;
}

/*
 * Load the given opcode value to the
 * memory
 */
int loadOpcode(Memory &memory, int textCounter , string s)
{
	auto it = operand.find(s);
	if( it != operand.end())
	{
		memory.text[textCounter] = it->second;
	}
	else
	{
		return -1;
	}
	return 0;
}

/*
 * Fill the instruction with all the operand required to
 * carry out the execution of the opcode
 *
 */
int loadOperandToInstr(SymbolTable &symbol, vector<string> op, Instr &instr)
{
	int regNum1 = -1;
	int regNum2 = -1;
	int regNum3 = -1;
	int sym1    = -1;

	switch(instr.opcode)
	{
		case NOP:
		case HALT:
		case RET:
			break;
		case CMP:
			regNum1 = findRegister(op[0]);
			regNum2 = findRegister(op[1]);
			if(regNum1 <= -1 || regNum2 <= -1)
			{
				cout << "ERR:loadOperandToInstr: Register input is wrong in CMP\n";
				return -1;
			}
			instr.rs1 = regNum1;
			instr.rs2 = regNum2;
			break;
		case LOAD:
			sym1 = findSymAddress(symbol, op[0]);
			regNum2 = findRegister(op[1]);
			if(sym1 <= -1 || regNum2 <= -1)
			{
				cout << "ERR:loadOperandToInstr: Register/Sym input is wrong in LOAD\n";
				return -1;
			}
			instr.imm = sym1;
			instr.rs1 = regNum2;
			break;
		case STORE:
			regNum1 = findRegister(op[0]);
			sym1 = findSymAddress(symbol, op[1]);
			if(sym1 <= -1 || regNum1 <= -1)
			{
				cout << "ERR:loadOperandToInstr: Register/Sym input is wrong in STORE\n";
				return -1;
			}
			instr.rs1 = regNum1;
			instr.imm = sym1;
			break;
		case ADD:
		case SUB:
		case AND:
			regNum1 = findRegister(op[0]);
			regNum2 = findRegister(op[1]);
			regNum3 = findRegister(op[2]);
			if(regNum1 <= -1 || regNum2 <= -1 || regNum3 <= -1)
			{
				cout << "ERR:loadOperandToInstr: Register input is wrong in ADD/SUB\n";
				return -1;
			}
			instr.rd = regNum1;
			instr.rs1 = regNum2;
			instr.rs2 = regNum3;
			break;
		case ADDI:
			regNum1 = findRegister(op[0]);
			regNum2 = findRegister(op[1]);
			if(isdigits(op[2]))
			{
				regNum3 = stoi(op[2]);
			}
			else
			{
				cout << "ERR:loadOperandToInstr: ADDI provided with invalid Immediate value\n";
				return -1;
			}
			if(regNum1 <= -1 || regNum2 <= -1)
			{
				cout << "ERR:loadOperandToInstr: Register input is wrong in ADD/SUB\n";
				return -1;
			}
			instr.rd = regNum1;
			instr.rs1 = regNum2;
			instr.imm = regNum3;
			break;
		case JMP:
		case JS:
		case JNS:
		case JAL:
			sym1 = findSymAddress(symbol, op[0]);
			if(sym1 <= -1)
			{
				cout << "ERR:loadOperandToInstr: Symbol input is wrong in JUMP\n";
				return -1;
			}
			instr.rs1 = sym1;
			break;
	}
	return 0;
}

/*
 * Function to load the given operand(Reg/Sym) at the given text segment memory
 */
int loadOperand(Memory &memory, SymbolTable &symbol, int textCounter , string s)
{
	auto it = register_number.find(s);
	if( it != register_number.end())
	{
		memory.text[textCounter] = it->second;
		//cout << " register " << it->second << endl;
	}
	else
	{
		int address = symbol.getVariableValue(s);
		if(address == NO_SYMBOL_FOUND)
		{
			cout << "ERR:loadOperand: Invalid Symbol name " <<  s << endl;
			return -1;
		}
		memory.text[textCounter] = address;
	}
	return 0;
}

/*
 * Second pass for the small compiler/encoding setup to get the
 * Jump address from the symbol table
 */
int updateJmpSymbols(Memory &memory, SymbolTable &symbol, string &inputFileNames)
{
	ifstream inputFile(inputFileNames);
	if(!inputFile.is_open())
	{
		cout << "ERR:updateJmpSymbols: Unable to open the file/n";
		return 1;
	}

	string instr;
	int textCnt = 0;
	bool startProcessing = false;
	int address;
	while(getline(inputFile,instr))
	{
		istringstream iss(instr);
		vector<string> words(istream_iterator<string>{iss},istream_iterator<string>());
		if(words.size() == 0)
		{
					continue;
		}

		if(words[0] == ".text")
		{
			startProcessing = true;
		}
		if(startProcessing)
		{
			Instruction_Opcode ins;
			/*
			 * Search for Opcode only as symbols are just place holders
			 */
			auto it = operand.find(words[0]);
			if( it != operand.end())
			{
				ins = it->second;
			}
			else
			{
				continue;
			}

			/*
			 * Increment the text counter to update the exact place for updating
			 * symbols in the program
			 */
			switch(ins)
			{
				case END_OF_B_TYPE:
				case END_OF_I_TYPE:
					break;
				case NOP:
				case HALT:
				case RET:
					textCnt += 1;
					break;
				case CMP:
				case LOAD:
				case LOADA:
				case STORE:
					textCnt += 3;
					break;
				case ADD:
				case SUB:
				case ADDI:
				case AND:
					textCnt += 4;
					break;
				case JMP:
				case JS:
				case JNS:
				case JAL:
				{
					textCnt++; // Increment the opcode location
					address = symbol.getVariableValue(words[1]);
					if(address < 0)
					{
						cout << "ERR:updateJmpSymbols: Symbol table not updated properly" << endl;
						return -1;
					}
					memory.text[textCnt++] = address;
					break;
				}
				default:
					cout << "ERR:updateJmpSymbols: Invalid instruction opcode " << ins << endl;
					break;
			}
		}
	}
	return 0;
}
/*
 * Second pass for the small compiler/encoding setup to get the
 * Jump address from the symbol table
 */
int updateJmpSymbolsInstr(Machine &machine, string &inputFileNames)
{
	ifstream inputFile(inputFileNames);
	if(!inputFile.is_open())
	{
		cout << "ERR:updateJmpSymbolsInstr:Unable to open the file/n";
		return 1;
	}

	string instrString;
	Instr  inst;
	int textCnt = 0;
	bool startProcessing = false;
	int address;
	while(getline(inputFile,instrString))
	{
		istringstream iss(instrString);
		vector<string> words(istream_iterator<string>{iss},istream_iterator<string>());
		if(words.size() == 0)
		{
					continue;
		}

		if(words[0] == ".text")
		{
			startProcessing = true;
		}
		if(startProcessing)
		{
			Instruction_Opcode ins;
			/*
			 * Search for Opcode only as symbols are just place holders
			 */
			auto it = operand.find(words[0]);
			if( it != operand.end())
			{
				ins = it->second;
			}
			else
			{
				continue; //No instr created for symbols or comments
			}

			/*
			 * Increment the text counter to update the exact place for updating
			 * symbols in the program
			 */
			switch(ins)
			{
				case END_OF_B_TYPE:
				case END_OF_I_TYPE:
					break;
				case NOP:
				case HALT:
				case RET:
				case CMP:
				case LOAD:
				case LOADA:
				case STORE:
				case ADD:
				case SUB:
				case ADDI:
				case AND:
					textCnt += 1;
					break;
				case JMP:
				case JS:
				case JNS:
				case JAL:
					{
						Instr currInst(machine.memory.text[textCnt]);
						address = machine.symbolTable.getVariableValue(words[1]);
						if(address < 0)
						{
							cout << "ERR:updateJmpSymbolsInstr: Symbol table not updated properly" << endl;
							return -1;
						}
						currInst.imm = address;
						currInst.buildInstruction(currInst.findInstructionType());
						machine.memory.text[textCnt++] = currInst.getInstruction();
						break;
					}
				default:
					cout << "ERR:updateJmpSymbolsInstr: Invalid instruction opcode " << ins << endl;
					break;
			}
		}
	}
	return 0;
}

/*
 * Prepare a the instruction based on the opcode and different
 * operands and create a Instruction object to loaded to the mmeory
 */
int updateAndLoadTextSegWithInstr(Machine &machine, vector<string> &words, unordered_map<int,string> &jmpSymbols, int jmpSymCnt)
{
	int jmpAddress = 0;
	static int uniqueIdentifier = -2;
	Instr instruction;

	int opcode = findOpcode(words[0]);
	if(opcode >= OPERATION_SUCCESS)
	{
		instruction.opcode = opcode;
	}

	if(words.size() == 1)
	{
		/*
		 * HALT and NOP have no more operand in the instruction
		 */
		if(words[0] == "HALT" || words[0] == "NOP" || words[0] == "RET")
		{

			Instruction_TYPE type = instruction.findInstructionType();
			instruction.buildInstruction(type);
			machine.memory.text[machine.memory.nextFreeText++] = instruction.getInstruction();
			return 0;
		}
		else
		{
			/*
			 * Remove the Colon in the end of the string to
			 * create the symbol for jmp
			 */
			if(words[0].length())
			{
				words[0].pop_back();
			}
			jmpAddress = machine.symbolTable.getVariableValue(words[0]);
			if(jmpAddress <= -2)
			{
				machine.symbolTable.updateSymbol(words[0], machine.memory.nextFreeText);
				jmpSymCnt--;
			}
			else
			{
				machine.symbolTable.addSymbol(words[0],machine.memory.nextFreeText);
			}
			return 0;
		}
	}

	/*
	 * Jump to the address pointed by the symbol in the symbol table
	 */
	if(words[0] == "JMP" || words[0] == "JNS" || words[0] == "JS" || words[0] == "JAL")
	{
		jmpAddress = machine.symbolTable.getVariableValue(words[1]);
		if(jmpAddress != NO_SYMBOL_FOUND)
		{
			instruction.rs1 = jmpAddress;
		}
		else
		{
			/*
			 * Symbol is already present but has not found the symbol address yet
			 * so add symbol unique identifier to update the address later
			 */
			if(jmpAddress <= -2)
			{
				/*
				 * TODO make it function if required
				 * find string based on second value
				 */
				for(auto it= jmpSymbols.begin(); it != jmpSymbols.end(); it++)
				{
					/* store the same identifier to re-occurance */
					if (it->second == words[1])
					{
						instruction.rs1 = it->first;
					}
				}
			}
			else // no symbol found yet in symbol table add to symbol table and local map
			{
				machine.symbolTable.addSymbol(words[1],uniqueIdentifier); // symbol still doesn't exist
				instruction.rs1 = 0;                    // just a place holder will be replaced in the future
				auto it = jmpSymbols.find(uniqueIdentifier);
				if(it != jmpSymbols.end())
				{
					cout << "ERR:updateAndLoadTextSegWithInstr: Something not right found symbol already added with unique number" << endl;
					return -1;
				}
				jmpSymbols[uniqueIdentifier--] = words[1];
				jmpSymCnt++;
			}
		}

		Instruction_TYPE type = instruction.findInstructionType();
		instruction.buildInstruction(type);
		machine.memory.text[machine.memory.nextFreeText++] = instruction.getInstruction();
		return 0;
	}

	string delimiter = ",";
	vector<string> op = split(words[1],delimiter);
	if(op.empty())
	{
		cout << "ERR:updateAndLoadTextSegWithInstr: Invalid opcode operands" << endl;
		return -1;
	}
	/*
	 *  function populates the instr with all the opcodes required
	 */
	if(loadOperandToInstr(machine.symbolTable, op, instruction) != 0)
	{
		cout << "ERR:updateAndLoadTextSegWithInstr: Mismatch/Wrong operands passed along\n";
		return -1;
	}

	Instruction_TYPE type = instruction.findInstructionType();
	instruction.buildInstruction(type);
	machine.memory.text[machine.memory.nextFreeText++] = instruction.getInstruction();

	return 0;
}


/*
 * Main function to load the assembly text directly into the text
 *  segment with out using the instruction Architecture
 */
int loadProgramToMemory(Machine &machine, string &inputFileNames)
{
	ifstream inputFile(inputFileNames);
	//ofstream otFile("hello.txt");
	int sector = SECTOR_SEGMENT;
	int jmpAddress = 0;
	unordered_map<int,string> jmpSymbols;
	int jmpSymCnt = 0;
	int uniqueIdentifier = -2;


	if(!inputFile.is_open())
	{
		cout << "ERR:loadProgramToMemory: Unable to open the file\n";
		return 1;
	}

	string instr;

	if(getline(inputFile,instr))
	{
		istringstream iss(instr);
		vector<string> words(istream_iterator<string>{iss},istream_iterator<string>());

		// TODO if the first few lines are empty it will fault
		if(words[0] == ".sector")
		{
			sector = SECTOR_SEGMENT;
		}
		else if (words[0] == ".text")
		{
			sector = TEXT_SEGMENT;
		}
		else
		{
			cout << "ERR:loadProgramToMemory: Input program invalid" << endl;
		}
	}

	while(getline(inputFile,instr))
	{
		istringstream iss(instr);
		vector<string> words(istream_iterator<string>{iss},istream_iterator<string>());

		if(words.size() == 0)
		{
			continue;
		}

		/*
		 * Check if we have enter text segment
		 */
		if(words[0] == ".text")
		{
			sector = TEXT_SEGMENT;
			continue;
		}

		if(sector == TEXT_SEGMENT)
		{
			if(words.size() == 1)
			{
				/*
				 * HALT and NOP have no more operand in the instruction
				 */
				if(words[0] == "HALT" || words[0] == "NOP" || words[0] == "RET")
				{
					loadOpcode(machine.memory, textCounter++, words[0]);
					continue;
				}
				else
				{
					/*
					 * Remove the Colon in the end of the string to
					 * create the symbol for jmp
					 */
					if(words[0].length())
					{
						words[0].pop_back();
					}
					jmpAddress = machine.symbolTable.getVariableValue(words[0]);
					if(jmpAddress <= -2)
					{
						machine.symbolTable.updateSymbol(words[0],textCounter);
						jmpSymCnt--;
					}
					else
					{
						machine.symbolTable.addSymbol(words[0],textCounter);
					}
					continue;
				}
			}

			//TODO check return code
			loadOpcode(machine.memory, textCounter++, words[0]);

			/*
			 * Jump to the address pointed by the symbol in the symbol table
			 */
			if(words[0] == "JMP" || words[0] == "JNS" || words[0] == "JS" || words[0] == "JAL")
			{
				//cout << words[1] << endl;
				jmpAddress = machine.symbolTable.getVariableValue(words[1]);
				if(jmpAddress != NO_SYMBOL_FOUND)
				{
					machine.memory.text[textCounter++] = jmpAddress;
				}
				else
				{
					/*
					 * Symbol is already present but has not found the symbol address yet
					 * so add symbol unique identifier to update the address later
					 */
					if(jmpAddress <= -2)
					{
						/*
						 * TODO make it function if required
						 * find string based on second value
						 */
						for(auto it= jmpSymbols.begin(); it != jmpSymbols.end(); it++)
						{
							/* store the same identifier to re-occurance */
							if (it->second == words[1])
							{
								machine.memory.text[textCounter++] = it->first;
							}
						}

					}
					else // no symbol found yet in symbol table add to symbol table and local map
					{
						machine.symbolTable.addSymbol(words[1],uniqueIdentifier); // symbol still doesn't exist
						machine.memory.text[textCounter++] = uniqueIdentifier;
						auto it = jmpSymbols.find(uniqueIdentifier);
						if(it != jmpSymbols.end())
						{
							cout << "ERR:loadProgramToMemory: Something not right found symbol already added with unique number" << endl;
							return -1;
						}
						jmpSymbols[uniqueIdentifier--] = words[1];
						jmpSymCnt++;
					}
				}
				continue;
			}

			string delimiter = ",";
			vector<string> op = split(words[1],delimiter);
			if(op.empty())
			{
				cout << "ERR:loadProgramToMemory: Invalid opcode operands" << endl;
				return -1;
			}
			//cout << "First " << words[0] << endl;
			int immCnt = 0;
			int immValue = 0;
			for( auto i : op)
			{
				immCnt++;
				if(words[0] == "ADDI" && immCnt == 3)
				{
					if(isdigits(i))
					{
						immValue = stoi(i);
					}
					else
					{
						cout << "ERR:loadProgramToMemory: ADDI provided with invalid Immediate value\n";
						return -1;
					}
					machine.memory.text[textCounter++] = immValue;
				}
				//cout << "Second word: " << i << endl;
				// TODO check return code
				else
				{
					loadOperand(machine.memory,machine.symbolTable,textCounter++, i);
				}
			}
		}
		if(sector == SECTOR_SEGMENT)
		{
			string delimiter = ",";
			vector<string> op = split(words[0],delimiter);
			string name = op[0];
			int value = stoi(op[1]);
			machine.symbolTable.addSymbol(name,machine.memory.nextFreeData);
			machine.memory.data[machine.memory.nextFreeData++] = value;
		}

	}
	if (jmpSymCnt != 0)
	{
		cout << "ERR:loadProgramToMemory: Symbol not defined" << endl;
		return -1;
	}
	if(updateJmpSymbols(machine.memory, machine.symbolTable, inputFileNames) != 0)
	{
		cout << "ERR:loadProgramToMemory: Issue with update Symbol" << endl;
		return -1;
	}

	machine.memory.nextFreeText = textCounter;

	return 0;
}
/*
 * Main function to convert the given assembly language into instruction set
 * Architecture and load the new Instruction into the memory
 */
int loadInstructionSetToMemory(Machine &machine, string &inputFileNames)
{

	ifstream inputFile(inputFileNames);
	int sector = SECTOR_SEGMENT;
	unordered_map<int,string> jmpSymbols;
	int jmpSymCnt = 0;

	if(!inputFile.is_open())
	{
		cout << "ERR:loadInstructionSetToMemory: Unable to open the file\n";
		return -1;
	}

	string instr;

	if(getline(inputFile,instr))
	{
		istringstream iss(instr);
		vector<string> words(istream_iterator<string>{iss},istream_iterator<string>());

		// TODO if the first few lines are empty it will fault
		if(words[0] == ".sector")
		{
			sector = SECTOR_SEGMENT;
		}
		else if (words[0] == ".text")
		{
			sector = TEXT_SEGMENT;
		}
		else
		{
			cout << "ERR:loadInstructionSetToMemory: Input program invalid" << endl;
		}
	}

	while(getline(inputFile,instr))
	{
		istringstream iss(instr);
		vector<string> words(istream_iterator<string>{iss},istream_iterator<string>());

		if(words.size() == 0)
		{
			continue;
		}

		/*
		 * Check if we have enter text segment
		 */
		if(words[0] == ".text")
		{
			sector = TEXT_SEGMENT;
			continue;
		}

		if(sector == TEXT_SEGMENT)
		{
			updateAndLoadTextSegWithInstr(machine, words, jmpSymbols,jmpSymCnt);
		}
		if(sector == SECTOR_SEGMENT)
		{
			string delimiter = ",";
			vector<string> op = split(words[0],delimiter);
			string name = op[0];
			int value = stoi(op[1]);
			machine.symbolTable.addSymbol(name,machine.memory.nextFreeData);
			machine.memory.data[machine.memory.nextFreeData++] = value;
		}

	}
	if (jmpSymCnt != 0)
	{
		cout << "ERR:loadInstructionSetToMemory: Symbol not defined" << endl;
		return -1;
	}
	if(updateJmpSymbolsInstr(machine, inputFileNames) != 0)
	{
		cout << "ERR:loadInstructionSetToMemory: Issue with update Symbol" << endl;
		return -1;
	}

	return 0;

}


