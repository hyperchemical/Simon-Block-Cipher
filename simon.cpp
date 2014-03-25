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
#include "transform_message.cpp"

using namespace std;

unsigned long circular_left_shift(unsigned long num, unsigned long shift, unsigned long size){
	return (num << shift) | (num >> (size - shift));
}

unsigned long circular_right_shift(unsigned long num, unsigned long shift, unsigned long size){
	return (num >> shift) | (num << (size-shift));
}

void Simon::encrypt_message(string input){
	read_keys();	
	key_expansion();
	vector<unsigned long> yo = split_message(input.c_str(), block_size);
	if(yo.size() % 2 == 1){//im really lazy
		yo.push_back(time(NULL));
	}

	encrypt(yo[0], yo[1]);
	latest_x = yo[0];
	latest_y = yo[1];
}

void Simon::decrypt_latest_message(){
	decrypt(latest_x, latest_y);
	print_long(latest_x);
	print_long(latest_y);
}

void Simon::print_long(unsigned long input){
	mpz_class tmp;
	tmp = input;
	cout << mpz_to_string(tmp) << endl;
}


void Simon::init(){
	//generate_keys();
	read_keys();
	key_expansion();
	print_keywords_to_file();
	vector<unsigned long> yo = split_message("tst message", block_size);
	for (int i = 0; i < yo.size(); ++i)
	{
		cout << yo[i] << endl;
		mpz_class tmp;
		tmp = yo[i];
		cout << mpz_to_string(tmp) << endl;
	}
	
	cout << "Encrypt:\n======\n";
	encrypt(yo[0], yo[1]);

	for (int i = 0; i < yo.size(); ++i)
	{
		cout << yo[i] << endl;
	}

	cout << "Decrypt:\n======\n";
	decrypt(yo[0], yo[1]);

	for (int i = 0; i < yo.size(); ++i)
	{
		cout << yo[i] << endl;
		mpz_class tmp;
		tmp = yo[i];
		cout << mpz_to_string(tmp) << endl;
	}
}



void Simon::generate_keys(){
	ofstream myfile;
	myfile.open ("private_key.txt");

	gmp_randstate_t r_state;
	mpz_class key;
	gmp_randinit_default (r_state);
    gmp_randseed_ui(r_state, time(NULL));
    for(int i = 0; i < z_m; i++){
	    mpz_urandomb(key.get_mpz_t(), r_state, key_size);
	    mpz_setbit(key.get_mpz_t(), key_size-1);
		myfile << key << endl;
	}

	myfile.close();
}

void Simon::read_keys(){
	keywords.clear();
	string line;
	ifstream myfile ("private_key.txt");
	for(int i = 0; i < z_m; i++){
		getline(myfile,line);
		mpz_class key;
		key = line;
		keywords.push_back(mpz_get_ui(key.get_mpz_t()));
	}

	myfile.close();
}

void Simon::key_expansion(){
	keywords.resize(rounds);
	for(int i = z_m; i < rounds; i++){
		unsigned long tmp;
		tmp = circular_right_shift(keywords[i-1], 3, block_size);
		if(z_m == 4){
			tmp = keywords[i-3] ^ tmp;

		}
		tmp = tmp ^ circular_right_shift(tmp, 1, block_size);
		keywords[i] = (~keywords[i-z_m]) ^ tmp ^ z[z_m][(i-z_m) & 62] ^ 3;

	}
}

void Simon::encrypt(unsigned long& x, unsigned long& y){
	for(int i = 0; i < rounds; i++){
		
		unsigned long tmp = x;
		x = y ^ (circular_left_shift(x, 1, block_size) & circular_left_shift(x, 8, block_size)) ^ 
				circular_left_shift(x, 2, block_size) ^ keywords[i];
		y = tmp;
	}	
}

void Simon::decrypt(unsigned long &x, unsigned long &y){
	// mpz_class one, eight, two;
	// one = "1", eight = "8", two = "2";
	for(int i = rounds-1; i >= 0; i--){
		unsigned long tmp = y;
		y = x ^ (circular_left_shift(y, 1, block_size) & circular_left_shift(y, 8, block_size)) ^ 
				circular_left_shift(y, 2, block_size) ^ keywords[i];
		x = tmp;
	}	
}

void Simon::print_keywords_to_file(){
	ofstream myfile;
	myfile.open ("keywords.txt");

    for(auto i : keywords){
		myfile << i << endl;
	}

	myfile.close();

}

