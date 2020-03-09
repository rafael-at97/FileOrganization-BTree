/*
 *	Trabalho 02 - Organização de Arquivos - Departamento de Ciencia da Computacao - Universidade de Brasilia
 *	Professor:	Camilo Dórea
 *	Alunos:		David Potolski - Matricula: 13/0018007 
 *				Rafael Torres  - Matricula: 15/0145365
 *	
 *	This program is supposed to open one file with various records and create a list of index for the file using B-tree	
 *	
 *	The files are arranged in the following format:
 *	NOME [40] | [1] | MATRICULA [6] | [1] | CURSO [3] | [1] | TURMA [1] | \r [1] | \n [1]
 *	
 *	Ex: 
 *	Carlos Dias Takase                       62364  EM  A		
 *	Bernardo Silva Bezerra                   81293  CC  A
 *	...
 *	...
 *	
 *	A header will be included in the file with a size of 38 bytes:
 *
 *	HEADER [6] | [1] | DAY/MONTH/YEAR:HOUR:MINUTE:SECOND [19] | [1] | Size [9] | \r [1] | \n [1] 
 *
 *	HEADER -> Just a string indicating there is a header
 *	DAY/MONTH/YEAR:HOUR:MINUTE:SECOND -> Time the file (and index, supposedly) was last changed (DD/MM/YYYY:HH:MM:SS)
 *	SIZE -> Representing number of records (Can grow up to 9 bytes, in ascii, meaning 9 '9's, up to 0.9 Gibibytes
 *
 *	The B-Tree file is arranged in the following format:
 *	HEADER (48 bytes):
 *		DAY/MONTH/YEAR:HOUR:MINUTE:SECOND [19] | [1] | ORDER [6] | [1] | ROOT [9] | [1] | NEXT_POS [9] | \r [1] | \n [1]
 *	DATA (variable size):
 *		COUNTER [N] | [1] | FATHER [9] | [1] | KEYS [(M-1)*8 + (M-2)] | [1] | DESCENDENTS [M*9 + (M-1)] | [1] | NRR [(M-1)*9 + (M-2)] | \r [1] | \n [1]
 *
 *	ORDER -> Ramification factor of B-Tree
 *	ROOT -> Index of root of B-Tree
 *	NEXT_POS -> Index of next space that can be used to create a node of the B-tree
 *	COUNTER -> Counter of how many keys the node has (N -> Number of ascii chars needed to represent the order)
 *	FATHER -> Index of the father of the node
 *	KEYS -> 3 first bytes of name and 5 bytes of 'matricula' (M -> Order, in decimal)
 *	DESCENDENTS -> 9 bytes to express pointers to the next nodes in the B-tree
 *	NRR -> Position of the register with given key in the original file
 *
 */

#include"filesio.h"
#include"tads.h"
#include"btree.h"
#include<iostream>
#include<cstdlib>
#include<ctime>
#include<fstream>
#include<string>
#include<bitset>
#include<vector>
#include<algorithm>
#include<map>
#include<unistd.h>

using namespace std;

short fill_student_list(fstream &file, vector<Student> &students){
	char line[64];
	string strline;
	
	while(!file.eof()){
		file.getline(line, 63);
		strline = string(line);
		strline = strline.substr(0, strline.find('\r'));
		
		if(file.eof() || !strline.length())
			break;
		
		if(strline.length() != RECORD_SIZE){
			cout << "Registro com tamanho errado identificado! Arquivo corrompido!" << endl;
			return 0;
		};
		
		//cout << strline << endl;
		students.push_back(Student(strline.substr(0, NAME_LEN), strline.substr(NAME_LEN+1, MAT_LEN), strline.substr(NAME_LEN+MAT_LEN+2, CURSO_LEN), strline.substr(NAME_LEN+MAT_LEN+CURSO_LEN+3, 1)));
	}
	
	return 0;
}

bool have_header(fstream &file){
	char line[64];
	
	file.getline(line, 63);
	
	string strline = string(line);
	strline = strline.substr(0, strline.find('\r'));
	
	//cout << strline << " " << strline.length() << endl;
	if(strline.length() != HEADER_SIZE){
		file.seekg(0, file.beg);
		return 0;
	};
	
	if(strline.substr(0, 6) == "HEADER"){
		file.seekg(0, file.beg);
		return 1;
	}	
	else{
		file.seekg(0, file.beg);
		return 0;
	};
	
	return 0;
}

F_Header read_header(fstream &file){
	char line[64];
	
	file.getline(line, 63);

	string strline = string(line);
	strline = strline.substr(0, strline.find('\r'));
	
	//cout << strline << endl;
	
	if(strline.length() != HEADER_SIZE){
		cout << "Nao foi possivel ler o cabecalho!" << endl;
		return F_Header(-1, -1, -1, -1, -1, -1, -1);
	};

	int day = strtol((strline.substr(7, 2)).c_str(), NULL, 10);
	int month = strtol((strline.substr(10, 2)).c_str(), NULL, 10);
	int year = strtol((strline.substr(13, 4)).c_str(), NULL, 10);
	int hour = strtol((strline.substr(18, 2)).c_str(), NULL, 10);
	int minute = strtol((strline.substr(21, 2)).c_str(), NULL, 10);
	int second = strtol((strline.substr(24, 2)).c_str(), NULL, 10);	

	int size = strtol((strline.substr(27, 8)).c_str(), NULL, 10);

	//cout << day << " " << month << " " << year << " " << hour << " " << minute << " " << second << " " << size << endl;
	
	return F_Header(day, month, year, hour, minute, second, size);
}

BTree_Header read_idx_header(fstream &file){
	char line[64];
	
	file.getline(line, 63);
	
	string strline = string(line);
	strline = strline.substr(0, strline.find('\r'));

	if(strline.length() != IDX_HEADER_SIZE){
		cout << "Nao foi possivel ler o cabecalho do indice!" << endl;
		return BTree_Header();
	};
	
	int day = strtol((strline.substr(0, 2)).c_str(), NULL, 10);
	int month = strtol((strline.substr(3, 2)).c_str(), NULL, 10);
	int year = strtol((strline.substr(6, 4)).c_str(), NULL, 10);
	int hour = strtol((strline.substr(11, 2)).c_str(), NULL, 10);
	int minute = strtol((strline.substr(14, 2)).c_str(), NULL, 10);
	int second = strtol((strline.substr(17, 2)).c_str(), NULL, 10);
	
	int order = strtol(strline.substr(20, 6).c_str(), NULL, 10);
	int root = strtol(strline.substr(27, 9).c_str(), NULL, 10);
	int next = strtol(strline.substr(37, 9).c_str(), NULL, 10);
	
	return BTree_Header(day, month, year, hour, minute, second, order, root, next);
}

F_Header make_header(int size){
	time_t t = time(0);
	tm* now = localtime(&t);
	
	return F_Header((now->tm_mday), (now->tm_mon + 1), (now->tm_year + 1900), (now->tm_hour), (now->tm_min), (now->tm_sec), size);
}

string change_name(char *original, string new_ending){
	string name = string(original);
	
	name = name.substr(0, name.find('.'));
	name += new_ending;
	
	return name;
}

string uppercase(string str){
	int i;
	for(i=0;i<(int)str.length();i++){
		if(str[i]<='z' && str[i]>='a')
			str[i] = str[i] - 'a' + 'A';
	};
	
	return str;
}

void update_headers(fstream &file, F_Header &file_header, fstream &idxfile, BTree_Header &idx_header){
	time_t t = time(0);
	tm* now = localtime(&t);
	
	file_header.set_datetime((now->tm_mday), (now->tm_mon + 1), (now->tm_year + 1900), (now->tm_hour), (now->tm_min), (now->tm_sec));
	file.seekp(7, file.beg);
	file << file_header.get_date() << ':' << file_header.get_time() << ' ' << file_header.get_size();
	
	idx_header.copy_datetime(file_header);
	idxfile.seekp(0);
	idxfile << idx_header.get_date() << ':' << idx_header.get_time() << ' ' << idx_header.get_order() << ' ' << transform_string(idx_header.get_root(), 9) << ' ' << transform_string(idx_header.get_next(), 9);
	
}

BTree_Header create_btree(fstream &file, F_Header &file_header, fstream &idxfile, string idx_fname){
	//First of all creates B Tree header
	BTree_Header btree_h;
	btree_h.copy_datetime(file_header);
	
	// Set root and next
	btree_h.set_root(-1);
	btree_h.set_next(0);
	
	int order, cnt;
	char line[64];
	string strline, nome, mat, curso, turma, key;
	
	// Ask user for order
	cout << "Qual fator de ramificacao deseja usar? (Maximo 524288, minimo 2)" << endl;
	cin >> order;
	if(order > 524288){
		cout << "Fator de ramificao arredondado para 524288" << endl;
		order = 524288;
	}
	else if(order<2){
		cout << "Fator de ramificacao arredondado para 2" << endl;
		order = 2;
	}
	
	btree_h.set_order(order);
	
	// Insert header into b tree index file
	if(!open_file(idxfile, idx_fname.c_str(), ".bt", "rw")){
		cout << "Nao foi possivel criar o arquivo de indices!" << endl;
		return BTree_Header();
	}
	
	if(start_BTree_file(idxfile, btree_h)){
		cout << "Erro ao inicializar o arquivo de indices!" << endl;
		return BTree_Header();
	}	
	
	cnt = 0;
	// Functions that reads an info from the file and writes the respective data into the index file
	while(!file.eof()){
		file.getline(line, 63);
		strline = string(line);
		strline = strline.substr(0, strline.find('\r'));
	
		// Security
		if(!strline.length())
			break;
	
		if(strline.length() != RECORD_SIZE){
			cout << "Registro com tamanho errado identificado! Arquivo corrompido!" << endl;
			return BTree_Header();
		};
		
		nome = strline.substr(0, NAME_LEN);
		mat = strline.substr(NAME_LEN+1, MAT_LEN);
		curso = strline.substr(NAME_LEN+MAT_LEN+2, CURSO_LEN);
		turma = strline.substr(NAME_LEN+MAT_LEN+CURSO_LEN+3, 1);
		
		key = uppercase(nome.substr(0, 3)) + mat.substr(0, 5);
		insert_tree(idxfile, btree_h, key, cnt);
		
		cnt++;
	}
	
	return btree_h;
}

void insert_new(fstream &file, F_Header &file_header, fstream &idxfile, BTree_Header &idx_header){
	string nome, matricula, curso, turma;
	
	cin.clear();
	cin.ignore();
	cout << "Digite o nome do aluno:  ";
	getline(cin, nome);
	cout << "Digite a matricula:  ";
	getline(cin, matricula);
	cout << "Digite o curso:  ";
	getline(cin, curso);
	cout << "Digite a turma:  ";
	getline(cin, turma);
	
	nome.resize(NAME_LEN, ' ');
	matricula.resize(MAT_LEN, ' ');
	curso.resize(CURSO_LEN, ' ');
	turma.resize(TURMA, ' ');
	
	Student student(nome, matricula, curso, turma);
	
	// Write data into file
	file.seekp(HEADER_SIZE + 2 + file_header.get_size_int()*(RECORD_SIZE + 2));
	file << student._nome << " " << student._matricula << " " << student._curso << " " << student._turma << '\r' << endl;

	// Update header
	int pos = file_header.get_size_int();
	file_header.size_inc();
	
	// Insert new key into b-tree
	string key = uppercase(nome.substr(0, 3)) + matricula.substr(0, 5);
	insert_tree(idxfile, idx_header, key, pos);
	
	update_headers(file, file_header, idxfile, idx_header);
}

void show_info(fstream &file, F_Header &file_header, fstream &idxfile, BTree_Header &idx_header, string f_name){
	string key;
	
	cin.clear();
	cin.ignore();
	cout << "Digite a chave de busca:  ";
	cin >> key;
	
	cout << "Realizando uma busca pela chave " << key << endl;
	
	idxfile.close();
	open_file(idxfile, f_name.c_str(), ".bt", "r");
	
	search_tree(file, idxfile, idx_header, key);
	
	idxfile.close();
}

int main(int argc, char **argv){
	// Check number of arguments
	if(argc != 2){
		cout << "Erro, numero de argumentos incorreto!" << endl;
		cout << "Correta utilizacao: ./arvoreB <nome do arquivo .txt>" << endl << "Ex: ./arvoreB lista.txt" << endl;
		return 0;
	};

	// Name of the index file
	string idx_fname = change_name(argv[1], ".bt");

	// File header
	F_Header file_header;
	
	// Index header
	BTree_Header idx_header;

	// Vector for writing student info
	vector<Student> students;

	// Virtual file
	fstream file, idxfile;

	// Connects virtual file with physical file
	if(!open_file(file, argv[1], ".txt", "wr"))
		return 0;
	
	bool result = 0;
		
	cout << "Verificando dados sobre o arquivo..." << endl;
	// Create index file for first file
	if((result = have_header(file)) == 1){
		cout << "Cabecalho identificado!" << endl;
		file_header = read_header(file);
		if(file_header.error()){
			cout << "Erro ao ler o cabecalho!" << endl;
			return 0;
		};
		
		file_header.print_modified();
		
		// Abrir o arquivo de indice e verificar se ele tem mesma data de modificacao
		
		if(!open_file(idxfile, idx_fname.c_str(), ".bt", "rw")){
			cout << "Arquivo de indice nao encontrado ou corrompido, criando um novo..." << endl;
			idx_header = create_btree(file, file_header, idxfile, idx_fname);			
		}
		else{
			cout << "Arquivo indice encontrado!" << endl;
			idx_header = read_idx_header(idxfile);
			if(idx_header.cmp_datetime(file_header)){
				cout << "Arquivo indice nao sincronizado, criando outro..." << endl;
				idxfile.close();
				idx_header = create_btree(file, file_header, idxfile, idx_fname);
			}
			else {
				cout << "Arquivo indice com arvore B de ordem " << idx_header.get_order_int() << " encontrado!" << endl;
			}
		}
	}
	else if(result == 0){
		cout << "Cabecalho nao encontrado! Inserindo cabecalho e criando arquivo de indices..." << endl;
		
		fill_student_list(file, students);
		file_header = make_header((int)students.size());
		file_header.print_modified();
		
		if(rewrite_file(file, argv[1], file_header, students)){
			cout << "Erro ao escrever o cabecalho!" << endl;
			return 0;
		};
		
		//Apos reescrever o principal, criar o arquivo de indice de arvore B com o header e os dados que temos na lista de estudantes
		idx_header = create_btree(file, file_header, idxfile, idx_fname);
		
	}
	else {
		cout << "Nao foi possivel ler o arquivo, possivel arquivo corrompido!" << endl;
	};

	update_headers(file, file_header, idxfile, idx_header);
	
	int choice = 1;
	cout << "O que desejas fazer?\n\t1 - Inserir novo registro\n\t2 - Pesquisar registro por chave\n\n\t0 - Sair" << endl;
	cin >> choice;
	
	while(choice){
		open_file(file, argv[1], ".txt", "wr");
		open_file(idxfile, idx_fname.c_str(), ".bt", "wr");
		
		if(choice == 1){
			insert_new(file, file_header, idxfile, idx_header);
		}
		else if(choice == 2){
			show_info(file, file_header, idxfile, idx_header, idx_fname);
		};
		cout << "O que desejas fazer?\n\t1 - Inserir novo registro\n\t2 - Pesquisar por registro\n\n\t0 - Sair" << endl;
		cin >> choice;
	}
	
	file.close();
	idxfile.close();
	
	return 0;
}
