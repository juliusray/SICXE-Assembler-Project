/*	Travis Barre, Michael Reese, Ehsan Eshragh, Julius Inocencio
	Team Oregon
	masc1187
	prog4
	CS530, Spring 2014
*/

#include "file_parser.h"

using namespace std;

// ctor, filename is the parameter.  A driver program will read
// the filename from the command line, and pass the filename to
// the file_parser constructor.  Filenames must not be hard-coded.
file_parser::file_parser(string filename) {
	ifstream sourcefile;
	string eachline;

	sourcefile.open(filename.c_str(), ios::in);
	if(!sourcefile.is_open())
		throw file_parse_exception("- Encountered problem opening file: " + filename);

	while(!sourcefile.eof()) {
		getline(sourcefile, eachline);
		sourcelines.push_back(eachline);
	}
	sourcefile.close();
}

file_parser::~file_parser() {}

// reads the source file, storing the information is some
// auxiliary data structure you define in the private section. 
// Throws a file_parse_exception if an error occurs.
// if the source code file fails to conform to the above
// specification, this is an error condition.     
void file_parser::read_file() {
	for(unsigned int i = 0; i < sourcelines.size(); i++) {
		line l;
		bool prev = true, is_comment = false, is_quote = false;
		unsigned int word_count = 0;
		string word = "";
		char *c = &sourcelines[i][0];

		while (*c && *c != '\n') {
			if (*c == '.' && !is_comment && !is_quote)
				is_comment = build_comment(c, l, word, word_count, i);
			else if (*c == '\'' && !is_comment)
				is_quote = skip_quote(c, is_quote, word, prev);
			else if (isspace(*c) && !is_quote)
				prev = check_space(c, l, is_comment, prev,is_quote, word, word_count, i);
			else {
				word += *c;
				prev = true;
			}
			c++;
			if(word_count > 3)
				throw file_parse_exception(build_exception("- Too many tokens at line ", i));
		}
		if(is_quote)
			throw file_parse_exception(build_exception("- Expected end quote at line ", i));
		l.put_word(word, word_count, i);
		tokens.push_back(l);
	}
}  
	        
// returns the token found at (row, column).  Rows and columns
// are zero based.  Returns the empty string "" if there is no 
// token at that location. column refers to the four fields
// identified above.
string file_parser::get_token(unsigned int row, unsigned int column) {
	if(row >= tokens.size()) {
		throw file_parse_exception(build_exception("- No such token exists at line ", row));
	}

	switch(column) {
		case 0:
			return tokens[row].label;
		case 1:
			return tokens[row].opcode;
		case 2:
			return tokens[row].operand;
		case 3:
			return tokens[row].comments;
		default:
			throw file_parse_exception(build_exception("- No such token exists at line ", row));
	}
}

// prints the source code file to stdout.  Should not repeat 
// the exact formatting of the original, but uses tabs to align
// similar tokens in a column. The fields should match the 
// order of token fields given above (label/opcode/operands/comments)
void file_parser::print_file() {
	for(unsigned int i = 0; i < tokens.size(); i++) {
		line tmpline = tokens[i];

		cout << tmpline.label << '\t' << tmpline.opcode << '\t'
				<< tmpline.operand << '\t' << tmpline.comments << endl;
	}
}

// returns the number of lines in the source code file
int file_parser::size() {
	return tokens.size();
}

string file_parser::build_exception(string s, int i) {
	stringstream ss;
	string temp;
	ss << i+1;
	ss >> temp;
	s += temp;
	return s;
}

bool file_parser::build_comment(char *c, line &l, string &word, unsigned int &word_count, unsigned int i) {
	l.put_word(word, word_count, i);
	word_count = 3;
	word += *c;
	return true;
}

bool file_parser::skip_quote(char *c, bool is_quote, string &word, bool &prev) {
	word += *c;
	prev = true;
	return !is_quote;
}

bool file_parser::check_space(char *c, line &l, bool is_comment, bool prev,bool is_quote, string &word, unsigned int &word_count, unsigned int i) {
	if (prev == true){
		if (!is_comment && !is_quote) {
			l.put_word(word, word_count, i);
			word_count++;
		} else
			word += *c;
	}
	prev = false;
	return prev;
}
