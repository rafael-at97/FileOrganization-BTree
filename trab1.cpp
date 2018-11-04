/*
 *	Trabalho 01 - Organização de Arquivos - Departamento de Ciencia da Computacao - Universidade de Brasilia
 *	Professor:	Camilo Dórea
 *	Alunos:		David Potolski - Matricula: 13/0018007 
 *				Rafael Torres  - Matricula: 15/0145365
 *	
 *	This program is supposed to open two files with various records and create a list of index for each file <...>	
 *	
 *	The files are arranged in the following format:
 *	MATRICULA [6] | [1] | NOME [40] | [1] | OP [3] | [1] | CURSO [8] | [1] | TURMA [2] | \n [1]
 *	
 *	Ex: 
 *	024312 Leonardo Nunes Augusto Henrique de Paula 354 E. Civil AB			
 *	057294 Amélia Antunes Barros Cardoso Galvão Mor	785 Medicina BC
 *	...
 *	...
 *	
 *	A header will be included in the files with a size of 64 bytes:
 *
 *	HEADER [6] | [1] | SIZE [3] | [1] | DAY/MONTH/YEAR:HOUR:MINUTE:SECOND [17] | [1] | LED_FRONT [2] | [33] 
 *
 *	HEADER -> Just a string indicating there is a header
 *	SIZE -> Representing number of 64 bytes records, including header 
 *	DAY/MONTH/YEAR:HOUR:MINUTE:SECOND -> Time the file (and index, supposedly) was last changed
 *	LED_FRONT -> First address pointed by the LED indicating an open position to insert new registers
 *	[33] -> Open space that can be used for something else later
 *
 *	The primary index files are arranged in the following format:
 *	MATRICULA + NOME [30] | [1] | Position of record in the original file [?]
 *	Ideia: Excluir o espaço [1] e colocar a posicao como 2 bytes, deixando cada registro de index com 32 bytes
 *
 *	Ex: (Adotando a ideia)
 *	024312LeonardoNunesAugustoHenr00
 *	057294AméliaAntunesBarrosCardo01
 *
 * 	Current version:
 *	->	V0.0 - Opening input files 
 */

#include<iostream>
#include<cstdlib>
#include<fstream>
#include<string>

/* Defines de tamanho */

#define matricula_len 6
#define nome_len 40
#define op_len 3
#define curso_len 8
#define turma_len 2

/* End defines */

using namespace std;

// Class that saves primary files data
class Student{
	public:
		Student();
		Student(string matricula, string nome, string op, string curso, string turma);
	private:
		string _matricula;
		string _nome;
		string _op;
		string _curso;
		string _turma;	
};

class Primary_index{
	public:
		Primary_index(string index, string RRP);
	private:
		string _index;
		// Relative Record Position
		int _RRP;
};

Student::Student(){
	// Begins all strings as empty;
	this->_matricula = "";
	this->_nome = "";
	this->_op = "";
	this->_curso = "";
	this->_turma = "";
}

Student::Student(string matricula, string nome, string op, string curso, string turma){
	// Begins all strings with the values provided;
	this->_matricula = matricula;
	this->_nome = nome;
	this->_op = op;
	this->_curso = curso;
	this->_turma = turma;
}

Primary_index::Primary_index(string index, string RRP){
	// Copy the index
	this->_index = index;
	
	// Transform the string RRP into an integer
	this->_RRP = strtol(RRP.c_str(), NULL, 10);
}

bool open_file(ifstream &source, char *name){
	string file_name = string(name);
	
	// Check if '.txt' extension and show warning if not found
	if(file_name.substr(file_name.length()-4, file_name.length()) != ".txt"){
		cout << "Warning! Expected file extension of type (.txt), continuing anyway." << endl;
	};

	// Open the source code file and verifies it is really open. OBS: 'open' function needs c string format to open file
	source.open(file_name.c_str());
	if(!source.is_open()) {
		cout << "Could not find or open source file!" << endl;
		return 0;
	};
	
	return 1;
	
}

void fill_index(ifstream &source){
	
}

short have_header(ifstream &file){
	char str[7];
	string first_reading;
	
	file.get(str, 7);	// Reads 6 bytes and put a '\0' in the char *
	
	if(!file.eof()){
		first_reading = string(str);
		cout << first_reading << endl;
		if(first_reading == "HEADER")
			return 1;
		else
			return 0;
	}
	else {
		cout << "Error while reading from file!" << endl;
	};
	
	return -1;
}

int main(int argc, char **argv){
	// Check number of arguments
	if(argc != 3){
		cout << "Error! Expected 2 source files!" << endl;
		return 0;
	};

	// Virtual file
	ifstream file;

	// Connects virtual file with physical file
	if(!open_file(file, argv[1]))
		return 0;
		
	short result;	
		
	// Create index file
	if((result = have_header(file)) == 1){
		cout << "Header success!, reading header file..." << endl;
	}
	else if(result == 0){
		cout << "File does not have any header yet." << endl;
	}
	else {
		cout << "File does not exist!" << endl;
	};

	return 0;
}
