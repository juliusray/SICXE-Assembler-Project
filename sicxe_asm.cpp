/*	Julius Inocencio, Ehsan Jahromi, Travis Barre, Michael Reese
	Team Oregon
	masc1187
	prog4
	CS530, Spring 2014
*/

#include "sicxe_asm.h"

using namespace std;

int main(int argc, char *argv[]) {
	if(argc != 2) {
        cout << "Error, you must supply the name of the file " <<
        "to process at the command line." << endl;
        exit(1);
    }
    new sicxe_asm(argv[1]);
}

void sicxe_asm::assembler(file_parser parser) {
	first_pass(parser);
	second_pass(parser);
}

void sicxe_asm::first_pass(file_parser parser) {
	base = -1;
	unsigned int x = fill_to_start(parser, true);
	unsigned int loc_counter = find_first_address(parser.get_token(x + 1, 2));
	
	do {
		x++;
		string curr_label = parser.get_token(x, 0);
		string curr_opcode = parser.get_token(x, 1);
		string curr_operand = parser.get_token(x, 2);
		if(curr_label != "") {
			try {
				symbol_tab.add_symbol(loc_counter, curr_label, curr_opcode, curr_operand);
			} catch(symtab_exception stexcpt) {
				cout << "**Sorry, error " << stexcpt.getMessage() << " at line " << x + 1 << endl;
				error = true;
			}
		}
		insert_listing(loc_counter, curr_label, curr_opcode, curr_operand);
		loc_counter += handle_directive(x, base, curr_opcode, curr_operand);
	} while(to_uppercase(parser.get_token(x, 1)) != "END");
}

void sicxe_asm::second_pass(file_parser parser) {
	unsigned int x = fill_to_start(parser, false);

	do {
		x++;
		string curr_label = parser.get_token(x, 0);
		string curr_opcode = parser.get_token(x, 1);
		string curr_operand = parser.get_token(x, 2);
		if(curr_label != "") {
			if(to_uppercase(curr_opcode) == "BASE") 
				base = handle_base(curr_operand, x);
			else if(to_uppercase(curr_opcode) == "NOBASE") 
				base = -1;
			if(!is_assembler_directive(to_uppercase(curr_opcode)))
				listing_file[x].machine_code = machine_code(x, curr_opcode, curr_operand);
		}
	} while(to_uppercase(parser.get_token(x, 1)) != "END");
	
	if(!error)
		print_listing_file(x);
}

unsigned int sicxe_asm::find_first_address(string operand) {
	if(operand == "0")
		return 0;
	else if(operand[0] == '$')
		return hex_to_int(operand.substr(1));
	else
		return str_to_int(operand);
}

unsigned int sicxe_asm::fill_to_start(file_parser parser, bool pass_one) {
	unsigned int x = 0;
	listing l;
	
	while(to_uppercase(parser.get_token(x,1)) != "START") {
		if(parser.get_token(x,1) != "")
			cout << "**Sorry, error - Illegal start of program" << endl;
		x++;
	}
	
	if(pass_one)
		for (unsigned int i=0; i < x; i++)
			listing_file.push_back(l);

	return x-1;
}

unsigned int sicxe_asm::handle_directive(unsigned int x, int& base, string curr_opcode, string curr_operand) {
	unsigned int size = 0;
	if (curr_opcode != "") {
		if(is_assembler_directive(to_uppercase(curr_opcode))) {
			if(to_uppercase(curr_opcode) == "BYTE")
				size = size_of_byte_dir(to_uppercase(curr_operand), x + 1);
			else if(to_uppercase(curr_opcode) == "WORD")
				size = 3;
			else if(to_uppercase(curr_opcode) == "RESB") {
				valid_operand(curr_operand, x);
				size = str_to_int(curr_operand);
			} else if(to_uppercase(curr_opcode) == "RESW") {
				valid_operand(curr_operand, x);
				size = str_to_int(curr_operand) * 3;
			}
		} else 
			size = optab.get_instruction_size(curr_opcode);
	}
	return size;
}

unsigned int sicxe_asm::size_of_byte_dir(string operand, unsigned int x) {
	unsigned int size = 1;
	if(operand[0] == 'C') {
		size_t pos1 = operand.find_first_of("'");
		size_t pos2 = operand.find_last_of("'");
		size = pos2 - pos1;
		if(size == 0)
			cout << "**Sorry, error - missing quote for BYTE directive at line " << x << endl;
		return size - 1;
	} else if(operand[0] == 'X') {
		size_t pos1 = operand.find_first_of("'");
		size_t pos2 = operand.find_last_of("'");
		size = pos2 - pos1;
		if(size == 0)
			cout << "**Sorry, error - missing quote for BYTE directive at line " << x << endl;
		else if((size - 1) % 2 == 1)
			cout << "**Sorry, error - odd number of hex bytes at line " << x << endl;
		return (size - 1) / 2;
	} else
		return size;
}

int sicxe_asm::handle_base(string operand, unsigned int x) {
	if (operand[0] == '$') {
		check_hex(operand.erase(0,1), x);
		return hex_to_int(operand);
	}
	if (isdigit(operand[0])) {
		valid_operand(operand, x);
		return str_to_int(operand);
	} else {
		int return_address = 0;
		try {
			if(valid_operand(symbol_tab.get_address(operand),x))
				return_address = str_to_int(symbol_tab.get_address(operand));
		} catch(symtab_exception stexcpt) {
			cout << "**Sorry, error " << stexcpt.getMessage() << " at line " << x + 1 << endl;
			error = true;
		}
		return return_address;
	}
}

void sicxe_asm::insert_listing(unsigned int lc, string label, string opcode, string operand) {
	listing l;
	l.address = int_to_hex(lc, LENGTH_OF_ADDRESS);
	l.label = label;
	l.opcode = opcode;
	l.operand = operand;
	listing_file.push_back(l);
}

string sicxe_asm::machine_code(unsigned int x, string opcode, string operand) {
	unsigned int opcode_machine_code = 0;
	bool indirect = false;
	try {
		opcode_machine_code = hex_to_int(optab.get_machine_code(opcode));
	} catch(opcode_error_exception e) {
		cout << "**Sorry, error " << e.getMessage() << " at line " << x + 1 << endl;
		error = true;
	}
	unsigned int ni;

	if(operand[0] == '@') {
		ni = 2;
		operand.erase(0,1);
	} else if(operand[0] == '#') {
		ni = 1;
		operand.erase(0,1);
		indirect = true;
	} else
		ni = 3;
	opcode_machine_code = opcode_machine_code | ni;
	return int_to_hex(opcode_machine_code, 2) + set_last(x, opcode, operand, indirect);
}

string sicxe_asm::set_last(unsigned int linenum, string opcode, string operand, bool indirect) {
	int bit = 0;
	string s = "";
	unsigned int size = 0;
	
	try {
		size = optab.get_instruction_size(opcode);
	} catch(opcode_error_exception e) {
		cout << "**Sorry, error " << e.getMessage() << " at line " << linenum + 1 << endl;
		error = true;
	}
	unsigned int comma = operand.find_first_of(",");
	if (to_uppercase(operand)[comma+1] == 'X') {
		operand.erase(comma, 2);
		bit += 8;
	}
	if (size == 4) {
		bit += 1;
		s = format4(operand, linenum);
	} else if (size == 3)
		s = format3(operand, linenum, bit, indirect);
	
	return int_to_hex(bit, 1) + s;
}

string sicxe_asm::format4(string operand, unsigned int linenum) {
	int address = 0; 
	if (check_numeric(operand))
		address = str_to_int(operand);
	else 
		address = str_to_int(get_label(operand, linenum));
	return int_to_hex(address, 5);
}

string sicxe_asm::format3(string operand, unsigned int linenum, int& bit, bool indirect) {
	int source = hex_to_int(listing_file[linenum].address);
	int offset = 0;
	if (indirect)
		return int_to_hex(str_to_int(operand), 3);
	if (check_numeric(operand))
		offset = str_to_int(operand);
	else 
		offset = str_to_int(get_label(operand, linenum));
	offset = (offset - source + 3); 
	if (offset < -2048 || offset > 2047) {
		if (base == -1 || offset > 4095 || offset < 0)
			cout << offset << endl;
			cout << "**Sorry, error - offset is too big at line " << linenum + 1 << endl;
			bit += 4;	
	} else 
		bit += 2;
	return int_to_hex(offset, 3);
}

string sicxe_asm::get_label(string label, unsigned int x) {
	try {
		if(check_numeric(symbol_tab.get_address(label)))
			return symbol_tab.get_address(label);
		if(check_numeric(symbol_tab.get_address(symbol_tab.get_address(label)))) {
			symbol_tab.edit_symbol(label);
			return symbol_tab.get_address(label);
		}
		else {
			cout << "**Sorry, error - complex forward reference at line " << x + 1 << endl;
			error = true;
		}
	} catch(symtab_exception stexcpt) {
		cout << "**Sorry, error " << stexcpt.getMessage() << " at line " << x + 1 << endl;
		error = true;
	}
	return "";
}

void sicxe_asm::print_listing_file(unsigned int x) {
	ofstream outFile;
	outFile.open((file.substr(0, file.length() -3) += "lis").c_str());

	outFile  << "**" << file << "**" << endl
		<< "Line#" << setw(10) << "Address" << setw(10) << "Label" << setw(10)
		<< "Opcode" << setw(10) << "Operand" << setw(15) << "Machine Code" << endl
		<< "=====" << setw(10) << "=======" << setw(10) << "=====" << setw(10)
		<< "======"	<< setw(10) << "=======" << setw(15) << "============" << endl;
	
	for (unsigned int i = 0; i <= x; i++) {
		outFile << setw(5) << right << i+1 << setw(5) << " "
		<< setw(10) << left << listing_file[i].address
		<< setw(10) << listing_file[i].label
		<< setw(10) << listing_file[i].opcode
		<< setw(10) << listing_file[i].operand 
		<< setw(15) << listing_file[i].machine_code << endl;
	}
	
	cout << "\nCompiling successful, .lis file created\n" << endl;
}

bool sicxe_asm::is_assembler_directive(string s) {
    for(unsigned int i = 0; i < NUM_OF_DIRECTIVES; i ++)
        if(s == assemblerdirs[i])
            return true;
    return false;
}

string sicxe_asm::to_uppercase(string s) {
    transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}

bool sicxe_asm::valid_operand(string s, unsigned int x) {
	if(!check_numeric(s)) {
		cout << "Sorry invalid operand " << s << " on line " << x << endl;
		error = true;
		return false;
	}
	return true;
}

bool sicxe_asm::check_numeric(string s) {
	if (s.find_first_not_of("0123456789") != string::npos)		
		return false;
	return true;
}

bool sicxe_asm::check_hex(string s, unsigned int line) {
	if (s.find_first_not_of("0123456789ABCDEF") != string::npos) {
		cout << "Sorry invalid operand " << s << " on line " << line << endl;
		error = true;
		return false;
	}
	return true;
}

int sicxe_asm::str_to_int(string s) {
    stringstream stream;
    int value;
    stream << s;
    stream >> value;
    return value;
}

int sicxe_asm::hex_to_int(string s){
	int value;
	sscanf(s.c_str(),"%x",&value);
	return value;
}

string sicxe_asm::int_to_hex(int num, unsigned int size){
	stringstream out;
	out << setw(size) << setfill('0') << hex << num;
	return to_uppercase(out.str());
}
