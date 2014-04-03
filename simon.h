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
#include <stdint.h>

using namespace std;

class Simon{

	public:
	Simon(){

	}

	vector<uint64_t> encrypt_public(vector<uint64_t> key, vector<uint64_t> plaintext);
	vector<uint64_t> decrypt_public(vector<uint64_t> key, vector<uint64_t> ciphertext);

	private: 

	vector<vector<int>> z {
		{1,1,1,1,1,0,1,0,0,0,1,0,0,1,0,1,0,1,1,0,0,0,0,1,1,1,0,0,1,1,0,1,1,1,1,1,0,1,0,0,0,1,0,0,1,0,1,0,1,1,0,0,0,0,1,1,1,0,0,1,1,0},
		{1,0,0,0,1,1,1,0,1,1,1,1,1,0,0,1,0,0,1,1,0,0,0,0,1,0,1,1,0,1,0,1,0,0,0,1,1,1,0,1,1,1,1,1,0,0,1,0,0,1,1,0,0,0,0,1,0,1,1,0,1,0},
		{1,0,1,0,1,1,1,1,0,1,1,1,0,0,0,0,0,0,1,1,0,1,0,0,1,0,0,1,1,0,0,0,1,0,1,0,0,0,0,1,0,0,0,1,1,1,1,1,1,0,0,1,0,1,1,0,1,1,0,0,1,1},
		{1,1,0,1,1,0,1,1,1,0,1,0,1,1,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,1,0,0,1,1,1,0,0,1,1,0,1,0,0,0,0,1,1,1,1},
		{1,1,0,1,0,0,0,1,1,1,1,0,0,1,1,0,1,0,1,1,0,1,1,0,0,0,1,0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,1,1,0,0,1,0,1,0,0,1,0,0,1,1,1,0,1,1,1,1}
	};
	
	const int block_size = 64;
	const int key_size = 64;
	const int rounds = 72;
	const int z_m = 4;
	const int word_size = 64;

	vector<uint64_t> x_y;
	vector<uint64_t> keywords;

	void key_expansion();

	void encrypt(uint64_t &x, uint64_t &y);

	void decrypt(uint64_t &x, uint64_t &y);

	void print_long(uint64_t input);

	void print_all(bool);
};

#endif