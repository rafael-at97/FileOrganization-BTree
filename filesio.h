#ifndef FILESIO_H
#define FILESIO_H

#include<fstream>
#include<string>
#include<vector>
#include"tads.h"

using namespace std;

bool open_file(fstream &, const char *, string, string);

bool rewrite_file(fstream &, char *, F_Header, vector<Student> &);

bool start_BTree_file(fstream &, BTree_Header);

#endif
