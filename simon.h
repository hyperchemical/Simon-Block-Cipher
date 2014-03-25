#ifndef _SIMON_H
#define _SIMON_H


#include <string>
#include <stdlib.h>
#include <iostream>
#include <gmpxx.h>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <stdexcept> 
#include <cstdio>
#include <fstream>
#include <ctime>
#include <cassert>
#include "simon.h"

using namespace std;

class Simon{

	public:
	Simon(){

	}

	void init();

	void encrypt_message(string input);

	void decrypt_latest_message();

	private: 

	vector<vector<int>> z {
		{1,1,1,1,1,0,1,0,0,0,1,0,0,1,0,1,0,1,1,0,0,0,0,1,1,1,0,0,1,1,0,1,1,1,1,1,0,1,0,0,0,1,0,0,1,0,1,0,1,1,0,0,0,0,1,1,1,0,0,1,1,0},
		{1,0,0,0,1,1,1,0,1,1,1,1,1,0,0,1,0,0,1,1,0,0,0,0,1,0,1,1,0,1,0,1,0,0,0,1,1,1,0,1,1,1,1,1,0,0,1,0,0,1,1,0,0,0,0,1,0,1,1,0,1,0},
		{1,0,1,0,1,1,1,1,0,1,1,1,0,0,0,0,0,0,1,1,0,1,0,0,1,0,0,1,1,0,0,0,1,0,1,0,0,0,0,1,0,0,0,1,1,1,1,1,1,0,0,1,0,1,1,0,1,1,0,0,1,1},
		{1,1,0,1,1,0,1,1,1,0,1,0,1,1,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,1,0,0,1,1,1,0,0,1,1,0,1,0,0,0,0,1,1,1,1},
		{1,1,0,1,0,0,0,1,1,1,1,0,0,1,1,0,1,0,1,1,0,1,1,0,0,0,1,0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,1,1,0,0,1,0,1,0,0,1,0,0,1,1,1,0,1,1,1,1}
	};
	
	int block_size = 64;
	int key_size = 64;
	int rounds = 68;
	int z_m = 2;
	int word_size = 64;

	unsigned long latest_x;
	unsigned long latest_y;

	vector<unsigned long> private_session_keys;

	mpz_class key;
	vector<unsigned long> keywords;

	void generate_keys();

	void read_keys();

	void key_expansion();

	void encrypt(unsigned long& x, unsigned long& y);

	void decrypt(unsigned long &x, unsigned long &y);

	void print_keywords_to_file();

	void print_long(unsigned long input);
};

#endif