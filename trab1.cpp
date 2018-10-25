/*
 *	Trabalho 01 - Organização de Arquivos - Departamento de Ciencia da Computacao - Universidade de Brasilia
 *	Professor:	Camilo Dórea
 *	Alunos:		David Potolski - Matricula: 13/0018007 
 *				Rafael Torres  - Matricula: 15/0145365
 *	
 *	This program is supposed to open two files with various records and create a list of index for each file <...>	
 *	
 *	The files are arranged in the following format:
 *	MATRICULA [6] | [1] | NOME [40] | [1] | OP [3] | [1] | CURSO [8] | [1] | TURMA [2] | [1] or TURMA [3] ????
 *	
 *	Ex: 
 *	024312 Leonardo Nunes Augusto Henrique de Paula 354 E. Civil AB			
 *	057294 Amélia Antunes Barros Cardoso Galvão Mor	785 Medicina BC
 *	...
 *	...
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
		Primary_index(string index, string RRN);
	private:
		string _index;
		// Relative Record number
		int _RRN;
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

Primary_index::Primary_index(string index, string RRN){
	// Copy the index
	this->_index = index;
	
	// Transform the string RRN into an integer
	this->_RRN = strtol(RRN.c_str(), NULL, 10);
}

bool open_file(ifstream &source, char *name){
	string file_name = string(name[0]);
	
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

int main(int argc, char **argc){
	// Check number of arguments
	if(argc != 3){
		cout << "Error! Expected 2 source files!" << endl;
		return 0;
	};

	// Virtual file
	ifstream file;

	// Connects virtual file with physical file
	if(!open_file(file, argc[0]))
		return 0;
		
	// Create index file
	fill_index(file);

	return 0;
}
