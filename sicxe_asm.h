/*	Travis Barre, Julius Inocencio, Ehsan Eshragh, Michael Reese
	Team Oregon
	masc1187
	prog4
	sicxe_asm.h
	CS530, Spring 2014
*/

#ifndef SICXE_ASM_H
#define SICXE_ASM_H

#include "file_parser.h"
#include "opcodetab.h"
#include "symtab.h"
#include <sstream>
#include <iomanip>

#define NUM_OF_DIRECTIVES 9
#define LENGTH_OF_ADDRESS 5
#define NUM_OF_REGISTERS 10

const string assemblerdirs[] = { "START", "END", "BYTE", "WORD", "RESB", "RESW", "BASE", "NOBASE", "EQU" };
const string registers[] = {"A", "X", "L", "B", "S", "T", "F", "PC", "SW", ""};
const string reg_vals[] = {"0", "1", "2", "3", "4", "5", "6", "8", "9", "0"};

using namespace std;

class sicxe_asm {
	public:
		sicxe_asm(string filename) {
				file = filename;
				error = false;
				
				for (int i = 0; i < NUM_OF_REGISTERS; i++)
					register_map[registers[i]] = reg_vals[i];
				
			try {
				file_parser parser(filename);
				parser.read_file();
				assembler(parser);
			} catch(file_parse_exception fpexcpt) {
				cout << "**Sorry, error " << fpexcpt.getMessage() << endl;
			}
		}

	private:
		struct listing {
			string address;
			string label;
			string opcode;
			string operand;
			string machine_code;
			
			listing() {
				address = "";
				label = "";
				opcode = "";
				operand = "";
				machine_code = "";
			}
		};
		
		opcodetab optab;
		symtab symbol_tab;
		vector<listing> listing_file;
		map<string, string> register_map;
		string file;
		bool error;
		int base;
	
		void assembler(file_parser);
		void first_pass(file_parser);
		void second_pass(file_parser);
		unsigned int find_first_address(string);
		unsigned int fill_to_start(file_parser, bool);
		unsigned int handle_directive(unsigned int, int&, string, string);
		unsigned int size_of_byte_dir(string, unsigned int);
		int handle_base(string, unsigned int);
		void insert_listing(unsigned int, string, string, string);
		string machine_code(unsigned int, string, string);
		string set_last(unsigned int, string, string, bool);
		string format4(string, unsigned int, int&);
		string format3(string, unsigned int, int&, bool);
		string format2(string, unsigned int);
		string get_label(string, unsigned int);
		
		void print_listing_file(unsigned int);
		bool is_assembler_directive(string);
		string to_uppercase(string);
		bool valid_operand(string, unsigned int);
		bool check_numeric(string);
		bool check_hex(string, unsigned int);
		int str_to_int(string);
		int hex_to_int(string);
		string int_to_hex(int, unsigned int);
		pair<string,string> split_string(string, string);
};

#endif
