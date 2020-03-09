#include"tads.h"
#include<string>
#include<iostream>
#include<cstdlib>
#include<algorithm>
#include<utility>
#include<fstream>
#include<vector>

using namespace std;

/* General use functions*/

bool my_comp(pair<string, int> p1, pair<string, int>p2){
	short result = p1.first.compare(p2.first);
	
	if(result<1)
		return 0;
		
	return 1;	
}

string transform_string(int number, int size){
	string n = "";

	int cnt = number;	

	if(cnt==0)
		n += '0';

	while(cnt>0){
		n += (cnt%10) + 0x30;
		cnt /= 10;
	}

	if(cnt<0){
		n+= '1';
		n+= '-';
	}

	reverse(n.begin(), n.end());

	while((int)n.length() < size)
		n += ' ';

	return n;	
}

int n_houses(int n){
	int cnt = 0;
	
	if(n == 0)
		return 1;
		
	while(n>0){
		n /= 10;
		cnt++;
	}	
	
	return cnt;
}

/* End of General Use Functions*/

F_Header::F_Header(){
	this->_day = this->_month = this->_year = this->_hour = this->_minute = this->_second = this->_size = -1;
}

F_Header::F_Header(int day, int month, int year, int hour, int minute, int second, int size){
	this->_size = size;
	this->_day = day;
	this->_month = month;
	this->_year = year;
	this->_hour = hour;
	this->_minute = minute;
	this->_second = second;	
}

int F_Header::get_size_int(){
	return this->_size;
}

void F_Header::size_inc(){
	this->_size += 1;
}

bool Header::error(){
	if(this->_day < 0)
		return 1;
	else
		return 0;	
}

void Header::print_modified(){
	cout << "Ultima modificacao em " << this->_day << "/" << this->_month << "/" << this->_year << " as " << this->_hour << ":" << this->_minute << ":" << this->_second << endl;
}

string Header::get_date(){
	char date[11];
	
	if(this->_day < 10){
		date[0] = '0';
		date[1] = this->_day + 0x30;
	}
	else {
		date[0] = this->_day/10 + 0x30;
		date[1] = this->_day%10 + 0x30;
	}
	
	date[2] = '/';
	
	if(this->_month < 10){
		date[3] = '0';
		date[4] = this->_month + 0x30;
	}
	else {
		date[3] = this->_month/10 + 0x30;
		date[4] = this->_month%10 + 0x30;
	}
	
	date[5] = '/';
	
	date[6] = this->_year/1000 + 0x30;
	date[7] = (this->_year/100)%10 + 0x30;
	date[8] = (this->_year/10)%10 + 0x30;
	date[9] = (this->_year%10) + 0x30;
	
	date[10] = '\0';
	
	return string(date);
}

string Header::get_time(){
	char time[9];
	
	if(this->_hour < 10){
		time[0] = '0';
		time[1] = this->_hour + 0x30;
	}
	else {
		time[0] = this->_hour/10 + 0x30;
		time[1] = this->_hour%10 + 0x30;
	}
	
	time[2] = ':';
	
	if(this->_minute < 10){
		time[3] = '0';
		time[4] = this->_minute + 0x30;
	}
	else {
		time[3] = this->_minute/10 + 0x30;
		time[4] = this->_minute%10 + 0x30;
	}
	
	time[5] = ':';
	
	if(this->_second < 10){
		time[6] = '0';
		time[7] = this->_second + 0x30;
	}
	else {
		time[6] = this->_second/10 + 0x30;
		time[7] = this->_second%10 + 0x30;
	}
	
	time[8] = '\0';
	
	return string(time);
}

string F_Header::get_size(){
	string size = "";

	int cnt = this->_size;

	while(cnt>0){
		size += (cnt%10) + 0x30;
		cnt /= 10;
	}

	reverse(size.begin(), size.end());

	while(size.length() < 9)
		size += ' ';


	return size;

}

void F_Header::set_datetime(int day, int month, int year, int hour, int minute, int second){
	this->_day = day;
	this->_month = month;
	this->_year = year;
	this->_hour = hour;
	this->_minute = minute;
	this->_second = second;	
}

/*
 *
 *	END OF FILE HEADER FUNCTIONS
 *
 */

BTree_Header::BTree_Header(){
	this->_day = this->_month = this->_year = this->_hour = this->_minute = this->_second = this->_order = this->_root = this->_next = -1;
}

BTree_Header::BTree_Header(int day, int month, int year, int hour, int minute, int second, int order, int root, int next){
	this->_day = day;
	this->_month = month;
	this->_year = year;
	this->_hour = hour;
	this->_minute = minute;
	this->_second = second;	
	this->_order = order;
	this->_root = root;
	this->_next = next;
}

void BTree_Header::copy_datetime(F_Header file_header){
	this->_day = file_header._day;
	this->_month = file_header._month;
	this->_year = file_header._year;
	this->_hour = file_header._hour;
	this->_minute = file_header._minute;
	this->_second = file_header._second;
}

void BTree_Header::set_root(int root){
	this->_root = root;
}

void BTree_Header::set_next(int next){
	this->_next = next;
}

void BTree_Header::set_order(int order){
	this->_order = order;
}

void BTree_Header::print_info(){
	cout << "Ordem: " << this->_order << endl << "Raiz: " << this->_root << endl << "Prox.: " << this->_next << endl;
}

int BTree_Header::get_order_int(){
	return this->_order;
}

int BTree_Header::get_root(){
	return this->_root;
}

int BTree_Header::get_next(){
	return this->_next;
}

string BTree_Header::get_order(){
	string order = "";

	int cnt = this->_order;

	while(cnt>0){
		order += (cnt%10) + 0x30;
		cnt /= 10;
	}

	reverse(order.begin(), order.end());

	while(order.length() < 6)
		order += ' ';

	return order;
}

/*
 *
 *
 *
 */

bool BTree_Header::cmp_datetime(F_Header file_header){
	if(this->_day != file_header._day)
		return 1;
		
	if(this->_month != file_header._month)
		return 1;
		
	if(this->_year != file_header._year)
		return 1;
		
	if(this->_hour != file_header._hour)
		return 1;
		
	if(this->_minute != file_header._minute)
		return 1;
	
	if(this->_second != file_header._second)
		return 1;
		
	return 0;						
}

void BTree_Header::update_next(){
	this->_next++;
}


/*
 *
 *	FUNCTIONS FOR NODES
 *
 */

BTree_Node::BTree_Node(){
	this->_father = -1;
}

BTree_Node::BTree_Node(int father, int order){
	this->_father = father;
	this->_order = order;
}

void BTree_Node::set_father(int father){
	this->_father = father;
}

int BTree_Node::get_father(){
	return this->_father;
}

int BTree_Node::record_size(){
	return n_houses(this->_order) - 9 + 30*this->_order;
}

bool BTree_Node::is_leaf(){
	if(this->sons[0] == -1)
		return 1;
	return 0;
}

int BTree_Node::get_son(string key){
	int cnt;
	for(cnt = 0; cnt < (int)(this->keys).size(); cnt++){
		if(key.compare((this->keys[cnt]).first) == 0)
			return -1;
		if(key.compare((this->keys[cnt]).first) < 0)
			break;	
	}
	return this->sons[cnt];	
}

int BTree_Node::contains(string key){
	int cnt;
	for(cnt = 0; cnt < (int)(this->keys).size(); cnt++){
		if(key.compare((this->keys[cnt]).first) == 0)
			return this->keys[cnt].second;
	}
	return -1;
}

void BTree_Node::reset(){
	this->keys.clear();
	this->sons.clear();
}

/*
 *
 *	FUNCTION TO MODIFY KEYS AND SONS OF NODE
 *
 */

void BTree_Node::modify(pair<vector<pair<string, int> >, vector<int> > data){
	this->keys.clear();
	this->sons.clear();
	int i;
	for(i=0;i<(int)data.first.size();i++){
		this->keys.push_back(data.first[i]);
		this->sons.push_back(data.second[i]);
	}
	this->sons.push_back(data.second[i]);
}

/*
 *
 *	FUNCTIONT THAT ACCESS SPECIFIC POINTS OF THE FILE AND CHANGE IT
 *
 */

void BTree_Node::update_sons(fstream &file, int pos){
	for(int i=0;i<(int)this->sons.size();i++){
		if(this->sons[i] != -1){
			// Set file pointer to father location
			file.seekp(IDX_HEADER_SIZE + 2 + this->sons[i]*(2+this->record_size()) + n_houses(this->_order) + 1);
			file << transform_string(pos, 9);
		}	
	};
}

/*
 *
 *	FUNCTIONS TO INSERT KEY INTO SEPARATE NODE
 *
 */

pair<vector<pair<string, int> >, vector<int> > BTree_Node::slice(string key, int nrr, int left, int right){
	vector<pair<string, int> >::iterator it;
	int cnt=0;
	bool done = 0;
	bool ignore = 0;
	
	vector<pair<string, int> > aux;
	vector<int> aux2;
	
	for(cnt = 0; cnt < (int)(this->keys).size(); cnt++){
		if(key.compare((this->keys[cnt]).first) == 0)
			return pair<vector<pair<string, int> >, vector<int> >();
		if(key.compare((this->keys[cnt]).first)<0 && !done){
			aux.push_back(make_pair(key, nrr));
			aux2.push_back(left);
			aux2.push_back(right);
			ignore = 1;
			done = 1;
		};
		aux.push_back(this->keys[cnt]);
		if(!ignore){
			aux2.push_back(this->sons[cnt]);
		}
		else
			ignore = 0;
		
	}
	aux2.push_back(this->sons[cnt]);
	
	if(!done){
		aux.push_back(make_pair(key, nrr));
		aux2.push_back(right);
	};
	
	this->keys.clear();
	this->sons.clear();
	for(cnt=0;cnt<(this->_order/2);cnt++){
		this->keys.push_back(aux.front());
		this->sons.push_back(aux2.front());
		aux.erase(aux.begin());
		aux2.erase(aux2.begin());
	}
	this->sons.push_back(aux2.front());
	aux2.erase(aux2.begin());
	
	return make_pair(aux, aux2);
}

pair<vector<pair<string, int> >, vector<int> > BTree_Node::insert(string key, int nrr, int left, int right){
	if((int)this->keys.size() == (this->_order - 1)){
		return this->slice(key, nrr, left, right);
	}
	else {
		vector<pair<string, int> >::iterator it;
		int cnt=0;
		
		for(it = (this->keys).begin();it != (this->keys).end();it++){
			if(key.compare(it->first) == 0)
				return pair<vector<pair<string, int> >, vector<int> >();			
			if(key.compare(it->first) < 0)
				break;
			cnt++;
		};
		
		(this->keys).insert(it, make_pair(key, nrr));
		
		if((int)this->sons.size() > 0){
			vector<int>::iterator v_it = (this->sons).begin();
		
			while(cnt--)
				v_it++;
			v_it++;	
				
			this->sons.insert(v_it, right);	
		}
		else{
			this->sons.push_back(left);
			this->sons.push_back(right);
		};
		
		return pair<vector<pair<string, int> >, vector<int> >();
	};
}

/*
 *
 *	FUNCTIONS TO READ FILE AND FILL NODE
 *
 */

void BTree_Node::make(fstream &file, int pos){
	file.seekg(IDX_HEADER_SIZE + 2 + pos*(this->record_size() + 2));
	char line[10];
	int counter = 0;
	int nrr;
	int son;
	
	string strline;
	string key;
	
	// First read counter
	file.get(line, n_houses(this->_order) + 1);
	counter = strtol(line, NULL, 10);
	
	// Read separator
	file.get();
	
	// Read father
	file.get(line, 10);
	this->_father = strtol(line, NULL, 10);
	
	// Read separator
	file.get();
	
	this->keys.clear();
	this->sons.clear();
	
	for(int i=0;i<(this->_order - 1);i++){
		// Read '<'
		file.get();
		// Read key
		file.get(line, 9);
		key = string(line);
		// Read ','
		file.get();
		// Read position in file
		file.get(line, 10);
		nrr = strtol(line, NULL, 10);
		// read '>'
		file.get();
		// Insert values into vector
		if(i<counter)
			this->keys.push_back(make_pair(key, nrr));
	};
	
	// Read separator
	file.get();
	
	for(int i=0;i<(this->_order);i++){
		file.get(line, 10);
		son = strtol(line, NULL, 10);
		// Read ' ' or '\r'
		file.get();
		if(i<=counter)
			this->sons.push_back(son);
	};
	
	// Read endl
	file.get();
}

/*
 *
 *	FUNCTIONS TO WRITE NODE INTO FILE
 *
 */

void BTree_Node::write(fstream &file, int pos){
	file.seekp(IDX_HEADER_SIZE + 2 + pos*(this->record_size() + 2));
	
	// Write counter
	string strline = transform_string((int)this->keys.size(), n_houses(this->_order));
	
	file << strline << '|';
	
	// Write father
	strline = transform_string(this->_father, 9);
	
	file << strline << '|';
	this->print_keys(file);
	file << '|';
	this->print_sons(file);
	file << '\r' << endl;
}

void BTree_Node::print_keys(fstream &file){
	int i = 0;
	
	for(i=0;i<(int)this->keys.size();i++){
		file << '<';
		file << (this->keys[i]).first;
		file << ',';
		file << transform_string((this->keys[i]).second, 9);
		file << '>';
	}
	
	for(;i<(this->_order - 1);i++){
		file << "<-1      ,-1       >";
	}
}

void BTree_Node::print_n_sons(){
	cout << this->sons.size() << endl;
}

void BTree_Node::print_sons(fstream &file){
	int i = 0;
	
	for(i=0;i<(int)this->sons.size();i++){
		if(i)
			file << ' ';
		file << transform_string(this->sons[i], 9);
	}
	
	for(;i<this->_order;i++){
		if(i)
			file << ' ';
		file << "-1       ";
	};
}

/*
 *
 *	END OF NODE FUNCTIONS
 *
 */

Student::Student(){
	// Begins all strings as empty;
	this->_matricula = "";
	this->_nome = "";
	this->_curso = "";
	this->_turma = "";
}

Student::Student(string nome, string matricula, string curso, string turma){
	// Begins all strings with the values provided;
	this->_matricula = matricula;
	this->_nome = nome;
	this->_curso = curso;
	this->_turma = turma;
}
