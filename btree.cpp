#include<fstream>
#include<string>
#include<iostream>
#include<unistd.h>
#include"btree.h"
#include"tads.h"

using namespace std;

void search_tree(fstream &file, fstream &idxfile, BTree_Header &head, string key){
	int n_seeks = 0, result=-1;
	int pos = head.get_root();
	
	while(pos != -1){
		BTree_Node node(-1, head.get_order_int());
		node.make(idxfile, pos);
		n_seeks++;
		
		result = node.contains(key);
		if(result>-1){
			cout << "Chave encontrada com " << n_seeks << " seeks! Registro esta na posicao " << result << " no arquivo original:" << endl;
			file.seekg(HEADER_SIZE + 2 + result*(RECORD_SIZE + 2));
			char line[64];
			file.getline(line, 64);
			
			cout << line << endl;
			
			break;
		}
		pos = node.get_son(key);
		
	};
	if(result == -1){
		cout << "Chave nao encontrada apos " << n_seeks << " seeks!" << endl;
	};
}

void insert_tree(fstream &file, BTree_Header &head, string key, int nrr){
	if(head.get_root() == -1){
		// Create first node
		BTree_Node node(-1, head.get_order_int());
		
		// Places key into node
		node.insert(key, nrr, -1, -1);
		
		// Insert node into file
		node.write(file, head.get_next());
		
		// Increment header
		head.update_next();
		
		// Update tree header
		head.set_root(0);
	}
	else {
		int root = head.get_root();
		int pos = root;
		BTree_Node node(-1, head.get_order_int());
		
		node.make(file, root);
		
		// Enquanto nao for folha
		while(!node.is_leaf()){
			pos = node.get_son(key);
			if(pos == -1)
				return;
			node.make(file, pos);
		};
		
		// Insere na folha
		pair<vector<pair<string, int> >, vector<int> > data;
		int left = -1, right = -1, father_nrr; 
		string father_key;
		bool ok = 0;
		
		while(!ok){
			data = node.insert(key, nrr, left, right);
			node.write(file, pos); // Overwrite in file
			
			if((int)data.first.size() > 0){
				// Page overflow

				father_key = (data.first.front()).first;
				father_nrr = (data.first.front()).second;
				left = pos;
				right = head.get_next();
		
				data.first.erase(data.first.begin());
			
				// Make brother node
				node.modify(data);
			
				node.update_sons(file, right);
			
				node.write(file, right);
				head.update_next();
							
				// Depends on father now
				if(node.get_father() == -1){
					// Special case, dealing with root
					node.reset();
					node.insert(father_key, father_nrr, left, right);
					int father_new_pos = head.get_next();
				
					node.write(file, father_new_pos);
					head.update_next();
				
					// Update header
					head.set_root(father_new_pos);
				
					// Correct father value in sons
					node.update_sons(file, father_new_pos);
					
					ok = 1;
				}
				else {
					pos = node.get_father();
					node.make(file, pos);
					
					key = father_key;
				}
			}
			else {
				ok = 1;
			};
		}
	}
}
