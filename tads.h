#ifndef TADS_H
#define TADS_H

#include<string>
#include<cstdlib>
#include<fstream>
#include<vector>

/* Defines de tamanho */

#define NAME_LEN 40
#define MAT_LEN 6
#define CURSO_LEN 3
#define TURMA 1
#define HEADER_SIZE 36
#define IDX_HEADER_SIZE 46
#define RECORD_SIZE 53
#define MAX_SIZE_LEN 9
#define ORD_LEN 6
#define ROOT_LEN 6
#define EMPTY_POS_LEN 6
#define ADDR_LEN 9

/* End defines */

using namespace std;

// Forward declaration
class F_Header;

// Necessary for sorting inside nodes
bool my_comp(pair<string, int>, pair<string, int>);

string transform_string(int, int);

int n_houses(int);

// Class that saves file data
class Student{
	public:
		Student();
		Student(string, string, string, string);
		string _nome;
		string _matricula;
		string _curso;
		string _turma;
	private:
};

class Header{
	public:
		string get_date();
		string get_time();
		bool error();
		void print_modified();
	protected:
		int _day;
		int _month;
		int _year;
		int _hour;
		int _minute;
		int _second;
};

class BTree_Header: public Header{
	public:
		BTree_Header();
		BTree_Header(int, int, int, int, int, int, int, int, int);
		void copy_datetime(F_Header);
		bool cmp_datetime(F_Header);
		void set_root(int root);
		void set_next(int next);
		void set_order(int order);
		int get_order_int();
		int get_root();
		int get_next();
		string get_order();
		void print_info();
		void update_next();
	private:
		int _order;
		int _root;
		int _next;
};

class BTree_Node{
	public:
		BTree_Node();
		BTree_Node(int, int);
		void set_father(int);
		pair<vector<pair<string, int> >, vector<int> > slice(string key, int nrr, int left, int right);
		pair<vector<pair<string, int> >, vector<int> > insert(string key, int nrr, int left, int right);
		void modify(pair<vector<pair<string, int> >, vector<int> >);
		void update_sons(fstream &, int);
		void print_keys(fstream &);
		void print_sons(fstream &);
		void print_n_sons();
		void readinfo(fstream &);
		void write(fstream &, int);
		int record_size();
		void make(fstream &, int);
		bool is_leaf();
		int contains(string);
		void reset();
		int get_son(string);
		int get_father();
	private:
		int _order;
		int _father;
		vector<pair<string, int> > keys;
		vector<int> sons;
};

class F_Header: public Header{
	public:
		F_Header();
		F_Header(int, int, int, int, int, int, int);
		void set_datetime(int, int, int, int, int, int);
		string get_size();
		int get_size_int();
		void size_inc();
		friend void BTree_Header::copy_datetime(F_Header);
		friend bool BTree_Header::cmp_datetime(F_Header);
	private:
		int _size;
};

#endif
