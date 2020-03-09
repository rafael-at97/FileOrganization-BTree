#ifndef BTREE_H
#define BTREE_H

#include<fstream>
#include<string>
#include"tads.h"

using namespace std;

void search_tree(fstream &, fstream &, BTree_Header &, string);

void insert_tree(fstream &, BTree_Header &, string, int);
#endif
