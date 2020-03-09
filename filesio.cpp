#include"filesio.h"
#include"tads.h"
#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<vector>

using namespace std;

bool open_file(fstream &file, const char *name, string ending, string mode){
	string file_name = string(name);
	
	if(mode == "w"){
		//cout << "Escrita" << endl;
		if((file_name.length() <= ending.length()) || (file_name.substr(file_name.length()-ending.length(), file_name.length()) != ending)){
			cout << "Esperado arquivo do tipo '" << ending << "', continuando mesmo assim..." << endl;
		};
		
		// Open the source code file and verifies it is really open. OBS: 'open' function needs c string format to open file
		file.open(file_name.c_str(), fstream::out);
		if(!file.is_open()) {
			cout << "Nao foi possivel abrir o arquivo especificado!" << endl;
			return 0;
		};		
		
		return 1;
	}
	else if(mode == "r"){
		//cout << "Leitura" << endl;
		if((file_name.length() <= ending.length()) || (file_name.substr(file_name.length()-ending.length(), file_name.length()) != ending)){
			cout << "Esperado arquivo do tipo '" << ending << "', continuando mesmo assim..." << endl;
		};
		
		// Open the source code file and verifies it is really open. OBS: 'open' function needs c string format to open file
		if(file.is_open())
			file.close();
		file.open(file_name.c_str(), fstream::in | fstream::app);
		if(!file.is_open()) {
			cout << "Nao foi possivel abrir o arquivo especificado!" << endl;
			return 0;
		};		
		
		return 1;
	}
	else if(mode == "rw" || mode == "wr"){
		if((file_name.length() <= ending.length()) || (file_name.substr(file_name.length()-ending.length(), file_name.length()) != ending)){
			cout << "Esperado arquivo do tipo '" << ending << "', continuando mesmo assim..." << endl;
		};
		
		// Open the source code file and verifies it is really open. OBS: 'open' function needs c string format to open file
		// I dont know why, but i gotta open for writing and then for reading, writing
		if(file.is_open())
			file.close();
		file.open(file_name.c_str(), fstream::out | fstream::app);
		file.close();
		
		file.open(file_name.c_str(), fstream::out | fstream::in);
		if(!file.is_open()) {
			cout << "Nao foi possivel abrir o arquivo especificado!" << endl;
			return 0;
		};		
		
		return 1;
	}
	else {
		cout << "Modo de abertura de arquivos nao identificado!" << endl;
	};
	return 0;
}

bool rewrite_file(fstream &file, char *name, F_Header to_save, vector<Student> &students){
	file.close();
	
	if(file.is_open())
		return 1;
	
	file.open(name, fstream::out | fstream::trunc);

	if(!file.is_open())
		return 1;
		
	file << "HEADER " << to_save.get_date() << ":" << to_save.get_time() << " " << to_save.get_size() << '\r' << endl;
	
	for(int i=0;i<(int)students.size();i++){
		file << students[i]._nome << " " << students[i]._matricula << " " << students[i]._curso << " " << students[i]._turma << '\r' << endl;
	};
	
	file.close();

	if(file.is_open())
		return 1;

	file.open(name, fstream::in);

	if(!file.is_open())
		return 1;

	file.seekg(HEADER_SIZE + 2, file.beg);	

	return 0;		
}

bool start_BTree_file(fstream &file, BTree_Header header){
	if(!file.is_open())
		return 1;
		
	file << header.get_date() << ":" << header.get_time() << " " << header.get_order() << " -1       " << " 0        " << '\r' << endl;
	return 0;
}
