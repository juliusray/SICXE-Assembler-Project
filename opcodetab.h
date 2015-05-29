/*	Travis Barre, Julius Inocencio, Ehsan Eshragh, Michael Reese
	Team Oregon
	masc1187
	prog4
	opcodetab.h
	CS530, Spring 2014
*/

#ifndef OPCODETAB_H
#define OPCODETAB_H

#include "opcode_error_exception.h"
#include <string>
#include <map>

using namespace std;

const string opcodes[] = { "ADD", "+ADD", "ADDF", "+ADDF", 
	"ADDR", "AND", "+AND", "CLEAR", "COMP", "+COMP", "COMPF", 
	"+COMPF", "COMPR", "DIV", "+DIV", "DIVF", "+DIVF", "DIVR", 
	"FIX", "FLOAT", "HIO", "J", "+J", "JEQ", "+JEQ", "JGT", 
	"+JGT", "JLT", "+JLT", "JSUB", "+JSUB", "LDA", "+LDA", "LDB", 
	"+LDB", "LDCH", "+LDCH", "LDF", "+LDF", "LDL", "+LDL", "LDS", 
	"+LDS", "LDT", "+LDT", "LDX", "+LDX", "LPS", "+LPS", "MUL", 
	"+MUL", "MULF", "+MULF", "MULR", "NORM", "OR", "+OR", "RD", 
	"+RD", "RMO", "RSUB", "+RSUB", "SHIFTL", "SHIFTR", "SIO", 
	"SSK", "+SSK", "STA", "+STA", "STB", "+STB", "STCH", "+STCH", 
	"STF", "+STF", "STI", "+STI", "STL", "+STL", "STS", "+STS", 
	"STSW", "+STSW", "STT", "+STT", "STX", "+STX", "SUB", "+SUB", 
	"SUBF", "+SUBF", "SUBR", "SVC", "TD", "+TD", "TIO", "TIX", 
	"+TIX", "TIXR", "WD", "+WD" };

const string machinecodes[] = { "18", "18", "58", "58", "90", 
	"40", "40", "B4", "28", "28", "88", "88", "A0", "24", "24", 
	"64", "64", "9C", "C4", "C0", "F4", "3C", "3C", "30", "30", 
	"34", "34", "38", "38", "48", "48", "00", "00", "68", "68", 
	"50", "50", "70", "70", "08", "08", "6C", "6C", "74", "74", 
	"04", "04", "D0", "D0", "20", "20", "60", "60", "98", "C8", 
	"44", "44", "D8", "D8", "AC", "4C", "4C", "A4", "A8", "F0", 
	"EC", "EC", "0C", "0C", "78", "78", "54", "54", "80", "80", 
	"D4", "D4", "14", "14", "7C", "7C", "E8", "E8", "84", "84", 
	"10", "10", "1C", "1C", "5C", "5C", "94", "B0", "E0", "E0", 
	"F8", "2C", "2C", "B8", "DC", "DC" };

const int sizes[] = { 3, 4, 3, 4, 2, 3, 4, 2, 3, 4, 3, 4, 2, 
	3, 4, 3, 4, 2, 1, 1, 1, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 
	4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 
	3, 4, 2, 1, 3, 4, 3, 4, 2, 3, 4, 2, 2, 1, 3, 4, 3, 4, 3, 
	4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 
	3, 4, 2, 2, 3, 4, 1, 3, 4, 2, 3, 4 };

class opcodetab {
	public:
		// ctor
		// creates a new dictionary structure and loads all of the opcodes for 
		// the SIC/XE architecture into the table.  Use the STL map for this.
		opcodetab(); 
		
		// takes a SIC/XE opcode and returns the machine code 
		// equivalent as a two byte string in hexadecimal.
		// Example:  get_machine_code("ADD") returns the value 18
		// Note that opcodes may be prepended with a '+'.
		// throws an opcode_error_exception if the opcode is not 
		// found in the table.
		string get_machine_code(string);  
		
		// takes a SIC/XE opcode and returns the number of bytes 
		// needed to encode the instruction, which is an int in
		// the range 1..4.
		// NOTE: the opcode must be prepended with a '+' for format 4.
		// throws an opcode_error_exception if the opcode is not 
		// found in the table.        
		int get_instruction_size(string);
						
	private:
		map <string, pair <string, int> > opcodemap;
		bool opcode_exists(string);
		void add_opcode(string, string, int);
		string to_uppercase(string);
};

#endif
